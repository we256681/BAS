_SMS = {
	apiData: {},
	confirmData: {},
	debug: false,
	
	paramClean: function(str){
		return _avoid_nil(str).toString().trim();
	},
	
	init: function(service, apiKey, serverUrl){
		service = this.getBasicName(this.paramClean(service));
		apiKey = this.paramClean(apiKey);
		serverUrl = this.paramClean(serverUrl);
		
		var data = {service:service, key:apiKey};
		if(!_is_nilb(serverUrl)){
			data.url = serverUrl;
		};
		
		var id = md5(JSON.stringify(data));
		if(_is_nilb(this.apiData)){
			this.apiData = {};
		};
		
		if(!this.apiData.hasOwnProperty(id)){
			this.apiData[id] = this.getServiceApi(data);
		};
		
		return this.apiData[id];
	},
	
	getConfirmData: function(number, copy){
		if(_is_nilb(_SMS.confirmData) || !_SMS.confirmData.hasOwnProperty(number)){
			fail((_K=="ru" ? 'Нет информации о номере' : 'No information about the number') + ' "' + number + '"');
		};
		
		var confirmData = _SMS.confirmData[number];
		
		if(!copy){
			return confirmData;
		};
		
		var api = confirmData.api;
		var confirmDataCopy = api.combineParams({}, confirmData);
		confirmDataCopy.api = {service: api.service, key: api.key, customUrl: api.customUrl};
		
		return confirmDataCopy;
	},
	
	setConfirmData: function(confirmData){
		var api = confirmData.api;
		if(!(api instanceof _SMS.BaseApi)){
			confirmData.api = _SMS.init(api.service, api.key, api.customUrl);
		};
		_SMS.confirmData[confirmData.number] = confirmData;
	},
	
	getBalance: function(){
		var service = _function_argument("service");
		var apiKey = _function_argument("apiKey");
		var serverUrl = _function_argument("serverUrl");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var api = _SMS.init(service, apiKey, serverUrl);
		
		_call_function(api.getBalance,{timeout:timeout, maxTime:maxTime})!
		var balance = _result_function();
		
		_function_return(_is_nilb(balance) ? null : parseFloat(balance));
	},
	
	getNumbersCount: function(){
		var service = _function_argument("service");
		var apiKey = _function_argument("apiKey");
		var serverUrl = _function_argument("serverUrl");
		var site = _function_argument("site");
		var country = _SMS.paramClean(_function_argument("country"));
		var operator = _SMS.paramClean(_function_argument("operator"));
		var customSite = _SMS.paramClean(_function_argument("customSite"));
		var customCountry = _SMS.paramClean(_function_argument("customCountry"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var api = _SMS.init(service, apiKey, serverUrl);
		
		api.validateMethod('getNumbersCount', _K=="ru" ? "Получить количество доступных номеров" : "Get the count of available numbers");
		
		site = _is_nilb(customSite) ? (site=="All" ? "All" : api.getRawSite(site)) : customSite;
		country = _is_nilb(customCountry) ? api.getRawCountry(country) : customCountry;
		
		_call_function(api.getNumbersCount,{site:site, country:country, operator:operator, timeout:timeout, maxTime:maxTime})!
		var count = _result_function();
		
		_function_return(site=="All" ? (_is_nilb(count) ? {} : count) : (_is_nilb(count) ? 0 : parseInt(count)));
	},
	
	getSites: function(){
		var service = _function_argument("service");
		var apiKey = _function_argument("apiKey");
		var serverUrl = _function_argument("serverUrl");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var api = _SMS.init(service, apiKey, serverUrl);
		
		api.validateMethod('getSites', _K=="ru" ? "Получить список сайтов" : "Get list of sites");
		
		_call_function(api.getSites,{timeout:timeout, maxTime:maxTime})!
		
		_function_return(_result_function());
	},
	
	getCountries: function(){
		var service = _function_argument("service");
		var apiKey = _function_argument("apiKey");
		var serverUrl = _function_argument("serverUrl");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var api = _SMS.init(service, apiKey, serverUrl);
		
		api.validateMethod('getCountries', _K=="ru" ? "Получить список стран" : "Get list of countries");
		
		_call_function(api.getCountries,{timeout:timeout, maxTime:maxTime})!
		
		_function_return(_result_function());
	},
	
	getNumber: function(){
		var service = _function_argument("service");
		var apiKey = _function_argument("apiKey");
		var serverUrl = _function_argument("serverUrl");
		var site = _function_argument("site");
		var country = _SMS.paramClean(_function_argument("country"));
		var operator = _SMS.paramClean(_function_argument("operator"));
		var customSite = _SMS.paramClean(_function_argument("customSite"));
		var customCountry = _SMS.paramClean(_function_argument("customCountry"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		if(_is_nilb(_SMS.confirmData)){
			_SMS.confirmData = {};
		};
		
		var api = _SMS.init(service, apiKey, serverUrl);
		
		site = _is_nilb(customSite) ? api.getRawSite(site) : customSite;
		country = _is_nilb(customCountry) ? api.getRawCountry(country) : customCountry;
		
		_call_function(api.getNumber,{site:site, country:country, operator:operator, timeout:timeout, maxTime:maxTime})!
		var confirmData = _result_function();
		var number = confirmData.number;
		
		_SMS.confirmData[number] = confirmData;
		
		_function_return(number);
	},
	
	getState: function(){
		var number = _SMS.paramClean(_function_argument("number"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var confirmData = _SMS.getConfirmData(number);
		var api = confirmData.api;
		
		_call_function(api.getState,{number:number, timeout:timeout, maxTime:maxTime})!
		_function_return(_result_function());
	},
	
	getCode: function(){
		var number = _SMS.paramClean(_function_argument("number"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var confirmData = _SMS.getConfirmData(number);
		var api = confirmData.api;
		
		_call_function(api.getCode,{number:number, timeout:timeout, maxTime:maxTime})!
		
		_function_return(_result_function());
	},
	
	waitCode: function(){
		var number = _SMS.paramClean(_function_argument("number"));
		var timeout = 60000 * _avoid_nilb(_function_argument("timeout"), 10);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		var interval = 1000 * _avoid_nilb(_function_argument("interval"), 5);
		
		var confirmData = _SMS.getConfirmData(number);
		var api = confirmData.api;
		var code = null;
		
		_if(!confirmData.ready, function(){
			_call_function(_SMS.setStatus,{number:number, status:"1", timeout:timeout, maxTime:maxTime})!
		})!
		
		_do(function(){
			if(Date.now() > maxTime){
				api.errorHandler("ACTION_TIMEOUT");
			};
			
			_call_function(api.getCode,{number:number, timeout:timeout, maxTime:maxTime})!
			code = _result_function();
			
			if(!_is_nilb(code) && code !== false){
				_break();
			};
			
			sleep(interval)!
		})!
		
		_function_return(code);
	},
	
	setStatus: function(){
		var number = _SMS.paramClean(_function_argument("number"));
		var status = _SMS.paramClean(_function_argument("status"));
		var deleteInfo = _avoid_nilb(_function_argument("deleteInfo"), true);
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var confirmData = _SMS.getConfirmData(number);
		var api = confirmData.api;
		
		_call_function(api.setStatus,{number:number, status:status, timeout:timeout, maxTime:maxTime})!
		
		if(status=="1"){
			confirmData.ready = true;
		};
		
		if(status=="3"){
			confirmData.repeat = true;
		};
		
		if(deleteInfo && ["-1","6","8"].indexOf(status) > -1){
			delete _SMS.confirmData[number];
		};
	},
	
	setDebug: function(enable){
		this.debug = (enable==true || enable=="true" || enable==1);
	}
};