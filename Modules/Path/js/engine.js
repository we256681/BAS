_L["Path"] = {"ru":"Путь"};
_L["To path"] = {"ru":"До пути"};
_L["From path"] = {"ru":"От пути"};
_L["Path object"] = {"ru":"Объект пути"};
_L["File extension to remove"] = {"ru":"Удаляемое расширение файла"};

_path = {
	sep: '/',
	delimiter: ';',
	
	isPathSeparator: function(code){
		return code===47 || code===92;
	},
	
	isDeviceRoot: function(code){
		return code >= 65 && code <= 90 || code >= 97 && code <= 122;
	},
	
	normalizeString: function(path, allowAboveRoot){
		var res = '';
		var lastSegmentLength = 0;
		var lastSlash = -1;
		var dots = 0;
		var code = 0;
		for(var i = 0; i <= path.length; ++i){
			if(i < path.length){
				code = path.charCodeAt(i);
			}else if(this.isPathSeparator(code)){
				break;
			}else{
				code = 47;
			};
			
			if(this.isPathSeparator(code)){
				if(lastSlash===i-1 || dots===1){
					
				}else if(dots === 2){
					if(res.length < 2 || !(lastSegmentLength===2) || !(res.charCodeAt(res.length-1)===46) || !(res.charCodeAt(res.length-2)===46)){
						if(res.length > 2){
							const lastSlashIndex = res.lastIndexOf(this.sep);
							if(lastSlashIndex===-1){
								res = '';
								lastSegmentLength = 0;
							}else{
								res = res.slice(0, lastSlashIndex);
								lastSegmentLength = res.length - 1 - res.lastIndexOf(this.sep);
							};
							lastSlash = i;
							dots = 0;
							continue;
						}else if(!(res.length===0)){
							res = '';
							lastSegmentLength = 0;
							lastSlash = i;
							dots = 0;
							continue;
						};
					};
					if(allowAboveRoot){
						res += res.length > 0 ? (this.sep + '..') : '..';
						lastSegmentLength = 2;
					};
				}else{
					if (res.length > 0){
						res += this.sep + path.slice(lastSlash + 1, i);
					}else{
						res = path.slice(lastSlash + 1, i);
					};
					lastSegmentLength = i - lastSlash - 1;
				};
				lastSlash = i;
				dots = 0;
			}else if(code===46 && !(dots===-1)){
				++dots;
			}else{
				dots = -1;
			};
		};
		return res;
	},
	
	resolve: function(args){
		if(!Array.isArray(args)){
			args = Array.prototype.slice.call(arguments);
		};
		var resolvedDevice = '';
		var resolvedTail = '';
		var resolvedAbsolute = false;
		
		for(var i = args.length - 1; i > -1; i--){
			var path = args[i];
			_validate_argument_type(path, 'string', 'Path', '_path.resolve');
			
			if(path.length===0){
				continue;
			};
			
			const len = path.length;
			var rootEnd = 0;
			var device = '';
			var isAbsolute = false;
			const code = path.charCodeAt(0);
			
			if(len===1){
				if(this.isPathSeparator(code)){
					rootEnd = 1;
					isAbsolute = true;
				};
			}else if(this.isPathSeparator(code)){
				isAbsolute = true;
				if(this.isPathSeparator(path.charCodeAt(1))){
					var j = 2;
					var last = j;
					while(j < len && !this.isPathSeparator(path.charCodeAt(j))){
						j++;
					};
					if(j < len && !(j===last)){
						const firstPart = path.slice(last, j);
						last = j;
						while(j < len && this.isPathSeparator(path.charCodeAt(j))){
							j++;
						};
						if(j < len && !(j===last)){
							last = j;
							
							while(j < len && !this.isPathSeparator(path.charCodeAt(j))){
								j++;
							};
							if(j===len || !(j===last)){
								device = this.sep + this.sep + firstPart + this.sep + path.slice(last, j);
								rootEnd = j;
							};
						};
					};
				}else{
					rootEnd = 1;
				};
			}else if(this.isDeviceRoot(code) && path.charCodeAt(1)===58){
				device = path.slice(0, 2);
				rootEnd = 2;
				if(len > 2 && this.isPathSeparator(path.charCodeAt(2))){
					isAbsolute = true;
					rootEnd = 3;
				};
			};
			
			if(device.length > 0){
				if(resolvedDevice.length > 0){
					if(!(device.toLowerCase()===resolvedDevice.toLowerCase())){
						continue;
					};
				}else{
					resolvedDevice = device;
				};
			};
			
			if(resolvedAbsolute){
				if(resolvedDevice.length > 0){
					break;
				};
			}else{
				resolvedTail = path.slice(rootEnd) + this.sep + resolvedTail;
				resolvedAbsolute = isAbsolute;
				if(isAbsolute && resolvedDevice.length > 0){
					break;
				};
			};
		};
		
		resolvedTail = this.normalizeString(resolvedTail, !resolvedAbsolute);

		return resolvedAbsolute ? resolvedDevice + this.sep + resolvedTail : (resolvedDevice + resolvedTail) || '.';
	},
	
	normalize: function(path, removeTrailingSlash){
		_validate_argument_type(path, 'string', 'Path', '_path.normalize');
		removeTrailingSlash = _avoid_nilb(removeTrailingSlash, true);
		_validate_argument_type(removeTrailingSlash, ['boolean', 'number'], 'Remove trailing slashes', '_path.normalize');
		
		const len = path.length;
		if(len===0){
			return '';
		};
		
		var rootEnd = 0;
		var device = undefined;
		var isAbsolute = false;
		const code = path.charCodeAt(0);
		
		if(len===1){
			return this.isPathSeparator(code) ? (removeTrailingSlash ? '' : this.sep) : path;
		};
		if(this.isPathSeparator(code)){
			isAbsolute = true;
			
			if(this.isPathSeparator(path.charCodeAt(1))){
				var j = 2;
				var last = j;
				while (j < len && !this.isPathSeparator(path.charCodeAt(j))){
					j++;
				};
				if(j < len && !(j===last)){
					const firstPart = path.slice(last, j);
					last = j;
					while (j < len && this.isPathSeparator(path.charCodeAt(j))){
						j++;
					};
					if(j < len && !(j===last)){
						last = j;
						while (j < len && !this.isPathSeparator(path.charCodeAt(j))){
							j++;
						};
						if(j===len){
							return (this.sep + this.sep + firstPart + this.sep + path.slice(last) + (removeTrailingSlash ? '' : this.sep));
						};
						if(!(j===last)){
							device = this.sep + this.sep + firstPart + this.sep + path.slice(last, j);
							rootEnd = j;
						};
					};
				};
			}else{
				rootEnd = 1;
			};
		}else if(this.isDeviceRoot(code) && path.charCodeAt(1)===58){
			device = path.slice(0, 2);
			rootEnd = 2;
			if(len > 2 && this.isPathSeparator(path.charCodeAt(2))){
				isAbsolute = true;
				rootEnd = 3;
			};
		};
		
		var tail = rootEnd < len ? this.normalizeString(path.slice(rootEnd), !isAbsolute) : '';
		
		if(tail.length > 0 && this.isPathSeparator(path.charCodeAt(len - 1)) && !removeTrailingSlash){
			tail += this.sep;
		};
		if(device===undefined){
			return isAbsolute ? ((tail.length===0 && removeTrailingSlash ? '' : this.sep) + tail) : tail;
		};
		return isAbsolute ? (device + (tail.length===0 && removeTrailingSlash ? '' : this.sep) + tail) : (device + tail);
	},
	
	isAbsolute: function(path){
		_validate_argument_type(path, 'string', 'Path', '_path.isAbsolute');
		const len = path.length;
		if(len===0){
			return false;
		};
		
		const code = path.charCodeAt(0);
		
		return this.isPathSeparator(code) || len > 2 && this.isDeviceRoot(code) && path.charCodeAt(1) === 58 && this.isPathSeparator(path.charCodeAt(2));
	},
	
	join: function(args){
		if(!Array.isArray(args)){
			args = Array.prototype.slice.call(arguments);
		};
		if(args.length===0){
			return '.';
		};
		
		var joined = undefined;
		var firstPart = undefined;
		
		for(var i = 0; i < args.length; ++i){
			const arg = args[i];
			_validate_argument_type(arg, 'string', 'Path', '_path.join');
			if(arg.length > 0){
				if(joined === undefined){
					joined = firstPart = arg;
				}else{
					joined += (this.sep + arg);
				};
			};
		};

		if(joined === undefined){
			return '.';
		};
		
		var needsReplace = true;
		var slashCount = 0;
		if(this.isPathSeparator(firstPart.charCodeAt(0))){
			++slashCount;
			const firstLen = firstPart.length;
			if(firstLen > 1 && this.isPathSeparator(firstPart.charCodeAt(1))){
				++slashCount;
				if(firstLen > 2){
					if(this.isPathSeparator(firstPart.charCodeAt(2))){
						++slashCount;
					}else{
						needsReplace = false;
					};
				};
			};
		};
		
		if(needsReplace){
			while(slashCount < joined.length && this.isPathSeparator(joined.charCodeAt(slashCount))){
				slashCount++;
			};
			
			if(slashCount >= 2){
				joined = this.sep + joined.slice(slashCount);
			};
		};
		
		return this.normalize(joined);
	},
	
	relative: function(from, to){
		_validate_argument_type(from, 'string', 'From path', '_path.relative');
		_validate_argument_type(to, 'string', 'To path', '_path.relative');
		
		if(from===to){
			return '';
		};
		
		const fromOrig = this.resolve(from);
		const toOrig = this.resolve(to);
		
		if(fromOrig===toOrig){
			return '';
		};
		
		from = fromOrig.toLowerCase();
		to = toOrig.toLowerCase();
		
		if(from===to){
			return '';
		};
		
		var fromStart = 0;
		while(fromStart < from.length && from.charCodeAt(fromStart)===this.sep.charCodeAt(0)){
			fromStart++;
		};
		
		var fromEnd = from.length;
		while(fromEnd - 1 > fromStart && from.charCodeAt(fromEnd - 1)===this.sep.charCodeAt(0)){
			fromEnd--;
		};
		const fromLen = fromEnd - fromStart;
		
		var toStart = 0;
		while(toStart < to.length && to.charCodeAt(toStart)===this.sep.charCodeAt(0)){
			toStart++;
		};
		
		var toEnd = to.length;
		while(toEnd - 1 > toStart && to.charCodeAt(toEnd - 1)===this.sep.charCodeAt(0)){
			toEnd--;
		};
		const toLen = toEnd - toStart;
		
		const length = fromLen < toLen ? fromLen : toLen;
		var lastCommonSep = -1;
		var i = 0;
		for(; i < length; i++){
			const fromCode = from.charCodeAt(fromStart + i);
			if(!(fromCode===to.charCodeAt(toStart + i))){
				break;
			}else if(fromCode===this.sep.charCodeAt(0)){
				lastCommonSep = i;
			};
		};
		
		if(!(i===length)){
			if(lastCommonSep===-1){
				return toOrig;
			};
		}else{
			if(toLen > length){
				if(to.charCodeAt(toStart + i)===this.sep.charCodeAt(0)){
					return toOrig.slice(toStart + i + 1);
				};
				if(i===2){
					return toOrig.slice(toStart + i);
				};
			};
			if(fromLen > length){
				if(from.charCodeAt(fromStart + i)===this.sep.charCodeAt(0)){
					lastCommonSep = i;
				}else if(i===2){
					lastCommonSep = 3;
				};
			};
			if(lastCommonSep===-1){
				lastCommonSep = 0;
			};
		};
		
		var out = '';
		
		for(i = fromStart + lastCommonSep + 1; i <= fromEnd; ++i){
			if(i===fromEnd || from.charCodeAt(i)===this.sep.charCodeAt(0)){
				out += out.length===0 ? '..' : this.sep + '..';
			};
		};

		toStart += lastCommonSep;
		
		if(out.length > 0){
			return out + toOrig.slice(toStart, toEnd);
		};
		if(toOrig.charCodeAt(toStart)===this.sep.charCodeAt(0)){
			++toStart;
		};
		return toOrig.slice(toStart, toEnd);
	},
	
	toNamespacedPath: function(path){
		if(typeof path!='string'){
			return path;
		};

		if(path.length===0){
			return '';
		};

		const resolvedPath = this.resolve(path);

		if(resolvedPath.length <= 2){
			return path;
		};
		
		if(resolvedPath.charCodeAt(0)===this.sep.charCodeAt(0)){
			if(resolvedPath.charCodeAt(1)===this.sep.charCodeAt(0)){
				const code = resolvedPath.charCodeAt(2);
				if(!(code===63) && !(code===46)){
					return (this.sep + this.sep + '?' + this.sep + 'UNC' + this.sep + resolvedPath.slice(2));
				};
			};
		}else if(this.isDeviceRoot(resolvedPath.charCodeAt(0)) && resolvedPath.charCodeAt(1)===58 && resolvedPath.charCodeAt(2)===this.sep.charCodeAt(0)){
			return (this.sep + this.sep + '?' + this.sep + resolvedPath);
		};
		
		return path;
	},
	
	dirname: function(path){
		_validate_argument_type(path, 'string', 'Path', '_path.dirname');
		const len = path.length;
		if(len===0){
			return '.';
		};
		var rootEnd = -1;
		var offset = 0;
		const code = path.charCodeAt(0);

		if(len===1){
			return this.isPathSeparator(code) ? path : '.';
		};
		
		if(this.isPathSeparator(code)){
			rootEnd = offset = 1;
			
			if(this.isPathSeparator(path.charCodeAt(1))){
				var j = 2;
				var last = j;
				
				while(j < len && !this.isPathSeparator(path.charCodeAt(j))){
					j++;
				};
				if(j < len && !(j===last)){
					last = j;
					
					while(j < len && this.isPathSeparator(path.charCodeAt(j))){
						j++;
					};
					if(j < len && !(j===last)){
						last = j;
						while (j < len && !this.isPathSeparator(path.charCodeAt(j))){
							j++;
						};
						if(j===len){
							return path;
						};
						if(!(j===last)){
							rootEnd = offset = j + 1;
						};
					};
				};
			};
		}else if(this.isDeviceRoot(code) && path.charCodeAt(1)===58){
			rootEnd = len > 2 && this.isPathSeparator(path.charCodeAt(2)) ? 3 : 2;
			offset = rootEnd;
		};
		
		var end = -1;
		var matchedSlash = true;
		for(var i = len - 1; i >= offset; --i){
			if(this.isPathSeparator(path.charCodeAt(i))){
				if(!matchedSlash){
					end = i;
					break;
				};
			}else{
				matchedSlash = false;
			};
		};
		
		if(end===-1){
			if(rootEnd===-1){
				return '.';
			};
			
			end = rootEnd;
		};
		return path.slice(0, end);
	},
	
	basename: function(path, ext){
		_validate_argument_type(path, 'string', 'Path', '_path.basename');
		_validate_argument_type(ext, ['string','undefined','null'], 'File extension to remove', '_path.basename');
		var start = 0;
		var end = -1;
		var matchedSlash = true;
		var i = 0;
		
		if(path.length >= 2 && this.isDeviceRoot(path.charCodeAt(0)) && path.charCodeAt(1)===58){
			start = 2;
		};
		
		if(!(_is_nilb(ext)) && ext.length > 0 && ext.length <= path.length){
			if(ext===path){
				return '';
			};
			var extIdx = ext.length - 1;
			var firstNonSlashEnd = -1;
			for(i = path.length - 1; i >= start; --i){
				const code = path.charCodeAt(i);
				if(this.isPathSeparator(code)){
					if(!matchedSlash){
						start = i + 1;
						break;
					};
				}else{
					if(firstNonSlashEnd===-1){
						matchedSlash = false;
						firstNonSlashEnd = i + 1;
					};
					if(ext==='*'){
						if(code===46 && end===-1){
							end = i;
						};
					}else{
						if(extIdx >= 0){
							if(code===ext.charCodeAt(extIdx)){
								if(--extIdx===-1){
									end = i;
								};
							}else{
								extIdx = -1;
								end = firstNonSlashEnd;
							};
						};
					};
				};
			};
			
			if(start===end){
				end = firstNonSlashEnd;
			}else if(end === -1){
				end = path.length;
			};
			return path.slice(start, end);
		};
		for(i = path.length - 1; i >= start; --i){
			if(this.isPathSeparator(path.charCodeAt(i))){
				if(!matchedSlash){
					start = i + 1;
					break;
				};
			}else if(end===-1){
				matchedSlash = false;
				end = i + 1;
			};
		};
		
		if(end===-1){
			return '';
		};
		return path.slice(start, end);
	},
	
	extname: function(path){
		_validate_argument_type(path, 'string', 'Path', '_path.extname');
		var start = 0;
		var startDot = -1;
		var startPart = 0;
		var end = -1;
		var matchedSlash = true;
		
		var preDotState = 0;
		
		if(path.length >= 2 && path.charCodeAt(1)===58 && this.isDeviceRoot(path.charCodeAt(0))){
			start = startPart = 2;
		};
		
		for(var i = path.length - 1; i >= start; --i){
			const code = path.charCodeAt(i);
			if(this.isPathSeparator(code)){
				if(!matchedSlash){
					startPart = i + 1;
					break;
				};
				continue;
			};
			if(end===-1){
				matchedSlash = false;
				end = i + 1;
			};
			if(code===46){
				if(startDot===-1){
					startDot = i;
				}else if(!(preDotState===1)){
					preDotState = 1;
				};
			}else if(!(startDot===-1)){
				preDotState = -1;
			};
		};
		
		if(startDot===-1 || end===-1 || preDotState===0 || (preDotState===1 && startDot===end - 1 && startDot===startPart + 1)){
			return '';
		};
		return path.slice(startDot, end);
	},
	
	format: function(pathObject){
		_validate_argument_type(pathObject, 'object', 'Path object', '_path.format');
		var dir = pathObject.dir || pathObject.root;
		var base = pathObject.base || (pathObject.name || '' + pathObject.ext || '');
		if(!dir){
			return base;
		};
		return dir===pathObject.root ? (dir + base) : (dir + this.sep + base);
	},
	
	parse: function(path){
		_validate_argument_type(path, 'string', 'Path', '_path.parse');
		
		const ret = { root: '', dir: '', base: '', ext: '', name: '', items: [] };
		if(path.length===0){
			return ret;
		};
		
		const len = path.length;
		var rootEnd = 0;
		var code = path.charCodeAt(0);
		
		if(len===1){
			if(this.isPathSeparator(code)){
				ret.root = ret.dir = path;
				return ret;
			};
			ret.base = ret.name = ret.items[0] = path;
			return ret;
		};
		
		ret.items = path.split(/[\\/]+/).filter(function(el){return el.length > 0});
		
		if(this.isPathSeparator(code)){
			rootEnd = 1;
			if(this.isPathSeparator(path.charCodeAt(1))){
				var j = 2;
				var last = j;
				while(j < len && !this.isPathSeparator(path.charCodeAt(j))){
					j++;
				};
				if(j < len && !(j===last)){
					last = j;
					while (j < len && this.isPathSeparator(path.charCodeAt(j))){
						j++;
					};
					if(j < len && !(j===last)){
						last = j;
						while (j < len && !this.isPathSeparator(path.charCodeAt(j))){
							j++;
						};
						if(j===len){
							rootEnd = j;
						}else if(!(j===last)){
							rootEnd = j + 1;
						};
					};
				};
			};
		}else if(this.isDeviceRoot(code) && path.charCodeAt(1)===58){
			if(len <= 2){
				ret.root = ret.dir = path;
				return ret;
			};
			rootEnd = 2;
			if(this.isPathSeparator(path.charCodeAt(2))){
				if (len===3){
					ret.root = ret.dir = path;
					return ret;
				};
				rootEnd = 3;
			};
		};
		if(rootEnd > 0){
			ret.root = path.slice(0, rootEnd);
		};
		
		var startDot = -1;
		var startPart = rootEnd;
		var end = -1;
		var matchedSlash = true;
		var i = path.length - 1;
		
		var preDotState = 0;
		
		for(; i >= rootEnd; --i){
			code = path.charCodeAt(i);
			if(this.isPathSeparator(code)){
				if(!matchedSlash){
					startPart = i + 1;
					break;
				};
				continue;
			};
			if(end===-1){
				matchedSlash = false;
				end = i + 1;
			};
			if(code===46){
				if(startDot===-1){
					startDot = i;
				}else if(!(preDotState===1)){
					preDotState = 1;
				};
			}else if(!(startDot===-1)){
				preDotState = -1;
			};
		};
		
		if(!(end===-1)){
			if(startDot===-1 || preDotState === 0 || (preDotState===1 && startDot===end-1 && startDot===startPart+1)){
				ret.base = ret.name = path.slice(startPart, end);
			}else{
				ret.name = path.slice(startPart, startDot);
				ret.base = path.slice(startPart, end);
				ret.ext = path.slice(startDot, end);
			};
		};
		
		if(startPart > 0 && !(startPart===rootEnd)){
			ret.dir = path.slice(0, startPart - 1);
		}else{
			ret.dir = ret.root;
		};
		
		return ret;
	}
};

