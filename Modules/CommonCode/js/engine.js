_STR_WHITESPACE = '\\x20\\u00a0\\u1680\\u180e\\u2000\\u2001\\u2002\\u2003\\u2004\\u2005\\u2006\\u2007\\u2008\\u2009\\u200a\\u202f\\u205f\\u2028\\u2029\\u3000\\uFEFF\\xA0';
_STR_SPECCHARS = '\\f\\n\\r\\t\\v';

_L["Data"] = {"ru":"Данные"};
_L["Array"] = {"ru":"Масив"};
_L["String"] = {"ru":"Строка"};
_L["Substring"] = {"ru":"Подстрока"};
_L["Trim left"] = {"ru":"Обрезать слева"};
_L["Trim right"] = {"ru":"Обрезать справа"};
_L["Trim characters"] = {"ru":"Обрезать символы"};
_L["Remove characters"] = {"ru":"Удалять символы"};
_L["Sort ascending"] = {"ru":"Сортировать по возрастанию"};
_L["Replace characters with space"] = {"ru":"Заменять многочисленные пробелы на одиночные"};
_L["Replace multiple spaces with single ones"] = {"ru":"Заменять многочисленные пробелы на одиночные"};

function _tr_type_name(name){
	if(Array.isArray(name)){
		if(name.length > 1){
			arr = name.map(function(t){return _tr_type_name(t)});
			return arr.slice(0, arr.length - 1).join(', ') + (_K==="ru" ? ' или ' : ' or ') + arr.slice(-1);
		}else{
			name = name[0];
		};
	};
	if(_K!=="ru"){
		return name;
	};
	switch(name){
		case 'string':
			return 'строкой';
		case 'number':
			return 'числом';
		case 'boolean':
			return 'логическим выражением';
		case 'function':
			return 'функцией';
		case 'object':
			return 'объектом';
		case 'array':
			return 'массивом';
		case 'date':
			return 'датой';
		case 'regexp':
			return 'регулярным выражением';
		default:
			return name;
	};
};
function _get_type(value){
	var value_type = typeof value; 
	return value===null ? 'null' : (value_type==='object' || value_type==='function' ? (Array.isArray(value) ? 'array' : (value instanceof Date ? 'date' : (value instanceof RegExp ? 'regexp' : value_type))) : value_type);
};
function _validate_argument_type(value, type, name, act){
	var value_type = _get_type(value);
	if(Array.isArray(type) ? type.filter(function(t){return value_type===t}).length < 1 : value_type!==type){
		fail((_is_nilb(act) ? '' : (act + ': ')) + (_K==="ru" ? ('Аргумент "' + tr(name) + '" должен быть ' + _tr_type_name(type) + ', а не ') : ('The "' + tr(name) + '" argument must be a ' + _tr_type_name(type) + ', not ')) + _tr_type_name(value_type));
	};
};
function _is_nil(data){
	return typeof data==="undefined" || data===null;
};
function _is_nilb(data){
	return _is_nil(data) || data==="";
};
function _avoid_nil(data, def){
	return _is_nil(data) ? _avoid_nil(def, "") : data;
};
function _avoid_nilb(data, def){
	return _is_nilb(data) ? _avoid_nil(def, "") : data;
};
function _is_string(data){
	return typeof data==="string";
};
function _is_not_empty_string(data){
	return _is_string(data) && data.length > 0;
};
function _is_json_string(str){
	if(_is_not_empty_string(str) && ((str.slice(0,1)=="[" && str.slice(-1)=="]") || (str.slice(0,1)=="{" && str.slice(-1)=="}"))){
		try{
			JSON.parse(str);
		}catch(e){
			return false;
		};
		return true;
	}else{
		return false;
	};
};
function _escape_regexp(str){
	_validate_argument_type(str, 'string', 'String', '_escape_regexp');
	return (str && /[\\^$.*+?()[\]{}|]/.test(str)) ? str.replace(new RegExp('([\\][^' + _STR_WHITESPACE + _STR_SPECCHARS + ']|[().^$*+?[\\]{}|])', 'g'), '\\$&') : (str || '');
};
function _to_arr(data){
	_validate_argument_type(data, ['string','array'], 'Data', '_to_arr');
	return (data==="" || typeof data=="object") ? data : (_is_json_string(data) ? JSON.parse(data) : data.split(/,\s|,/));
};
function _uniq_arr(arr){
	_validate_argument_type(arr, 'array', 'Array', '_uniq_arr');
	return arr.filter(function(e,i){return arr.indexOf(e)===i});
};
function _natural_compare(a, b){
	var i = undefined;
	var codeA = undefined;
	var codeB = 1;
	var posA = 0;
	var posB = 0;
	var alphabet = String.alphabet;

	function getCode(str, pos, code){
		if(code){
			for(i = pos; code = getCode(str, i), code < 76 && code > 65;){
				++i;
			};
			return +str.slice(pos - 1, i)
		};
		code = alphabet && alphabet.indexOf(str.charAt(pos));
		return code > -1 ? code + 76 : ((code = str.charCodeAt(pos) || 0), code < 45 || code > 127) ? code
			: code < 46 ? 65               // -
			: code < 48 ? code - 1
			: code < 58 ? code + 18        // 0-9
			: code < 65 ? code - 11
			: code < 91 ? code + 11        // A-Z
			: code < 97 ? code - 37
			: code < 123 ? code + 5        // a-z
			: code - 63
	};

	if((a+="") != (b+="")){
		for(;codeB;){
			codeA = getCode(a, posA++);
			codeB = getCode(b, posB++);

			if(codeA < 76 && codeB < 76 && codeA > 66 && codeB > 66){
				codeA = getCode(a, posA, posA);
				codeB = getCode(b, posB, posA = i);
				posB = i;
			};

			if(codeA != codeB){
				return (codeA < codeB) ? -1 : 1;
			};
		};
	};
	return 0;
}
function _sort_arr(arr, asc){
	_validate_argument_type(arr, 'array', 'Array', '_sort_arr');
	asc = _avoid_nilb(asc, true);
	_validate_argument_type(asc, ['boolean','number'], 'Sort ascending', '_sort_arr');
	return arr.sort(function(a, b){
		a = typeof a==="string" ? (isNaN(a) ? a.toLowerCase() : Number(a)) : a;
		b = typeof b==="string" ? (isNaN(b) ? b.toLowerCase() : Number(b)) : b;
		if(typeof a==="number" && typeof b==="number"){
			if(asc ? (a > b) : (a < b)){
				return 1;
			};
			if(asc ? (a < b) : (a > b)){
				return -1;
			};
			return 0;
		}else{
			return _natural_compare(asc ? a : b, asc ? b : a);
		};
	});
};
function _trim_left(str, chars){
	var act = '_trim_left';
	_validate_argument_type(str, 'string', 'String', act);
	chars = _avoid_nilb(chars, _STR_WHITESPACE + _STR_SPECCHARS);
	_validate_argument_type(chars, 'string', 'Trim characters', act);
	return str.replace(new RegExp('^[' + _escape_regexp(chars) + ']+', 'g'), '');
};
function _trim_right(str, chars){
	var act = '_trim_right';
	_validate_argument_type(str, 'string', 'String', act);
	chars = _avoid_nilb(chars, _STR_WHITESPACE + _STR_SPECCHARS);
	_validate_argument_type(chars, 'string', 'Trim characters', act);
	return str.replace(new RegExp('[' + _escape_regexp(chars) + ']+$', 'g'), '');
};
function _trim(str, chars, left, right){
	var act = '_trim';
	_validate_argument_type(str, 'string', 'String', act);
	chars = _avoid_nilb(chars, _STR_WHITESPACE + _STR_SPECCHARS);
	_validate_argument_type(chars, 'string', 'Trim characters', act);
	left = _avoid_nilb(left, true);
	_validate_argument_type(left, ['boolean','number'], 'Trim left', act);
	right = _avoid_nilb(right, true);
	_validate_argument_type(right, ['boolean','number'], 'Trim right', act);
	if(left){
		str = _trim_left(str, chars);
	};
	if(right){
		str = _trim_right(str, chars);
	};
	return str;
};
function _trim_arr(arr, chars, left, right){
	var act = '_trim_arr';
	_validate_argument_type(arr, 'array', 'Array', act);
	chars = _avoid_nilb(chars, _STR_WHITESPACE + _STR_SPECCHARS);
	_validate_argument_type(chars, 'string', 'Trim characters', act);
	left = _avoid_nilb(left, true);
	_validate_argument_type(left, ['boolean','number'], 'Trim left', act);
	right = _avoid_nilb(right, true);
	_validate_argument_type(right, ['boolean','number'], 'Trim right', act);
	return arr.map(function(e){return _trim(e, chars, left, right)});
};
function _clean(str, chars_to_delete, chars_to_space, multiple_spaces){
	var act = '_clean';
	_validate_argument_type(str, 'string', 'String', act);
	chars_to_space = _avoid_nil(chars_to_space, '\\r\\n\\f');
	_validate_argument_type(chars_to_space, 'string', 'Replace characters with space', act);
	chars_to_delete = _avoid_nil(chars_to_delete, '\\t\\v');
	_validate_argument_type(chars_to_delete, 'string', 'Remove characters', act);
	multiple_spaces = _avoid_nilb(multiple_spaces, true);
	_validate_argument_type(multiple_spaces, ['boolean','number'], 'Replace multiple spaces with single ones', act);
	if(chars_to_space){
		str = str.replace(new RegExp('[' + _escape_regexp(chars_to_space) + ']+', 'g'), ' ');
	};
	if(chars_to_delete){
		str = str.replace(new RegExp('[' + _escape_regexp(chars_to_delete) + ']+', 'g'), '');
	};
	str = _trim(str);
	if(multiple_spaces){
		str = str.replace(new RegExp('[' + _STR_WHITESPACE + ']+', 'g'), ' ');
	}else{
		str = str.replace(new RegExp('[' + _STR_WHITESPACE + ']', 'g'), ' ');
	};
	str = str.replace(new RegExp('(?:[' + _STR_WHITESPACE + ']+)?([' + _STR_SPECCHARS + ']+)(?:[' + _STR_WHITESPACE + ']+)?', 'g'), '$1');
	return str;
};
function _clean_arr(arr, chars_to_delete, chars_to_space, multiple_spaces){
	var act = '_clean_arr';
	_validate_argument_type(arr, 'array', 'Array', act);
	chars_to_space = _avoid_nil(chars_to_space, '\\r\\n\\f');
	_validate_argument_type(chars_to_space, 'string', 'Replace characters with space', act);
	chars_to_delete = _avoid_nil(chars_to_delete, '\\t\\v');
	_validate_argument_type(chars_to_delete, 'string', 'Remove characters', act);
	multiple_spaces = _avoid_nilb(multiple_spaces, true);
	_validate_argument_type(multiple_spaces, ['boolean','number'], 'Replace multiple spaces with single ones', act);
	return arr.map(function(e){return _clean(e, chars_to_delete, chars_to_space, multiple_spaces)});
};
function _starts_with(str, sub, from){
	var act = '_starts_with';
	_validate_argument_type(str, 'string', 'String', act);
	_validate_argument_type(sub, ['string','number'], 'Substring', act);
	if(typeof sub==='number'){
		sub = _no_exponents(sub);
	};
	from = _avoid_nilb(from, 0);
	_validate_argument_type(from, 'number', 'From index', act);
	return str.indexOf(sub) === from;
};
function _ends_with(str, sub, lenght){
	var act = '_ends_with';
	_validate_argument_type(str, 'string', 'String', act);
	_validate_argument_type(sub, ['string','number'], 'Substring', act);
	if(typeof sub==='number'){
		sub = _no_exponents(sub);
	};
	lenght = _avoid_nilb(lenght, str.length);
	_validate_argument_type(lenght, 'number', 'Lenght', act);
	if(lenght > str.length){
		lenght = str.length;
	};
	lenght -= sub.length;
	var last_index = str.indexOf(sub, lenght);
	return last_index !== -1 && last_index === lenght;
};