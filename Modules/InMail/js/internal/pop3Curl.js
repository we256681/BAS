_InMail.pop3 = _InMail.assignApi(function(config){
	const api = this;
	_InMail.baseApi.call(this, true, "pop3", config);
	
	this.validateCriteria = function(criteria){
		api.validateArgType(criteria, ['array','string'], 'Search criteria');
		if(typeof criteria === 'string'){
			criteria = [criteria];
		};
		if(typeof criteria[0] === 'string'){
			criteria[0] = criteria[0].toUpperCase();
		};
		if(criteria[0] != "ALL" || criteria.length > 1){
			api.errorHandler('NOT_AVAILABLE_POP3', _K=="ru" ? 'Фильтрация' : 'Filtration');
		};
	};
	
	this.prepareUIDs = function(uids){
		if(_is_nilb(uids) || (Array.isArray(uids) && uids.length === 0)){
			api.errorHandler('EMPTY_UID_LIST');
		};
		
		if(!Array.isArray(uids)){
			if(typeof uids == "string"){
				uids = _to_arr(uids);
			}else{
				uids = [uids];
			};
		};
		
		for(var i = 0, len = uids.length, intval; i < len; ++i){			
			intval = parseInt('' + uids[i], 10);
			
			if(isNaN(intval)){
				api.errorHandler('WRONG_FORMAT_UID', uids[i]);
			}else if (intval <= 0){
				api.errorHandler('UID_IS_SMALLER');
			}else if(typeof uids[i] !== 'number'){
				uids[i] = intval;
			};
		};
		
		if(uids.length === 0){
			api.errorHandler('EMPTY_UID_LIST');
		};
		
		return uids;
	};
	
	this.makeRequest = function(){
		var query = _function_argument("query");
		var path = _function_argument("path");
		var noBody = _avoid_nilb(_function_argument("noBody"), false);
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.request, {path: path, query: query, noBody: noBody, timeout: timeout, maxTime: maxTime})!
		var resp = _result_function();
		
		if(noBody){
			var indexStart = resp.trace.lastIndexOf(query);
			indexStart = resp.trace.indexOf('\r\n', indexStart) + 2;
			indexStart = resp.trace.indexOf(' ', indexStart) + 1;
			var indexEnd = resp.trace.indexOf('\r\n', indexStart);
			_function_return(resp.trace.slice(indexStart, indexEnd));
		}else{
			_function_return(resp.result);
		};
	};
	
	this.list = function(){
		var uid = _function_argument("uid");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var opts = {query: "LIST", timeout: timeout, maxTime: maxTime};
		if(uid){
			opts.path = uid;
			opts.noBody = true;
		};
		
		_call_function(api.makeRequest, opts)!
		var info = _result_function();
		
		var list = info.split("\r\n").map(function(ell){
			var temp = ell.split(' ');
			return {
				uid: parseInt(temp[0]),
				size: parseInt(temp[1])
			};
		});
		
		if(uid){
			_function_return(list[0]);
		}else{
			_function_return(list);
		};
	};
	
	this.stat = function(){
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest, {query: "STAT", noBody: true, timeout: timeout, maxTime: maxTime})!
		var info = _result_function();
		
		var temp = info.split(' ');
		
		_function_return({
			count: parseInt(temp[0]),
			size: parseInt(temp[1])
		});
	};
	
	this.retr = function(){
		var uid = _function_argument("uid");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest, {query: "RETR", path: uid, timeout: timeout, maxTime: maxTime})!
		var resp = _result_function();
		
		_function_return(resp);
	};
	
	this.top = function(){
		var uid = _function_argument("uid");
		var lines = _avoid_nilb(_function_argument("lines"), 0);
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest, {query: ("TOP " + uid + " " + lines), timeout: timeout, maxTime: maxTime})!
		var resp = _result_function();
		
		_function_return(resp);
	};
	
	this.dele = function(){
		var uid = _function_argument("uid");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest, {query: "DELE", path: uid, noBody: true, timeout: timeout, maxTime: maxTime})!
		var info = _result_function();
	};
	
	this.rset = function(){
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest, {query: "RSET", noBody: true, timeout: timeout, maxTime: maxTime})!
		var info = _result_function();
	};
	
	this.quit = function(){
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest, {query: "QUIT", noBody: true, timeout: timeout, maxTime: maxTime})!
		var info = _result_function();
	};
	
	this.status = function(){
		api.errorHandler('NOT_AVAILABLE_POP3', _K=="ru" ? 'Информация о папке' : 'Folder info');
	};
	
	this.getBoxes = function(){
		api.errorHandler('NOT_AVAILABLE_POP3', _K=="ru" ? 'Получить список папок' : 'Get list of folders');
	};
	
	this.addBox = function(){
		api.errorHandler('NOT_AVAILABLE_POP3', _K=="ru" ? 'Создать папку' : 'Create folder');
	};
	
	this.delBox = function(){
		api.errorHandler('NOT_AVAILABLE_POP3', _K=="ru" ? 'Удалить папку' : 'Delete folder');
	};
	
	this.renameBox = function(){
		api.errorHandler('NOT_AVAILABLE_POP3', _K=="ru" ? 'Переименовать папку' : 'Rename folder');
	};
	
	this.sort = function(){
		api.errorHandler('NOT_AVAILABLE_POP3', _K=="ru" ? 'Сортировка' : 'Sorting');
	};
	
	this.getFlags = function(){
		api.errorHandler('NOT_AVAILABLE_POP3', _K=="ru" ? 'Получить флаги' : 'Get flags');
	};
	
	this.setFlags = function(){
		api.errorHandler('NOT_AVAILABLE_POP3', _K=="ru" ? 'Установить флаги' : 'Set flags');
	};
	
	this.addFlags = function(){
		api.errorHandler('NOT_AVAILABLE_POP3', _K=="ru" ? 'Добавить флаги' : 'Add Flags');
	};
	
	this.delFlags = function(){
		api.errorHandler('NOT_AVAILABLE_POP3', _K=="ru" ? 'Удалить флаги' : 'Remove flags');
	};
	
	this.expunge = function(){
		api.errorHandler('NOT_AVAILABLE_POP3', 'Expunge');
	};
	
	this.copyMessages = function(){
		api.errorHandler('NOT_AVAILABLE_POP3', _K=="ru" ? 'Копировать письмо' : 'Copy message');
	};
	
	this.moveMessages = function(){
		api.errorHandler('NOT_AVAILABLE_POP3', _K=="ru" ? 'Переместить письмо' : 'Move message');
	};
	
	this.search = function(){
		api.validateCriteria(_function_argument("criteria"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.list, {timeout: timeout, maxTime: maxTime})!
		var list = _result_function();
		
		list = list.map(function(ell){return ell.uid});
		
		_function_return(list);
	};
	
	this.searchLast = function(){
		api.validateCriteria(_function_argument("criteria"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.list, {timeout: timeout, maxTime: maxTime})!
		var list = _result_function();
		
		var last = 0;
		
		if(list.length){
			last = list.pop().uid;
		};
		
		_function_return(last);
	};
	
	this.count = function(){
		api.validateCriteria(_function_argument("criteria"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.stat, {timeout: timeout, maxTime: maxTime})!
		var stat = _result_function();
		
		_function_return(stat.count || 0);
	};
	
	this.delMessages = function(){
		var uids = api.prepareUIDs(_function_argument("uids"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_do_with_params({uids: uids}, function(){
			var uid_index = _iterator() - 1;
			if(uid_index > _cycle_param("uids").length - 1){
				_break();
			};
			var uid = _cycle_param("uids")[uid_index];
			_call_function(api.dele, {uid: uid, timeout: timeout, maxTime: maxTime})!
		})!
		
		_call_function(api.quit, {timeout: timeout, maxTime: maxTime})!
	};
	
	this.parseHeaderParams = function(name, value){
		var items = value.split(';');
		value = items[0].trim();
		items = items.slice(1);
		var obj = { params: {} };
		var temp = [];
		
		if(name == "content-type"){
			temp = value.toLowerCase().split('/');
			obj.type = temp[0];
			obj.subtype = temp[1];
		}else if(name == "content-disposition"){
			obj.type = value.toLowerCase();
		}else{
			obj.value = value;
		};
		
		if(items.length){
			for(var i in items){
				var item = items[i].trim();
				if(item){
					temp = item.split('=');
					var key = temp[0].trim().toLowerCase();
					var val = temp.slice(1).join('=').trim();
					if(_starts_with(val, '"') && _ends_with(val, '"')){
						val = val.slice(1, -1);
					};
					obj.params[key] = val;
				};
			};
		};
		
		return obj;
	};
	
	this.parseMultipart = function(multipart, boundary, parts){
		parts = _avoid_nilb(parts, []);
		var state = 0;
		var lastline = '';
		var headers = '';
		var body = '';
		for(var i = 0; i < multipart.length; ++i){
			var oneChar = multipart.charAt(i);
			var prevChar = multipart.charAt(i - 1);
			var newLineDetected = oneChar === '\n' && prevChar === '\r';
			var newLineChar = oneChar === '\n' || oneChar === '\r';
			if(!newLineChar){
				lastline += oneChar;
			};
			if(0 === state && newLineDetected){
				if('--' + boundary === lastline){
					state = 1;
				};
				lastline = '';
			}else if(1 === state && newLineDetected){
				if(lastline === ''){
					state = 2;
				}else{
					if(headers.length){
						headers += '\r\n';
					};
					headers += lastline;
				};
				lastline = '';
			}else if(2 === state){
				if(lastline.length > boundary.length + 4){
					lastline = ''; // mem save
				};
				if('--' + boundary === lastline){
					body = body.slice(0, body.length - lastline.length - 1).trim();
					headers = api.parseHeader(headers.trim());
					if(headers["content-type"]){
						headers["content-type"] = api.parseHeaderParams('content-type', headers["content-type"]);
					};
					if(headers["content-disposition"]){
						headers["content-disposition"] = api.parseHeaderParams('content-disposition', headers["content-disposition"]);
					};
					if(headers["content-type"] && headers["content-type"].type == 'multipart'){
						api.parseMultipart(body, headers["content-type"].params.boundary, parts);
					}else{
						parts.push({
							headers: headers,
							body: body
						});
					};
					body = '';
					lastline = '';
					state = 3;
					headers = '';
				}else{
					body += oneChar;
				};
				if(newLineDetected){
					lastline = '';
				};
			}else if(3 === state){
				if(newLineDetected){
					state = 1;
				};
			};
		};
		return parts;
	};
	
	this.processHeaders = function(data, message, headers, date){
		var parsed = api.parseHeader(data.trim());
		for(var key in parsed){
			var value = parsed[key];
			if(headers.raw){
				message.headers.raw[key] = value;
			};
			if(headers.base && headers.data.indexOf(key) > -1){
				message.headers[key] = value;
			};
			if(date && key == "date"){
				message.date = new Date(value);
			};
		};
		return parsed["content-type"];
	};
	
	this.getMessages = function(){
		var uids = api.prepareUIDs(_function_argument("uids"));
		var body = api.getParamInfo(_function_argument("body"));
		var headers = api.getParamInfo(_function_argument("headers"));
		var attachments = _function_argument("attachments");
		var size = _avoid_nilb(_function_argument("size"), false);
		var date = _avoid_nilb(_function_argument("date"), false);
		var attachnames = _avoid_nilb(_function_argument("attachnames"), false);
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		if(_function_argument("flags")){
			api.errorHandler('NOT_AVAILABLE_POP3', _K=="ru" ? 'Получить флаги' : 'Get flags');
		};
		
		var messages = [];
		
		_do_with_params({uids:uids}, function(){
			var uid_index = _iterator() - 1;
			if(uid_index > _cycle_param("uids").length - 1){
				_break();
			};
			var uid = _cycle_param("uids")[uid_index];
			var message = {uid:uid};
			if(body.any){
				message.body = {};
			};
			if(body.data.indexOf('html') > -1){
				message.body.html = "";
			};
			if(body.data.indexOf('plain') > -1){
				message.body.plain = "";
			};
			if(headers.any){
				message.headers = {};
				if(headers.raw){
					message.headers.raw = {};
				};
			};
			if(attachnames){
				message.attachnames = [];
			};
			if(attachments){
				message.attachments = [];
			};
			messages.push(message);
			
			_if(size, function(){
				_call_function(api.list, {uid: uid, timeout: timeout, maxTime: maxTime})!
				var info = _result_function();
				
				message.size = info.size || 0;
			})!
			
			_if_else((headers.any || date) && !(body.any || attachnames || attachments), function(){
				_call_function(api.top, {uid: uid, timeout: timeout, maxTime: maxTime})!
				var resp = _result_function();
				
				api.processHeaders(resp, message, headers, date);
			}, function(){
				_call_function(api.retr, {uid: uid, timeout: timeout, maxTime: maxTime})!
				var resp = _result_function();
				
				var temp = resp.split('\r\n\r\n');
				var bodyContentType = api.processHeaders(temp[0], message, headers, date);
				var bodyData = temp.slice(1).join('\r\n\r\n').trim();
				delete temp;
				
				if(body.raw){
					message.body.raw = bodyData;
				};
				
				if(body.base || attachnames || attachments){
					bodyContentType = api.parseHeaderParams('content-type', bodyContentType);
					if(bodyContentType.type == 'multipart'){
						bodyData = api.parseMultipart(bodyData, bodyContentType.params.boundary);
						if(bodyData.length){
							if(body.base){
								for(var type_index in body.data){
									var type = body.data[type_index];
									for(var part_index in bodyData){
										var part = bodyData[part_index];
										if(part.headers["content-type"] && part.headers["content-type"].type === 'text' && part.headers["content-type"].subtype === type && !part.headers["content-disposition"]){
											var encoding = part.headers["content-transfer-encoding"].toLowerCase();
											var charset = (part.headers["content-type"].params.charset ? part.headers["content-type"].params.charset.toLowerCase() : '') || 'utf-8';
											message.body[type] += api.processPartData(part.body, encoding, charset);
										};
									};
									message.body[type] = message.body[type].trim();
								};
							};
							
							if(attachnames || attachments){
								for(var part_index in bodyData){
									var part = bodyData[part_index];
									if(part.headers["content-disposition"] && ['inline', 'attachment'].indexOf(part.headers["content-disposition"].type) > -1){
										var filename = '';
										if(part.headers["content-disposition"].params && part.headers["content-disposition"].params.filename){
											filename = part.headers["content-disposition"].params.filename;
										}else if(part.headers["content-type"].params && part.headers["content-type"].params.name){
											filename = part.headers["content-type"].params.name;
										};
										if(attachnames){
											message.attachnames.push(filename);
										};
										
										if(attachments){
											var saveThis = true;
											if(attachments !== true && attachments.toString().trim() !== '*'){
												if(!Array.isArray(attachments)){
													if(attachments instanceof RegExp){
														attachments = [attachments];
													}else{
														attachments = attachments.split(';');
													};
												};
												for(var mask_index in attachments){
													var mask = attachments[mask_index];
													var match = true;
													if(typeof mask === "string"){
														if(_starts_with(mask, '!')){
															match = false;
															mask = mask.slice(1);
														};
														mask = api.maskToRegExp(mask);
													};
													if(mask.test(filename) === match){
														saveThis = true;
														break;
													}else{
														saveThis = false;
													};
												};
											};
											if(saveThis){
												var randomFile = api.randStr() + '.file';
												var encoding = part.headers["content-transfer-encoding"].toLowerCase();
												var charset = (part.headers["content-type"].params.charset ? part.headers["content-type"].params.charset.toLowerCase() : '') || 'utf-8';
												api.processPartData(part.body, encoding, charset, randomFile);
												var randomFileDir = JSON.parse(native("filesystem", "fileinfo", randomFile)).directory;
												message.attachments.push({
													name: filename,
													type: part.headers["content-type"].type + '/' + part.headers["content-type"].subtype,
													path: randomFileDir + '/' + randomFile
												});
											};
										};
									};
								};
							};
						};
					};
				};
			})!
		})!
		
		_function_return(messages);
	};
});