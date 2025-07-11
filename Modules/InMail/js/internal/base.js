_InMail.baseApi = function(isCurl, protocol, config){
	const api = this;
	this.protocol = protocol;
	this.config = config;
	this.action = protocol;
	
	if(isCurl){
		
		this.reset = function(){
			native("curlwrapper", "easycleanup");
		};
		
		api.reset();
		
		this.curlOpts = {
			"CURLOPT_URL": api.protocol + (api.config.encrypt=="ssl" ? 's' : '') + '://' + api.config.host,
			"CURLOPT_PORT": api.config.port,
			"CURLOPT_USERNAME": api.config.username,
			"CURLOPT_PASSWORD": api.config.password,
			"CURLOPT_USE_SSL": api.config.encrypt=="none" ? 0 : 3,
			"CURLOPT_SSL_VERIFYPEER": false
		};
		
		this.setProxy = function(proxy){
			api.curlOpts["CURLOPT_PROXY"] = (proxy.type=="socks5" ? "socks5h" : proxy.type) + '://' + proxy.host + ':' + proxy.port;
			api.curlOpts["CURLOPT_PROXYUSERNAME"] = _is_nilb(proxy.username) ? "" : proxy.username;
			api.curlOpts["CURLOPT_PROXYPASSWORD"] = _is_nilb(proxy.password) ? "" : proxy.password;
			api.reset();
		};
		
		this.clearProxy = function(){
			delete api.curlOpts["CURLOPT_PROXY"];
			delete api.curlOpts["CURLOPT_PROXYUSERNAME"];
			delete api.curlOpts["CURLOPT_PROXYPASSWORD"];
			api.reset();
		};
		
		this.setConnectTimeout = function(ms){
			api.curlOpts["CURLOPT_CONNECTTIMEOUT_MS"] = ms;
		};
		
		this.wrapper = function(){
			var params = _function_arguments();
			
			native_async("curlwrapper", "easyperform", JSON.stringify(params))!
			
			_function_return(JSON.parse(_result()));
		},
		
		this.request = function(){
			var path = _function_argument("path");
			var query = _function_argument("query");
			var isFetch = _avoid_nilb(_function_argument("isFetch"), false);
			var noBody = _avoid_nilb(_function_argument("noBody"), false);
			var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
			var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
			
			var options = {};
			
			for(var key in api.curlOpts){
				if(!_is_nilb(api.curlOpts[key])){
					options[key] = api.curlOpts[key];
				};
			};
			
			if(!_is_nilb(path)){
				path = '' + path;
				options["CURLOPT_URL"] += (path.slice(0, 1) != '/' ? '/' : '') + path;
			};
			
			if(noBody){
				options["CURLOPT_NOBODY"] = true;
			}else{
				options["CURLOPT_NOBODY"] = false;
			};
			
			if(timeout && maxTime){
				var timeLeft = maxTime - Date.now();
				var requestTimeout = timeout < 60000 ? timeout : (timeLeft < 60000 ? 60000 : timeLeft);
				options["CURLOPT_TIMEOUT_MS"] = requestTimeout;
				if(requestTimeout > 60000){
					general_timeout_next(requestTimeout + 5000);
				};
			};
			
			options["CURLOPT_CUSTOMREQUEST"] = _is_nil(query) ? "" : query.trim();
			
			_InMail.log(api.protocol + ' ' + (_K=="ru" ? 'запрос' : 'request') + ': «‎' + query + '», url: «‎' + options["CURLOPT_URL"] + '»');
			
			_call_function(api.wrapper, {write_to_string: true, options: options, trace: true, is_fetch: isFetch, save_session: true, timeout: (api.resetTimeout || 5 * 60 * 1000)})!
			var resp = _result_function();
			
			resp.code = resp.code.replace('CURLE_', '');
			
			var msg = api.protocol + ' ' + (_K=="ru" ? 'ответ' : 'response') + ': «‎' + resp.code + '»';
			
			if(resp.result){
				resp.result = resp.result.trim();
				msg += (', ' + (_K=="ru" ? 'результат' : 'result') + ': «‎' + resp.result + '»');
			};
			
			if(resp.error){
				resp.error = resp.error.trim();
				msg += (', ' + (_K=="ru" ? 'ошибка' : 'error') + ': «‎' + resp.error + '»');
			};
			
			_InMail.log(msg);
			
			if(resp.code == "OK"){
				_function_return(resp);
			}else{
				var error = resp.error;
				
				if((resp.code == "QUOTE_ERROR" && error == "Quote command returned error") || (resp.code == "RECV_ERROR" && error == "Failure when receiving data from the peer")){
					var debug = resp.trace.trim().split(/\r?\n/);
					for(var i = debug.length - 1; i > -1; --i){
						var ell = debug[i];
						if(resp.code == "QUOTE_ERROR"){
							ell = ell.slice(ell.indexOf(" ") + 1);
						};
						if((resp.code == "QUOTE_ERROR" && (_starts_with(ell, "BAD") || _starts_with(ell, "NO"))) || (resp.code == "RECV_ERROR" && _starts_with(ell, "-ERR"))){
							error = ell.slice(ell.indexOf(" ") + 1).trim();
							break;
						};
					};
				};
				
				api.errorHandler(resp.code, error);
			};
		};
	
		this.decoder = function(charset, encoding, data){
			encoding = encoding.toLowerCase();
			var resp = JSON.parse(native("curlwrapper", "decoder", JSON.stringify({charset: charset, encoding: encoding, data: data})));
			if(!resp.success){
				api.errorHandler('FAIL_DECODE', resp.error);
			};
			return resp.result;
		};
		
		this.decodeWords = function(str){
			return (
				(str || '')
					.toString()
					// remove spaces between mime encoded words
					.replace(/(=\?[^?]+\?[QqBb]\?[^?]*\?=)\s+(?==\?[^?]+\?[QqBb]\?[^?]*\?=)/g, '$1')
					// convert all underscores to spaces
					.replace(/[_\s]/g, ' ')
					// decode words
					.replace(/=\?([\w_\-*]+)\?([QqBb])\?([^?]*)\?=/g, function(m, charset, encoding, text){return api.decoder(charset, encoding, text) || text})
			);
		};

		this.parseHeader = function(str, noDecode){
			var lines = str.split('\r\n');
			var len = lines.length;
			var header = {};
			var h = undefined;
			var i = undefined;
				
			for(i = 0; i < len; ++i){
				if(lines[i].length === 0){
					break; // empty line separates message's header and body
				};
				if(lines[i][0] === '\t' || lines[i][0] === ' '){
					if(!Array.isArray(header[h])){
						continue; // ignore invalid first line
					};
					// folded header content
					var val = lines[i];
					if(!noDecode){
						if(/=\?([^?*]*?)(?:\*.*?)?\?([qb])\?(.*?)\?=$/i.test(lines[i - 1]) && /^[ \t]=\?([^?*]*?)(?:\*.*?)?\?([qb])\?(.*?)\?=/i.test(val)){
							// RFC2047 says to *ignore* leading whitespace in folded header values
							// for adjacent encoded-words ...
							val = val.substring(1);
						};
					};
					header[h][header[h].length - 1] += val;
				}else{
					var m = /^([^:]+):[ \t]?(.+)?$/.exec(lines[i]);
					if(m){
						h = m[1].toLowerCase().trim();
						if(m[2]){
							if(typeof header[h] === "undefined"){
								header[h] = [m[2]];
							}else{
								header[h].push(m[2]);
							};
						}else{
							header[h] = [''];
						};
					}else{
						break;
					};
				};
			};
			if(!noDecode){
				for(h in header){
					var hvs = header[h];
					for(i = 0, len = header[h].length; i < len; ++i){
						hvs[i] = api.decodeWords(hvs[i]);
					};
					if(header[h].length < 2){
						header[h] = hvs[0];
					};
				};
			};
			return header;
		};
		
		this.uuedecode = function(str){
			var stop = false;
			var i = 0;
			var out = '';
			
			do{
				if(i < str.length){
					var n = str.charCodeAt(i) - 32 & 0x3F;
					
					++i;
					
					if(n > 45){
						api.errorHandler('UEE_INVALID_DATA');
					};
					
					if(n < 45){
						stop = true;
					};
					
					while(n > 0){
						var c1 = str.charCodeAt(i);
						var c2 = str.charCodeAt(i + 1);
						var c3 = str.charCodeAt(i + 2);
						var c4 = str.charCodeAt(i + 3);
						
						out += String.fromCharCode(((c1 - 32 & 0x3F) << 2 | (c2 - 32 & 0x3F) >> 4) & 0xFF);
						out += String.fromCharCode(((c2 - 32 & 0x3F) << 4 | (c3 - 32 & 0x3F) >> 2) & 0xFF);
						out += String.fromCharCode(((c3 - 32 & 0x3F) << 6 | c4 - 32 & 0x3F) & 0xFF);
						i += 4;
						n -= 3;
					};
					
					++i;
				}else{
					stop = true;
				};
			}while(!stop);
			
			return out;
		};
		
		this.getParamInfo = function(data){
			data = _avoid_nilb(data, []);
			if(!Array.isArray(data)){
				data = [data];
			};
			var raw = false;
			data = data.map(function(d){return d.toLowerCase()}).filter(function(d){return !(raw = (d === 'raw'))});
			var base = data.length > 0;
			return {
				base: base,
				raw: raw,
				any: base || raw,
				data: data
			};
		};
		
		this.processPartData = function(data, encoding, charset, saveToFile){
			charset = charset || 'utf-8';
			saveToFile = _avoid_nilb(saveToFile, false);
			
			var result = '';
			if(encoding === 'base64'){
				result = data.trim().split('\r\n').join('');
				if(!saveToFile){
					result = api.decoder(charset, 'b', result);
				};
			}else if(encoding === 'quoted-printable'){
				result = api.decoder(charset, 'q', data);
			}else if(['7bit', '7bits'].indexOf(encoding) > -1){
				result = data;
			}else if(['8bit', '8bits', 'binary'].indexOf(encoding) > -1){
				result = api.decoder(charset, '', data);
			}else if(encoding === 'uuencode'){
				var parts = data.split('\n');
				result = api.uuedecode(parts.splice(1, parts.length - 4).join(''));
			}else{
				api.errorHandler('UNKNOWN_ENCODING', part.encoding);
			};
			
			if(saveToFile){
				native("filesystem", "writefile", JSON.stringify({path:saveToFile, value:result, base64:(encoding === 'base64'), append:false}));
			}else{
				return result;
			};
		};
		
		this.maskToRegExp = function(mask){
			return (new RegExp(mask.replace(/([.])/g, '\\$1').replace(/\*/g, '.+').replace(/\?/g, '.')));
		};
		
		this.randStr = function(length, chars){
			length = _avoid_nilb(length, 10);
			chars = _avoid_nilb(chars, 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789');
			var str  = '';
			for(var i = 0; i < length; ++i){
				str += chars.charAt(Math.floor(Math.random() * chars.length));
			};
			return str;
		};
	};
	
	this.setAction = function(action){
		action = action || api.protocol;
		
		if(!_starts_with(action, api.protocol)){
			action = api.protocol + '.' + action;
		};
		
		api.action = action;
	};
	
	this.clearAction = function(){
		api.action = api.protocol;
	};
	
	this.cutAction = function(){
		var act = api.action;
		api.clearAction();
		return act;
	};
	
	this.validateArgType = function(value, type, name){
		var act = api.cutAction();
		_InMail.validateArgType(value, type, name, act);
		api.action = act;
	};
	
	this.errorHandler = function(error, data){
		error = error.toString();
		data = _avoid_nil(data).toString();
		
		var errors = {
			"INVALID_VALUE": {
				"ru": "Неверное значение аргумента: " + data,
				"en": "Invalid argument value: " + data
			},
			"INCORRECT_ARGS_NUM": {
				"ru": "Неверное количество аргументов для параметра поиска: " + data,
				"en": "Incorrect number of arguments for search option: " + data
			},
			"ARG_NOT_DATE": {
				"ru": 'Аргумент параметра поиска "' + data + '" должен быть Объектом даты или строкой даты, доступной для синтаксического анализа',
				"en": 'Search option "' + data + '" argument must be a Date object or a parseable date string'
			},
			"ARG_NOT_NUM": {
				"ru": 'Аргумент параметра поиска "' + data + '" должен быть числом',
				"en": 'Search option "' + data + '" argument must be a number'
			},
			"OR_NOT_TWO_ARGS": {
				"ru": "ИЛИ должно иметь ровно два аргумента",
				"en": "OR must have exactly two arguments"
			},
			"SERVER_NOT_SUPPORT": {
				"ru": data + " не поддерживается сервером",
				"en": data + " is not supported by the server"
			},
			"SORT_NOT_SUPPORT": {
				"ru": "Сортировка не поддерживается на сервере, используйте поиск без сортировки",
				"en": "Sort is not supported on the server, use non-sorting search"
			},
			"ESEARCH_NOT_SUPPORT": {
				"ru": "ESEARCH не поддерживается на сервере",
				"en": "ESEARCH is not supported on the server"
			},
			"MOVE_NOT_SUPPORT": {
				"ru": "Перемещение не поддерживается на сервере",
				"en": "Move is not supported on the server"
			},
			"UNEXPECTED_OPTION": {
				"ru": "Неожиданный параметр поиска: " + data,
				"en": "Unexpected search option: " + data
			},
			"UNEXPECTED_OPTION_TYPE": {
				"ru": "Неожиданный тип данных параметра поиска. Ожидалась строка или массив. Получено: " + data,
				"en": "Unexpected search option data type. Expected string or array. Received: " + data
			},
			"UNEXPECTED_CRITERION": {
				"ru": "Неожиданный критерий сортировки: " + data,
				"en": "Unexpected sort criterion: " + data
			},
			"UNEXPECTED_CRITERION_TYPE": {
				"ru": "Неожиданный тип данных критерия сортировки. Ожидалась строка. Получено: " + data,
				"en": "Unexpected sort criterion data type. Expected string. Received: " + data
			},
			"MAILBOX_NOT_SELECTED": {
				"ru": "Почтовый ящик не выбран или выбран неправильно",
				"en": "Mailbox not selected or selected incorrectly"
			},
			"TOBOX_NOT_SPECIFIED": {
				"ru": "Конечный почтовый ящик не указан или указан неправильно",
				"en": "Destination mailbox not specified or specified incorrectly"
			},
			"EMPTY_SORT_CRITERIA": {
				"ru": "Ожидается массив хотя бы с одним критерием сортировки",
				"en": "Expected array with at least one sort criteria"
			},
			"EMPTY_UID_LIST": {
				"ru": "Пустой список uid",
				"en": "Empty uid list"
			},
			"EMPTY_SEQUENCE_LIST": {
				"ru": "Пустой список порядковых номеров",
				"en": "Empty sequence number list"
			},
			"UID_IS_SMALLER": {
				"ru": "UID должен быть больше нуля",
				"en": "UID must be greater than zero"
			},
			"WRONG_FORMAT_UID": {
				"ru": 'UID должен быть целым числом, "*" или диапазоном: ' + data,
				"en": 'UID must be an integer, "*", or a range: ' + data
			},
			"NOT_FLAGS_KEYWORDS": {
				"ru": 'Не Флаги не Ключевые слова не указаны',
				"en": 'No Flags no Keywords are not specified'
			},
			"KEYWORD_INVALID_CHARS": {
				"ru": 'Ключевое слово "' + data + '" содержит недопустимые символы.',
				"en": 'The keyword "' + data + '" contains invalid characters'
			},
			"KEYWORDS_INVALID_ARGS": {
				"ru": 'Аргумент ключевых слов должен быть строкой или непустым массивом',
				"en": 'Keywords argument must be a string or a non-empty Array'
			},
			"FLAGS_INVALID_ARGS": {
				"ru": 'Аргумент флагов должен быть строкой или непустым массивом',
				"en": 'Flags argument must be a string or a non-empty Array'
			},
			"UNKNOWN_ENCODING": {
				"ru": 'Неизвестная кодировка' + data,
				"en": 'Unknown encoding ' + data
			},
			"UEE_INVALID_DATA": {
				"ru": 'Неверные UUE данные',
				"en": 'Invalid UUE data'
			},
			"EMPTY_MSGS_LIST": {
				"ru": 'Не удалось найти письмо, соответствующее указанному идентификатору, в указанной папке почтового ящика',
				"en": 'Could not find a message matching the specified id in the specified mailbox folder'
			},
			"NOT_AVAILABLE_POP3": {
				"ru": 'Функция "' + data + '" недоступна по pop3,  используйте подключение по imap, если это возможно',
				"en": '"' + data + '" function is not available on pop3, use imap connection if it possible'
			},
			"FAILED_PARSE": {
				"ru": 'Не удалось распарсить результат: "' + data + '"',
				"en": 'Failed to parse result: "' + data + '"'
			},
			"OPERATION_TIMEDOUT": {
				"ru": "Превышено максимальное время выполнения действия.",
				"en": "The maximum execution time for the action has been exceeded."
			}
		};
		
		var errorObj = errors[error];
		var message = error;
		
		if(_is_nilb(errorObj)){
			if(error!=data && !_is_nilb(data)){
				message += " - " + data;
			};
		}else{
			message += " - " + errorObj[_K];;
		};
		
		_InMail.error(message, null, api.cutAction());
	};
};
_InMail.assignApi = function(fn){
	fn.prototype = Object.create(this.baseApi.prototype);
	fn.prototype.constructor = fn;
	return fn;
};