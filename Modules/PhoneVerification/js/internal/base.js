_SMS.BaseApi = function(config, data, path){
	const api = this;
	
	this.id = md5(JSON.stringify(data));
	this.key = data.key;
	this.service = data.service;
	
	if(_is_nilb(data.url) || data.url==config.url){
		this.url = config.url;
		this.name = config.name;
	}else{
		this.customUrl = data.url;
		var url = _trim_right(data.url, '/\\ ');
		var name = url.replace(new RegExp('https?://'),"").replace(/^(?:\d+)?api(?:\d+)?./,"");
		this.url = url;
		this.name = name.slice(0, 1).toLocaleUpperCase() + name.slice(1);
	};
	this.url += _is_nilb(config.path) ? _avoid_nil(path) : config.path;
	
	this.supportedMethods = config.supportedMethods;
	
	if(!_is_nilb(config.ref)){
		this.ref = config.ref;
		this.refTitle = _is_nilb(config.refTitle) ? 'ref' : config.refTitle;
	};
	
	if(!_is_nilb(config.limits) && config.limits.length){
		this.limits = [];
		for(var key in config.limits){
			var limit = config.limits[key];
			var limiter = new _SMS.rateLimiter({
				tokensPerInterval: limit.requestsPerInterval,
				interval: limit.interval,
				type: limit.type,
				id: (limit.type==="service" && _is_nilb(limit.id) ? (api.id + "_" + key) : limit.id),
				queue: _avoid_nilb(limit.queue, true)
			});
			this.limits.push(limiter);
		};
	}else{
		this.limits = false;
	};
	
	this.combineParams = function(params, options){
		for(var key in options){
			if(!_is_nilb(options[key])){
				params[key] = options[key];
			};
		};
		return params;
	};
	
	this.paramsToString = function(params){
		return Object.keys(params).map(function(key){
			return encodeURIComponent(key) + '=' + encodeURIComponent(params[key]);
		}).join('&');
	};
	
	this.paramsToArray = function(params){
		var arr = [];
		for(var key in params){
			if(!_is_nilb(params[key])){
				arr.push(key);
				arr.push(params[key]);
			};
		};
		return arr;
	};
	
	this.reduceString = function(str, length){
		length = _avoid_nilb(length, 100);
		str = _clean(str);
		return str.length > length ? str.slice(0, length) + '...' : str;
	};
	
	this.removePlus = function(number){
		return number.slice(0,1)=='+' ? number.slice(1) : number;
	};
	
	this.parseJSON = function(str){
		try{
			var json = JSON.parse(str);
		}catch(e){
			api.errorHandler("RESPONSE_IS_NOT_JSON", api.reduceString(str));
		};
		return json;
	};
	
	this.validateMethod = function(method, name){
		if(api.supportedMethods.indexOf(method) < 0){
			api.errorHandler('UNSUPPORTED_METHOD', name);
		};
	};
	
	this.ban = 0;
	
	this.banThread = function(seconds){
		if(seconds){
			api.ban = Date.now() + seconds * 1000;
		};
	};
	
	this.banService = function(seconds){
		if(seconds){
			PSet("sms", api.id, (Date.now() + seconds * 1000).toString());
		};
	};
	
	this.beforeRequest = function(){
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_do(function(){
			var sleepTime = 0;
			var nowDate = Date.now();
			
			if(nowDate > maxTime){
				api.errorHandler("ACTION_TIMEOUT");
			};
			
			if(api.ban > 0 && api.ban - nowDate > 0){
				sleepTime = api.ban - nowDate;
			};
			
			if(P("sms", api.id).length > 0 && parseInt(P("sms", api.id)) - nowDate > 0){
				var time = parseInt(P("sms", api.id)) - nowDate + rand(0,30);
				if(time > sleepTime){
					sleepTime = time;
				};
			};

			_if_else(sleepTime > 0, function(){
				if(nowDate + sleepTime > maxTime){
					sleepTime = maxTime - nowDate + rand(0,30);
				};
				
				api.log((_K=="ru" ? 'Ждем ' : 'Wait ') + (sleepTime/1000) + (_K=="ru" ? ' секунд перед запросом к ' : ' seconds before requesting ') + api.name);
				
				_call_function(api.sleep,{time:sleepTime})!
			}, function(){
				_break("function");
			})!
		})!
		
		_if(api.limits && api.limits.length, function(){
			_do(function(){
				var limiter_index = _iterator() - 1;
				if(limiter_index > api.limits.length - 1){
					_break();
				};
				var limiter = api.limits[limiter_index];
				
				_call_function(limiter.removeTokens, {api:api, count:1, timeout:timeout, maxTime:maxTime})!
				var remainingTokens = _result_function();
			})!
		})!
	};
	
	this.sleep = function(){
		var time = _function_argument("time");
		
		sleep(time)!
	};
	
	this.log = function(ruText, enText){
		if(_SMS.debug){
			enText = _avoid_nilb(enText, ruText);
			
			_info('[PhoneVerification debug] ' + (_K=="ru" ? ruText : enText));
		};
	};
	
	this.request = function(){
		var url = _function_argument("url");
		var method = _avoid_nilb(_function_argument("method"), "GET");
		var params = _function_argument("params");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var data = [];
		
		if(!_is_nilb(api.ref)){
			params[api.refTitle] = api.ref;
		};
		
		if(Object.keys(params).length > 0){
			if(method=="GET"){
				url += '?' + api.paramsToString(params);
			}else{
				data = api.paramsToArray(params);
			};
		};
		
		_call_function(api.beforeRequest,{timeout:timeout, maxTime:maxTime})!
		
		_switch_http_client_internal();
		
		_ensure_http_client();
		_BAS_FAIL_ON_ERROR = FAIL_ON_ERROR;
		http_client_set_fail_on_error(false);
		
		_do(function(){
			var cycle_index = _iterator();
			
			if(cycle_index > 10 || Date.now() > maxTime){
				_switch_http_client_main();
				FAIL_ON_ERROR = _BAS_FAIL_ON_ERROR;
				if(cycle_index > 10){
					api.errorHandler("FAILED_REQUEST");
				}else{
					api.errorHandler("ACTION_TIMEOUT");
				};
			};
			
			_if(cycle_index > 1, function(){
				_call_function(api.sleep,{time:2000})!
			})!
			
			_call(function(){
				_on_fail(function(){
					VAR_LAST_ERROR = _result();
					VAR_ERROR_ID = ScriptWorker.GetCurrentAction();
					VAR_WAS_ERROR = false;
					_break(1,true);
				});
				
				CYCLES.Current().RemoveLabel("function");
				
				var timeLeft = maxTime - Date.now();
				var requestTimeout = timeout < 60000 ? timeout : (timeLeft < 60000 ? 60000 : timeLeft);
				
				_if_else(method=="GET", function(){
					api.log((_K=="ru" ? 'Запрос к' : 'Request') + ' ' + api.name + ': ' + url);
					
					general_timeout_next(requestTimeout);
					http_client_get2(url, {"method":"GET"})!
				}, function(){
					api.log((_K=="ru" ? 'Запрос к' : 'Request') + ' ' + api.name + ': ' + url + ', ' + (_K=="ru" ? 'данные' : 'data') + ': ' + api.paramsToString(params));
					
					general_timeout_next(requestTimeout);
					http_client_post(url, data, {"content-type":"urlencode", "encoding":"UTF-8", "method":method})!
				})!
				
			}, null)!
			
			if(!http_client_was_error() && !VAR_WAS_ERROR){
				_break();
			}else{
				api.log((_K=="ru" ? 'Ошибка произошедшая во время запроса к' : 'An error occurred during the request to') + ' ' + api.name + ': ' + _clean(VAR_WAS_ERROR ? VAR_LAST_ERROR : http_client_error_string()));
			};
		})!
		
		FAIL_ON_ERROR = _BAS_FAIL_ON_ERROR;

		var content = http_client_content('auto');
		
		api.log((_K=="ru" ? 'Ответ от' : 'Response') + ' ' + api.name + ': ' + _clean(content));

		_switch_http_client_main();
		
		_function_return(content);
	};
	
	this.errorHandler = function(error, data){
		error = error.toString();
		data = _avoid_nil(data).toString();
		
		var baseErrors = {
			"FAILED_REQUEST": {
				"ru": "Не удалось успешно выполнить запрос к сервису за 10 попыток.",
				"en": "Failed to successfully complete the request to the service in 10 attempts."
			},
			"RESPONSE_IS_NOT_JSON": {
				"ru": "Не удалось распарсить ответ от сервиса. Содержание ответа: " + data,
				"en": "Failed to parse the response from the service. Response content: " + data
			},
			"ACTION_TIMEOUT": {
				"ru": "Превышено максимальное время выполнения действия.",
				"en": "The maximum execution time for the action has been exceeded."
			},
			"UNSUPPORTED_METHOD": {
				"ru": 'Метод "' + data + '" не поддерживается.',
				"en": 'Method "' + data + '" is not supported.'
			},
			"UNSUPPORTED_SITE": {
				"ru": 'Сайт "' + data + '" не поддерживается.',
				"en": 'Site "' + data + '" is not supported.'
			},
			"BAD_KEY": {
				"ru": "Неверный API-ключ.",
				"en": "Invalid API key."
			},
			"NO_KEY": {
				"ru": "API-ключ не указан.",
				"en": "API key not specified."
			},
			"NO_BALANCE": {
				"ru": "Закончился баланс.",
				"en": "Balance ended."
			},
			"LOW_BALANCE": {
				"ru": "Недостаточно денег на счету.",
				"en": "Not enough money in the account."
			},
			"NO_NUMBERS": {
				"ru": "Нет номеров.",
				"en": "No numbers."
			}
		};
		
		var errorObj = baseErrors.hasOwnProperty(error) ? baseErrors[error] : api.getError(error, data);
		
		if(!_is_nilb(errorObj) && errorObj.hasOwnProperty('base')){
			api.errorHandler(errorObj.base);
		};
		
		var message = api.name + ": " + error;
		if(_is_nilb(errorObj)){
			if(error==data || _is_nilb(data)){
				fail(message);
			}else{
				fail(message + ", " + data);
			};
		}else{
			fail(message + " - " + errorObj[_K]);
		};
	};
};
_SMS.assignApi = function(fn){
	fn.prototype = Object.create(this.BaseApi.prototype);
	fn.prototype.constructor = fn;
	return fn;
};