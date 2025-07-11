_L["Index"] = {"ru":"Индекс"};
_L["Number"] = {"ru":"Число"};
_L["Lenght"] = {"ru":"Длина"};
_L["Template"] = {"ru":"Шаблон"};
_L["To index"] = {"ru":"До индекса"};
_L["From index"] = {"ru":"С индекса"};
_L["Separator"] = {"ru":"Разделитель"};
_L["Replace To"] = {"ru":"Заменить На"};
_L["Replace From"] = {"ru":"Заменить Из"};
_L["List of strings"] = {"ru":"Список строк"};
_L["List of items"] = {"ru":"Список элементов"};
_L["Left substring"] = {"ru":"Левая подстрока"};
_L["Right substring"] = {"ru":"Правая подстрока"};
_L["Allowed Chars"] = {"ru":"Разрешенные Символы"};
_L["Convert types"] = {"ru":"Преобразовывать типы"};
_L["List of separators"] = {"ru":"Список разделителей"};
_L["Remove empty lines"] = {"ru":"Удалить пустые строки"};
_L["Thousandth separator"] = {"ru":"Разделитель тысячных"};
_L["Latinize Cyrillic"] = {"ru":"Латинизировать кириллицу"};
_L["Insertable substring"] = {"ru":"Вставляемая подстрока"};
_L["Result String Length"] = {"ru":"Длина Генерируемой Строки"};
_L["Capitalize all words"] = {"ru":"Сделать все слова заглавными"};
_L["Decimal separator"] = {"ru":"Разделитель целой и дробной части"};
_L["Number of decimal places"] = {"ru":"Количество знаков в дробной части"};
_L["Number of characters to remove"] = {"ru":"Количество удаляемых символов"};
_L["All other letters in lowercase"] = {"ru":"Все остальные буквы в нижний регистр"};
_L["Minimum number of characters in the number"] = {"ru":"Минимальное количество символов в числе"};

