_SMS.SmsRegApi = _SMS.assignApi(function(config, data){
	const api = this;
	_SMS.BaseApi.call(this, config, data, '');
	
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
		
		if(checkErrors && !_is_nilb(resp.response) && resp.response !== "1"){
			api.errorHandler(resp.error_msg ? resp.error_msg : resp.response);
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
	
	this.getSites = function(){
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest,{action:"getList", options:{extended:1}, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		_function_return(resp.services.map(function(el){return {id:el.service,name:el.description}}));
	};
	
	this.getNumber = function(){
		var site = _function_argument("site");
		var country = _function_argument("country");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest,{action:"getNum", options:{service:site, country:country}, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		var confirmData = {api:api, id:resp.tzid, number:null};
		
		var maxNumberWait = Date.now() + 600000;
		_do(function(){
			if(Date.now() > maxNumberWait){
				api.errorHandler("ACTION_TIMEOUT");
			};
			
			_call_function(api.getState,{confirmData:confirmData, timeout:timeout, maxTime:maxTime})!
			var resp = _result_function();

			if(["TZ_NUM_PREPARE","TZ_NUM_WAIT","TZ_NUM_ANSWER"].indexOf(resp.response) > -1){
				confirmData.number = api.removePlus(resp.number);
				_function_return(confirmData);
			};

			if(resp.response!="TZ_INPOOL"){
				api.errorHandler(resp.error_msg ? resp.error_msg : resp.response);
			};
			
			_call_function(api.sleep,{time:5000})!
		})!
	};
	
	this.getState = function(){
		var number = _function_argument("number");
		var confirmData = _function_argument("confirmData");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		if(_is_nilb(confirmData)){
			confirmData = _SMS.getConfirmData(number);
		};
		
		_call_function(api.makeRequest,{action:"getState", options:{tzid:confirmData.id}, checkErrors:false, timeout:timeout, maxTime:maxTime})!
		
		_function_return(_result_function());
	};
	
	this.setStatus = function(){
		var number = _function_argument("number");
		var status = _function_argument("status").toString();
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var actions = {
			"-1":"setOperationUsed",
			"1":"setReady",
			//"3":"getNumRepeat", //https://sms-reg.com/news.html#!020718
			"6":"setOperationOk",
			"8":"setOperationUsed"
		};
		
		if(!actions.hasOwnProperty(status)){
			_function_return();
			return;
		};
		
		var taskId = _SMS.getConfirmData(number).id;
		
		_call_function(api.makeRequest,{action:actions[status], options:{tzid:taskId}, checkErrors:("1" !== status), timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
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
			var new_code = _is_nilb(resp.msg) ? resp.full_msg : resp.msg;
			if(!confirmData.repeat || _is_nilb(confirmData.code) || new_code !== confirmData.code){
				code = new_code;
				confirmData.code = new_code;
			};
		}else{
			if(['TZ_NUM_WAIT','TZ_NUM_PREPARE'].indexOf(resp.response) < 0){
				api.errorHandler(resp.error_msg ? resp.error_msg : resp.response);
			};
		};
			
		_function_return(code);
	};
	
	this.getError = function(error, data){
		var errors = {
			"ERROR_WRONG_KEY": {
				"base": "BAD_KEY"
			},
			"ERROR_KEY_NEED_CHANGE": {
				"ru": "API-ключ требует замены. (BAD_KEY)",
				"en": "API key needs to be replaced. (BAD_KEY)"
			},
			"ERROR_NO_KEY": {
				"base": "NO_KEY"
			},
			"WARNING_LOW_BALANCE": {
				"base": "LOW_BALANCE"
			},
			"Service not define": {
				"ru": "Сервис не определен.",
				"en": "Service not defined."
			},
			"TZID must be number": {
				"ru": "Значение TZID должно быть числом.",
				"en": "The TZID value must be a number."
			},
			"There is no TZID value": {
				"ru": "TZID не указано.",
				"en": "TZID not specified."
			},
			"Wrong characters in parameters": {
				"ru": "Недопустимые символы в передаваемых данных.",
				"en": "Invalid characters in the transmitted data."
			},
			"Rate change can be made when all current operations finished": {
				"ru": "Изменение ставки возможно после завершения всех операций.",
				"en": "Changing the rate is possible after the completion of all operations."
			},
			"WARNING_WAIT15MIN": {
				"ru": "Вы не использовали многие из выданных номеров, поэтому выдача новых номеров заморожена на 15 минут.",
				"en": "You have not used many of the issued numbers and therefore the issuance of new numbers is frozen for 15 minutes."
			},
			"WARNING_NO_NUMS": {
				"base": "NO_NUMBERS"
			},
			"TZ_OVER_OK": {
				"ru": "Операция завершена.",
				"en": "Operation completed."
			},
			"TZ_OVER_EMPTY": {
				"ru": "Ответ не поступил за отведенное время.",
				"en": "The answer was not received within the allotted time."
			},
			"TZ_OVER_NR": {
				"ru": "Вы не отправили запрос методом setReady.",
				"en": "You did not send the request with the setReady method."
			},
			"TZ_DELETED": {
				"ru": "Операция удалена, средства возвращены.",
				"en": "Operation deleted, funds returned."
			},
			"0": {
				"ru": "Повтор по указанной операции невозможен.",
				"en": "Repeat for the specified operation is not possible."
			},
			"2": {
				"ru": "Номер оффлайн, используйте метод getNumRepeatOffline.",
				"en": "Number offline, use the getNumRepeatOffline method."
			},
			"3": {
				"ru": "Операция удалена, средства возвращены.",
				"en": "Operation deleted, funds returned."
			}
		};
		var errorObj = errors[error];
		if(_is_nilb(errorObj)){
			var reg = /(\S+) to this TZID not applicable/;
			if(reg.test(error)){
				var method = error.match(reg)[1];
				errorObj = {
					"ru": "Метод " + method + " не применим к указанному TZID.",
					"en": "The " + method + " method is not applicable to the specified TZID."
				};
			}else{
				var reg = /There is no (\S+) value/;
				if(reg.test(error)){
					var parameter = error.match(reg2)[1];
					errorObj = {
						"ru": "Параметр " + parameter + " не указан.",
						"en": "Parameter " + parameter + " not specified."
					};
				};
			};
		};
		return errorObj;
	};
});