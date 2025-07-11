_SMS.OnlineSimApi = _SMS.assignApi(function(config, data){
	const api = this;
	_SMS.BaseApi.call(this, config, data, '/api');
	
	this.makeRequest = function(){
		var action = _function_argument("action");
		var options = _avoid_nilb(_function_argument("options"), {});
		var method = _avoid_nilb(_function_argument("method"), "GET");
		var checkErrors = _avoid_nilb(_function_argument("checkErrors"), true);
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var url = api.url + '/' + action + '.php';
		var params = api.combineParams({apikey:api.key}, options);
		
		_call_function(api.request,{url:url, method:method, params:params, timeout:timeout, maxTime:maxTime})!
		var content = _result_function();
		
		var resp = api.parseJSON(content);
		
		if(checkErrors && !_is_nilb(resp.response) && resp.response!=1){
			api.errorHandler(resp.response);
		};

		_function_return(resp);
	};
	
	this.getBalance = function(){
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest,{action:"getBalance", timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		_function_return(resp.balance);
	};
	
	this.getNumbersCount = function(){
		var site = _function_argument("site");
		var country = _function_argument("country");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest,{action:"getNumbersStats", options:{country:country}, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		var sites = {};
		var count = null;
		(country=="all" && _is_nilb(resp.name) ? Object.keys(resp) : [0]).forEach(function(key){
			var services = country=="all" ? resp[key].services : resp.services;
			var keys = Object.keys(services);
			if(site=="All"){
				keys.forEach(function(key){
					var data = services[key];
					if(_is_nilb(sites[data.slug])){
						sites[data.slug] = parseInt(data.count);
					}else{
						sites[data.slug] += parseInt(data.count);
					};
				});
				_function_return(sites);
			}else{
				var name = keys.filter(function(key){
					var service = services[key];
					return service.slug==site || service.service==site;
				})[0];
				if(!_is_nilb(name)){
					if(_is_nilb(count)){
						count = parseInt(services[name].count);
					}else{
						count += parseInt(services[name].count);
					};
				};
			};
		});
		_function_return(site=="All" ? sites : count);
	};
	
	this.getNumber = function(){
		var site = _function_argument("site");
		var country = _function_argument("country");
		var operator = _function_argument("operator");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest,{action:"getNum", options:{service:site, country:country, simoperator:operator, number:true}, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		_function_return({api:api, id:resp.tzid, lastId:resp.tzid, number:api.removePlus(resp.number)});
	};
	
	this.getState = function(){
		var number = _function_argument("number");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		var confirmData = _SMS.getConfirmData(number);
		
		_call_function(api.makeRequest,{action:"getState", options:{tzid:confirmData.lastId, msg_list:0, clean:1}, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		if(Array.isArray(resp)){
			resp = resp[0];
		};
		
		_function_return(resp);
	};
	
	this.setStatus = function(){
		var number = _function_argument("number");
		var status = _function_argument("status").toString();
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var actions = {
			"3":"setOperationRevise",
			"6":"setOperationOk",
			"8":"setOperationOk"
		};
		
		if(!actions.hasOwnProperty(status)){
			_function_return();
			return;
		};
		
		var confirmData = _SMS.getConfirmData(number);
		
		_call_function(api.makeRequest,{action:actions[status], options:{tzid:confirmData.id}, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		if(status=="3"){
			confirmData.lastId = resp.tzid;
		};
	};
	
	this.getCode = function(){
		var number = _function_argument("number");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		var confirmData = _SMS.getConfirmData(number);
		var code = null;
		
		_call_function(api.getState,{number:number, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		if(['TZ_NUM_ANSWER','TZ_OVER_OK'].indexOf(resp.response) > -1){
			code = resp.msg;
		}else{
			if(['TZ_NUM_WAIT','TZ_NUM_PREPARE'].indexOf(resp.response) < 0){
				api.errorHandler(resp.response);
			};
		};
		
		_function_return(code);
	};
	
	this.getError = function(error, data){
		var errors = {
			"ERROR_WRONG_KEY": {
				"base": "BAD_KEY"
			},
			"ERROR_NO_KEY": {
				"base": "NO_KEY"
			},
			"WARNING_LOW_BALANCE": {
				"base": "LOW_BALANCE"
			},
			"ACCOUNT_BLOCKED": {
				"ru": "Аккаунт заблокирован.",
				"en": "Account is blocked."
			},
			"ERROR_NO_SERVICE": {
				"ru": "Сервис не указан.",
				"en": "Service not specified."
			},
			"REQUEST_NOT_FOUND": {
				"ru": "Метод API не указан.",
				"en": "API method not specified."
			},
			"API_ACCESS_DISABLED": {
				"ru": "API выключено.",
				"en": "API disabled."
			},
			"API_ACCESS_IP": {
				"ru": "Доступ с данного ip выключен в профиле.",
				"en": "Access from this ip is disabled in the profile."
			},
			"EXCEEDED_CONCURRENT_OPERATIONS": {
				"ru": "Превышено количество одновременно заказанных номеров для вашего аккаунта.",
				"en": "Maximum quantity of numbers booked concurrently is exceeded for your account."
			},
			"NO_NUMBER": {
				"base": "NO_NUMBERS"
			},
			"TIME_INTERVAL_ERROR": {
				"ru": "Отложенный прием SMS не возможен в данный интервал времени.",
				"en": "Delayed SMS reception is not possible at this interval of time."
			},
			"INTERVAL_CONCURRENT_REQUESTS_ERROR": {
				"ru": "Превышено количество одновременных запросов на выдачу номера, повторите запрос позднее.",
				"en": "Maximum quantity of concurrent requests for number issue is exceeded, try again later."
			},
			"TRY_AGAIN_LATER": {
				"ru": "Запрос временно не может быть выполнен.",
				"en": "Temporarily unable to perform the request."
			},
			"NO_FORWARD_FOR_DEFFER": {
				"ru": "Активация переадресации возможна только на онлайн приеме.",
				"en": "Forwarding can be activated only for online reception."
			},
			"NO_NUMBER_FOR_FORWARD": {
				"ru": "Нет номеров для переадресации.",
				"en": "There are no numbers for forwarding."
			},
			"ERROR_LENGTH_NUMBER_FOR_FORWARD": {
				"ru": "Номер для переадресации имеет неверную длину.",
				"en": "Wrong length of the number for forwarding."
			},
			"DUPLICATE_OPERATION": {
				"ru": "Добавление операций с одинаковыми параметрами.",
				"en": "Adding operations with identical parameters."
			},
			"ERROR_NO_TZID": {
				"ru": "TZID не указано.",
				"en": "TZID not specified."
			},
			"ERROR_NO_OPERATIONS": {
				"ru": "Нет операций.",
				"en": "No operations."
			},
			"ACCOUNT_IDENTIFICATION_REQUIRED": {
				"ru": "Необходимо пройти идентификацию: для заказа мессенджера - любым способом, для переадресации - по паспорту.",
				"en": "You have to go through an identification process: to order a messenger - in any way, for forward - on the passport."
			},
			"ERROR_WRONG_TZID": {
				"ru": "Неверный номер операции.",
				"en": "Wrong operation number."
			},
			"NO_COMPLETE_TZID": {
				"ru": "Невозможно завершить операцию.",
				"en": "Unable to complete the operation."
			},
			"UNDEFINED_COUNTRY": {
				"ru": "Страна указана неверно.",
				"en": "Country specified incorrectly."
			},
			"UNDEFINED_DAYS": {
				"ru": "Количество дней указано неверно.",
				"en": "The number of days specified incorrectly."
			}
		};
		return errors[error];
	};
});