function _from_string(str){
    return _is_not_empty_string(str) ? (isNaN(str) ? (str=="true" || str=="false" ? str=="true" : (_is_json_string(str) ? JSON.parse(str) : (str=="null" ? null : (str=="undefined" ? undefined : str)))) : Number(str)) : str;
};
function _to_string(data){
	return _is_string(data) ? data : ((typeof data=="object" && !(data instanceof Date) && !(data instanceof RegExp)) ? JSON.stringify(data) : (typeof data=="number" ? _no_exponents(data) : ((data instanceof RegExp) ? _escape_regexp(data.source) : data.toString())));
};
function _no_exponents(num){
	_validate_argument_type(num, ['string','number'], 'Number', '_no_exponents');
	var data = String(num).split(/[eE]/);
	if(data.length == 1){return data[0]};

	var z = '';
	var sign = num < 0 ? '-' : '';
	var str = data[0].replace('.', '');
	var mag = Number(data[1]) + 1;

	if(mag < 0){
		z = sign + '0.';
		while (mag++) z += '0';
		return z + str.replace(/^\-/, '');
	};
	mag -= str.length;
	while (mag--) z += '0';
	return str + z;
};
function _to_number(str, dec, dsep, tsep){
	var act = '_to_number';
	_validate_argument_type(str, ['string','number'], 'String', act);
	str = _to_string(_avoid_nil(str));
	dec = _avoid_nilb(dec, -1);
	dsep = _avoid_nil(dsep, '.');
	tsep = _avoid_nil(tsep, ',');
	_validate_argument_type(dec, 'number', 'Number of decimal places', act);
	_validate_argument_type(dsep, 'string', 'Decimal separator', act);
	_validate_argument_type(tsep, 'string', 'Thousandth separator', act);
	
	str = _replace_string(str, _avoid_nil(tsep, ','), '');
	str = _replace_string(str, _avoid_nil(dsep, '.'), '.');
	
	if(dec===-1){return Number(str)};
	
	var factor = Math.pow(10, isFinite(dec) ? dec : 0);
	return Math.round(str * factor)/factor;
};
function _number_format(num, dec, digits, dsep, tsep){
	var act = '_number_format';
	_validate_argument_type(num, ['string','number'], 'Number', act);
	if(_is_nil(num) || isNaN(num)){return ''};
	num = typeof num==="number" ? num : Number(num);
	dec = _avoid_nilb(dec, -1);
	_validate_argument_type(dec, 'number', 'Number of decimal places', act);
	digits = _avoid_nilb(digits, 0);
	_validate_argument_type(digits, 'number', 'Minimum number of characters in the number', act);
	dsep = _avoid_nil(dsep, '.');
	_validate_argument_type(dsep, 'string', 'Decimal separator', act);
	tsep = _avoid_nil(tsep, ',');
	_validate_argument_type(tsep, 'string', 'Thousandth separator', act);

	num = dec===-1 ? _no_exponents(num) : num.toFixed(dec);

	var parts = num.split('.');
	var fnums = parts[0];
	var decimals = parts[1] ? dsep + parts[1] : '';
	
	if(digits > 0 && fnums.length < digits){
		while(fnums.length < digits){
			fnums = '0' + fnums;
		};
	};
	
	if(fnums.length < 4 || tsep===''){
		return fnums + decimals;
	};
	
	var c = '';
	var n = 0;
	for(var i = fnums.length - 1; i > -1; i--){
		n++
		c = fnums.charAt(i) + c;
		if(n===3 && i!==0){
			c = tsep + c;
			n = 0;
		};
	};

	return c + decimals;
};
function _count_substrings(str, sub){
	var act = '_count_substrings';
	_validate_argument_type(str, 'string', 'String', act);
	_validate_argument_type(sub, ['string','number'], 'Substring', act);
	
	if(typeof sub==='number'){
		sub = _no_exponents(sub);
	};
	if(sub.length <= 0){
		return (str.length + 1)
	};
	var n = 0;
	var pos = 0;
	var step = sub.length;
	while(pos != -1){
        pos = str.indexOf(sub, pos);
        if(pos > -1){
            ++n;
            pos += step;
        };
    };
    return n;
};
function _get_substring(str, from, to){
	var act = '_get_substring';
	_validate_argument_type(str, 'string', 'String', act);
	from = _avoid_nilb(from, 0);
	_validate_argument_type(from, 'number', 'From index', act);
	if(_is_nilb(to)){
		return str.slice(from);
	}else{
		_validate_argument_type(to, 'number', 'To index', act);
		return str.slice(from, to);
	};
};
function _get_substring_between(str, left, right){
	var act = '_get_substring_between';
	_validate_argument_type(str, 'string', 'String', '_get_substring_between');
    left = _avoid_nil(left);
    right = _avoid_nil(right);
	_validate_argument_type(left, ['string','number'], 'Left substring', act);
	_validate_argument_type(right, ['string','number'], 'Right substring', act);
	
	if(typeof left==='number'){
		left = _no_exponents(left);
	};
	if(typeof right==='number'){
		right = _no_exponents(right);
	};
	
	var li = left==="" ? -1 : str.indexOf(left);
	var from = li===-1 ? 0 : (li + left.length);
	var to = right==="" ? -1 : str.indexOf(right, from);
	
	if(li===-1 && to===-1){
		return str;
	}else{
		if(to===-1){
			return str.substring(from);
		}else{
			return str.slice(from, to);
		};
	};
};
function _splice_string(str, from, count, add){
	var act = '_splice_string';
	_validate_argument_type(str, 'string', 'String', act);
	from = _avoid_nilb(from, 0);
	count = _avoid_nilb(count, 0);
	add = _avoid_nil(add);
	_validate_argument_type(from, 'number', 'From index', act);
	_validate_argument_type(count, 'number', 'Number of characters to remove', act);
	_validate_argument_type(add, ['string','number'], 'Insertable substring', act);
	if(typeof add==='number'){
		add = _no_exponents(add);
	};
	if(count < 0){
		count = 0;
	};
	if(from < 0){
		from = str.length + from;
		if(from < 0){
			from = 0;
		};
	};
	return str.slice(0, from) + add + str.slice(from + count);
};
function _to_lower_case(str){
	_validate_argument_type(str, 'string', 'String', '_to_lower_case');
	return str.toLowerCase();
};
function _to_upper_case(str){
	_validate_argument_type(str, 'string', 'String', '_to_upper_case');
	return str.toUpperCase();
};
function _string_contains(str, sub, from){
	var act = '_string_contains';
	_validate_argument_type(str, 'string', 'String', act);
	_validate_argument_type(sub, ['string','number'], 'Substring', act);
	if(typeof sub==='number'){
		sub = _no_exponents(sub);
	};
	if(_is_nilb(from)){
		return str.indexOf(sub) > -1;
	}else{
		_validate_argument_type(from, 'number', 'From index', act);
		return str.indexOf(sub, from) > -1;
	};
};
function _string_length(str){
	_validate_argument_type(str, 'string', 'String', '_string_length');
	return str.length;
};
function _capitalize(str, all, lower){
	var act = '_capitalize';
	_validate_argument_type(str, 'string', 'String', act);
	all = _avoid_nilb(all, false);
	lower = _avoid_nilb(lower, false);
	_validate_argument_type(all, ['boolean','number'], 'Capitalize all words', act);
	_validate_argument_type(lower, ['boolean','number'], 'All other letters in lowercase', act);
	return (lower ? str.toLowerCase() : str).replace(new RegExp("(?:^|\\s|[\"'([{])+\\S", (all ? "g" : "")), function(match){return match.toUpperCase()});
};
function _sentences(str){
	_validate_argument_type(str, 'string', 'String', '_sentences');
    return _clean(str).replace(/(\.+|\:|\!|\?)(\"*|\'*|\)*|}*|]*)(\s|\n|\r|\r\n)/gm, "$1$2|+|").split("|+|");
};
function _count_words(str){
	_validate_argument_type(str, 'string', 'String', '_count_words');
    return _words(str).length;
};
function _find_substring(str, sub, from){
	var act = '_find_substring';
	_validate_argument_type(str, 'string', 'String', act);
	_validate_argument_type(sub, ['string','number'], 'Substring', act);
	if(typeof sub==='number'){
		sub = _no_exponents(sub);
	};
	if(_is_nilb(from)){
		return str.indexOf(sub);
	}else{
		_validate_argument_type(from, 'number', 'From index', act);
		return str.indexOf(sub, from);
	};
};
function _insert_substring(str, index, sub){
	var act = '_insert_substring';
	_validate_argument_type(str, 'string', 'String', act);
	_validate_argument_type(sub, ['string','number'], 'Insertable substring', act);
	if(typeof sub==='number'){
		sub = _no_exponents(sub);
	};
	index = _avoid_nilb(index, 0);
	_validate_argument_type(index, 'number', 'Index', act);
	return _splice_string(str, index, 0, sub);
};
function _split_string(str, sep, remove, limit){
	var act = '_split_string';
	_validate_argument_type(str, 'string', 'String', act);
	sep = _avoid_nil(sep);
	_validate_argument_type(sep, ['string','regexp'], 'Separator', act);
	remove = _avoid_nilb(remove, true);
	_validate_argument_type(remove, ['boolean','number'], 'Remove empty lines', act);
	limit = _avoid_nilb(limit, 0);
	_validate_argument_type(limit, 'number', 'Limit', act);
	
    var list = limit ? str.split(sep, limit) : str.split(sep);
	if(remove){
		list = list.filter(function(e){return !_is_nilb(e)});
	};
	return list;
};
function _join_strings(list, sep){
	var act = '_join_strings';
	list = _avoid_nilb(list, []);
	_validate_argument_type(list, 'array', 'List of strings', act);
	sep = _avoid_nil(sep);
	_validate_argument_type(sep, 'string', 'Separator', act);
	return list.join(sep);
};
function _latinize(str, cyrillic){
	var act = '_latinize';
	_validate_argument_type(str, 'string', 'String', act);
	if(str===''){
		return '';
	};
	cyrillic = _avoid_nilb(cyrillic, true);
	_validate_argument_type(cyrillic, ['boolean','number'], 'Latinize Cyrillic', act);
	
	var diacritics_map = {'Á':'A','Ă':'A','Ắ':'A','Ặ':'A','Ằ':'A','Ẳ':'A','Ẵ':'A','Ǎ':'A','Â':'A','Ấ':'A','Ậ':'A','Ầ':'A','Ẩ':'A','Ẫ':'A','Ä':'A','Ǟ':'A','Ȧ':'A','Ǡ':'A','Ạ':'A','Ȁ':'A','À':'A','Ả':'A','Ȃ':'A','Ā':'A','Ą':'A','Å':'A','Ǻ':'A','Ḁ':'A','Ⱥ':'A','Ã':'A','Ꜳ':'AA','Æ':'AE','Ǽ':'AE','Ǣ':'AE','Ꜵ':'AO','Ꜷ':'AU','Ꜹ':'AV','Ꜻ':'AV','Ꜽ':'AY','Ḃ':'B','Ḅ':'B','Ɓ':'B','Ḇ':'B','Ƀ':'B','Ƃ':'B','Ć':'C','Č':'C','Ç':'C','Ḉ':'C','Ĉ':'C','Ċ':'C','Ƈ':'C','Ȼ':'C','Ď':'D','Ḑ':'D','Ḓ':'D','Ḋ':'D','Ḍ':'D','Ɗ':'D','Ḏ':'D','ǲ':'D','ǅ':'D','Đ':'D','Ð':'D','Ƌ':'D','Ǳ':'DZ','Ǆ':'DZ','É':'E','Ĕ':'E','Ě':'E','Ȩ':'E','Ḝ':'E','Ê':'E','Ế':'E','Ệ':'E','Ề':'E','Ể':'E','Ễ':'E','Ḙ':'E','Ë':'E','Ė':'E','Ẹ':'E','Ȅ':'E','È':'E','Ẻ':'E','Ȇ':'E','Ē':'E','Ḗ':'E','Ḕ':'E','Ę':'E','Ɇ':'E','Ẽ':'E','Ḛ':'E','Ꝫ':'ET','Ḟ':'F','Ƒ':'F','Ǵ':'G','Ğ':'G','Ǧ':'G','Ģ':'G','Ĝ':'G','Ġ':'G','Ɠ':'G','Ḡ':'G','Ǥ':'G','Ḫ':'H','Ȟ':'H','Ḩ':'H','Ĥ':'H','Ⱨ':'H','Ḧ':'H','Ḣ':'H','Ḥ':'H','Ħ':'H','Í':'I','Ĭ':'I','Ǐ':'I','Î':'I','Ï':'I','Ḯ':'I','İ':'I','Ị':'I','Ȉ':'I','Ì':'I','Ỉ':'I','Ȋ':'I','Ī':'I','Į':'I','Ɨ':'I','Ĩ':'I','Ḭ':'I','І':'I','Ꝺ':'D','Ꝼ':'F','Ᵹ':'G','Ꞃ':'R','Ꞅ':'S','Ꞇ':'T','Ꝭ':'IS','Ĵ':'J','Ɉ':'J','Ḱ':'K','Ǩ':'K','Ķ':'K','Ⱪ':'K','Ꝃ':'K','Ḳ':'K','Ƙ':'K','Ḵ':'K','Ꝁ':'K','Ꝅ':'K','Ĺ':'L','Ƚ':'L','Ľ':'L','Ļ':'L','Ḽ':'L','Ḷ':'L','Ḹ':'L','Ⱡ':'L','Ꝉ':'L','Ḻ':'L','Ŀ':'L','Ɫ':'L','ǈ':'L','Ł':'L','Ǉ':'LJ','Ḿ':'M','Ṁ':'M','Ṃ':'M','Ɱ':'M','Ń':'N','Ň':'N','Ņ':'N','Ṋ':'N','Ṅ':'N','Ṇ':'N','Ǹ':'N','Ɲ':'N','Ṉ':'N','Ƞ':'N','ǋ':'N','Ñ':'N','Ǌ':'NJ','Ó':'O','Ŏ':'O','Ǒ':'O','Ô':'O','Ố':'O','Ộ':'O','Ồ':'O','Ổ':'O','Ỗ':'O','Ö':'O','Ȫ':'O','Ȯ':'O','Ȱ':'O','Ọ':'O','Ő':'O','Ȍ':'O','Ò':'O','Ỏ':'O','Ơ':'O','Ớ':'O','Ợ':'O','Ờ':'O','Ở':'O','Ỡ':'O','Ȏ':'O','Ꝋ':'O','Ꝍ':'O','Ō':'O','Ṓ':'O','Ṑ':'O','Ɵ':'O','Ǫ':'O','Ǭ':'O','Ø':'O','Ǿ':'O','Õ':'O','Ṍ':'O','Ṏ':'O','Ȭ':'O','Ƣ':'OI','Ꝏ':'OO','Ɛ':'E','Ɔ':'O','Ȣ':'OU','Ṕ':'P','Ṗ':'P','Ꝓ':'P','Ƥ':'P','Ꝕ':'P','Ᵽ':'P','Ꝑ':'P','Ꝙ':'Q','Ꝗ':'Q','Ŕ':'R','Ř':'R','Ŗ':'R','Ṙ':'R','Ṛ':'R','Ṝ':'R','Ȑ':'R','Ȓ':'R','Ṟ':'R','Ɍ':'R','Ɽ':'R','Ꜿ':'C','Ǝ':'E','Ś':'S','Ṥ':'S','Š':'S','Ṧ':'S','Ş':'S','Ŝ':'S','Ș':'S','Ṡ':'S','Ṣ':'S','Ṩ':'S','ß':'ss','Ť':'T','Ţ':'T','Ṱ':'T','Ț':'T','Ⱦ':'T','Ṫ':'T','Ṭ':'T','Ƭ':'T','Ṯ':'T','Ʈ':'T','Ŧ':'T','Ɐ':'A','Ꞁ':'L','Ɯ':'M','Ʌ':'V','Ꜩ':'TZ','Ú':'U','Ŭ':'U','Ǔ':'U','Û':'U','Ṷ':'U','Ü':'U','Ǘ':'U','Ǚ':'U','Ǜ':'U','Ǖ':'U','Ṳ':'U','Ụ':'U','Ű':'U','Ȕ':'U','Ù':'U','Ủ':'U','Ư':'U','Ứ':'U','Ự':'U','Ừ':'U','Ử':'U','Ữ':'U','Ȗ':'U','Ū':'U','Ṻ':'U','Ų':'U','Ů':'U','Ũ':'U','Ṹ':'U','Ṵ':'U','Ꝟ':'V','Ṿ':'V','Ʋ':'V','Ṽ':'V','Ꝡ':'VY','Ẃ':'W','Ŵ':'W','Ẅ':'W','Ẇ':'W','Ẉ':'W','Ẁ':'W','Ⱳ':'W','Ẍ':'X','Ẋ':'X','Ý':'Y','Ŷ':'Y','Ÿ':'Y','Ẏ':'Y','Ỵ':'Y','Ỳ':'Y','Ƴ':'Y','Ỷ':'Y','Ỿ':'Y','Ȳ':'Y','Ɏ':'Y','Ỹ':'Y','Ї':'YI','Ź':'Z','Ž':'Z','Ẑ':'Z','Ⱬ':'Z','Ż':'Z','Ẓ':'Z','Ȥ':'Z','Ẕ':'Z','Ƶ':'Z','Þ':'TH','Ĳ':'IJ','Œ':'OE','ᴀ':'A','ᴁ':'AE','ʙ':'B','ᴃ':'B','ᴄ':'C','ᴅ':'D','ᴇ':'E','ꜰ':'F','ɢ':'G','ʛ':'G','ʜ':'H','ɪ':'I','ʁ':'R','ᴊ':'J','ᴋ':'K','ʟ':'L','ᴌ':'L','ᴍ':'M','ɴ':'N','ᴏ':'O','ɶ':'OE','ᴐ':'O','ᴕ':'OU','ᴘ':'P','ʀ':'R','ᴎ':'N','ᴙ':'R','ꜱ':'S','ᴛ':'T','ⱻ':'E','ᴚ':'R','ᴜ':'U','ᴠ':'V','ᴡ':'W','ʏ':'Y','ᴢ':'Z','á':'a','ă':'a','ắ':'a','ặ':'a','ằ':'a','ẳ':'a','ẵ':'a','ǎ':'a','â':'a','ấ':'a','ậ':'a','ầ':'a','ẩ':'a','ẫ':'a','ä':'a','ǟ':'a','ȧ':'a','ǡ':'a','ạ':'a','ȁ':'a','à':'a','ả':'a','ȃ':'a','ā':'a','ą':'a','ᶏ':'a','ẚ':'a','å':'a','ǻ':'a','ḁ':'a','ⱥ':'a','ã':'a','ꜳ':'aa','æ':'ae','ǽ':'ae','ǣ':'ae','ꜵ':'ao','ꜷ':'au','ꜹ':'av','ꜻ':'av','ꜽ':'ay','ḃ':'b','ḅ':'b','ɓ':'b','ḇ':'b','ᵬ':'b','ᶀ':'b','ƀ':'b','ƃ':'b','ɵ':'o','ć':'c','č':'c','ç':'c','ḉ':'c','ĉ':'c','ɕ':'c','ċ':'c','ƈ':'c','ȼ':'c','ď':'d','ḑ':'d','ḓ':'d','ȡ':'d','ḋ':'d','ḍ':'d','ɗ':'d','ᶑ':'d','ḏ':'d','ᵭ':'d','ᶁ':'d','đ':'d','ɖ':'d','ƌ':'d','ð':'d','ı':'i','ȷ':'j','ɟ':'j','ʄ':'j','ǳ':'dz','ǆ':'dz','é':'e','ĕ':'e','ě':'e','ȩ':'e','ḝ':'e','ê':'e','ế':'e','ệ':'e','ề':'e','ể':'e','ễ':'e','ḙ':'e','ë':'e','ė':'e','ẹ':'e','ȅ':'e','è':'e','ẻ':'e','ȇ':'e','ē':'e','ḗ':'e','ḕ':'e','ⱸ':'e','ę':'e','ᶒ':'e','ɇ':'e','ẽ':'e','ḛ':'e','ꝫ':'et','ḟ':'f','ƒ':'f','ᵮ':'f','ᶂ':'f','ǵ':'g','ğ':'g','ǧ':'g','ģ':'g','ĝ':'g','ġ':'g','ɠ':'g','ḡ':'g','ᶃ':'g','ǥ':'g','ḫ':'h','ȟ':'h','ḩ':'h','ĥ':'h','ⱨ':'h','ḧ':'h','ḣ':'h','ḥ':'h','ɦ':'h','ẖ':'h','ħ':'h','ƕ':'hv','í':'i','ĭ':'i','ǐ':'i','î':'i','ï':'i','ḯ':'i','ị':'i','ȉ':'i','ì':'i','ỉ':'i','ȋ':'i','ī':'i','į':'i','ᶖ':'i','ɨ':'i','ĩ':'i','ḭ':'i','і':'i','ꝺ':'d','ꝼ':'f','ᵹ':'g','ꞃ':'r','ꞅ':'s','ꞇ':'t','ꝭ':'is','ǰ':'j','ĵ':'j','ʝ':'j','ɉ':'j','ḱ':'k','ǩ':'k','ķ':'k','ⱪ':'k','ꝃ':'k','ḳ':'k','ƙ':'k','ḵ':'k','ᶄ':'k','ꝁ':'k','ꝅ':'k','ĺ':'l','ƚ':'l','ɬ':'l','ľ':'l','ļ':'l','ḽ':'l','ȴ':'l','ḷ':'l','ḹ':'l','ⱡ':'l','ꝉ':'l','ḻ':'l','ŀ':'l','ɫ':'l','ᶅ':'l','ɭ':'l','ł':'l','ǉ':'lj','ſ':'s','ẜ':'s','ẛ':'s','ẝ':'s','ḿ':'m','ṁ':'m','ṃ':'m','ɱ':'m','ᵯ':'m','ᶆ':'m','ń':'n','ň':'n','ņ':'n','ṋ':'n','ȵ':'n','ṅ':'n','ṇ':'n','ǹ':'n','ɲ':'n','ṉ':'n','ƞ':'n','ᵰ':'n','ᶇ':'n','ɳ':'n','ñ':'n','ǌ':'nj','ó':'o','ŏ':'o','ǒ':'o','ô':'o','ố':'o','ộ':'o','ồ':'o','ổ':'o','ỗ':'o','ö':'o','ȫ':'o','ȯ':'o','ȱ':'o','ọ':'o','ő':'o','ȍ':'o','ò':'o','ỏ':'o','ơ':'o','ớ':'o','ợ':'o','ờ':'o','ở':'o','ỡ':'o','ȏ':'o','ꝋ':'o','ꝍ':'o','ⱺ':'o','ō':'o','ṓ':'o','ṑ':'o','ǫ':'o','ǭ':'o','ø':'o','ǿ':'o','õ':'o','ṍ':'o','ṏ':'o','ȭ':'o','ƣ':'oi','ꝏ':'oo','ɛ':'e','ᶓ':'e','ɔ':'o','ᶗ':'o','ȣ':'ou','ṕ':'p','ṗ':'p','ꝓ':'p','ƥ':'p','ᵱ':'p','ᶈ':'p','ꝕ':'p','ᵽ':'p','ꝑ':'p','ꝙ':'q','ʠ':'q','ɋ':'q','ꝗ':'q','ŕ':'r','ř':'r','ŗ':'r','ṙ':'r','ṛ':'r','ṝ':'r','ȑ':'r','ɾ':'r','ᵳ':'r','ȓ':'r','ṟ':'r','ɼ':'r','ᵲ':'r','ᶉ':'r','ɍ':'r','ɽ':'r','ↄ':'c','ꜿ':'c','ɘ':'e','ɿ':'r','ś':'s','ṥ':'s','š':'s','ṧ':'s','ş':'s','ŝ':'s','ș':'s','ṡ':'s','ṣ':'s','ṩ':'s','ʂ':'s','ᵴ':'s','ᶊ':'s','ȿ':'s','ɡ':'g','ᴑ':'o','ᴓ':'o','ᴝ':'u','ť':'t','ţ':'t','ṱ':'t','ț':'t','ȶ':'t','ẗ':'t','ⱦ':'t','ṫ':'t','ṭ':'t','ƭ':'t','ṯ':'t','ᵵ':'t','ƫ':'t','ʈ':'t','ŧ':'t','ᵺ':'th','ɐ':'a','ᴂ':'ae','ǝ':'e','ᵷ':'g','ɥ':'h','ʮ':'h','ʯ':'h','ᴉ':'i','ʞ':'k','ꞁ':'l','ɯ':'m','ɰ':'m','ᴔ':'oe','ɹ':'r','ɻ':'r','ɺ':'r','ⱹ':'r','ʇ':'t','ʌ':'v','ʍ':'w','ʎ':'y','ꜩ':'tz','ú':'u','ŭ':'u','ǔ':'u','û':'u','ṷ':'u','ü':'u','ǘ':'u','ǚ':'u','ǜ':'u','ǖ':'u','ṳ':'u','ụ':'u','ű':'u','ȕ':'u','ù':'u','ủ':'u','ư':'u','ứ':'u','ự':'u','ừ':'u','ử':'u','ữ':'u','ȗ':'u','ū':'u','ṻ':'u','ų':'u','ᶙ':'u','ů':'u','ũ':'u','ṹ':'u','ṵ':'u','ᵫ':'ue','ꝸ':'um','ⱴ':'v','ꝟ':'v','ṿ':'v','ʋ':'v','ᶌ':'v','ⱱ':'v','ṽ':'v','ꝡ':'vy','ẃ':'w','ŵ':'w','ẅ':'w','ẇ':'w','ẉ':'w','ẁ':'w','ⱳ':'w','ẘ':'w','ẍ':'x','ẋ':'x','ᶍ':'x','ý':'y','ŷ':'y','ÿ':'y','ẏ':'y','ỵ':'y','ỳ':'y','ƴ':'y','ỷ':'y','ỿ':'y','ȳ':'y','ẙ':'y','ɏ':'y','ỹ':'y','ї':'yi','ź':'z','ž':'z','ẑ':'z','ʑ':'z','ⱬ':'z','ż':'z','ẓ':'z','ȥ':'z','ẕ':'z','ᵶ':'z','ᶎ':'z','ʐ':'z','ƶ':'z','ɀ':'z','þ':'th','ﬀ':'ff','ﬃ':'ffi','ﬄ':'ffl','ﬁ':'fi','ﬂ':'fl','ĳ':'ij','œ':'oe','ﬆ':'st','ₐ':'a','ₑ':'e','ᵢ':'i','ⱼ':'j','ₒ':'o','ᵣ':'r','ᵤ':'u','ᵥ':'v','ₓ':'x','Ё':'YO','Й':'I','Ц':'TS','У':'U','К':'K','Е':'E','Н':'N','Г':'G','Ґ':'G','Ш':'SH','Щ':'SCH','З':'Z','Х':'H','Ъ':'','ё':'yo','й':'i','ц':'ts','у':'u','к':'k','е':'e','н':'n','г':'g','ґ':'g','ш':'sh','щ':'sch','з':'z','х':'h','ъ':'','Ф':'F','Ы':'I','В':'V','А':'A','П':'P','Р':'R','О':'O','Л':'L','Д':'D','Ж':'ZH','Э':'E','ф':'f','ы':'i','в':'v','а':'a','п':'p','р':'r','о':'o','л':'l','д':'d','ж':'zh','э':'e','Я':'Ya','Ч':'CH','С':'S','М':'M','И':'I','Т':'T','Ь':'','Б':'B','Ю':'YU','я':'ya','ч':'ch','с':'s','м':'m','и':'i','т':'t','ь':'','б':'b','ю':'yu'};
	return str.replace(cyrillic ? /[^A-Za-z0-9]/g : /[^A-ZА-ЯЁa-zа-яё0-9]/g, function(char){return diacritics_map.hasOwnProperty(char) ? diacritics_map[char] : char;});
};
function _declination(num, words){
	_validate_argument_type(num, ['string','number'], 'Number', '_declination');
	num = Math.abs(Number(num)) % 100;
	var num1 = num % 10;
	
    if(num > 10 && num < 20){return words[2]};
    if(num1 > 1 && num1 < 5){return words[1]};
    if(num1==1){return words[0]};
	
    return words[2];
};
function _csv_generate(list, sep){
	var act = '_csv_generate';
	list = _avoid_nilb(list, []);
	_validate_argument_type(list, 'array', 'List of items', act);
	sep = _avoid_nilb(sep, ':');
	_validate_argument_type(sep, 'string', 'Separator', act);
    var res = '';
    var first = true;
    list.forEach(function(item){
		item = _to_string(item);
        var add = item;
        if(typeof item=="string" && (item.indexOf(':') > -1 || item.indexOf(';') > -1 || item.indexOf(',') > -1 || item.indexOf(sep) > -1)){
            add = '"' + add.replace(/["]/gi, '""') + '"';
        };
        if(!first){
            res += sep;
        }else{
            first = false;
        };
        res += add;
    });
    return res;
};
function _csv_parse(str, seps, convert){
	var act = '_csv_parse';
	_validate_argument_type(str, 'string', 'String', act);
	seps = _avoid_nilb(seps, [":", ";", ","]);
	_validate_argument_type(seps, ['string','array'], 'List of separators', act);
	convert = _avoid_nilb(convert, false);
	_validate_argument_type(convert, ['boolean','number'], 'Convert types', act);
    var res = [];
    var index = 0;
    var len = str.length;
    var InsideQuotes = false;
    var CurrentElement = '';
    
    while(index<len){
        var c = str[index];
        var n = '';
        var HasNext = (index + 1) < len;
        if(HasNext){
            n = str[index + 1];
        };
        index++;

        if(c=='"'){
            if(InsideQuotes){
                if(!HasNext || n!='"'){
                    InsideQuotes = false;
                    continue;
                }else{
                    CurrentElement += '"';
                    index++;
                    continue;
                }
            }else{
                InsideQuotes = true;
                continue;
            }
        };

        if(seps.indexOf(c) > -1){
            if(InsideQuotes){
                CurrentElement += c;
                continue;
            }else{
                res.push(convert ? _from_string(CurrentElement) : CurrentElement);
                CurrentElement = '';
                continue;
            }
        };
        CurrentElement += c;
    };
    res.push(convert ? _from_string(CurrentElement) : CurrentElement);

    return res;
};
function _replace_string(str, from, to){
	var act = '_replace_string';
	_validate_argument_type(str, 'string', 'String', act);
	_validate_argument_type(from, ['string','number'], 'Replace From', act);
	if(typeof from==='number'){
		from = _no_exponents(from);
	};
	_validate_argument_type(to, ['string','number'], 'Replace To', act);
	if(typeof to==='number'){
		to = _no_exponents(to);
	};
	return str.split(from).join(to);
};
function _random_string(length, chars){
	var act = '_random_string';
	_validate_argument_type(length, 'number', 'Result String Length', act);
	chars = _avoid_nil(chars, 'abcdefghijklmnopqrstuvwxyz0123456789');
	_validate_argument_type(chars, 'string', 'Allowed Chars', act);
	var str  = '';
	for(var i = 0; i < length; i++){
		str += chars.charAt(Math.floor(Math.random() * chars.length));
	};
	return str;
};
function _escape_html(str){
	_validate_argument_type(str, 'string', 'String', '_escape_html');
	var chars = {'¢' : 'cent', '£' : 'pound', '¥' : 'yen', '€': 'euro', '©' :'copy', '®' : 'reg', '<' : 'lt', '>' : 'gt', '"' : 'quot', '&' : 'amp', '\'' : '#39'};
	var regex_string = '[';
	for(var key in chars){
		regex_string += key;
	};
	regex_string += ']';
	var regex = new RegExp(regex_string, 'g');
	var escape_char = function(m){
		return '&' + chars[m] + ';';
	};
	return str.replace(regex, escape_char);
};
function _unescape_html(str){
	_validate_argument_type(str, 'string', 'String', '_unescape_html');
	var entities = {nbsp: ' ', cent: '¢', pound: '£', yen: '¥', euro: '€', copy: '©', reg: '®', lt: '<', gt: '>', quot: '"', amp: '&', apos: '\''};
	var unescape_char = function(entity, entity_code){
		var match = "";
		if(entity_code in entities){
			return entities[entity_code];
		}else if(match = entity_code.match(/^#x([\da-fA-F]+)$/)){
			return String.fromCharCode(parseInt(match[1], 16));
		}else if(match = entity_code.match(/^#(\d+)$/)){
			return String.fromCharCode(~~match[1]);
		}else{
			return entity;
		};
	};
	return str.replace(/\&([^;]{1,10});/g, unescape_char);
};
function _get_non_latin_chars(){
	return {
		letters: {
			upper: 'А-ЯЁ\\xD8-\\xDE\\u00C0-\\u00D6\\u0376\\u037F\\u0386\\u0388-\\u038A\\u038C\\u038E-\\u03A1\\u0531-\\u0556\\u10A0-\\u10C5\\u10C7\\u10CD\\u13A0-\\u13F5\\u1F18-\\u1F1D\\u1F48-\\u1F4D\\u1F59\\u1F5B\\u1F5D\\u2126\\u2183\\u2C00-\\u2C2E\\u2CF2\\uA78B-\\uA7AD\\uFF21-\\uFF3A',
			lower: 'а-яё\\xDF-\\xF6\\xF8-\\xFF\\u00AA\\u00B5\\u00BA\\u00D8-\\u00F6\\u00F8-\\u02C1\\u02C6-\\u02D1\\u02E0-\\u02E4\\u02EC\\u02EE\\u0370-\\u0374\\u0377\\u037A-\\u037D\\u03A3-\\u03F5\\u048A-\\u052F\\u0559\\u0561-\\u0587\\u05D0-\\u05EA\\u05F0-\\u05F2\\u0620-\\u064A\\u066E\\u066F\\u0671-\\u06D3\\u06D5\\u06E5\\u06E6\\u06EE\\u06EF\\u06FA-\\u06FC\\u06FF\\u0710\\u0712-\\u072F\\u074D-\\u07A5\\u07B1\\u07CA-\\u07EA\\u07F4\\u07F5\\u07FA\\u0800-\\u0815\\u081A\\u0824\\u0828\\u0840-\\u0858\\u08A0-\\u08B4\\u0904-\\u0939\\u093D\\u0950\\u0958-\\u0961\\u0971-\\u0980\\u0985-\\u098C\\u098F\\u0990\\u0993-\\u09A8\\u09AA-\\u09B0\\u09B2\\u09B6-\\u09B9\\u09BD\\u09CE\\u09DC\\u09DD\\u09DF-\\u09E1\\u09F0\\u09F1\\u0A05-\\u0A0A\\u0A0F\\u0A10\\u0A13-\\u0A28\\u0A2A-\\u0A30\\u0A32\\u0A33\\u0A35\\u0A36\\u0A38\\u0A39\\u0A59-\\u0A5C\\u0A5E\\u0A72-\\u0A74\\u0A85-\\u0A8D\\u0A8F-\\u0A91\\u0A93-\\u0AA8\\u0AAA-\\u0AB0\\u0AB2\\u0AB3\\u0AB5-\\u0AB9\\u0ABD\\u0AD0\\u0AE0\\u0AE1\\u0AF9\\u0B05-\\u0B0C\\u0B0F\\u0B10\\u0B13-\\u0B28\\u0B2A-\\u0B30\\u0B32\\u0B33\\u0B35-\\u0B39\\u0B3D\\u0B5C\\u0B5D\\u0B5F-\\u0B61\\u0B71\\u0B83\\u0B85-\\u0B8A\\u0B8E-\\u0B90\\u0B92-\\u0B95\\u0B99\\u0B9A\\u0B9C\\u0B9E\\u0B9F\\u0BA3\\u0BA4\\u0BA8-\\u0BAA\\u0BAE-\\u0BB9\\u0BD0\\u0C05-\\u0C0C\\u0C0E-\\u0C10\\u0C12-\\u0C28\\u0C2A-\\u0C39\\u0C3D\\u0C58-\\u0C5A\\u0C60\\u0C61\\u0C85-\\u0C8C\\u0C8E-\\u0C90\\u0C92-\\u0CA8\\u0CAA-\\u0CB3\\u0CB5-\\u0CB9\\u0CBD\\u0CDE\\u0CE0\\u0CE1\\u0CF1\\u0CF2\\u0D05-\\u0D0C\\u0D0E-\\u0D10\\u0D12-\\u0D3A\\u0D3D\\u0D4E\\u0D5F-\\u0D61\\u0D7A-\\u0D7F\\u0D85-\\u0D96\\u0D9A-\\u0DB1\\u0DB3-\\u0DBB\\u0DBD\\u0DC0-\\u0DC6\\u0E01-\\u0E30\\u0E32\\u0E33\\u0E40-\\u0E46\\u0E81\\u0E82\\u0E84\\u0E87\\u0E88\\u0E8A\\u0E8D\\u0E94-\\u0E97\\u0E99-\\u0E9F\\u0EA1-\\u0EA3\\u0EA5\\u0EA7\\u0EAA\\u0EAB\\u0EAD-\\u0EB0\\u0EB2\\u0EB3\\u0EBD\\u0EC0-\\u0EC4\\u0EC6\\u0EDC-\\u0EDF\\u0F00\\u0F40-\\u0F47\\u0F49-\\u0F6C\\u0F88-\\u0F8C\\u1000-\\u102A\\u103F\\u1050-\\u1055\\u105A-\\u105D\\u1061\\u1065\\u1066\\u106E-\\u1070\\u1075-\\u1081\\u108E\\u10D0-\\u10FA\\u10FC-\\u1248\\u124A-\\u124D\\u1250-\\u1256\\u1258\\u125A-\\u125D\\u1260-\\u1288\\u128A-\\u128D\\u1290-\\u12B0\\u12B2-\\u12B5\\u12B8-\\u12BE\\u12C0\\u12C2-\\u12C5\\u12C8-\\u12D6\\u12D8-\\u1310\\u1312-\\u1315\\u1318-\\u135A\\u1380-\\u138F\\u13F8-\\u13FD\\u1401-\\u166C\\u166F-\\u167F\\u1681-\\u169A\\u16A0-\\u16EA\\u16F1-\\u16F8\\u1700-\\u170C\\u170E-\\u1711\\u1720-\\u1731\\u1740-\\u1751\\u1760-\\u176C\\u176E-\\u1770\\u1780-\\u17B3\\u17D7\\u17DC\\u1820-\\u1877\\u1880-\\u18A8\\u18AA\\u18B0-\\u18F5\\u1900-\\u191E\\u1950-\\u196D\\u1970-\\u1974\\u1980-\\u19AB\\u19B0-\\u19C9\\u1A00-\\u1A16\\u1A20-\\u1A54\\u1AA7\\u1B05-\\u1B33\\u1B45-\\u1B4B\\u1B83-\\u1BA0\\u1BAE\\u1BAF\\u1BBA-\\u1BE5\\u1C00-\\u1C23\\u1C4D-\\u1C4F\\u1C5A-\\u1C7D\\u1CE9-\\u1CEC\\u1CEE-\\u1CF1\\u1CF5\\u1CF6\\u1D00-\\u1DBF\\u1E00-\\u1F15\\u1F20-\\u1F45\\u1F50-\\u1F57\\u1F5F-\\u1F7D\\u1F80-\\u1FB4\\u1FB6-\\u1FBC\\u1FBE\\u1FC2-\\u1FC4\\u1FC6-\\u1FCC\\u1FD0-\\u1FD3\\u1FD6-\\u1FDB\\u1FE0-\\u1FEC\\u1FF2-\\u1FF4\\u1FF6-\\u1FFC\\u2071\\u207F\\u2090-\\u209C\\u2102\\u2107\\u210A-\\u2113\\u2115\\u2119-\\u211D\\u2124\\u2128\\u212A-\\u212D\\u212F-\\u2139\\u213C-\\u213F\\u2145-\\u2149\\u214E\\u2184\\u2C30-\\u2C5E\\u2C60-\\u2CE4\\u2CEB-\\u2CEE\\u2CF3\\u2D00-\\u2D25\\u2D27\\u2D2D\\u2D30-\\u2D67\\u2D6F\\u2D80-\\u2D96\\u2DA0-\\u2DA6\\u2DA8-\\u2DAE\\u2DB0-\\u2DB6\\u2DB8-\\u2DBE\\u2DC0-\\u2DC6\\u2DC8-\\u2DCE\\u2DD0-\\u2DD6\\u2DD8-\\u2DDE\\u2E2F\\u3005\\u3006\\u3031-\\u3035\\u303B\\u303C\\u3041-\\u3096\\u309D-\\u309F\\u30A1-\\u30FA\\u30FC-\\u30FF\\u3105-\\u312D\\u3131-\\u318E\\u31A0-\\u31BA\\u31F0-\\u31FF\\u3400-\\u4DB5\\u4E00-\\u9FD5\\uA000-\\uA48C\\uA4D0-\\uA4FD\\uA500-\\uA60C\\uA610-\\uA61F\\uA62A\\uA62B\\uA640-\\uA66E\\uA67F-\\uA69D\\uA6A0-\\uA6E5\\uA717-\\uA71F\\uA722-\\uA788\\uA7B0-\\uA7B7\\uA7F7-\\uA801\\uA803-\\uA805\\uA807-\\uA80A\\uA80C-\\uA822\\uA840-\\uA873\\uA882-\\uA8B3\\uA8F2-\\uA8F7\\uA8FB\\uA8FD\\uA90A-\\uA925\\uA930-\\uA946\\uA960-\\uA97C\\uA984-\\uA9B2\\uA9CF\\uA9E0-\\uA9E4\\uA9E6-\\uA9EF\\uA9FA-\\uA9FE\\uAA00-\\uAA28\\uAA40-\\uAA42\\uAA44-\\uAA4B\\uAA60-\\uAA76\\uAA7A\\uAA7E-\\uAAAF\\uAAB1\\uAAB5\\uAAB6\\uAAB9-\\uAABD\\uAAC0\\uAAC2\\uAADB-\\uAADD\\uAAE0-\\uAAEA\\uAAF2-\\uAAF4\\uAB01-\\uAB06\\uAB09-\\uAB0E\\uAB11-\\uAB16\\uAB20-\\uAB26\\uAB28-\\uAB2E\\uAB30-\\uAB5A\\uAB5C-\\uAB65\\uAB70-\\uABE2\\uAC00-\\uD7A3\\uD7B0-\\uD7C6\\uD7CB-\\uD7FB\\uF900-\\uFA6D\\uFA70-\\uFAD9\\uFB00-\\uFB06\\uFB13-\\uFB17\\uFB1D\\uFB1F-\\uFB28\\uFB2A-\\uFB36\\uFB38-\\uFB3C\\uFB3E\\uFB40\\uFB41\\uFB43\\uFB44\\uFB46-\\uFBB1\\uFBD3-\\uFD3D\\uFD50-\\uFD8F\\uFD92-\\uFDC7\\uFDF0-\\uFDFB\\uFE70-\\uFE74\\uFE76-\\uFEFC\\uFF41-\\uFF5A\\uFF66-\\uFFBE\\uFFC2-\\uFFC7\\uFFCA-\\uFFCF\\uFFD2-\\uFFD7\\uFFDA-\\uFFDC'
		},
		numbers: '\\u0660-\\u0669\\u06F0-\\u06F9\\u07C0-\\u07C9\\u0966-\\u096F\\u09E6-\\u09EF\\u0A66-\\u0A6F\\u0AE6-\\u0AEF\\u0B66-\\u0B6F\\u0BE6-\\u0BEF\\u0C66-\\u0C6F\\u0CE6-\\u0CEF\\u0D66-\\u0D6F\\u0DE6-\\u0DEF\\u0E50-\\u0E59\\u0ED0-\\u0ED9\\u0F20-\\u0F29\\u1040-\\u1049\\u1090-\\u1099\\u17E0-\\u17E9\\u1810-\\u1819\\u1946-\\u194F\\u19D0-\\u19D9\\u1A80-\\u1A89\\u1A90-\\u1A99\\u1B50-\\u1B59\\u1BB0-\\u1BB9\\u1C40-\\u1C49\\u1C50-\\u1C59\\uA620-\\uA629\\uA8D0-\\uA8D9\\uA900-\\uA909\\uA9D0-\\uA9D9\\uA9F0-\\uA9F9\\uAA50-\\uAA59\\uABF0-\\uABF9\\uFF10-\\uFF19'
	};
};
function _get_regexp_extract_string(){
	if(typeof _regexp_extract_string_temp=="undefined" || _is_nilb(_regexp_extract_string_temp)){
		var tlds = "COM|RU|ORG|NET|IR|IN|UK|CN|AU|UA|DE|BY|IO|ICU|INFO|SITE|ONLINE|TOP|XYZ|TK|NL|LY|YT";
		var non_latin_chars = _get_non_latin_chars();
		var non_latin_alphabet_ranges = non_latin_chars.letters.upper + non_latin_chars.letters.lower + non_latin_chars.numbers;
		var email_address = "([a-z0-9!#$%&'*+=?^_`{|}~-]+(\\.[a-z0-9!#$%&'*+=?^_`{|}~-]+)*)";
		var domain_with_tld = "([a-z0-9]+(-+[a-z0-9]+)*\\.)+(" + tlds + ")";
		var domain_with_any_tld = "([a-z0-9]+(-+[a-z0-9]+)*\\.)+([a-z0-9][a-z0-9-]{0,22}[a-z0-9])";
		var allowed_in_path = "a-zA-Zа-яёА-ЯЁ\\d\\-._~\\!$&*+,;=:@%\\[\\]()";
		var path = "(((\\/(([" + allowed_in_path + "]+(\\/[" + allowed_in_path + "]*)*))?)?)((\\?([" + allowed_in_path + "\\/?]*))?)((\\#([" + allowed_in_path + "\\/?]*))?))?";
		var ipv4 = "((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)";
		var ipv6 = "\\[(([a-f0-9:]+:+)+[a-f0-9]+)\\]";
		var port = "(:(\\d{1,5}))?";
		var protocol = "((?:ftp|http|ws)s?:)\\/\\/";
		var username = "[a-zA-Z\\d\\-._~\\!$&*+,;=:%\\[\\]()]+";
		var password = ":[a-zA-Z\\d\\-._~\\!$&*+,;=:%\\[\\]()]+";
		var confirmed_by_protocol = "(" + protocol + ")\\S+";
		var additional_slashes = "(([\\/]?))+";
		var protocol_and_auth = protocol + "(" + username + "(" + password + ")?@)?"
		var fqdn = "(((" + protocol_and_auth + ")?(" + domain_with_tld + "|" + ipv4 + "|(" + protocol_and_auth + ")(" + ipv6 + "|" + domain_with_any_tld + "))(?!@\\w)" + port + ")|(" + confirmed_by_protocol + "))";
		var non_latin_matches = fqdn + "((((\\/(([" + allowed_in_path + "]+(\\/[" + allowed_in_path + non_latin_alphabet_ranges + "]*)*))?)?)((\\?([" + allowed_in_path + "\\/?]*))?)((\\#([" + allowed_in_path + "\\/?]*))?))?\\b((([" + allowed_in_path + "\\/" + non_latin_alphabet_ranges + "][a-zA-Z\\d\\-_~+=\\/" + non_latin_alphabet_ranges + "]+)?))+)";
		var email = "\\b" + email_address + "@(" + domain_with_tld + "|" + ipv4 + ")\\b";
		var url = "(" + non_latin_matches + ")|(\\b" + fqdn + path + "\\b" + additional_slashes + ")";
		
		_regexp_extract_string_temp = {
			url: new RegExp("(" + url + ")", "gi"),
			email: new RegExp("(" + email + ")", "gi"),
			phone: /(?:(?:(?:(\+)?\d{1,3}[-\040.]?)?\s?\(?\d{3}\)?\s?[-\040.]?\d{3}[-\040.]?(\d{4}|\d{2}[-\040.]?\d{2}))|(?:(\+)(?:9[976]\d|8[987530]\d|6[987]\d|5[90]\d|42\d|3[875]\d|2[98654321]\d|9[8543210]|8[6421]|6[6543210]|5[87654321]|4[987654310]|3[9643210]|2[70]|7|1)[-\040.]?(?:\d[-\040.]?){6,12}\d+))([,;]+[0-9]+#?)*/g
		};
	};
	return _regexp_extract_string_temp;
};
function _words(str){
	_validate_argument_type(str, 'string', 'String', '_words');
	var non_latin_chars = _get_non_latin_chars();
	var uppercase_letters = 'A-Z' + non_latin_chars.letters.upper;
	var lowercase_letters = 'a-z' + non_latin_chars.letters.lower;
	var number_chars = '0-9' + non_latin_chars.numbers;
	var reg_words = new RegExp('[' + uppercase_letters + '\\-]+(?![' + lowercase_letters + number_chars + '\\-])|[' + uppercase_letters + '\\-]?[' + lowercase_letters + number_chars + '\\-]+', 'g');
    return _avoid_nil(_clean(str).replace(/[\-]{2,}/g, " ").match(reg_words), []).filter(function(s){return /([^\-]+)/.test(s)}).map(function(s){return _trim(s, _STR_WHITESPACE + _STR_SPECCHARS + "-")});
};
function _extract_list_by_regexp(str, regexp){
	_validate_argument_type(regexp, 'regexp', 'Regular expression', '_extract_string');
	return _uniq_arr(_trim_arr(_avoid_nil(str.match(regexp), [])));
};
function _extract_urls(str){
	_validate_argument_type(str, 'string', 'String', '_extract_urls');
	return _extract_list_by_regexp(str, _get_regexp_extract_string().url);
};
function _extract_emails(str){
	_validate_argument_type(str, 'string', 'String', '_extract_emails');
	return _extract_list_by_regexp(str, _get_regexp_extract_string().email);
};
function _extract_phone_numbers(str){
	_validate_argument_type(str, 'string', 'String', '_extract_phone_numbers');
	return _extract_list_by_regexp(str, _get_regexp_extract_string().phone);
};