_InMail = {
	api: null,
	proxy: null,
	debug: false,
	
	setDebug: function(enable){
		this.debug = [true, "true", 1].indexOf(enable) > -1;
	},
	
	log: function(enText, ruText){
		if(this.debug){
			ruText = _avoid_nilb(ruText, enText);
			
			_info('[InMail debug] ' + (_K==="en" ? enText : ruText));
		};
	},
	
	error: function(enText, ruText, act){
		ruText = _avoid_nilb(ruText, enText);
		act = this.prepareAct(act);
		
		fail(act + ': ' + (_K==="en" ? enText : ruText));
	},
	
	validateArgType: function(value, type, name, act){
		act = this.prepareAct(act);
		
		_validate_argument_type(value, type, name, act);
	},
	
	paramClean: function(str){
		return _avoid_nil(str).toString().trim();
	},
	
	configure: function(protocol, autoConfig, host, port, encrypt, username, password, box, connectTimeout, resetTimeout){
		var act = 'configure';
		this.validateArgType(protocol, 'string', 'Protocol', act);
		this.validateArgType(username, 'string', 'Username', act);
		this.validateArgType(password, 'string', 'Password', act);
		this.validateArgType(box, 'string', 'Folder name', act);
		this.validateArgType(connectTimeout, 'number', 'Connection timeout', act);
		this.validateArgType(resetTimeout, 'number', 'Reset timeout', act);
		
		protocol = this.paramClean(protocol).toLocaleLowerCase();
		if(["imap","pop3"].indexOf(protocol) < 0){
			this.error("Invalid protocol specified, mail module only supports imap and pop3 protocols", "Указан неверный протокол, почтовый модуль поддерживает только протоколы imap и pop3", act);
		};
		
		var config = {};
		
		if([true, "true", 1].indexOf(autoConfig) > -1){
			var split = username.split("@");
			var login = split[0];
			var domain = (split[1] || "").trim();
			
			if(!domain){
				this.error('Failed to configure ' + protocol + ' connection, the specified username does not contain a domain', 'Не удалось настроить подключение по ' + protocol + ', указанный логин не содержит домена', act);
			};
			
			config = this.findConfig(domain, protocol);
			
			if(!config){
				this.error('Failed to configure ' + protocol + ' connection for mail "' + domain + '", please use manual configuration', 'Не удалось настроить подключение по ' + protocol + ' для почты "' + domain + '", пожалуйста используйте ручную настройку', act);
			};
			
			config.host = config.host.replace("%domain%", domain);
			config.username = config.username.replace("%email%", username).replace("%login%", login).replace("%domain%", domain);
		}else{
			this.validateArgType(host, 'string', 'Server address', act);
			this.validateArgType(port, ['number','string'], 'Port', act);
			this.validateArgType(encrypt, 'string', 'Encryption', act);
			
			encrypt = this.paramClean(encrypt).toLocaleLowerCase();
			if(["none","ssl","starttls"].indexOf(encrypt) < 0){
				this.error('Invalid encryption type specified, mail module only supports SSL, STARTTLS and none', 'Указан неверный тип шифрования, почтовый модуль поддерживает только SSL, STARTTLS и none', act);
			};
			
			config.host = this.paramClean(host);
			port = this.paramClean(port).toLocaleLowerCase();
			config.port = port=="auto" ? (protocol=="imap" ? (encrypt=="ssl" ? 993 : 143) : encrypt=="ssl" ? 995 : 110) : Number(port);
			config.encrypt = encrypt;
			config.username = username;
		};

		config.password = password;
		
		var api = this.getApi(true);
		if(!api || protocol != api.protocol || JSON.stringify(config) != JSON.stringify(api.config)){
			try{
				this.api = new _InMail[protocol](config);
			}catch(err){
				die(act + ': ' + (_K==="en" ? ('Class of protocol ' + protocol + ' is corrupted or missing | Error: ') : ('Класс протокола ' + protocol + ' поврежден или отсутствует | Ошибка: ')) + err, true);
			};
		};
		
		this.api.box = _InMail.paramClean(box);
		this.api.resetTimeout = 1000 * (resetTimeout || 300);
		this.api.setConnectTimeout(1000 * (connectTimeout || 300));
		
		if(!_is_nilb(this.proxy) && typeof this.proxy==="object"){
			this.api.setProxy(this.proxy);
		};
	},
	
	getApi: function(noError){
		if(!_is_nilb(this.api) && typeof this.api==="object"){
			return this.api;
		}else{
			noError = _avoid_nil(noError, false);
			if(noError){
				return false;
			}else{
				this.error("Mail module configuration failed or incorrect", "Настройка почтового модуля не выполнена или выполнена неправильно");
			};
		};
	},
	
	setProxy: function(proxyString, type, username, password){
		if(proxyString){
			var act = 'setProxy';
			this.validateArgType(proxyString, 'string', 'Proxy', act);
			
			var proxyObj = proxy_parse(proxyString);
			
			if(!_is_nilb(type)){
				this.validateArgType(type, 'string', 'Proxy type', act);
				
				type = this.paramClean(type);
				if(!_is_nilb(type) && type != "auto"){
					proxyObj["IsHttp"] = _starts_with(type, "http");
				};
			};
			
			if(!_is_nilb(username) && !_is_nilb(password)){
				this.validateArgType(username, 'string', 'Proxy login', act);
				this.validateArgType(password, 'string', 'Proxy password', act);
				
				proxyObj["name"] = username;
				proxyObj["password"] = password;
			};
			
			this.proxy = {
				host: proxyObj["server"],
				port: proxyObj["Port"],
				type: proxyObj["IsHttp"] ? "http" : "socks5",
				username: proxyObj["name"],
				password: proxyObj["password"]
			};
			
			var api = this.getApi(true);
			if(api){
				api.setProxy(this.proxy);
			};
		}else{
			this.clearProxy();
		};
	},
	
	clearProxy: function(){
		this.proxy = null;
		var api = this.getApi(true);
		if(api){
			api.clearProxy();
		};
	},
	
	status: function(){
		var act = 'status';
		var name = _function_argument("name");
		_InMail.validateArgType(name, 'string', 'Folder name', act);
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var api = _InMail.getApi();
		api.setAction(act);
		
		_call_function(api.status, {name: name, timeout: timeout, maxTime: maxTime})!
		var info = _result_function();
		api.clearAction();
		
		_function_return(info);
	},
	
	getBoxes: function(){
		var act = 'getBoxes';
		var format = _function_argument("format");
		_InMail.validateArgType(format, 'string', 'Data format', act);
		format = _InMail.paramClean(format).toLocaleLowerCase();
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var api = _InMail.getApi();
		api.setAction(act);
		
		_call_function(api.getBoxes, {timeout: timeout, maxTime: maxTime})!
		var boxes = _result_function();
		api.clearAction();
		
		if(format != 'object'){
			boxes = _InMail.boxesToList(boxes);
			if(_starts_with(format, 'csv')){
				boxes = boxes.map(function(box){return csv_generate([box.name, csv_generate(box.attribs, ','), box.delimiter], ':')});
				if(format == 'csv string'){
					boxes = boxes.join('\n');
				};
			};
		};
		
		_function_return(boxes);
	},
	
	addBox: function(){
		var act = 'addBox';
		var name = _function_argument("name");
		_InMail.validateArgType(name, 'string', 'Folder name', act);
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var api = _InMail.getApi();
		api.setAction(act);
		
		_call_function(api.addBox, {name: name, timeout: timeout, maxTime: maxTime})!
		api.clearAction();
	},
	
	delBox: function(){
		var act = 'delBox';
		var name = _function_argument("name");
		_InMail.validateArgType(name, 'string', 'Folder name', act);
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var api = _InMail.getApi();
		api.setAction(act);
		
		_call_function(api.delBox, {name: name, timeout: timeout, maxTime: maxTime})!
		api.clearAction();
	},
	
	renameBox: function(){
		var act = 'renameBox';
		var oldName = _function_argument("oldName");
		_InMail.validateArgType(oldName, 'string', 'Old folder name', act);
		var newName = _function_argument("newName");
		_InMail.validateArgType(newName, 'string', 'New folder name', act);
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var api = _InMail.getApi();
		api.setAction(act);
		
		_call_function(api.renameBox, {oldName: oldName, newName: newName, timeout: timeout, maxTime: maxTime})!
		api.clearAction();
	},
	
	search: function(){
		var act = 'search';
		var criteria = _InMail.prepareCriteria(_function_argument("criteria"), act);
		var sorts = _InMail.prepareSorts(_function_argument("sorts"), act);
		var maxResults = _function_argument("maxResults");
		if(maxResults){
			_InMail.validateArgType(maxResults, 'number', 'Maximum number of results', act);
		};
		var offset = _function_argument("offset");
		if(offset){
			_InMail.validateArgType(offset, 'number', 'Offset', act);
		};
		var box = _InMail.prepareBox(_function_argument("box"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var api = _InMail.getApi();
		
		_if_else(sorts, function(){
			api.setAction('sort');
			
			_call_function(api.sort, {sorts: sorts, criteria: criteria, box: box, timeout: timeout, maxTime: maxTime})!
		}, function(){
			api.setAction(act);
			
			_call_function(api.search, {criteria: criteria, box: box, timeout: timeout, maxTime: maxTime})!
		})!
		
		var res = _result_function();
		api.clearAction();
		
		if(res.length){
			if(maxResults){
				res = res.slice(0, maxResults);
			};
			
			if(offset){
				res = res.map(function(uid){return uid + offset});
			};
			
			_function_return(res);
		}else{
			_function_return([]);
		};
	},
	
	wait: function(){
		var act = 'wait';
		var criteria = _function_argument("criteria");
		var sorts = _function_argument("sorts");
		var minResults = _avoid_nilb(_function_argument("minResults"), 1);
		_InMail.validateArgType(minResults, 'number', 'Number of messages', act);
		var interval = _avoid_nilb(_function_argument("interval"), 5000);
		_InMail.validateArgType(interval, 'number', 'Interval', act);
		var timeout = _avoid_nilb(_function_argument("timeout"), 300000);
		_InMail.validateArgType(timeout, 'number', 'Timeout', act);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		var maxResults = _function_argument("maxResults");
		if(maxResults){
			_InMail.validateArgType(maxResults, 'number', 'Maximum number of results', act);
		};
		var offset = _function_argument("offset");
		if(offset){
			_InMail.validateArgType(offset, 'number', 'Offset', act);
		};
		var box = _function_argument("box");
		
		var api = _InMail.getApi();
		
		var res = [];
		
		_do(function(i){
			if(Date.now() > maxTime){
				_InMail.error('Failed to wait for the required number of messages matching the specified criteria in the specified mailbox folder', 'Не удалось дождаться нужного количества писем, соответствующих указанным критериям, в указанной папке почтового ящика', 'wait');
			};
			
			if(i && i % 3 === 0){
				api.reset();
			};
			
			_call_function(_InMail.search, {criteria:criteria, sorts:sorts, box:box, timeout: timeout, maxTime: maxTime})!
			res = _result_function();
			
			if(res.length >= minResults){
				_break();
			};
			
			sleep(interval)!
		})!
		
		if(maxResults){
			res = res.slice(0, maxResults);
		};
		
		if(offset){
			res = res.map(function(uid){return uid + offset});
		};
		
		_function_return(res);
	},
	
	searchLast: function(){
		var act = 'searchLast';
		var criteria = _InMail.prepareCriteria(_function_argument("criteria"), act);
		var box = _InMail.prepareBox(_function_argument("box"));
		var errorNotFound = _avoid_nilb(_function_argument("errorNotFound"), true);
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var api = _InMail.getApi();
		api.setAction(act);
		
		_call_function(api.searchLast, {criteria: criteria, box: box, timeout: timeout, maxTime: maxTime})!
		var last = _result_function();
		api.clearAction();
		
		if(last){
			_function_return(last);
		}else{
			if(errorNotFound){
				_InMail.error('Could not find the last message in the specified mailbox folder', 'Не удалось найти последнее письмо в указанной папке почтового ящика', act);
			}else{
				_function_return(0);
			};
		};
	},
	
	searchOne: function(){
		var act = 'searchOne';
		var criteria = _InMail.prepareCriteria(_function_argument("criteria"), act);
		var sorts = _InMail.prepareSorts(_function_argument("sorts"), act);
		var box = _InMail.prepareBox(_function_argument("box"));
		var errorNotFound = _avoid_nilb(_function_argument("errorNotFound"), true);
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var api = _InMail.getApi();
		
		var one = 0;
		
		_if_else(sorts, function(){
			api.setAction('sort');
			
			_call_function(api.sort, {sorts: sorts, criteria: criteria, box: box, timeout: timeout, maxTime: maxTime})!
			one = _result_function().pop() || 0;
			api.clearAction();
		}, function(){
			api.setAction('searchLast');
			
			_call_function(api.searchLast, {criteria: criteria, box: box, timeout: timeout, maxTime: maxTime})!
			one = _result_function();
			api.clearAction();
		})!
		
		if(one){
			_function_return(one);
		}else{
			if(errorNotFound){
				_InMail.error('Could not find any messages matching the specified criteria in the specified mailbox folder', 'Не удалось найти ни одного письма, соответствующего указанным критериям, в указанной папке почтового ящика', act);
			}else{
				_function_return(0);
			};
		};
	},
	
	waitOne: function(){
		var args = _function_arguments();
		
		_call_function(_InMail.wait, args)!
		var res = _result_function();
		
		_function_return(res.pop());
	},
	
	count: function(){
		var act = 'count';
		var criteria = _InMail.prepareCriteria(_function_argument("criteria"), act);
		var box = _InMail.prepareBox(_function_argument("box"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var api = _InMail.getApi();
		api.setAction(act);
		
		_call_function(api.count, {criteria: criteria, box: box, timeout: timeout, maxTime: maxTime})!
		var count = _result_function();
		api.clearAction();
		
		_function_return(count);
	},
	
	getFlags: function(){
		var uid = _InMail.uidsToOne(_function_argument("uid"));
		var box = _InMail.prepareBox(_function_argument("box"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var api = _InMail.getApi();
		api.setAction('getFlags');
		
		_call_function(api.getFlags, {uid: uid, box: box, timeout: timeout, maxTime: maxTime})!
		var flags = _result_function();
		api.clearAction();
		
		_function_return(flags);
	},
	
	addFlags: function(){
		var uids = _function_argument("uids");
		var flags = _to_arr(_function_argument("flags"));
		var box = _InMail.prepareBox(_function_argument("box"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var api = _InMail.getApi();
		api.setAction('addFlags');
		
		_call_function(api.addFlags, {uids: uids, flags: flags, box: box, timeout: timeout, maxTime: maxTime})!
		api.clearAction();
	},
	
	delFlags: function(){
		var uids = _function_argument("uids");
		var flags = _to_arr(_function_argument("flags"));
		var box = _InMail.prepareBox(_function_argument("box"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var api = _InMail.getApi();
		api.setAction('delFlags');
		
		_call_function(api.delFlags, {uids: uids, flags: flags, box: box, timeout: timeout, maxTime: maxTime})!
		api.clearAction();
	},
	
	setFlags: function(){
		var uids = _function_argument("uids");
		var flags = _to_arr(_function_argument("flags"));
		var box = _InMail.prepareBox(_function_argument("box"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var api = _InMail.getApi();
		api.setAction('setFlags');
		
		_call_function(api.setFlags, {uids: uids, flags: flags, box: box, timeout: timeout, maxTime: maxTime})!
		api.clearAction();
	},
	
	expunge: function(){
		var uids = _function_argument("uids");
		var box = _InMail.prepareBox(_function_argument("box"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var api = _InMail.getApi();
		api.setAction('expunge');
		
		_call_function(api.expunge, {uids: uids, box: box, timeout: timeout, maxTime: maxTime})!
		api.clearAction();
	},
	
	delMessages: function(){
		var uids = _function_argument("uids");
		var box = _InMail.prepareBox(_function_argument("box"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var api = _InMail.getApi();
		api.setAction('delMessages');
		
		_call_function(api.delMessages, {uids: uids, box: box, timeout: timeout, maxTime: maxTime})!
		api.clearAction();
	},
	
	copyMessages: function(){
		var uids = _function_argument("uids");
		var toBox = _InMail.paramClean(_function_argument("toBox"));
		var box = _InMail.prepareBox(_function_argument("box"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var api = _InMail.getApi();
		api.setAction('copyMessages');
		
		_call_function(api.copyMessages, {uids: uids, toBox: toBox, box: box, timeout: timeout, maxTime: maxTime})!
		api.clearAction();
	},
	
	moveMessages: function(){
		var uids = _function_argument("uids");
		var toBox = _InMail.paramClean(_function_argument("toBox"));
		var box = _InMail.prepareBox(_function_argument("box"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var api = _InMail.getApi();
		api.setAction('moveMessages');
		
		_call_function(api.moveMessages, {uids: uids, toBox: toBox, box: box, timeout: timeout, maxTime: maxTime})!
		api.clearAction();
	},
	
	getMessages: function(){
		var uids = _function_argument("uids");
		var body = _avoid_nilb(_function_argument("body"), true);
		var headers = _avoid_nilb(_function_argument("headers"), false);
		var size = _avoid_nilb(_function_argument("size"), false);
		var flags = _avoid_nilb(_function_argument("flags"), false);
		var date = _avoid_nilb(_function_argument("date"), false);
		var attachnames = _avoid_nilb(_function_argument("attachnames"), false);
		var attachments = _avoid_nilb(_function_argument("attachments"), false);
		var markSeen = _avoid_nilb(_function_argument("markSeen"), false);
		var box = _InMail.prepareBox(_function_argument("box"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var api = _InMail.getApi();
		api.setAction('getMessages');
		
		_call_function(api.getMessages, {uids: uids, body: body, headers: headers, size: size, flags: flags, date: date, attachnames: attachnames, attachments: attachments, markSeen: markSeen, box: box, timeout: timeout, maxTime: maxTime})!
		var messages = _result_function();
		api.clearAction();
		
		_function_return(messages);
	},
	
	getMessage: function(){
		var args = _function_arguments();
		args.uids = _InMail.uidsToOne(args.uid);
		delete args.uid;
		
		_call_function(_InMail.getMessages, args)!
		var messages = _result_function();
		
		if(!messages.length){
			_InMail.error('Could not find a message matching the specified id in the specified mailbox folder', 'Не удалось найти письмо, соответствующее указанному идентификатору, в указанной папке почтового ящика', 'getMessages');
		};
		
		_function_return(messages[0]);
	},
	
	getLastMessage: function(){
		var args = _function_arguments();
		args.timeout = _avoid_nilb(args.timeout, 60000);
		args.maxTime = _avoid_nilb(args.maxTime, Date.now() + args.timeout);
		
		_call_function(_InMail.searchLast, {box: args.box, timeout: args.timeout, maxTime: args.maxTime})!
		var uid = _result_function();
		
		args.uid = uid;
		
		_call_function(_InMail.getMessage, args)!
		var message = _result_function();
		
		_function_return(message);
	},
	
	findMessage: function(){
		var args = _function_arguments();
		args.timeout = _avoid_nilb(args.timeout, 60000);
		args.maxTime = _avoid_nilb(args.maxTime, Date.now() + args.timeout);
		
		_call_function(_InMail.searchOne, args)!
		var uid = _result_function();
		
		args.uid = uid;
		
		_call_function(_InMail.getMessage, args)!
		var message = _result_function();
		
		_function_return(message);
	},
	
	waitMessage: function(){
		var args = _function_arguments();
		args.timeout = _avoid_nilb(args.timeout, 60000);
		args.maxTime = _avoid_nilb(args.maxTime, Date.now() + args.timeout);
		
		_call_function(_InMail.waitOne, args)!
		var uid = _result_function();
		
		args.uid = uid;
		
		_call_function(_InMail.getMessage, args)!
		var message = _result_function();
		
		_function_return(message);
	},
	
	uidsToOne: function(uids){
		if(typeof uids == "string" && uids.indexOf(',') > 0){
			uids.split(',');
		};
		
		if(Array.isArray(uids)){
			uids = uids[0];
		};
		return uids;
	},
	
	boxesToList: function(boxes, parent){
		var api = this.getApi();
		var list = [];
		
		for(var name in boxes){
			var box = boxes[name];
			
			if(box.isChildren = !!parent){
				name = (parent.name + parent.delimiter) + name;
				parent.children.push(name);
				box.parent = parent.name;
			}else{
				box.parent = null;
			};
			
			box.name = name;
			
			list.push(box);
			
			if(box.children){
				var children = box.children;
				box.children = [];
				children = this.boxesToList(children, box);
				list = list.concat(children);
			};
		};
		
		return list;
	},
	
	prepareAct: function(act){
		act = _avoid_nil(act, '_InMail');
		
		if(!_starts_with(act, '_InMail')){
			act = '_InMail.' + act;
		};
		
		return act;
	},
	
	prepareBox: function(box){
		return box ? this.paramClean(box) : box;
	},
	
	prepareCriteria: function(criteria, act){
		if(!_is_nilb(criteria)){
			_InMail.validateArgType(criteria, ['array','string','object'], 'Search criteria', act);
			
			if(Array.isArray(criteria)){
				return criteria;
			}else{
				if(typeof criteria == 'string'){
					if(criteria.trim().length){
						return [criteria];
					};
				}else{
					var keys = Object.keys(criteria).filter(function(key){
						if(!_is_nilb(key)){
							var value = criteria[key];
							if(!_is_nilb(value)){
								return (typeof value != "string" || !!value.trim().length);
							};
						};
						return false;
					});
					
					if(keys.length){
						var arr = [];
						for(var i = 0; i < keys.length; ++i){
							var key = keys[i];
							var keyLow = key.toLocaleLowerCase();
							var value = criteria[key];
							if(['flags', '!flags'].indexOf(keyLow) > -1){
								var flags = _to_arr(value).map(function(flag){return flag.slice(0, 1) == '\\' ? flag.slice(1) : flag});
								if(keyLow=='!flags'){
									flags = flags.map(function(flag){return flag.slice(0, 1) != '!' ? '!' + flag : flag});
								};
								arr = arr.concat(flags);
							}else{
								arr.push([key, value]);
							};
						};
						return arr;
					};
				};
			};
		};
		
		return ['ALL'];
	},
	
	prepareSorts: function(sorts, act){
		if(!_is_nilb(sorts)){
			_InMail.validateArgType(sorts, ['array','string','object'], 'Sorting criteria', act);
			
			if(Array.isArray(sorts)){
				return sorts;
			}else{
				if(typeof sorts == 'string'){
					sorts = sorts.trim();
					if(sorts.length){
						return [sorts];
					};
				}else{
					var type = sorts.type.trim();
					var field = sorts.field.trim();
					if(!_is_nilb(type) && type != 'no sorting' && !_is_nilb(field)){
						return [(type=='descending' ? '-' : '') + field];
					};
				};
			};
		};
		
		return false;
	}
};