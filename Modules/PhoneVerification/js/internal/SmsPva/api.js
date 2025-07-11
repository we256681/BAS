_SMS.SmsPvaApi = _SMS.assignApi(function(config, data){
	const api = this;
	_SMS.BaseApi.call(this, config, data, '/priemnik.php');
	
	this.makeRequest = function(){
		var action = _function_argument("action");
		var options = _avoid_nilb(_function_argument("options"), {});
		var checkErrors = _avoid_nilb(_function_argument("checkErrors"), true);
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var params = api.combineParams({metod:action, apikey:api.key}, options);
		
		_call_function(api.request,{url:api.url, method:"GET", params:params, timeout:timeout, maxTime:maxTime})!
		var content = _result_function();
		
		if(!_is_json_string(content) && !_starts_with(content, '<!DOCTYPE html>') && !_starts_with(content, '<html>')){
			api.errorHandler(content);
		};
		
		var resp = api.parseJSON(content);
		
		if(resp.response=="2" && !_is_nilb(resp.balance)){
			if(Number(resp.balance) > 0){
				api.errorHandler('LOW_BALANCE');
			}else{
				api.errorHandler('NO_BALANCE');
			};
		};
	
		if(resp.response=="5"){
			api.banService(60);
		};
		
		if(resp.response=="6"){
			api.banService(600);
		};
		
		if(checkErrors && !_is_nilb(resp.response) && resp.response !== "1" && resp.response !== "ok"){
			if(!(resp.response=="2" && ["","null"].indexOf(resp.number) > -1 && [-1,0,"-1","0"].indexOf(resp.id) > -1)){
				api.errorHandler((resp.error_msg || resp.not_number) ? (resp.error_msg ? resp.error_msg : resp.not_number) : resp.response);
			};
		};

		_function_return(resp);
	};
	
	this.getBalance = function(){
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest,{action:"get_balance", timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		_function_return(resp.balance);
	};
	
	this.getNumbersCount = function(){
		var site = _function_argument("site");
		var country = _function_argument("country");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		if(site=="All"){
			fail(api.name + ': ' + (_K=="ru" ? 'Данный сервис не поддерживает получение количества номеров для всех сайтов, возможно получить количество номеров для одного сайта за раз.' : 'This service does not support getting the count of numbers for all sites, it is possible to get the count of numbers for one site at a time.'));
		};
		
		_call_function(api.makeRequest,{action:"get_count_new", options:{service:site, country:country}, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		_function_return(resp.online);
	};
	
	this.getNumber = function(){
		var site = _function_argument("site");
		var country = _function_argument("country");
		var number = _function_argument("number");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		var numberWithoutPrefix = "";
		var confirmData = {};
		if(!_is_nilb(number)){
			confirmData = _SMS.getConfirmData(number);
			site = confirmData.site;
			country = confirmData.country;
			numberWithoutPrefix = confirmData.numberWithoutPrefix;
		};
		
		var maxNumberWait = Date.now() + 600000;
		_do(function(){
			if(Date.now() > maxNumberWait){
				api.errorHandler("ACTION_TIMEOUT");
			};
			
			_call_function(api.makeRequest,{action:"get_number", options:{service:site, country:country, number:numberWithoutPrefix}, timeout:timeout, maxTime:maxTime})!
			var resp = _result_function();
			
			if(resp.response=="1"){
				if(_is_nilb(resp.CountryCode)){
					confirmData.id = resp.id;
					
					_function_return(confirmData);
				}else{
					var id = resp.id;
					var prefix = api.removePlus(resp.CountryCode);
					var numberWithoutPrefix = resp.number;
					var number = prefix + resp.number;
					
					_function_return({api:api, id:id, number:number, numberWithoutPrefix:numberWithoutPrefix, prefix:prefix, site:site, country:country});
				};
			};
			
			_call_function(api.sleep,{time:5000})!
		})!
	};
	
	this.getState = function(){
		var number = _function_argument("number");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		var confirmData = _SMS.getConfirmData(number);
		
		_call_function(api.makeRequest,{action:"get_sms", options:{service:confirmData.site, country:confirmData.country, id:confirmData.id}, checkErrors:false, timeout:timeout, maxTime:maxTime})!
		
		_function_return(_result_function());
	};
	
	this.setStatus = function(){
		var number = _function_argument("number");
		var status = _function_argument("status").toString();
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var actions = {
			"-1":"denial",
			"3":"get_proverka",
			"8":"ban"
		};
		
		if(!actions.hasOwnProperty(status)){
			_function_return();
			return;
		};
		
		var confirmData = _SMS.getConfirmData(number);
		var options = {service:confirmData.site, country:confirmData.country};
		if(status=="3"){
			options.number = confirmData.numberWithoutPrefix;
		}else{
			options.id = confirmData.id;
		};
		
		_call_function(api.makeRequest,{action:actions[status], options:options, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		_if(status=="3", function(){
			_if(["","null"].indexOf(resp.number) > -1 && [-1,0,"-1","0"].indexOf(resp.id) > -1, function(){
				_call_function(api.getNumber,{site:site, country:country, number:number, timeout:timeout, maxTime:maxTime})!
				resp = _result_function();
			})!
			
			confirmData.id = resp.id;
		})!
	};
	
	this.getCode = function(){
		var number = _function_argument("number");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		var code = null;
		
		_call_function(api.getState,{number:number, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		if(resp.response=="1"){
			code = _is_nilb(resp.sms) ? resp.text : resp.sms;
		}else{
			if(resp.response !== "2"){
				api.errorHandler(resp.error_msg ? resp.error_msg : resp.response);
			};
		};
			
		_function_return(code);
	};
	
	this.getError = function(error, data){
		var errors = {
			"API KEY NOT FOUND!": {
				"base": "BAD_KEY"
			},
			"API KEY не получен!": {
				"base": "BAD_KEY"
			},
			"Недостаточно средств!": {
				"base": "NO_BALANCE"
			},
			"NO_BALANCE": {
				"base": "NO_BALANCE"
			},
			"LOW_BALANCE": {
				"base": "LOW_BALANCE"
			},
			"Service NOT FOUND!": {
				"ru": "Сервис не найден.",
				"en": "Service not found."
			},
			"2": {
				"ru": "Не удалось изменить статус активации.",
				"en": "Failed to change activation status."
			},
			"3": {
				"ru": "id неверный или уже истек его срок.",
				"en": "id is invalid or has already expired."
			},
			"5": {
				"ru": "Превышено количество запросов в минуту.",
				"en": "You have exceeded the number of requests per minute."
			},
			"6": {
				"ru": "Вы забанены на 10 минут, т.к. набрали отрицательную карму.",
				"en": "You will be banned for 10 minutes, because scored negative karma."
			},
			"6": {
				"ru": "Превышено количество одновременных потоков. Дождитесь SMS от предыдущих заказов.",
				"en": "You have exceeded the number of concurrent streams. SMS Wait from previous orders."
			},
			"Превышено количество попыток!": {
				"ru": "Задайте больший интервал между вызовами к серверу API.",
				"en": "Set a longer interval between calls to API server."
			},
			"Произошла неизвестная ошибка.": {
				"ru": "Попробуйте повторить запрос позже.",
				"en": "Try to repeat your request later."
			},
			"Неверный запрос.": {
				"ru": "Проверьте синтаксис запроса и список используемых параметров (его можно найти на странице с описанием метода).",
				"en": "Check the request syntax and the list of parameters used (can be found on the page with method description)."
			},
			"Произошла внутренняя ошибка сервера.": {
				"ru": "Попробуйте повторить запрос позже.",
				"en": "Try to repeat your request later."
			}
		};
		return errors[error];
	};
});