function project_directory(){
	var path = project_path();
	var c = '';
	var s = 0;
	var end = -1;
	for(var i = path.length - 1; i > -1; i--){
		if(_path.isPathSeparator(path.charCodeAt(i))){
			s++;
			if(c==='appsremote' || c==='appslocal'){
				end = i;
				break;
			}else{
				if(s===1){
					end = i;
					if(!(c==='project.xml')){
						break;
					};
				};
				if(s===2 && !(c==='engine')){
					break;
				};
				c = '';
			};
		}else{
			c = path.charAt(i) + c;
		};
	};
	return end===-1 ? _path.dirname(path) : path.slice(0, end);
};

function installation_path(){
	return JSON.parse(native("filesystem", "fileinfo", "settings.ini")).directory;
};

function _get_system_data(){
	RANDOM_FILE = "temp_" + rand() + ".bat";

	native("filesystem","writefile",JSON.stringify({path:RANDOM_FILE,value:"chcp 65001\r\nSET",base64:false,append:false}));

	native_async("processmanager","start",JSON.stringify({location:RANDOM_FILE,working_folder:"",waitfinish:true,arguments:"",version:2}))!

	var data_list = base64_decode(_result().split(",")[0]).split('\r\n').slice(2,-1);

	sleep(1000)!

	native("filesystem","removefile",RANDOM_FILE);
	
	SYSTEM_ENV_DATA = {};
	
	for(var i = 0; i < data_list.length; i++){
		if(data_list[i].indexOf('=') > -1){
			var data = data_list[i].split('=');
			var name = data[0];
			var value = data[1];
			SYSTEM_ENV_DATA[name] = value.indexOf("\\") > -1 ? _path.normalize(value) : value;
		};
	};
	
	if(SYSTEM_ENV_DATA["USERPROFILE"]){
		["Desktop","Downloads","Documents","Pictures","Videos","Music","Favorites"].forEach(function(e){SYSTEM_ENV_DATA[e] = _path.join(SYSTEM_ENV_DATA["USERPROFILE"], e)});
	};
};

function _get_system_path(){
	var name = _function_argument("name");
	
	_if(typeof SYSTEM_ENV_DATA=="undefined",function(){
		_call_function(_get_system_data,{})!
		_result_function();
	})!
	
	var labels = {
		"App Data":"APPDATA",
		"Local App Data": "LOCALAPPDATA",
		"Program Files":"ProgramFiles",
		"Program Files (x86)":"ProgramFiles(x86)",
		"Program Data":"ProgramData",
		"Public":"PUBLIC",
		"System Drive":"SystemDrive",
		"System Root":"SystemRoot",
		"Windows Directory":"windir",
		"Temp":"TEMP",
		"User Name":"USERNAME",
		"User Profile":"USERPROFILE",
		"Computer Name":"COMPUTERNAME"
	};
	
	var label = _avoid_nilb(labels[name], name);
	
	if(_is_nilb(SYSTEM_ENV_DATA[label])){
		fail(_K=="ru" ? 'Не удалось найти путь "' + name + '" в системных данных.' : 'Could not find path "' + name + '" in system data.');
	};
	
	_function_return(SYSTEM_ENV_DATA[label]);
};