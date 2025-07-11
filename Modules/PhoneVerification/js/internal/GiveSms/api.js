_SMS.GiveSmsApi = _SMS.assignApi(function(config, data){
	const api = this;
	_SMS.BaseApi.call(this, config, data, '/api/v1/');
	
	this.makeRequest = function(){
		var action = _function_argument("action");
		var options = _avoid_nilb(_function_argument("options"), {});
		var checkErrors = _avoid_nilb(_function_argument("checkErrors"), true);
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var params = api.combineParams({method:action, userkey:api.key}, options);
		
		_call_function(api.request,{url:api.url, method:"GET", params:params, timeout:timeout, maxTime:maxTime})!
		var content = _result_function();
		
		var resp = api.parseJSON(content);
		
		if(checkErrors && resp.status!=200){
			api.errorHandler(resp.status, resp.data.advice ? resp.data.advice : resp.data.msg);
		};

		_function_return(checkErrors ? resp.data : resp);
	};
	
	this.getBalance = function(){
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest,{action:"getbalance", timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		_function_return(resp.balance);
	};
	
	this.getNumbersCount = function(){
		var site = _function_argument("site");
		var operator = _function_argument("operator");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest,{action:"getcount", timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		var sites = resp[_is_nilb(operator) ? "ANY" : operator.toLocaleUpperCase()];
		
		if(site=="All"){
			for(var key in sites){
				sites[key] = parseInt(sites[key].count);
			};
			_function_return(sites);
		}else{
			_function_return(sites[site].count);
		};
	};
	
	this.getNumber = function(){
		var site = _function_argument("site");
		var country = _function_argument("country");
		var operator = _function_argument("operator");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest,{action:"getnumber", options:{service:site, country:country, operator:operator}, checkErrors:false, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		if(resp.status==200){
			_function_return({api:api, id:resp.order_id, lastId:resp.order_id, number:resp.phone});
		}else{
			if(resp.status==500){
				resp.status = "NO_NUMBERS";
			};
			api.errorHandler(resp.status, resp.data.msg);
		};
	};
	
	this.getState = function(){
		var number = _function_argument("number");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		var confirmData = _SMS.getConfirmData(number);
		
		var options = {order_id:confirmData.id};
		if(confirmData.repeat){
			options.last_id = confirmData.lastId;
		};
		
		_call_function(api.makeRequest,{action:(confirmData.repeat ? "wrongcode" : "getcode"), options:options, checkErrors:false, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
			
		if(!_is_nilb(resp.data.id)){
			confirmData.lastId = resp.data.id;
		};
		
		_function_return(resp);
	};
	
	this.setStatus = function(){
		var number = _function_argument("number");
		var status = _function_argument("status").toString();
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_if(status=="-1" || status=="8", function(){
			var taskId = _SMS.getConfirmData(number).id;
			_call_function(api.makeRequest,{action:(status=="-1" ? "refusenumber" : "bannumber"), options:{order_id:taskId}, timeout:timeout, maxTime:maxTime})!
		})!
	};
	
	this.getCode = function(){
		var number = _function_argument("number");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		var confirmData = _SMS.getConfirmData(number);
		var code = null;
		
		_call_function(api.getState,{number:number, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		if(resp.status==200){
			code = _is_nilb(resp.data.code) ? resp.data.fullSms : resp.data.code;
			if(!confirmData.repeat){
				confirmData.repeat = true;
			};
		}else{
			if(!(resp.status === 400 || (confirmData.repeat && resp.status === 502))){
				api.errorHandler(resp.status, resp.data.msg);
			};
		};
		
		_function_return(code);
	};
	
	this.getError = function(error, data){
		var errors = {
			"401": {
				"base": "BAD_KEY"
			},
			"404": {
				"ru": "Неправильно задан параметр method.",
				"en": "The method parameter is set incorrectly."
			},
			"500": {
				"ru": "Ошибка при обработке запроса / Истекло время заказа.",
				"en": "Error processing request / Time of order expired."
			},
			"502": {
				"ru": "Сервис не существует.",
				"en": "Service does not exist."
			},
			"503": {
				"ru": "Оператора не существует.",
				"en": "Operator does not exist."
			},
			"504": {
				"base": "LOW_BALANCE"
			},
			"505": {
				"ru": "Страна не существует.",
				"en": "Country does not exist."
			},
			"506": {
				"ru": "Не указан параметр order_id.",
				"en": "order_id parameter not specified."
			},
			"666": {
				"ru": "Многократный бан номеров.",
				"en": "Multiple ban of numbers."
			}
		};
		return errors[error];
	};
});