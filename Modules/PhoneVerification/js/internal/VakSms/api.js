_SMS.VakSmsApi = _SMS.assignApi(function(config, data){
	const api = this;
	_SMS.BaseApi.call(this, config, data, '/api');
	
	this.makeRequest = function(){
		var action = _function_argument("action");
		var options = _avoid_nilb(_function_argument("options"), {});
		var checkErrors = _avoid_nilb(_function_argument("checkErrors"), true);
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var url = api.url + '/' + action + '/';
		var params = api.combineParams({apiKey:api.key}, options);
		
		_call_function(api.request,{url:url, method:"GET", params:params, timeout:timeout, maxTime:maxTime})!
		var content = _result_function();
		
		var resp = api.parseJSON(content);
		
		if(checkErrors && resp.error){
			api.errorHandler(resp.error);
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
		var operator = _function_argument("operator");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest,{action:(site=="All" ? "getCountNumberList" : "getCountNumber"), options:{service:(site=="All" ? "" : site), country:country, operator:operator}, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		if(site=="All"){
			for(var key in resp){
				resp[key] = parseInt(resp[key].count);
			};
		}else{
			resp = resp[Object.keys(resp)[0]];
		};
		
		_function_return(resp);
	};
	
	this.getNumber = function(){
		var site = _function_argument("site");
		var country = _function_argument("country");
		var operator = _function_argument("operator");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest,{action:"getNumber", options:{service:site, country:country, operator:operator}, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		_function_return({api:api, id:resp.idNum, number:api.removePlus(resp.tel.toString())});
	};
	
	this.getState = function(){
		var number = _function_argument("number");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		var taskId = _SMS.getConfirmData(number).id;
		
		_call_function(api.makeRequest,{action:"getSmsCode", options:{idNum:taskId}, timeout:timeout, maxTime:maxTime})!
		
		_function_return(_result_function());
	};
	
	this.setStatus = function(){
		var number = _function_argument("number");
		var status = _function_argument("status").toString();
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var actions = {
			"-1":"end",
			"3":"send",
			"8":"bad"
		};
		
		if(!actions.hasOwnProperty(status)){
			_function_return();
			return;
		};
		
		var taskId = _SMS.getConfirmData(number).id;
		
		_call_function(api.makeRequest,{action:"setStatus", options:{idNum:taskId, status:actions[status]}, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		if(["ready","update"].indexOf(resp.status) < 0){
			api.errorHandler(resp.status);
		};
	};
	
	this.getCode = function(){
		var number = _function_argument("number");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.getState,{number:number, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
			
		_function_return(resp.smsCode);
	};
	
	this.getError = function(error, data){
		var errors = {
			"apiKeyNotFound": {
				"base": "BAD_KEY"
			},
			"noMoney": {
				"base": "LOW_BALANCE"
			},
			"noService": {
				"ru": "Данный сервис не поддерживается, свяжитесь с администрацией сайта.",
				"en": "This service is not supported, please contact the site administration."
			},
			"noCountry": {
				"ru": "Запрашиваемая страна отсутствует.",
				"en": "The requested country is missing."
			},
			"noOperator": {
				"ru": "Оператор не найден для запрашиваемой страны.",
				"en": "Operator not found for the requested country."
			},
			"noNumber": {
				"base": "NO_NUMBERS"
			},
			"badStatus": {
				"ru": "Неверный статус.",
				"en": "Invalid status."
			},
			"idNumNotFound": {
				"ru": "Неверный ID операции.",
				"en": "Invalid operation ID."
			},
			"badService": {
				"ru": "Неверный код сайта, сервиса, соц.сети.",
				"en": "Invalid website, service, social network code."
			},
			"badData": {
				"ru": "Отправлены неверные данные.",
				"en": "Invalid data sent."
			},
			"smsReceived": {
				"ru": "На этот номер уже получен код подтверждения, отмена невозможна.",
				"en": "This number has already received a confirmation code, cancellation is not possible."
			},
			"waitSMS": {
				"ru": "На этот номер уже отправлено SMS, отмена невозможна.",
				"en": "This number has already been sent SMS, cancellation is not possible."
			}
		};
		return errors[error];
	};
});