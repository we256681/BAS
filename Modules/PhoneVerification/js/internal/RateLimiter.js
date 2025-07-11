/**
 * A generic rate limiter. Underneath the hood, this uses a token bucket plus
 * an additional check to limit how many tokens we can remove each interval.
 *
 * @param options
 * @param options.tokensPerInterval Maximum number of tokens that can be
 *  removed at any given moment and over the course of one interval.
 * @param options.interval The interval length in milliseconds, or as
 *  one of the following strings: 'second', 'minute', 'hour', day'.
 * @param options.fireImmediately Whether or not the promise will resolve
 *  immediately when rate limiting is in effect (default is false).
 * @param options.parentLimiter Optional. A limiter that will act as
 *  the parent of this limiter.
 */
_SMS.rateLimiter = function(options){
	const limiter = this;
	
	this.type = options.type;
	
	if(options.type==="service"){
		if(_is_nilb(options.id)){
			fail('RateLimiter id not specified, this parameter is required when working in multithreaded mode');
		};
		this.id = options.id;
		if(_is_nilb(P("sms", options.id))){
			var now = Date.now();
			var params = {
				content: options.tokensPerInterval,
				lastDrip: now,
				curIntervalStart: now,
				tokensThisInterval: 0
			};
			if(options.queue){
				params.queue = [];
			};
			PSet("sms", options.id, JSON.stringify(params));
		};
	};
	
	this.tokenBucket = new _SMS.tokenBucket({
		bucketSize: options.tokensPerInterval,
		tokensPerInterval: options.tokensPerInterval,
		interval: options.interval,
		parentBucket: options.parentLimiter,
		type: options.type,
		id: options.id,
		queue: options.queue
	});
	
	if(options.type!=="service"){
		// Fill the token bucket to start
		this.tokenBucket.setContent(options.tokensPerInterval);
		this.curIntervalStart = Date.now();
		this.tokensThisInterval = 0;
	};
	
	this.fireImmediately = _avoid_nilb(options.fireImmediately, false);
	
	this.getParams = function(){
		return limiter.type==="service" ? JSON.parse(P("sms", limiter.id)) : limiter;
	};
	
	this.changeParams = function(changer){
		var params = limiter.getParams();
		var result = changer(params);
		if(limiter.type==="service"){
			PSet("sms", limiter.id, JSON.stringify(params));
		};
		return result;
	};
	
	this.getTokensThisInterval = function(){
		return limiter.getParams().tokensThisInterval;
	};
	
	this.setTokensThisInterval = function(newTokensThisInterval){
		limiter.changeParams(function(params){
			return params.tokensThisInterval = newTokensThisInterval;
		});
	};
	
	this.plusTokensThisInterval = function(count){
		limiter.changeParams(function(params){
			return params.tokensThisInterval += count;
		});
	};
	
	this.getCurIntervalStart = function(){
		return limiter.getParams().curIntervalStart;
	};
	
	this.setCurIntervalStart = function(newCurIntervalStart){
		limiter.changeParams(function(params){
			return params.curIntervalStart = newCurIntervalStart;
		});
	};
	
	/**
	 * Asynchronous function
	 * 
	 * Remove the requested number of tokens. If the rate limiter contains enough
	 * tokens and we haven't spent too many tokens in this interval already, this
	 * will happen immediately. Otherwise, the removal will happen when enough
	 * tokens become available.
	 * @param {Number} count The number of tokens to remove.
	 * @returns {Number} The remainingTokens count.
	 */
	this.removeTokens = function(count){
		var args = _function_arguments();
		var count = _avoid_nilb(args.count, 1);
		
		// Make sure the request isn't for more than we can handle
		if(count > limiter.tokenBucket.bucketSize){
			fail('Requested tokens ' + count + ' exceeds maximum tokens per interval ' + limiter.tokenBucket.bucketSize);
		};
		var now = Date.now();
		// Advance the current interval and reset the current interval token count
		// if needed
		if(now < limiter.getCurIntervalStart() || now - limiter.getCurIntervalStart() >= limiter.tokenBucket.interval){
			limiter.setCurIntervalStart(now);
			limiter.setTokensThisInterval(0);
		};
		// If we don't have enough tokens left in this interval, wait until the
		// next interval
		_if(count > limiter.tokenBucket.tokensPerInterval - limiter.getTokensThisInterval(), function(){
			_if_else(limiter.fireImmediately, function(){
				_function_return(-1);
			}, function(){
				var waitMs = Math.ceil(limiter.getCurIntervalStart() + limiter.tokenBucket.interval - now);
				_call_function(limiter.wait,{ms:waitMs})!
				_call_function(limiter.tokenBucket.removeTokens, args)!
				var remainingTokens = _result_function();
				limiter.plusTokensThisInterval(count);
				_function_return(remainingTokens);
			})!
		})!
		// Remove the requested number of tokens from the token bucket
		_call_function(limiter.tokenBucket.removeTokens, args)!
		var remainingTokens = _result_function();
		limiter.plusTokensThisInterval(count);
		_function_return(remainingTokens);
	};
	
	/**
	 * Asynchronous function
	 * 
	 * Wait for a specified number of milliseconds
	 * @param {Number} Number of milliseconds to wait.
	 */
	this.wait = function(){
		var ms = _function_argument("ms");
		
		sleep(ms)!
	};
	
	/**
	 * Attempt to remove the requested number of tokens and return immediately.
	 * If the bucket (and any parent buckets) contains enough tokens and we
	 * haven't spent too many tokens in this interval already, this will return
	 * true. Otherwise, false is returned.
	 * @param {Number} count The number of tokens to remove.
	 * @returns {Boolean} True if the tokens were successfully removed, otherwise
	 *  false.
	 */
	this.tryRemoveTokens = function(count){
		count = _avoid_nilb(_function_argument("count"), 1);
		
		// Make sure the request isn't for more than we can handle
		if(count > limiter.tokenBucket.bucketSize){
			return false;
		};
		var now = Date.now();
		// Advance the current interval and reset the current interval token count
		// if needed
		if(now < limiter.getCurIntervalStart() || now - limiter.getCurIntervalStart() >= limiter.tokenBucket.interval){
			limiter.setCurIntervalStart(now);
			limiter.setTokensThisInterval(0);
		};
		// If we don't have enough tokens left in this interval, return false
		if(count > limiter.tokenBucket.tokensPerInterval - limiter.getTokensThisInterval()){
			return false;
		};
		// Try to remove the requested number of tokens from the token bucket
		var removed = limiter.tokenBucket.tryRemoveTokens(count);
		if(removed){
			limiter.plusTokensThisInterval(count);
		};
		return removed;
	};
	
	/**
	 * Returns the number of tokens remaining in the TokenBucket.
	 * @returns {Number} The number of tokens remaining.
	 */
	this.getTokensRemaining = function(){
		limiter.tokenBucket.drip();
		return limiter.tokenBucket.getContent();
	};
};