_SMS.SmsAcktiwatorApi = _SMS.assignApi(function(config, data){
	const api = this;
	_SMS.BaseApi.call(this, config, data, '/api');
	
	this.makeRequest = function(){
		var action = _function_argument("action");
		var options = _avoid_nilb(_function_argument("options"), {});
		var checkErrors = _avoid_nilb(_function_argument("checkErrors"), true);
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var url = api.url + '/' + action + '/' + api.key;
		var params = api.combineParams({}, options);
		
		_call_function(api.request,{url:url, method:"GET", params:params, timeout:timeout, maxTime:maxTime})!
		var content = _result_function();
		var resp = content;
		
		if(_is_json_string(content) || _starts_with(content, '<!DOCTYPE html>') || _starts_with(content, '<html>')){
			resp = api.parseJSON(content);
			
			if(checkErrors && (resp.code=="error" || !_is_nilb(resp.code) || resp.error)){
				api.errorHandler(!_is_nilb(resp.code) ? resp.code : resp.error, resp.message);
			};	
		};

		_function_return(resp);
	};
	
	this.getBalance = function(){
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest,{action:"getbalance", timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		_function_return(resp);
	};
	
	this.getNumbersCount = function(){
		var site = _function_argument("site");
		var country = _function_argument("country");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest,{action:"numbersstatus", options:{code:country}, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		if(site=="All"){
			var sites = {};
			resp.forEach(function(data){
				sites[data.id] = parseInt(data.count);
			});
			_function_return(sites);
		}else{
			var data = resp.filter(function(e){return e.id==parseInt(site) || e.name==site})[0];
			_function_return(_is_nilb(data) ? null : data.count);
		};
	};
	
	this.getSites = function(){
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest,{action:"getservices", timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		_function_return(resp);
	};
	
	this.getCountries = function(){
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest,{action:"countries", timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		_function_return(resp.map(function(el){
			el.id = el.code;
			delete el.code;
			return el;
		}));
	};
	
	this.getNumber = function(){
		var site = _function_argument("site");
		var country = _function_argument("country");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest,{action:"getnumber", options:{id:site, code:country}, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		_function_return({api:api, id:resp.id, number:api.removePlus(resp.number)});
	};
	
	this.getState = function(){
		var number = _function_argument("number");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		var taskId = _SMS.getConfirmData(number).id;
		
		_call_function(api.makeRequest,{action:"getstatus", options:{id:taskId}, timeout:timeout, maxTime:maxTime})!
		
		_function_return(_result_function());
	};
	
	this.setStatus = function(){
		var number = _function_argument("number");
		var status = _function_argument("status").toString();
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_if(status=="-1" || status=="8" || status=="3", function(){
			var taskId = _SMS.getConfirmData(number).id;
			var options = {id:taskId};
			if(status !== "3"){
				options.status = "1";
			};
			
			_call_function(api.makeRequest,{action:(status=="3" ? "play" : "setstatus"), options:options, timeout:timeout, maxTime:maxTime})!
		})!
	};
	
	this.getCode = function(){
		var number = _function_argument("number");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		var code = null;
		
		_call_function(api.getState,{number:number, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		if(resp !== "null" && [resp.small,resp.text].filter(function(e){return !_is_nilb(e) && e !== '-'}).length > 0){
			code = (_is_nilb(resp.small) || resp.small=='-') ? resp.text : resp.small;
		};
		
		_function_return(code);
	};
	
	this.getError = function(error, data){
		var errors = {
			"101": {
				"ru": "Cервис не найден.",
				"en": "Service not found."
			},
			"102": {
				"base": "LOW_BALANCE"
			},
			"103": {
				"base": "NO_NUMBERS"
			},
			"201": {
				"base": "NO_KEY"
			},
			"202": {
				"base": "BAD_KEY"
			},
			"203": {
				"ru": "Аккаунт заблокирован | " + data,
				"en": "Account blocked | " + data
			},
		};
		return errors[error];
	};
});