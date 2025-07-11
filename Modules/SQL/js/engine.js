_SQL_CONFIG = {
	id: (rand(10) + thread_number()),
	data: {
		dialect: "",
		dialectOptions: "",
		host: "",
		port: "",
		username: "",
		password: "",
		database: "",
		storage: ""
	},
    connect_timeout: "",
	timeout: 5*60*1000,
	debug: false
};

function SQL_Setup(dialect, host, port, username, password, database, storage, connect_timeout, timeout, dialect_options){
	dialect_options = _avoid_nil(dialect_options);
	if(_is_json_string(dialect_options)){
		dialect_options = JSON.parse(dialect_options);
	};
	_SQL_CONFIG["data"]["dialect"] = dialect;
	_SQL_CONFIG["data"]["host"] = host;
	_SQL_CONFIG["data"]["port"] = (port==="auto" || port==="") ? "" : Number(port);
	_SQL_CONFIG["data"]["username"] = username;
	_SQL_CONFIG["data"]["password"] = password;
	_SQL_CONFIG["data"]["database"] = database;
	_SQL_CONFIG["data"]["storage"] = storage.split("\\").join("/");
	_SQL_CONFIG["data"]["dialectOptions"] = dialect_options;
	_SQL_CONFIG["connect_timeout"] = connect_timeout!=="" ? connect_timeout*1000 : "";
	_SQL_CONFIG["timeout"] = timeout*1000;
};
function SQL_Query(){
	var query = _function_argument("query");
	var query_parameterize = _function_argument("query_parameterize");
	var data_format = _function_argument("data_format");
	var timeout = _function_argument("timeout");
	
	SQL_CheckDialect();
	
	var query_type = ["SELECT", "INSERT", "UPDATE", "BULKUPDATE", "BULKDELETE", "DELETE", "UPSERT", "VERSION", "SHOWTABLES", "SHOWINDEXES", "DESCRIBE", "FOREIGNKEYS", "SHOWCONSTRAINTS"].filter(function(type){return query.indexOf(type)===0})[0] || "RAW";
	
	_call_function(SQL_PreParameterization,{"query":query,"parameterize":query_parameterize})!
	query = _result_function();
	
	VAR_SQL_NODE_PARAMETERS = [_SQL_CONFIG, query, query_type, data_format];
	
	_embedded("SQL_Query", "Node", "18.10.0", "SQL_NODE_PARAMETERS", timeout)!
	
	var results = VAR_SQL_NODE_PARAMETERS[0];
	var data_format = VAR_SQL_NODE_PARAMETERS[1];
	var query_type = VAR_SQL_NODE_PARAMETERS[2];
	
	_function_return(query_type=="SELECT" ? SQL_RestoreDates(results, data_format) : results);
};
function SQL_CountRecords(){
	var table = _function_argument("table");
	var where = _function_argument("where");
	var where_parameterize = _function_argument("where_parameterize");
	var timeout = _function_argument("timeout");
	
	SQL_CheckDialect();
	
	_call_function(SQL_PreParameterization,{"query":where,"parameterize":where_parameterize})!
	where = _result_function();
	
	VAR_SQL_NODE_PARAMETERS = [_SQL_CONFIG, table, where];
	
	_embedded("SQL_CountRecords", "Node", "18.10.0", "SQL_NODE_PARAMETERS", timeout)!
	
	_function_return(VAR_SQL_NODE_PARAMETERS);
};
function SQL_SelectRecords(){
	var table = _function_argument("table");
	var where = _function_argument("where");
	var where_parameterize = _function_argument("where_parameterize");
	var included_columns = _to_arr(_function_argument("included_columns"));
	var excluded_columns = _to_arr(_function_argument("excluded_columns"));
	var order_column = _function_argument("order_column");
	var order_direction = _function_argument("order_direction");
	var offset = _function_argument("offset");
	var limit = _function_argument("limit");
	var data_format = _function_argument("data_format");
	var timeout = _function_argument("timeout");
	var order = order_direction=="no sorting" ? "" : [ [order_column, order_direction=="ascending" ? "ASC" : "DESC"] ];
	
	SQL_CheckDialect();
	
	_call_function(SQL_PreParameterization,{"query":where,"parameterize":where_parameterize})!
	where = _result_function();
	
	VAR_SQL_NODE_PARAMETERS = [_SQL_CONFIG, table, where, included_columns, excluded_columns, order, offset, limit, data_format];
	
	_embedded("SQL_SelectRecords", "Node", "18.10.0", "SQL_NODE_PARAMETERS", timeout)!
	
	var results = VAR_SQL_NODE_PARAMETERS[0];
	var data_format = VAR_SQL_NODE_PARAMETERS[1];
	
	_function_return(SQL_RestoreDates(results, data_format));
};
function SQL_UpdateRecords(){
	var table = _function_argument("table");
	var values = _function_argument("values");
	var convert = _avoid_nilb(_function_argument("convert"), true);
	var where = _function_argument("where");
	var where_parameterize = _function_argument("where_parameterize");
	var fields = _to_arr(_function_argument("fields"));
	var limit = _function_argument("limit");
	var timeout = _function_argument("timeout");
	
	SQL_CheckDialect();
	
	_call_function(SQL_PreParameterization,{"query":where,"parameterize":where_parameterize})!
	where = _result_function();
	
	_call_function(SQL_ConvertValuesToObject,{"values":values})!
	values = _result_function();
	
	VAR_SQL_NODE_PARAMETERS = [_SQL_CONFIG, table, values, convert, where, fields, limit];
	
	_embedded("SQL_UpdateRecords", "Node", "18.10.0", "SQL_NODE_PARAMETERS", timeout)!
};
function SQL_DeleteRecords(){
	var table = _function_argument("table");
	var where = _function_argument("where");
	var where_parameterize = _function_argument("where_parameterize");
	var limit = _function_argument("limit");
	var timeout = _function_argument("timeout");
	
	SQL_CheckDialect();
	
	_call_function(SQL_PreParameterization,{"query":where,"parameterize":where_parameterize})!
	where = _result_function();
	
	VAR_SQL_NODE_PARAMETERS = [_SQL_CONFIG, table, where, limit];
	
	_embedded("SQL_DeleteRecords", "Node", "18.10.0", "SQL_NODE_PARAMETERS", timeout)!
};
function SQL_InsertRecord(){
	var table = _function_argument("table");
	var fields = _to_arr(_function_argument("fields"));
	var data = _function_argument("data");
	var convert = _avoid_nilb(_function_argument("convert"), true);
	var idFieldName = _function_argument("idFieldName");
	var timeout = _function_argument("timeout");
	
	if(Array.isArray(data) && ['object', 'array'].indexOf(_get_type(data[0])) > -1){
		data = data.slice(0, 1);
	};
	
	data = SQL_DataPreparation(data);
	
	SQL_CheckDialect();
	
	VAR_SQL_NODE_PARAMETERS = [_SQL_CONFIG, table, fields, data, convert, idFieldName];
	
	_if(true, function(){
		_embedded("SQL_InsertRecord", "Node", "18.10.0", "SQL_NODE_PARAMETERS", timeout)!
	})!
	
	_function_return(idFieldName ? VAR_SQL_NODE_PARAMETERS : undefined);
};
function SQL_Insert(){
	var table = _function_argument("table");
	var fields = _to_arr(_function_argument("fields"));
	var data = SQL_DataPreparation(_function_argument("data"));
	var convert = _avoid_nilb(_function_argument("convert"), true);
	var timeout = _function_argument("timeout");
	
	SQL_CheckDialect();
	
	VAR_SQL_NODE_PARAMETERS = [_SQL_CONFIG, table, fields, data, convert];
	
	_embedded("SQL_InsertMultipleRecords", "Node", "18.10.0", "SQL_NODE_PARAMETERS", timeout)!
};
function SQL_Debug(enable){
	_SQL_CONFIG["debug"] = (enable==true || enable=="true");
};
function SQL_Close(){
	VAR_SQL_NODE_PARAMETERS = _SQL_CONFIG["id"];
	
	_embedded("SQL_Close", "Node", "18.10.0", "SQL_NODE_PARAMETERS", 60000)!
};
function SQL_ConvertDates(data){
	return data instanceof Date ? {isDate:true,date:data} : (Array.isArray(data) ? data.map(function(e){return e instanceof Date ? {isDate:true,date:e} : e}) : data);
};
function SQL_RestoreDates(results, format){
	if(format=="Object list"){
		results.forEach(function(row, ir){return Object.keys(row).forEach(function(key){return results[ir][key] = (row[key]!==null && row[key].isDate) ? _parse_date(row[key].date,"auto") : row[key]})});
	};
	if(format=="2D list"){
		results = results.map(function(row){return row.map(function(cell){return (cell!==null && cell.isDate) ? _parse_date(cell.date,"auto") : cell})});
	};
	return results;
};
function SQL_Template(){
	var e = _function_argument("e");
	
	_template(e.trim())!
	var r = _result();
	
	_function_return(r);
};
function SQL_PreParameterization(){
	var query = _function_argument("query");
	var parameterize = _function_argument("parameterize");
	
	_if_else(parameterize, function(){
		var reg = new RegExp("\\[\\[[^\\]]+\\]\\]|\\{\\{[^\\}]+\\}\\}", "g");
		var replacements = query.match(reg) || [];
		parameterize = (replacements && replacements.length > 0) ? true : false;
		_if(parameterize,function(){
			_do_with_params({"foreach_data":replacements},function(){
				var cycle_index = _iterator() - 1;
				if(cycle_index > _cycle_param("foreach_data").length - 1){_break()};
				var ell = _cycle_param("foreach_data")[cycle_index];
				
				_call_function(SQL_Template,{"e":ell})!
				replacements[cycle_index] = _result_function();
			})!
			
			query = query.replace(reg, "?");
		})!
		
		_function_return([query, parameterize, replacements]);
	}, function(){
		_function_return([query, false]);
	})!
};
function SQL_ConvertValuesToObject(){
	var values = _function_argument("values");
	
	_if(values.indexOf("=") < 0,function(){
		_call_function(SQL_Template,{"e":values})!
		values = _result_function();
		
		_if(typeof values=="object",function(){
			_function_return(values);
		})!
	})!
	
	if(values.slice(-1)==","){
		values = values.slice(0, -1);
	};
	
	var values_object = {};
	var values_array = values.split(/,?\r?\n/);
	
	_do_with_params({"foreach_data":values_array},function(){
		var cycle_index = _iterator() - 1;
		if(cycle_index > _cycle_param("foreach_data").length - 1){_break()};
		var data = _cycle_param("foreach_data")[cycle_index];
		
		var sep_index = data.indexOf('=');
		
		_if(sep_index > -1,function(){
			var key = data.slice(0, sep_index - (data.charAt(sep_index - 1)== ' ' ? 1 : 0));
			var value = data.slice(sep_index + 1 + (data.charAt(sep_index + 1)== ' ' ? 1 : 0));
			
			_call_function(SQL_Template,{"e":key})!
			var key = _result_function();
			
			_call_function(SQL_Template,{"e":value})!
			var value = _result_function();
			
			values_object[key] = SQL_ConvertDates(value);
		})!
	})!
	
	_function_return(values_object);
};
function SQL_DataPreparation(data){
	if(_is_json_string(data)){
		data = JSON.parse(data);
	};
	if(_get_type(data)=="object" || (Array.isArray(data) && typeof data[0]=="string" && csv_parse(data[0]).length==1)){
		data = [data];
	};
	if(Array.isArray(data) && ['object', 'array'].indexOf(_get_type(data[0])) > -1){
		if(Array.isArray(data[0])){
			data = data.map(function(row){return row.map(function(cell){return SQL_ConvertDates(cell)})});
		}else{
			data = data.map(function(row){
				for(var key in row){
					row[key] = SQL_ConvertDates(row[key]);
				};
				return row;
			});
		};
	};
	return data;
};
function SQL_CheckDialect(){
	var dialect = _SQL_CONFIG["data"]["dialect"];
	if(["mysql","mariadb","postgres","sqlite","mssql"].indexOf(dialect) < 0){
		fail(_K=="ru" ? ("Настройка доступа к базе данных не выполнена или выполнена неправильно") : ("Database access configuration failed or incorrect"));
	};
};