_SMS.SmsActivateApi = _SMS.assignApi(function(config, data){
	const api = this;
	_SMS.BaseApi.call(this, config, data, '/stubs/handler_api.php');
	
	this.makeRequest = function(){
		var action = _function_argument("action");
		var options = _avoid_nilb(_function_argument("options"), {});
		var method = _avoid_nilb(_function_argument("method"), "GET");
		var isJSON = _function_argument("isJSON");
		var checkErrors = _avoid_nilb(_function_argument("checkErrors"), true);
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var params = api.combineParams({api_key:api.key, action:action}, options);
		
		_call_function(api.request,{url:api.url, method:method, params:params, timeout:timeout, maxTime:maxTime})!
		var content = _result_function();
		
		if(_is_json_string(content)){
			var resp = api.parseJSON(content);
		}else{
			content = content.split(':');
			var resp = {status:content[0]};
			var data = content.slice(1);
			if(resp.status=="ACCESS_NUMBER"){
				resp.id = data[0];
				resp.number = data[1];
			}else{
				resp.data = data.join(':');
			};
			if(isJSON || (checkErrors && !_starts_with(resp.status, 'ACCESS_'))){
				api.errorHandler(resp.status, resp.data);
			};
		};

		_function_return(resp);
	};
	
	this.getBalance = function(){
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest,{action:"getBalance", timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		_function_return(resp.data);
	};
	
	this.getNumbersCount = function(){
		var site = _function_argument("site");
		var country = _function_argument("country");
		var operator = _function_argument("operator");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest,{action:"getNumbersStatus", options:{country:country, operator:operator}, isJSON:true, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		if(site=="All"){
			var sites = {};
			for(var key in resp){
				var end = key.slice(-2);
				if(end != '_1'){
					sites[end=='_0' ? key.slice(0,-2) : key] = parseInt(resp[key]);
				};
			};
			_function_return(sites);
		}else{
			_function_return(_is_nilb(resp[site + '_0']) ? resp[site] : resp[site + '_0']);
		};
	};
	
	this.getSites = function(){
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest,{action:"getServices", isJSON:true, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		_function_return(resp);
	};
	
	this.getCountries = function(){
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest,{action:"getCountries", isJSON:true, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		_function_return(Array.isArray(resp) ? resp : Object.keys(resp).map(function(key){
			resp[key].id = String(resp[key].id);
			resp[key].name = resp[key].rus;
			resp[key].name_en = resp[key].eng;
			resp[key].name_ch = resp[key].chn;
			delete resp[key].rus;
			delete resp[key].eng;
			delete resp[key].chn;
			return resp[key];
		}));
	};
	
	this.getNumber = function(){
		var site = _function_argument("site");
		var country = _function_argument("country");
		var operator = _function_argument("operator");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest,{action:"getNumber", options:{service:site, country:country, operator:operator}, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		_function_return({api:api, id:resp.id, number:api.removePlus(resp.number)});
	};
	
	this.getState = function(){
		var number = _function_argument("number");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		var taskId = _SMS.getConfirmData(number).id;
		
		_call_function(api.makeRequest,{action:"getStatus", options:{id:taskId}, checkErrors:false, timeout:timeout, maxTime:maxTime})!
		
		_function_return(_result_function());
	};
	
	this.setStatus = function(){
		var number = _function_argument("number");
		var status = _function_argument("status").toString();
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		var taskId = _SMS.getConfirmData(number).id;
		
		if(status=="8" && api.service=="getsms.online"){
			status = "10";
		};
		
		_call_function(api.makeRequest,{action:"setStatus", options:{id:taskId, status:status}, checkErrors:false, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		if(!_starts_with(resp.status, 'ACCESS_') && resp.status !== "BAD_STATUS"){
			api.errorHandler(resp.status, resp.data);
		};
		
		_if(resp.status=="BAD_STATUS" && status=="8", function(){
			_call_function(api.setStatus,{number:number, status:"6", timeout:timeout, maxTime:maxTime})!
		})!
	};
	
	this.getCode = function(){
		var number = _function_argument("number");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		var code = null;
		
		_call_function(api.getState,{number:number, timeout:timeout, maxTime:maxTime})!
		var resp = _result_function();
		
		if(resp.status=='STATUS_OK'){
			code = resp.data;
		}else{
			if(resp.status.indexOf('STATUS_WAIT_') != 0){
				api.errorHandler(resp.status, resp.data);
			};
		};
			
		_function_return(code);
	};
	
	this.getError = function(error, data){
		var errors = {
			"BAD_KEY": {
				"base": "BAD_KEY"
			},
			"NO_KEY": {
				"base": "NO_KEY"
			},
			"NO_BALANCE": {
				"base": "NO_BALANCE"
			},
			"NO_MEANS": {
				"base": "NO_BALANCE"
			},
			"NO_NUMBERS": {
				"base": "NO_NUMBERS"
			},
			"NO_NUMBER": {
				"base": "NO_NUMBERS"
			},
			"ERROR_SQL": {
				"ru": "Ошибка SQL-сервера.",
				"en": "SQL Server Error."
			},
			"BAD_ACTION": {
				"ru": "Некорректное действие.",
				"en": "Incorrect action."
			},
			"BAD_SERVICE": {
				"ru": "Некорректное наименование сервиса.",
				"en": "Incorrect service name."
			},
			"WRONG_SERVICE": {
				"ru": "Неверный идентификатор сервиса.",
				"en": "Invalid service identifier."
			},
			"WRONG_EXCEPTION_PHONE": {
				"ru": "Некорректные исключающие префиксы.",
				"en": "Invalid exclusion prefixes."
			},
			"NO_BALANCE_FORWARD": {
				"ru": "Недостаточно средств для покупки переадресации.",
				"en": "Iinsufficient funds to buy call forwarding."
			},
			"BAD_FORWARD": {
				"ru": "Некорректно указана переадресация.",
				"en": "Redirection specified incorrectly."
			},
			"NOT_AVAILABLE": {
				"ru": "Для страны, которую вы используете, недоступна покупка мультисервисов.",
				"en": "Multiservice purchase is not available for the country you are using."
			},
			"NO_ACTIVATION": {
				"ru": "id активации не существует.",
				"en": "Activation id does not exist."
			},
			"NUMBER_OFFLINE": {
				"ru": "Сим-карта не онлайн.",
				"en": "The SIM card is not online."
			},
			"IMPOSSIBLE_AGAIN": {
				"ru": "Невозможно повторить этот заказ.",
				"en": "It is impossible to repeat the order."
			},
			"ORDER_NOT_FOUND": {
				"ru": "Номер старой операции не найден.",
				"en": "The old operation number was not found."
			},
			"WRONG_ADDITIONAL_SERVICE": {
				"ru": "Неверный дополнительный сервис (допустимы только сервисы для переадресации).",
				"en": "Invalid additional service (only services for redirection are allowed)."
			},
			"WRONG_ACTIVATION_ID": {
				"ru": "Неверный ID родительской активации.",
				"en": "Invalid Parent Activation ID."
			},
			"WRONG_SECURITY": {
				"ru": "Ошибка при попытке передать ID активации без переадресации, или же завершенной/не активной активации.",
				"en": "An error occurred while trying to transfer an activation ID without forwarding, or a completed/inactive activation."
			},
			"REPEAT_ADDITIONAL_SERVICE": {
				"ru": "Ошибка возникает при попытке заказать купленный сервис еще раз.",
				"en": "An error occurs when trying to order a purchased service again."
			},
			"ACCOUNT_INACTIVE": {
				"ru": "Свободных номеров нет.",
				"en": "No numbers."
			},
			"NO_ID_RENT": {
				"ru": "Не указан id Аренды.",
				"en": "Rent id not specified."
			},
			"INVALID_PHONE": {
				"ru": "Номер арендован не вами (неправильный id аренды).",
				"en": "Number is not rented by you (wrong rental id)."
			},
			"STATUS_FINISH": {
				"ru": "Аренда оплачена и завершена.",
				"en": "Rent paid and completed."
			},
			"STATUS_CANCEL": {
				"ru": "Аренда отменена с возвратом денег.",
				"en": "Rent canceled with refund."
			},
			"STATUS_WAIT_CODE": {
				"ru": "Ожидание первой SMS.",
				"en": "Waiting for the first SMS."
			},
			"INCORECT_STATUS": {
				"ru": "Отсутствует или неправильно указан статус.",
				"en": "Missing or incorrect status."
			},
			"CANT_CANCEL": {
				"ru": "Невозможно отменить аренду (более 20 мин.).",
				"en": "It is impossible to cancel the rental (more than 20 minutes)."
			},
			"ALREADY_FINISH": {
				"ru": "Аренда уже завершена.",
				"en": "Rental is already completed."
			},
			"ALREADY_CANCEL": {
				"ru": "Аренда уже отменена.",
				"en": "Rent already canceled."
			},
			"RENT_DIE": {
				"ru": "Аренду невозможно продлить, так как срок жизни номера истёк.",
				"en": "Rent cannot be extended because the number has expired."
			},
			"WRONG_OPERATOR": {
				"ru": "Оператор переданной аренды не MTT.",
				"en": "Rent transferred operator not MTT."
			},
			"INVALID_TIME": {
				"ru": "Неверное время. Доступное количество часов от 4 до 1344.",
				"en": "Wrong time. Available number of hours from 4 to 1344."
			},
			"BANNED": {
				"ru": "Аккаунт заблокирован до " + data,
				"en": "Account blocked until " + data
			}
		};
		return errors[error];
	};
});