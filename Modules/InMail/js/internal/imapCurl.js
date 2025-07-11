_InMail.imap = _InMail.assignApi(function(config){
	const api = this;
	_InMail.baseApi.call(this, true, "imap", config);
	
	this.caps = null;
	this.months = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'];
	this.specialAttrs = ['\\All', '\\Archive', '\\Drafts', '\\Flagged', '\\Important', '\\Junk', '\\Sent', '\\Trash'];

	this.escape = function(str){
		return str.replace(/\\/g, '\\\\').replace(/"/g, '\\"');
	};
	
	// RFC 4648, section 4 Base64 encoding/decoding.
	this.base64 = {
		table: "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=",
		encode: function(str){
			var padding = str.length % 3;
			var ret = '';
			var position = -1;
			var a = undefined;
			var b = undefined;
			var c = undefined;
			var buffer = undefined;
			// Make sure any padding is handled outside of the loop.
			var length = str.length - padding;
			
			while(++position < length){
				// Read three bytes, i.e. 24 bits.
				a = str.charCodeAt(position) << 16;
				b = str.charCodeAt(++position) << 8;
				c = str.charCodeAt(++position);
				buffer = a + b + c;
				// Turn the 24 bits into four chunks of 6 bits each, and append the
				// matching character for each of them to the ret.
				ret += (this.table.charAt(buffer >> 18 & 0x3F) + this.table.charAt(buffer >> 12 & 0x3F) + this.table.charAt(buffer >> 6 & 0x3F) + this.table.charAt(buffer & 0x3F));
			};
			
			if(padding == 2){
				a = str.charCodeAt(position) << 8;
				b = str.charCodeAt(++position);
				buffer = a + b;
				ret += (this.table.charAt(buffer >> 10) + this.table.charAt((buffer >> 4) & 0x3F) + this.table.charAt((buffer << 2) & 0x3F) + '=');
			}else if(padding == 1){
				buffer = str.charCodeAt(position);
				ret += (this.table.charAt(buffer >> 2) + this.table.charAt((buffer << 4) & 0x3F) + '==');
			};
			
			return ret;
		},
		decode: function(str){
			str = str.replace(/==?$/, '');
			var length = str.length;
			var bitCounter = 0;
			var bitStorage = undefined;
			var buffer = undefined;
			var ret = '';
			var position = -1;
			while(++position < length){
				buffer = this.table.indexOf(str.charAt(position));
				bitStorage = bitCounter % 4 ? bitStorage * 64 + buffer : buffer;
				// Unless this is the first of a group of 4 characters…
				if(bitCounter++ % 4){
					// …convert the first 8 bits to a single ASCII character.
					ret += String.fromCharCode(0xFF & bitStorage >> (-2 * bitCounter & 6));
				};
			};
			return ret;
		}
	};
	
	// RFC 3501, section 5.1.3 UTF-7 encoding/decoding.
	this.utf7 = {
		encode: function(str){
			// All printable ASCII chars except for & must be represented by themselves.
			// We replace subsequent non-representable chars with their escape sequence.
			return str.replace(/&/g, '&-').replace(/[^\x20-\x7e]+/g, function(chunk){
				// & is represented by an empty sequence &-, otherwise the encoding is performed.
				if(chunk === '&'){
					chunk = '';
				}else{
					var b = '';
					for(var i = 0; i < chunk.length; ++i){
						// Note that we can't simply convert a UTF-8 string to Base64 because
						// UTF-8 uses a different encoding. In modified UTF-7, all characters
						// are represented by their two byte Unicode ID.
						var c = chunk.charCodeAt(i);
						// Upper 8 bits shifted into lower 8 bits so that they fit into 1 byte.
						b += String.fromCharCode(c >> 8);
						// Lower 8 bits. Cut off the upper 8 bits so that they fit into 1 byte.
						b += String.fromCharCode(c & 0xFF);
					};
					// Modified Base64 uses , instead of / and omits trailing =.
					chunk = api.base64.encode(b).replace(/=+$/, '');
					
					chunk = chunk.replace(new RegExp('\\/', 'g'), ',');
				};
				return '&' + chunk + '-';
			});
		},
		
		decode: function(str){
			return str.replace(/&([^-]*)-/g, function(_, chunk){
				// &- represents &.
				if(chunk === ''){
					return '&';
				};
				var b = api.base64.decode(chunk.replace(/,/g, '/'));
				var ret = '';
				for(var i = 0; i < b.length;){
					// Calculate charcode from two adjacent bytes.
					ret += (String.fromCharCode(b.charCodeAt(i++) << 8 | b.charCodeAt(i++)));
				};
				return ret;
			});
		}
	};
	
	this.encodeName = function(name){
		return api.escape(api.utf7.encode('' + name));
	};
	
	this.encodeNameUrl = function(name){
		return encodeURIComponent(api.utf7.encode('' + name));
	};
	
	this.parseCaps = function(str){
		var start = str.lastIndexOf('CAPABILITY');
		if(start > -1){
			var end = start;
			for(; end < str.length; ++end){
				var code = str.charCodeAt(end);
				if(code === 10 || code === 13 || code === 93){
					break;
				};
			};
			if(end != start){
				return str.slice(start, end).split(' ').slice(1).filter(function(cap){
					return !_is_nilb(cap);
				}).map(function(cap){
					return cap.toUpperCase();
				});
			};
		};
		return [];
	};
	
	this.capability = function(){
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_if(_is_nilb(api.caps), function(){
			_call_function(api.request, {query: 'CAPABILITY', timeout: timeout, maxTime: maxTime})!
			var resp = _result_function();
			
			api.caps = api.parseCaps(resp.result);
		})!;
		
		_function_return(api.caps);
	};
	
	this.serverSupports = function(cap){
		return (api.caps && api.caps.indexOf(cap) > -1);
	};
	
	this.prepareBox = function(box, allowBlank){
		box = allowBlank ? _avoid_nil(box, api.box) : _avoid_nilb(box, api.box);
		
		api.validateArgType(box, 'string', 'Folder name');
		if(!allowBlank && !box.length){
			api.errorHandler('MAILBOX_NOT_SELECTED');
		};
		
		return box;
	};
	
	this.parseExpr = function(o, result, start, useBrackets){
		result = _avoid_nilb(result, []);
		start = _avoid_nilb(start, 0);
		useBrackets = _avoid_nilb(useBrackets, true);
		var inQuote = false;
		var lastPos = start - 1;
		var isTop = false;
		var isBody = false;
		var escaping = false;
		var val = undefined;
		
		if(typeof o === 'string'){
			o = {str: o};
			isTop = true;
		};
		
		for(var i = start, len = o.str.length; i < len; ++i){
			if(!inQuote){
				if(isBody){
					if (o.str[i] === ']') {
						val = api.convStr(o.str.substring(lastPos + 1, i + 1));
						result.push(val);
						lastPos = i;
						isBody = false;
					};
				}else if(o.str[i] === '"'){
					inQuote = true;
				}else if (o.str[i] === ' ' || o.str[i] === ')' || (useBrackets && o.str[i] === ']')){
					if(i - (lastPos + 1) > 0){
						val = api.convStr(o.str.substring(lastPos + 1, i));
						result.push(val);
					};
					if((o.str[i] === ')' || (useBrackets && o.str[i] === ']')) && !isTop){
						return i;
					};
					lastPos = i;
				}else if((o.str[i] === '(' || (useBrackets && o.str[i] === '['))){
					if(o.str[i] === '[' && i - 4 >= start && o.str.substring(i - 4, i).toUpperCase() === 'BODY'){
						isBody = true;
						lastPos = i - 5;
					}else{
						var innerResult = [];
						i = api.parseExpr(o, innerResult, i + 1, useBrackets);
						lastPos = i;
						result.push(innerResult);
					};
				};
			}else if(o.str[i] === '\\'){
				escaping = !escaping;
			}else if(o.str[i] === '"'){
				if(!escaping){
					inQuote = false;
				};
				escaping = false;
			};
			if(i + 1 === len && len - (lastPos + 1) > 0){
				result.push(api.convStr(o.str.substring(lastPos + 1)));
			};
		};
		
		return (isTop ? result : start);
	};

	this.convStr = function(str){
		if(str[0] === '"'){
			str = str.substring(1, str.length - 1);
			var newstr = '';
			var isEscaping = false;
			var p = 0;
			for(var i = 0, len = str.length; i < len; ++i){
				if(str[i] === '\\'){
					if(!isEscaping){
						isEscaping = true;
					}else{
						isEscaping = false;
						newstr += str.substring(p, i - 1);
						p = i;
					};
				}else if(str[i] === '"'){
					if(isEscaping){
						isEscaping = false;
						newstr += str.substring(p, i - 1);
						p = i;
					};
				};
			};
			if(p === 0){
				return str;
			}else{
				newstr += str.substring(p);
				return newstr;
			};
		}else if(str === 'NIL'){
			return null;
		}else if(/^\d+$/.test(str)){
			// some IMAP extensions utilize large (64-bit) integers, which JavaScript
			// can't handle natively, so we'll just keep it as a string if it's too big
			var val = parseInt(str, 10);
			return (val.toString() === str ? val : str);
		};

		return str;
	};
	
	this.delUpdateData = function(str){
		str = str.replace(/\* \d+ EXISTS\r?\n?/g, '').replace(/\* \d+ RECENT\r?\n?/g, '');
		return str;
	};
	
	this.parse = function(str, expectedType){
		str = api.delUpdateData(str).trim(); //Delete update data when mailbox changes
		if(str.indexOf('\n') > -1){ //Delete other data not related to the current request
			var list = str.split(/\r?\n/);
			if(!_is_nilb(expectedType)){
				str = list[0];
			}else{
				str = list.filter(function(ell){return _starts_with(ell.toLowerCase(), '* ' + expectedType)})[0] || list[0];
			};
		};
		
		var m = /^\* (?:(OK|NO|BAD|BYE|FLAGS|ID|LIST|XLIST|LSUB|SEARCH|STATUS|CAPABILITY|NAMESPACE|PREAUTH|SORT|THREAD|ESEARCH|QUOTA|QUOTAROOT)|(\d+) (EXPUNGE|FETCH|RECENT|EXISTS))(?:(?: \[([^\]]+)\])?(?: (.+))?)?$/i.exec(str);
		
		if(!m){
			api.errorHandler('FAILED_PARSE', str);
		};
		
		return {
			type: (m[1] || m[3]).toLowerCase(),
			num: parseInt(m[2], 10),
			text: m[5]
		};	
	};
	
	this.parseSearch = function(str){
		var info = api.parse(str, 'search');
		if(info.text){
			var regSearchModseq = /^(.+) \(MODSEQ (.+?)\)$/i;
			if(info.type === 'search' && regSearchModseq.test(info.text)){
				// CONDSTORE search response
				var p = regSearchModseq.exec(info.text);
				return p[1].split(' ');
			}else{
				var val = [];
				if(info.text[0] === '('){
					val = /^\((.*)\)$/.exec(info.text)[1].split(' ');
				}else{
					val = info.text.split(' ');
				};
				
				if(['search','sort'].indexOf(info.type) > -1){
					val = val.map(function(v){return parseInt(v, 10)});
				};
				if(info.type == 'capability'){
					val = val.map(function(v){return v.toUpperCase()});
				};
				return val;
			};
		}else{
			return [];
		};
	};

	this.parseBodyStructure = function(cur, prefix, partID){
		var ret = [], i, len;
		if(typeof prefix == "undefined"){
			var result = (Array.isArray(cur) ? cur : api.parseExpr(cur));
			if(result.length){
				ret = api.parseBodyStructure(result, '', 1);
			};
		}else{
			var part, partLen = cur.length, next;
			if(Array.isArray(cur[0])){ // multipart
				next = -1;
				while(Array.isArray(cur[++next])){
					ret.push(api.parseBodyStructure(cur[next], (prefix + (prefix !== '' ? '.' : '') + (partID++).toString()), 1));
				};
				part = {
					type: cur[next++].toLowerCase()
				};
				if(partLen > next){
					if(Array.isArray(cur[next])){
						part.params = {};
						for(i = 0, len = cur[next].length; i < len; i += 2){
							part.params[cur[next][i].toLowerCase()] = api.decodeWords(cur[next][i + 1]);
						};
					}else{
						part.params = cur[next];
					};
					++next;
				};
			}else{ // single part
				next = 7;
				if(typeof cur[1] === 'string'){
					part = {
						// the path id for this part, useful for fetching specific
						// parts of a message
						partID: (prefix !== '' ? prefix : '1'),
						// required fields as per RFC 3501 -- null or otherwise
						type: cur[0].toLowerCase(),
						subtype: cur[1].toLowerCase(),
						params: null,
						id: cur[3],
						description: cur[4],
						encoding: cur[5],
						size: cur[6]
					};
				}else{
					// type information for malformed multipart body
					part = {
						type: cur[0] ? cur[0].toLowerCase() : null,
						params: null 
					};
					cur.splice(1, 0, null);
					++partLen;
					next = 2;
				};
				if(Array.isArray(cur[2])){
					part.params = {};
					for(i = 0, len = cur[2].length; i < len; i += 2){
						part.params[cur[2][i].toLowerCase()] = api.decodeWords(cur[2][i + 1]);
					};
					if(cur[1] === null){
						++next;
					};
				};
				if(part.type === 'message' && part.subtype === 'rfc822'){
					// envelope
					if(partLen > next && Array.isArray(cur[next])){
						part.envelope = api.parseFetchEnvelope(cur[next]);
					}else{
						part.envelope = null;
					};
					++next;
					
					// body
					if(partLen > next && Array.isArray(cur[next])){
						part.body = api.parseBodyStructure(cur[next], prefix, 1);
					}else{
						part.body = null;
					};
					++next;
				};
				if((part.type === 'text' || (part.type === 'message' && part.subtype === 'rfc822')) && partLen > next){
					part.lines = cur[next++];
				};
				if(typeof cur[1] === 'string' && partLen > next){
					part.md5 = cur[next++];
				};
			};
			// add any extra fields that may or may not be omitted entirely
			api.parseStructExtra(part, partLen, cur, next);
			ret.unshift(part);
		};
		return ret;
	};

	this.parseStructExtra = function(part, partLen, cur, next){
		if(partLen > next){
			// disposition
			// null or a special k/v list with these kinds of values:
			// e.g.: ['Foo', null]
			//		 ['Foo', ['Bar', 'Baz']]
			//		 ['Foo', ['Bar', 'Baz', 'Bam', 'Pow']]
			var disposition = {
				type: null,
				params: null 
			};
			if(Array.isArray(cur[next])){
				disposition.type = cur[next][0];
				if(Array.isArray(cur[next][1])){
					disposition.params = {};
					for(var i = 0, len = cur[next][1].length, key; i < len; i += 2){
						key = cur[next][1][i].toLowerCase();
						disposition.params[key] = api.decodeWords(cur[next][1][i + 1]);
					};
				};
			}else if(cur[next] !== null){
				disposition.type = cur[next];
			};
			if(disposition.type === null){
				part.disposition = null;
			}else{
				part.disposition = disposition;
			};
			
			++next;
		};
		if(partLen > next){
			// language can be a string or a list of one or more strings, so let's
			// make this more consistent ...
			if(cur[next] !== null){
				part.language = (Array.isArray(cur[next]) ? cur[next] : [cur[next]]);
			}else{
				part.language = null;
			};
			++next;
		};
		if(partLen > next){
			part.location = cur[next++];
		};
		if(partLen > next){
			// extension stuff introduced by later RFCs
			// this can really be any value: a string, number, or (un)nested list
			// let's not parse it for now ...
			part.extensions = cur[next];
		};
	};

	this.parseFetchEnvelope = function(list){
		return {
			date: new Date(list[0]),
			subject: api.decodeWords(list[1]),
			from: api.parseEnvelopeAddresses(list[2]),
			sender: api.parseEnvelopeAddresses(list[3]),
			replyTo: api.parseEnvelopeAddresses(list[4]),
			to: api.parseEnvelopeAddresses(list[5]),
			cc: api.parseEnvelopeAddresses(list[6]),
			bcc: api.parseEnvelopeAddresses(list[7]),
			inReplyTo: list[8],
			messageId: list[9]
		};
	};

	this.parseEnvelopeAddresses = function(list){
		var addresses = null;
		if(Array.isArray(list)){
			addresses = [];
			var inGroup = false, curGroup;
			for(var i = 0, len = list.length, addr; i < len; ++i){
				addr = list[i];
				if(addr[2] === null){ // end of group addresses
					inGroup = false;
					if(curGroup){
						addresses.push(curGroup);
						curGroup = undefined;
					};
				}else if(addr[3] === null){ // start of group addresses
					inGroup = true;
					curGroup = {
						group: addr[2],
						addresses: []
					};
				}else{ // regular user address
					var info = {
						name: api.decodeWords(addr[0]),
						mailbox: addr[2],
						host: addr[3]
					};
					if(inGroup){
						curGroup.addresses.push(info);
					}else if(!inGroup){
						addresses.push(info);
					};
				};
				list[i] = addr;
			};
			if(inGroup){
				// no end of group found, assume implicit end
				addresses.push(curGroup);
			};
		};
		return addresses;
	};
	
	this.makeRequest = function(){
		var query = _function_argument("query");
		var isUTF8 = _avoid_nilb(_function_argument("isUTF8"), false);
		var box = api.prepareBox(_function_argument("box"), true);
		var path = api.encodeNameUrl(box);
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_if(isUTF8, function(){
			_call_function(api.capability, {timeout: timeout, maxTime: maxTime})!
			
			_if(api.serverSupports('ENABLE'), function(){
				_call_function(api.request, {query: 'ENABLE UTF8=ACCEPT', timeout: timeout, maxTime: maxTime})!
			})!			
		})!
		
		_call_function(api.request, {query: query, path: path, timeout: timeout, maxTime: maxTime})!
		var resp = _result_function();
		
		try{
			
			if(_is_nilb(api.caps)){
				var caps = api.parseCaps(resp.trace);
				if(caps.length){
					api.caps = caps;
				};
			};
		}catch(_){}
		
		_function_return(resp.result);
	};

	this.validateUIDList = function(uids, noError){
		for(var i = 0, len = uids.length, intval; i < len; ++i){
			if(typeof uids[i] === 'string'){
				if(uids[i] === '*' || uids[i] === '*:*'){
					if (len > 1){
						uids = ['*'];
					};
					break;
				}else if(/^(?:[\d]+|\*):(?:[\d]+|\*)$/.test(uids[i])){
					continue;
				};
			};
			
			intval = parseInt('' + uids[i], 10);
			
			if(isNaN(intval)){
				var err = 'WRONG_FORMAT_UID';
				if(noError){
					return err;
				}else{
					api.errorHandler(err, uids[i]);
				};
			}else if (intval <= 0){
				var err = 'UID_IS_SMALLER';
				if(noError){
					return err;
				}else{
					api.errorHandler(err);
				};
			}else if(typeof uids[i] !== 'number'){
				uids[i] = intval;
			};
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
		
		api.validateUIDList(uids);
		
		if(uids.length === 0){
			api.errorHandler('EMPTY_UID_LIST');
		};
		
		return uids.join(',');
	};
	
	this.prepareCriteria = function(criteria){
		api.validateArgType(criteria, ['array','string'], 'Search criteria');
		if(typeof criteria === 'string'){
			criteria = [criteria];
		};
		return criteria;
	};
	
	this.status = function(){
		var name = _function_argument("name");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var info = ['MESSAGES', 'RECENT', 'UNSEEN'];
		
		info = info.join(' ');
		
		var cmd = 'STATUS "' + api.encodeName(name) + '" (' + info + ')';
		
		_call_function(api.makeRequest, {query: cmd, box: "", timeout: timeout, maxTime: maxTime})!
		var resp = _result_function();
		
		var info = api.parse(resp, 'status');
		
		var r = api.parseExpr(info.text);
		var attrs = {};
		// r[1] is [KEY1, VAL1, KEY2, VAL2, .... KEYn, VALn]
		for(var j = 0, len = r[1].length; j < len; j += 2){
			attrs[r[1][j].toLowerCase()] = r[1][j + 1];
		};
		
		var box = {
			name: api.utf7.decode(''+r[0]),
			messages: {
				total: 0,
				'new': 0,
				unseen: 0
			}
		};
		
		if(attrs){
			if(typeof attrs.recent != "undefined"){
				box.messages['new'] = attrs.recent;
			};
			if(typeof attrs.unseen != "undefined"){
				box.messages.unseen = attrs.unseen;
			};
			if(typeof attrs.messages != "undefined"){
				box.messages.total = attrs.messages;
			};
		};
		
		_function_return(box);
	};
	
	this.getBoxes = function(){
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest, {query: 'LIST "" "*"', box: "", timeout: timeout, maxTime: maxTime})!
		var resp = _result_function();
		
		var lines = resp.split('\r\n');
		var boxes = {};
		
		for(var i = 0; i < lines.length; i++){
			var line = lines[i];
			
			if(line.indexOf('LIST') < 0){
				continue;
			};
			
			var info = api.parse(line, 'list');
			
			var r = api.parseExpr(info.text);
			
			var name = api.utf7.decode(''+r[2]);
			
			var box = {
				attribs: r[0],
				delimiter: r[1],
				children: null
			};
			
			for(var j = 0, len = api.specialAttrs.length; j < len; ++j){
				if(box.attribs.indexOf(api.specialAttrs[j]) > -1){
					box.special_use_attrib = api.specialAttrs[j];
				};
			};
			
			var curChildren = boxes;
			
			if(box.delimiter){
				var path = name.split(box.delimiter);
				name = path.pop();
				for(var j = 0, len = path.length; j < len; ++j){
					if(!curChildren[ path[j] ]){
						curChildren[ path[j] ] = {};
					};
					if(!curChildren[ path[j] ].children){
						curChildren[ path[j] ].children = {};
					};
					curChildren = curChildren[ path[j] ].children;
				};
			};
			if(curChildren[name]){
				box.children = curChildren[name].children;
			};
			curChildren[name] = box;
		};
		
		_function_return(boxes);
	};
	
	this.addBox = function(){
		var name = _function_argument("name");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest, {query: 'CREATE "' + api.encodeName(name) + '"', box: "", timeout: timeout, maxTime: maxTime})!
		var resp = _result_function();
	};
	
	this.delBox = function(){
		var name = _function_argument("name");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest, {query: 'DELETE "' + api.encodeName(name) + '"', box: "", timeout: timeout, maxTime: maxTime})!
		var resp = _result_function();
	};
	
	this.renameBox = function(){
		var oldName = _function_argument("oldName");
		var newName = _function_argument("newName");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.makeRequest, {query: 'RENAME "' + api.encodeName(oldName) + '" "' + api.encodeName(newName) + '"', box: "", timeout: timeout, maxTime: maxTime})!
		var resp = _result_function();
	};

	this.hasNonASCII = function(str){
		for(var i = 0; i < str.length; ++i){
			if(str.charCodeAt(i) > 0x7F){
				return true;
			};
		};
		return false;
	};

	this.buildString = function(str, info){
		if(typeof str !== 'string'){
			str = '' + str;
		};
		
		if(info && !info.hasUTF8 && api.hasNonASCII(str)){
			info.hasUTF8 = true;
		};
		
		return '"' + api.escape(str) + '"';
	};

	this.buildSearchQuery = function(options, info, isOrChild){
		var searchargs = '';		
		
		for(var i = 0, len = options.length; i < len; ++i){
			var criteria = (isOrChild ? options : options[i]);
			var args = null;
			var modifier = (isOrChild ? '' : ' ');
			
			if(typeof criteria === 'string'){
				criteria = criteria.toUpperCase();
			}else if(Array.isArray(criteria)){
				if(criteria.length > 1){
					args = criteria.slice(1);
				};
				if(criteria.length > 0){
					criteria = criteria[0].toUpperCase();
				};
			}else{
				api.errorHandler('UNEXPECTED_OPTION_TYPE', typeof criteria);
			};
			
			if(criteria === 'OR'){
				if(args.length !== 2){
					api.errorHandler('OR_NOT_TWO_ARGS');
				};
				
				if(isOrChild){
					searchargs += 'OR (';
				}else{
					searchargs += ' OR (';
				};
				
				searchargs += api.buildSearchQuery(args[0], info, true);
				searchargs += ') (';
				searchargs += api.buildSearchQuery(args[1], info, true);
				searchargs += ')';
			}else{
				if(criteria[0] === '!'){
					modifier += 'NOT ';
					criteria = criteria.substr(1);
				};
				
				switch(criteria){
					// -- Standard criteria --
					case 'ALL':
					case 'ANSWERED':
					case 'DELETED':
					case 'DRAFT':
					case 'FLAGGED':
					case 'NEW':
					case 'SEEN':
					case 'RECENT':
					case 'OLD':
					case 'UNANSWERED':
					case 'UNDELETED':
					case 'UNDRAFT':
					case 'UNFLAGGED':
					case 'UNSEEN':
						searchargs += modifier + criteria;
						break;
					case 'BCC':
					case 'BODY':
					case 'CC':
					case 'FROM':
					case 'SUBJECT':
					case 'TEXT':
					case 'TO':
						if(!args || args.length !== 1){
							api.errorHandler('INCORRECT_ARGS_NUM', criteria);
						};
						searchargs += modifier + criteria + ' ' + api.buildString(args[0], info);
						break;
					case 'BEFORE':
					case 'ON':
					case 'SENTBEFORE':
					case 'SENTON':
					case 'SENTSINCE':
					case 'SINCE':
						if(!args || args.length !== 1){
							api.errorHandler('INCORRECT_ARGS_NUM', criteria);
						}else if(!(args[0]instanceof Date)){
							if((args[0] = new Date(args[0])).toString() === 'Invalid Date'){
								api.errorHandler('ARG_NOT_DATE', criteria);
							};
						};
						searchargs += modifier + criteria + ' ' + args[0].getDate() + '-' + api.months[args[0].getMonth()] + '-' + args[0].getFullYear();
						break;
					case 'KEYWORD':
					case 'UNKEYWORD':
						if(!args || args.length !== 1){
							api.errorHandler('INCORRECT_ARGS_NUM', criteria);
						};
						searchargs += modifier + criteria + ' ' + args[0];
						break;
					case 'LARGER':
					case 'SMALLER':
						if(!args || args.length !== 1){
							api.errorHandler('INCORRECT_ARGS_NUM', criteria);
						};
						var num = parseInt(args[0], 10);
						if(isNaN(num)){
							api.errorHandler('ARG_NOT_NUM', criteria);
						};
						searchargs += modifier + criteria + ' ' + args[0];
						break;
					case 'HEADER':
						if(!args || args.length !== 2){
							api.errorHandler('INCORRECT_ARGS_NUM', criteria);
						};
						searchargs += modifier + criteria + ' "' + api.escape('' + args[0]) + '" ' + api.buildString(args[1], info);
						break;
					case 'UID':
						if(!args){
							api.errorHandler('INCORRECT_ARGS_NUM', criteria);
						};
						api.validateUIDList(args);
						if(args.length === 0){
							api.errorHandler('EMPTY_UID_LIST');
						};
						searchargs += modifier + criteria + ' ' + args.join(',');
						break;
						// Extensions ==========================================================
					case 'X-GM-MSGID': // Gmail unique message ID
					case 'X-GM-THRID': // Gmail thread ID
						if(!api.serverSupports('X-GM-EXT-1')){
							api.errorHandler('SERVER_NOT_SUPPORT', criteria);
						};
						if(!args || args.length !== 1){
							api.errorHandler('INCORRECT_ARGS_NUM', criteria);
						}else{
							if(!(/^\d+$/.test(args[0]))){
								api.errorHandler('INVALID_VALUE', criteria);
							};	
						};
						searchargs += modifier + criteria + ' ' + args[0];
						break;
					case 'X-GM-RAW': // Gmail search syntax
						if(!api.serverSupports('X-GM-EXT-1')){
							api.errorHandler('SERVER_NOT_SUPPORT', criteria);
						};
						if(!args || args.length !== 1){
							api.errorHandler('INCORRECT_ARGS_NUM', criteria);
						};
						searchargs += modifier + criteria + ' ' + api.buildString(args[0], info);
						break;
					case 'X-GM-LABELS': // Gmail labels
						if(!api.serverSupports('X-GM-EXT-1')){
							api.errorHandler('SERVER_NOT_SUPPORT', criteria);
						};
						if(!args || args.length !== 1){
							api.errorHandler('INCORRECT_ARGS_NUM', criteria);
						};
						searchargs += modifier + criteria + ' ' + args[0];
						break;
					case 'MODSEQ':
						if(!api.serverSupports('CONDSTORE')){
							api.errorHandler('SERVER_NOT_SUPPORT', criteria);
						};
						if(!args || args.length !== 1){
							api.errorHandler('INCORRECT_ARGS_NUM', criteria);
						};
						searchargs += modifier + criteria + ' ' + args[0];
						break;
					default:
						// last hope it's a seqno set
						// http://tools.ietf.org/html/rfc3501#section-6.4.4
						var seqnos = (args ? [criteria].concat(args) : [criteria]);
						if(!api.validateUIDList(seqnos, true)){
							if(seqnos.length === 0){
								api.errorHandler('EMPTY_SEQUENCE_LIST');
							};
							searchargs += modifier + seqnos.join(',');
						}else{
							api.errorHandler('UNEXPECTED_OPTION', criteria);
						};	
				};
			};
			
			if(isOrChild){
				break;
			};
		};
		
		return searchargs;
	};
	
	this.search = function(){
		var criteria = api.prepareCriteria(_function_argument("criteria"));
		var box = api.prepareBox(_function_argument("box"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var cmd = 'UID SEARCH';
		var info = {
			hasUTF8: false
		};
		
		_call_function(api.capability, {timeout: timeout, maxTime: maxTime})!
		
		var query = api.buildSearchQuery(criteria, info);
		
		if(info.hasUTF8){
			cmd += ' CHARSET UTF-8';
		};
		
		cmd += query;
		
		_call_function(api.makeRequest, {query: cmd, box: box, isUTF8: info.hasUTF8, timeout: timeout, maxTime: maxTime})!
		var resp = _result_function();
		
		var result = api.parseSearch(resp);
		
		_function_return(result);
	};
	
	this.esearch = function(){
		var criteria = api.prepareCriteria(_function_argument("criteria"));
		var options = _avoid_nil(_function_argument("options"));
		api.validateArgType(options, ['array','string'], 'Options');
		var box = api.prepareBox(_function_argument("box"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.capability, {})!
		
		if(!api.serverSupports('ESEARCH')){
			api.errorHandler('ESEARCH_NOT_SUPPORT');
		};
		
		if(Array.isArray(options)){
			options = options.join(' ');
		};
		
		var cmd = 'UID SEARCH RETURN (' + options + ')';
		var info = {
			hasUTF8: false
		};
		
		var query = api.buildSearchQuery(criteria, info);
		
		if(info.hasUTF8){
			cmd += ' CHARSET UTF-8';
		};
		
		cmd += query;
		
		_call_function(api.makeRequest, {query: cmd, box: box, isUTF8: info.hasUTF8, timeout: timeout, maxTime: maxTime})!
		var resp = _result_function();
		
		var info = api.parse(resp, 'esearch');
		
		var r = api.parseExpr(info.text.toUpperCase().replace('UID', ''));
		var attrs = {};
		
		// RFC4731 unfortunately is lacking on documentation, so we're going to
		// assume that the response text always begins with (TAG "A123") and skip that
		// part ...
		
		for(var j = 1, key, val; j < r.length; j += 2){
			key = r[j].toLowerCase();
			val = r[j + 1];
			if(key === 'all'){
				val = val.toString().split(',');
			};
			attrs[key] = val;
		};
		
		_function_return(attrs);
	};
	
	this.searchLast = function(){
		var args = _function_arguments();
		args.criteria = _avoid_nilb(args.criteria, ['ALL']);
		args.timeout = _avoid_nilb(args.timeout, 60000);
		args.maxTime = _avoid_nilb(args.maxTime, Date.now() + args.timeout);
		
		var last = 0;
		
		_call_function(api.capability, {timeout: args.timeout, maxTime: args.maxTime})!
		
		_if_else(api.serverSupports('ESEARCH'), function(){
			args.options = 'MAX';
			_call_function(api.esearch, args)!
			last = _result_function().max || 0;
		}, function(){
			_call_function(api.search, args)!
			last = _result_function().pop() || 0;
		})!;
		
		_function_return(last);
	};
	
	this.count = function(){
		var args = _function_arguments();
		args.timeout = _avoid_nilb(args.timeout, 60000);
		args.maxTime = _avoid_nilb(args.maxTime, Date.now() + args.timeout);
		
		var count = 0;
		
		_call_function(api.capability, {timeout: args.timeout, maxTime: args.maxTime})!
		
		_if_else(api.serverSupports('ESEARCH'), function(){
			args.options = 'COUNT';
			_call_function(api.esearch, args)!
			count = _result_function().count;
		}, function(){
			_call_function(api.search, args)!
			count = _result_function().length;
		})!;
		
		_function_return(count);
	};
	
	this.sort = function(){
		var sorts = _function_argument("sorts");
		var criteria = api.prepareCriteria(_function_argument("criteria"));
		var box = api.prepareBox(_function_argument("box"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		api.validateArgType(sorts, ['array','string'], 'Sorting criteria');
		if(typeof sorts === 'string'){
			sorts = [sorts];
		};
		if(!sorts.length){
			api.errorHandler('EMPTY_SORT_CRITERIA');
		};
		
		_call_function(api.capability, {timeout: timeout, maxTime: maxTime})!
		
		if(!api.serverSupports('SORT')){
			api.errorHandler('SORT_NOT_SUPPORT');
		};
		
		sorts = sorts.map(function(c){
			if(typeof c !== 'string'){
				api.errorHandler('UNEXPECTED_CRITERION_TYPE', typeof c);
			};
			
			var modifier = '';
			if(c[0] === '-'){
				modifier = 'REVERSE ';
				c = c.substring(1);
			};
			
			switch(c.toUpperCase()){
				case 'ARRIVAL':
				case 'CC':
				case 'DATE':
				case 'FROM':
				case 'SIZE':
				case 'SUBJECT':
				case 'TO':
					break;
				default:
					api.errorHandler('UNEXPECTED_CRITERION', c);
			};
			
			return modifier + c;
		});
		
		sorts = sorts.join(' ');
		
		var info = {
			hasUTF8: false
		};
		
		var query = api.buildSearchQuery(criteria, info);
		
		var cmd = 'UID SORT (' + sorts + ') ' + (info.hasUTF8 ? 'UTF-8' : 'US-ASCII') + query;
		
		_call_function(api.makeRequest, {query: cmd, box: box, isUTF8: info.hasUTF8, timeout: timeout, maxTime: maxTime})!
		var resp = _result_function();
		
		var result = api.parseSearch(resp);
		
		_function_return(result);
	};
	
	this.store = function(){
		var uids = api.prepareUIDs(_function_argument("uids"));
		var config = _function_argument("config");
		var box = api.prepareBox(_function_argument("box"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		api.validateArgType(config, 'object', 'Config');
		if(_is_nilb(config.flags) && _is_nilb(config.keywords)){
			api.errorHandler('NOT_FLAGS_KEYWORDS');
		};
		
		var isFlags = !_is_nilb(config.flags);
		var items = (isFlags ? config.flags : config.keywords);
		
		if((!Array.isArray(items) && typeof items !== 'string') || (Array.isArray(items) && items.length === 0)){
			if(isFlags){
				api.errorHandler('FLAGS_INVALID_ARGS');
			}else{
				api.errorHandler('KEYWORDS_INVALID_ARGS');
			};
		};
		
		if(!Array.isArray(items)){
			items = [items];
		};
		
		for(var i = 0; i < items.length; ++i){
			if(isFlags){
				if(items[i][0] !== '\\'){
					items[i] = '\\' + items[i];
				};
			}else{
				// keyword contains any char except control characters (%x00-1F and %x7F)
				// and: '(', ')', '{', ' ', '%', '*', '\', '"', ']'
				if(/[\(\)\{\\\"\]\%\*\x00-\x20\x7F]/.test(items[i])){
					api.errorHandler('KEYWORD_INVALID_CHARS', items[i]);
				};
			};
		};
		
		items = items.join(' ');
		
		var cmd = 'UID STORE ' + uids + ' ' + config.mode + 'FLAGS.SILENT (' + items + ')';
		
		_call_function(api.makeRequest, {query: cmd, box: box, timeout: timeout, maxTime: maxTime})!
		var resp = _result_function();
	};
	
	this.addFlags = function(){
		var args = _function_arguments();
		
		_call_function(api.store, {
			uids: args.uids,
			config: {
				mode: '+',
				flags: args.flags
			},
			box: args.box,
			timeout: args.timeout,
			maxTime: args.maxTime
		})!
	};
	
	this.delFlags = function(){
		var args = _function_arguments();
		
		_call_function(api.store, {
			uids: args.uids,
			config: {
				mode: '-',
				flags: args.flags
			},
			box: args.box,
			timeout: args.timeout,
			maxTime: args.maxTime
		})!
	};
	
	this.setFlags = function(){
		var args = _function_arguments();
		
		_call_function(api.store, {
			uids: args.uids,
			config: {
				mode: '',
				flags: args.flags
			},
			box: args.box,
			timeout: args.timeout,
			maxTime: args.maxTime
		})!
	};
	
	this.addKeywords = function(){
		var args = _function_arguments();
		
		_call_function(api.store, {
			uids: args.uids,
			config: {
				mode: '+',
				keywords: args.keywords
			},
			box: args.box,
			timeout: args.timeout,
			maxTime: args.maxTime
		})!
	};
	
	this.delKeywords = function(){
		var args = _function_arguments();
		
		_call_function(api.store, {
			uids: args.uids,
			config: {
				mode: '-',
				keywords: args.keywords
			},
			box: args.box,
			timeout: args.timeout,
			maxTime: args.maxTime
		})!
	};
	
	this.setKeywords = function(){
		var args = _function_arguments();
		
		_call_function(api.store, {
			uids: args.uids,
			config: {
				mode: '',
				keywords: args.keywords
			},
			box: args.box,
			timeout: args.timeout,
			maxTime: args.maxTime
		})!
	};
	
	this.expunge = function(){
		var uids = _function_argument("uids");
		var box = api.prepareBox(_function_argument("box"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_if(uids, function(){
			uids = api.prepareUIDs(uids);
			
			_call_function(api.capability, {timeout: timeout, maxTime: maxTime})!
		})!
		
		var cmd = (uids && api.serverSupports('UIDPLUS')) ? ('UID EXPUNGE ' + uids) : 'EXPUNGE';
		
		_call_function(api.makeRequest, {query: cmd, box: box, timeout: timeout, maxTime: maxTime})!
		var resp = _result_function();
	};
	
	this.delMessages = function(){
		var uids = _function_argument("uids");
		var box = _function_argument("box");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.addFlags, {uids: uids, flags: '\\Deleted', box: box, timeout: timeout, maxTime: maxTime})!
		
		_call_function(api.expunge, {uids: uids, box: box, timeout: timeout, maxTime: maxTime})!
	};
	
	this.copyMessages = function(){
		var uids = api.prepareUIDs(_function_argument("uids"));
		var box = api.prepareBox(_function_argument("box"));
		var toBox = _function_argument("toBox");
		api.validateArgType(toBox, 'string', 'To folder');
		if(!toBox.length){
			api.errorHandler('TOBOX_NOT_SPECIFIED');
		};
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var cmd = 'UID COPY ' + uids + ' "' + api.encodeName(toBox) + '"';
		
		_call_function(api.makeRequest, {query: cmd, box: box, timeout: timeout, maxTime: maxTime})!
		var resp = _result_function();
	};
	
	this.moveMessages = function(){
		var uids = api.prepareUIDs(_function_argument("uids"));
		var box = api.prepareBox(_function_argument("box"));
		var toBox = _function_argument("toBox");
		api.validateArgType(toBox, 'string', 'To folder');
		if(!toBox.length){
			api.errorHandler('TOBOX_NOT_SPECIFIED');
		};
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.capability, {timeout: timeout, maxTime: maxTime})!
		
		if(!api.serverSupports('MOVE')){
			api.errorHandler('MOVE_NOT_SUPPORT');
		};
		
		var cmd = 'UID MOVE ' + uids + ' "' + api.encodeName(toBox) + '"';
		
		_call_function(api.makeRequest, {query: cmd, box: box, timeout: timeout, maxTime: maxTime})!
		var resp = _result_function();
	};
	
	this.parseFetchParts = function(str){
		var firstBreak = str.indexOf('\r\n');
		var fetch = {};
		if(firstBreak > -1){
			fetch.head = str.slice(0, firstBreak);
			fetch.body = str.slice(firstBreak + 2);
		}else{
			fetch.head = str;
			fetch.body = '';
		};
		return fetch;
	};
	
	this.parseFetch = function(fetch, msg){
		var list = api.parseExpr(fetch.head);
		if(Array.isArray(list[0])){
			list = list[0];
		};
		// list is [KEY1, VAL1, KEY2, VAL2, .... KEYn, VALn]
		for(var j = 0, len = list.length, key, val, m; j < len; j += 2){
			key = list[j].toLowerCase();
			val = list[j + 1];
			if(key === 'envelope'){
				val = api.parseFetchEnvelope(val);
			}else if(key === 'internaldate'){
				key = 'date';
				val = new Date(val);
			}else if(key === 'rfc822.size'){
				key = 'size';
			}else if(key === 'modseq'){ // always a list of one value
				val = ''+val[0];
			}else if(key === 'body' || key === 'bodystructure'){
				key = 'struct';
				val = api.parseBodyStructure(val);
			}else if(m = /^BODY\[(.*)\]$/i.exec(list[j])){
				key = m[1];
				if(key && /\{(\d+)\}$/.test(val)){
					val = parseInt(val.slice(1, -1), 10);
					msg.parts.push({
						which: key,
						size: val,
						body: fetch.body.slice(0, val)
					});
					fetch.body = fetch.body.slice(val);
				};
				continue;
			};
			msg.attributes[key] = val;
		};
	};
	
	this.fetch = function(){
		var uids = api.prepareUIDs(_function_argument("uids"));
		var options = _function_argument("options");
		var box = api.prepareBox(_function_argument("box"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		if(options){
			api.validateArgType(options, 'object', 'Options');
		};
		
		var cmd = 'UID FETCH ' + uids + ' (';
		
		if(options){
			
			var fetching = [];
			
			if(options.uid){
				fetching.push('UID');
			};
			if(options.flags){
				fetching.push('FLAGS');
			};
			if(options.date){
				fetching.push('INTERNALDATE');
			};
			if(options.envelope){
				fetching.push('ENVELOPE');
			};
			if(options.struct){
				fetching.push('BODYSTRUCTURE');
			};
			if(options.size){
				fetching.push('RFC822.SIZE');
			};
			if(Array.isArray(options.extensions)){
				options.extensions.forEach(function(extension){
					fetching.push(extension.toUpperCase());
				});
			};
			
			if(!_is_nilb(options.bodies)){
				var bodies = options.bodies;
				var prefix = (options.markSeen ? '' : '.PEEK');
				if(!Array.isArray(bodies)){
					bodies = [bodies];
				};
				for(var i = 0; i < bodies.length; ++i){
					fetching.push('BODY' + prefix + '[' + bodies[i] + ']');
				};
			};
			
			cmd += fetching.join(' ');
		};
		
		cmd += ')';
		
		_call_function(api.request, {path: api.encodeNameUrl(box), query: cmd, isFetch: true, timeout: timeout, maxTime: maxTime})!
		var resp = _result_function();
		
		var fetchCache = {};
		var fetchList = resp.fetchlist;
		
		for(var i = 0; i < fetchList.length; ++i){
			fetchList[i] = api.delUpdateData(fetchList[i]); //Delete update data when mailbox changes
			var fetch = api.parseFetchParts(fetchList[i]);
			var info = api.parse(fetch.head);
			var seqno = info.num;
			fetch.head = info.text;
			var msg = fetchCache[seqno];
			if(typeof msg == "undefined"){
				msg = fetchCache[seqno] = {
					attributes: {},
					parts: []
				};
			};
			api.parseFetch(fetch, msg);
			var remains = fetch.body.trim();
			while(remains != '' && remains != ')' && _starts_with(remains, 'BODY[')){
				fetch = api.parseFetchParts(remains);
				api.parseFetch(fetch, msg);
				remains = fetch.body.trim();
			};
		};
		
		var messages = [];
		
		for(var seqno in fetchCache){
			messages.push(fetchCache[seqno]);
		};
		
		_function_return(messages);
	};
	
	this.getParts = function(struct, parts){
		parts = _avoid_nilb(parts, []);
		for(var i = 0; i < struct.length; ++i){
			part = struct[i];
			if(Array.isArray(part)){
				api.getParts(part, parts);
			}else if(part.partID){
				parts.push(part);
			};
		};
		return parts;
	};
	
	this.getPartData = function(){
		var uid = _function_argument("uid");
		var part = _function_argument("part");
		var saveToFile = _avoid_nilb(_function_argument("saveToFile"), false);
		var markSeen = _avoid_nilb(_function_argument("markSeen"), false);
		var box = _function_argument("box");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.fetch, {uids: uid, options: {bodies: [part.partID], markSeen: markSeen}, box: box, timeout: timeout, maxTime: maxTime})!
		var res = _result_function();
		if(!res.length || !res[0].parts || !res[0].parts.length){
			_function_return(null);
			return;
		};
		var data = res[0].parts[0].body;
		var encoding = part.encoding.toLowerCase();
		var charset = ((part.params && part.params.charset) ? part.params.charset.toLowerCase() : '') || 'utf-8';
		
		var result = api.processPartData(data, encoding, charset, saveToFile);
		
		if(!saveToFile){
			_function_return(result);
		}else{
			_function_return(true);
		};
	};
	
	this.getMessages = function(){
		var uids = api.prepareUIDs(_function_argument("uids"), 'getMessages');
		var body = api.getParamInfo(_function_argument("body"));
		var headers = api.getParamInfo(_function_argument("headers"));
		var attachments = _function_argument("attachments");
		var size = _avoid_nilb(_function_argument("size"), false);
		var flags = _avoid_nilb(_function_argument("flags"), false);
		var date = _avoid_nilb(_function_argument("date"), false);
		var attachnames = _avoid_nilb(_function_argument("attachnames"), false);
		var markSeen = _avoid_nilb(_function_argument("markSeen"), false);
		var box = _function_argument("box");
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		var options = {
			uid: true,
			markSeen: markSeen,
			size: size,
			flags: flags,
			date: date
		};

		if(headers.any || body.raw){
			options.bodies = [];
		};

		if(headers.base){
			options.bodies.push('HEADER.FIELDS (' + headers.data.map(function(h){return h.toUpperCase()}).join(' ') + ')');
		};

		if(headers.raw){
			options.bodies.push('HEADER');
		};

		if(body.raw){
			options.bodies.push('TEXT');
		};

		if(body.base || attachnames || attachments){
			options.struct = true;
		};
		
		_call_function(api.fetch, {uids: uids, options: options, markSeen: markSeen, box: box, timeout: timeout, maxTime: maxTime})!
		var msgs = _result_function();
		
		var messages = [];
		
		_do_with_params({msgs:msgs}, function(){
			var msg_index = _iterator() - 1;
			if(msg_index > _cycle_param("msgs").length - 1){
				_break();
			};
			var msg = _cycle_param("msgs")[msg_index];
			var attrs = msg.attributes;
			var parts = msg.parts;
			var message = {};
			if(body.any){
				message.body = {};
			};
			if(headers.any){
				message.headers = {};
				if(headers.raw){
					message.headers.raw = {};
				};
			};
			messages.push(message);
			for(var key in attrs){
				if(key !== 'struct'){
					message[key] = attrs[key];
				};
			};
			for(var part_index in parts){
				var part = parts[part_index];
				if(_starts_with(part.which, 'HEADER')){
					var parsed = api.parseHeader(part.body.trim());
					var obj = part.which === 'HEADER' ? message.headers.raw : message.headers;
					for(var key in parsed){
						obj[key] = parsed[key];
					};
				}else{
					message.body.raw = part.body.trim();
				};
			};
			
			_if(body.base || attachnames || attachments, function(){
				parts = api.getParts(attrs.struct);
				_if(body.base, function(){
					_do_with_params({types:body.data}, function(){
						var type_index = _iterator() - 1;
						if(type_index > _cycle_param("types").length - 1){
							_break();
						};
						var type = _cycle_param("types")[type_index];
						message.body[type] = '';
						var filtered = parts.filter(function(p){return p.type.toLowerCase() === 'text' && p.subtype.toLowerCase() === type && !p.disposition});
						_do_with_params({parts:filtered}, function(){
							var part_index = _iterator() - 1;
							if(part_index > _cycle_param("parts").length - 1){
								_break();
							};
							_call_function(api.getPartData, {uid: attrs.uid, part: _cycle_param("parts")[part_index], markSeen: markSeen, box: box, timeout: timeout, maxTime: maxTime})!
							var res = _result_function();
							if(res){
								message.body[type] += res;
							};
						})!
						message.body[type] = message.body[type].trim();
					})!
				})!
				_if(attachnames || attachments, function(){
					var filtered = parts.filter(function(p){return p.disposition && p.disposition.type && ['inline', 'attachment'].indexOf(p.disposition.type.toLowerCase()) > -1});
					if(attachnames){
						message.attachnames = filtered.map(function(p){
							var filename = '';
							if(p.disposition.params && p.disposition.params.filename){
								filename = p.disposition.params.filename;
							}else if(p.params && p.params.name){
								filename = p.params.name;
							};
							return filename;
						});
					};
					_if(attachments, function(){
						message.attachments = [];
						if(filtered.length && attachments !== true && attachments.toString().trim() !== '*'){
							if(!Array.isArray(attachments)){
								if(attachments instanceof RegExp){
									attachments = [attachments];
								}else{
									attachments = attachments.split(';');
								};
							};
							var maskFiltered = [];
							var ids = [];
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
								for(var part_index in parts){
									var part = parts[part_index];
									var filename = '';
									if(part.disposition.params && part.disposition.params.filename){
										filename = part.disposition.params.filename;
									}else if(part.params && part.params.name){
										filename = part.params.name;
									};
									if(mask.test(filename) === match && ids.indexOf(part.partID) < 0){
										maskFiltered.push(part);
										ids.push(part.partID);
									};
								};
							};
							filtered = maskFiltered;
						};
						_do_with_params({parts:filtered}, function(){
							var part_index = _iterator() - 1;
							if(part_index > _cycle_param("parts").length - 1){
								_break();
							};
							var part = _cycle_param("parts")[part_index];
							var filename = '';
							if(part.disposition.params && part.disposition.params.filename){
								filename = part.disposition.params.filename;
							}else if(part.params && part.params.name){
								filename = part.params.name;
							};
							var randomFile = api.randStr() + '.file';
							_call_function(api.getPartData, {uid: attrs.uid, part: part, markSeen: markSeen, saveToFile: randomFile, box: box, timeout: timeout, maxTime: maxTime})!
							var res = _result_function();
							if(res){
								var randomFileDir = JSON.parse(native("filesystem", "fileinfo", randomFile)).directory;
								message.attachments.push({
									name: filename,
									type: part.type + '/' + part.subtype,
									path: randomFileDir + '/' + randomFile
								});
							};
						})!
					})!
				})!
			})!
		})!

		_function_return(messages);
	};
	
	this.getFlags = function(){
		var uid = api.prepareUIDs(_function_argument("uid"));
		var box = api.prepareBox(_function_argument("box"));
		var timeout = _avoid_nilb(_function_argument("timeout"), 60000);
		var maxTime = _avoid_nilb(_function_argument("maxTime"), Date.now() + timeout);
		
		_call_function(api.fetch, {uids: uid, options: {flags: true}, markSeen: false, box: box, timeout: timeout, maxTime: maxTime})!
		var msgs = _result_function();
		
		if(!msgs.length){
			api.errorHandler('EMPTY_MSGS_LIST');
		};
		
		_function_return(msgs[0].attributes.flags);
	};
});