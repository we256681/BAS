/**
 * A hierarchical token bucket for rate limiting. See
 * http://en.wikipedia.org/wiki/Token_bucket for more information.
 *
 * @param options
 * @param options.bucketSize Maximum number of tokens to hold in the bucket.
 *  Also known as the burst rate.
 * @param options.tokensPerInterval Number of tokens to drip into the bucket
 *  over the course of one interval.
 * @param options.interval The interval length in milliseconds, or as
 *  one of the following strings: 'second', 'minute', 'hour', day'.
 * @param options.parentBucket Optional. A token bucket that will act as
 *  the parent of this bucket.
 */
_SMS.tokenBucket = function(options){
	const bucket = this;
	
	this.bucketSize = options.bucketSize;
	this.tokensPerInterval = options.tokensPerInterval;
	if(typeof options.interval === "string"){
		switch(options.interval){
			case "sec":
			case "second":
				this.interval = 1000;
				break;
			case "min":
			case "minute":
				this.interval = 1000 * 60;
				break;
			case "hr":
			case "hour":
				this.interval = 1000 * 60 * 60;
				break;
			case "day":
				this.interval = 1000 * 60 * 60 * 24;
				break;
			default:
				fail('Invalid interval ' + options.interval);
		};
	}else{
		this.interval = options.interval;
	};
	this.parentBucket = options.parentBucket;
	this.type = options.type;
	
	if(options.type==="service"){
		if(_is_nilb(options.id)){
			fail('TokenBucket id not specified, this parameter is required when working in multithreaded mode');
		};
		this.id = options.id;
		this.queue = options.queue;
		if(_is_nilb(P("sms", options.id))){
			var params = {content:0, lastDrip:Date.now()};
			if(options.queue){
				params.queue = [];
			};
			PSet("sms", options.id, JSON.stringify(params));
		};
	}else{
		this.content = 0;
		this.lastDrip = Date.now();
	};
	
	this.getParams = function(){
		return bucket.type==="service" ? JSON.parse(P("sms", bucket.id)) : bucket;
	};
	
	this.changeParams = function(changer){
		var params = bucket.getParams();
		var result = changer(params);
		if(bucket.type==="service"){
			PSet("sms", bucket.id, JSON.stringify(params));
		};
		return result;
	};
	
	this.getContent = function(){
		return bucket.getParams().content;
	};
	
	this.setContent = function(newContent){
		bucket.changeParams(function(params){
			return params.content = newContent;
		});
	};
	
	this.minusContent = function(count){
		bucket.changeParams(function(params){
			return params.content -= count;
		});
	};
	
	this.getLastDrip = function(){
		return bucket.getParams().lastDrip;
	};
	
	this.setLastDrip = function(newLastDrip){
		bucket.changeParams(function(params){
			return params.lastDrip = newLastDrip;
		});
	};
	
	this.getQueueIndex = function(){
		if(bucket.queue){
			return bucket.getParams().queue.indexOf(thread_number());
		}else{
			return -1;
		};
	};
	
	this.addToQueue = function(threadNumber){
		if(bucket.queue){
			threadNumber = _avoid_nilb(threadNumber, thread_number());
			bucket.changeParams(function(params){
				if(params.queue.indexOf(threadNumber) < 0){
					return params.queue.push(threadNumber);
				};
				return null;
			});
		};
	};
	
	this.removeFromQueue = function(threadNumber){
		if(bucket.queue){
			threadNumber = _avoid_nilb(threadNumber, thread_number());
			bucket.changeParams(function(params){
				var index = params.queue.indexOf(threadNumber);
				if(index > -1){
					return params.queue.splice(index, 1);
				};
				return null;
			});
		};
	};
	
	this.clearQueue = function(threadNumber){
		if(bucket.queue){
			threadNumber = _avoid_nilb(threadNumber, thread_number());
			bucket.changeParams(function(params){
				var index = params.queue.indexOf(threadNumber);
				if(index > -1){
					return params.queue.splice(0, index);
				};
				return null;
			});
		};
	};
	
	/**
	 * Asynchronous function
	 * 
	 * Remove the requested number of tokens. If the bucket (and any parent
	 * buckets) contains enough tokens this will happen immediately. Otherwise,
	 * the removal will happen when enough tokens become available.
	 * @param {Number} count The number of tokens to remove.
	 * @returns {Number} The remainingTokens count.
	 */
	this.removeTokens = function(){
		var count = _avoid_nilb(_function_argument("count"), 1);
		var timeout = _function_argument("timeout");
		var maxTime = _function_argument("maxTime");
		if(_is_nilb(maxTime) && !_is_nilb(timeout)){
			maxTime = Date.now() + timeout;
		};
		var api = _function_argument("api");
		
		// Is this an infinite size bucket?
		if(bucket.bucketSize === 0){
			_function_return(Number.POSITIVE_INFINITY);
		};
		// Make sure the bucket can hold the requested number of tokens
		if(count > bucket.bucketSize){
			fail('Requested tokens ' + count + ' exceeds bucket size ' + bucket.bucketSize);
		};
		
		var result = null;
		var attemptQueue = 0;
		
		bucket.addToQueue();
		
		_do(function(){
			if(maxTime && Date.now() > maxTime){
				bucket.removeFromQueue();
				if(_is_nilb(api)){
					fail('ACTION_TIMEOUT - ' + (_K=="ru" ? 'Превышено максимальное время выполнения действия.' : 'The maximum execution time for the action has been exceeded.'));
				}else{
					api.errorHandler("ACTION_TIMEOUT");
				};
			};
			
			// Drip new tokens into this bucket
			bucket.drip();
			
			var params = bucket.getParams();
			var queueIndex = bucket.queue ? params.queue.indexOf(thread_number()) : -1;
			var content = params.content;
			
			// If the queue has not yet reached this thread, come back later
			_if(queueIndex > 0, function(){
				// How long do we need to wait to make up the difference in tokens?
				var waitMs = Math.ceil((count * (queueIndex + 1) - content) * (bucket.interval / bucket.tokensPerInterval));
				_if_else(attemptQueue > 2 || (attemptQueue > 1 && waitMs % (bucket.interval / bucket.tokensPerInterval) == 0), function(){
					// Clear the queue before the current thread, since its queue should be in this time
					bucket.clearQueue();
				}, function(){
					_call_function(bucket.wait,{ms:waitMs, maxTime:maxTime})!
					
					attemptQueue++;
				})!
				
				_next("function");
			})!
			
			// If we don't have enough tokens in this bucket, come back later
			_if(count > content, function(){
				// How long do we need to wait to make up the difference in tokens?
				var waitMs = Math.ceil((count - content) * (bucket.interval / bucket.tokensPerInterval));
				_call_function(bucket.wait,{ms:waitMs, maxTime:maxTime})!
				
				_next("function");
			})!
			
			_if_else(!_is_nilb(bucket.parentBucket), function(){
				// Remove the requested from the parent bucket first
				_call_function(bucket.parentBucket.removeTokens,{count:count})!
				var remainingTokens = _result_function();
				var content = bucket.getContent();
				// Check that we still have enough tokens in this bucket
				_if(count > content, function(){
					// How long do we need to wait to make up the difference in tokens?
					var waitMs = Math.ceil((count - content) * (bucket.interval / bucket.tokensPerInterval));
					_call_function(bucket.wait,{ms:waitMs, maxTime:maxTime})!
					
					_next("function");
				})!
				// Tokens were removed from the parent bucket, now remove them from
				// this bucket. Note that we look at the current bucket and parent
				// bucket's remaining tokens and return the smaller of the two values
				bucket.minusContent(count);
				result = Math.min(remainingTokens, bucket.getContent());
				
				_break("function");
			}, function(){
				// Remove the requested tokens from this bucket
				bucket.minusContent(count);
				result = bucket.getContent();
				
				_break("function");
			})!
		})!
		
		bucket.removeFromQueue();
		
		_function_return(result);
	};
	
	/**
	 * Asynchronous function
	 * 
	 * Wait for a specified number of milliseconds
	 * @param {Number} Number of milliseconds to wait.
	 */
	this.wait = function(){
		var ms = _function_argument("ms");
		var maxTime = _function_argument("maxTime");
		
		if(maxTime){
			var now = Date.now();
			if(now + ms > maxTime){
				ms = maxTime - now + rand(0,30);
			};
		};
		
		_if(ms > 0, function(){
			sleep(ms)!
		})!
	};
	
	/**
	 * Attempt to remove the requested number of tokens and return immediately.
	 * If the bucket (and any parent buckets) contains enough tokens this will
	 * return true, otherwise false is returned.
	 * @param {Number} count The number of tokens to remove.
	 * @returns {Boolean} True if the tokens were successfully removed, otherwise
	 *  false.
	 */
	this.tryRemoveTokens = function(count){
		count = _avoid_nilb(_function_argument("count"), 1);
		
		// Is this an infinite size bucket?
		if(!bucket.bucketSize){
			return true;
		};
		// Make sure the bucket can hold the requested number of tokens
		if(count > bucket.bucketSize){
			return false;
		};
		// Drip new tokens into this bucket
		bucket.drip();
		// If we don't have enough tokens in this bucket, return false
		if(count > bucket.getContent()){
			return false;
		};
		// Try to remove the requested tokens from the parent bucket
		if(bucket.parentBucket && !bucket.parentBucket.tryRemoveTokens(count)){
			return false;
		};
		// Remove the requested tokens from this bucket and return
		bucket.minusContent(count);
		return true;
	};
	
	/**
	 * Add any new tokens to the bucket since the last drip.
	 * @returns {Boolean} True if new tokens were added, otherwise false.
	 */
	this.drip = function(){
		if(bucket.tokensPerInterval === 0){
			var prevContent = bucket.getContent();
			bucket.setContent(bucket.bucketSize);
			return bucket.getContent() > prevContent;
		};
		var now = Date.now();
		var deltaMS = Math.max(now - bucket.getLastDrip(), 0);
		bucket.setLastDrip(now);
		var dripAmount = deltaMS * (bucket.tokensPerInterval / bucket.interval);
		var prevContent = bucket.getContent();
		bucket.setContent(Math.min(bucket.getContent() + dripAmount, bucket.bucketSize));
		return Math.floor(bucket.getContent()) > Math.floor(prevContent);
	};
};