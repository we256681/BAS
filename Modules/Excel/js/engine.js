_XLSX_DATE_FORMAT = "dd\\.mm\\.yyyy\\ hh:mm:ss";
_XLSX_LAST_ACTION = {ru:"Модуль Excel еще не использовался",en:"Excel module has not been used yet"};

function Excel_CreateFile(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sync = _function_argument("Sync");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Создать файл",en:"Create file"};
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sync];
	
	_embedded("Excel_CreateFile", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
};
function Excel_GetSheetsList(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Получить список листов",en:"Get sheets list"};
	
	VAR_XLSX_NODE_PARAMETERS = file_path;
	
	_embedded("Excel_GetSheetsList", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
	
	_function_return(VAR_XLSX_NODE_PARAMETERS);
};
function Excel_AddSheet(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_name = _function_argument("SheetName");
	var sheet_index = _function_argument("SheetIndex");
	var sync = _function_argument("Sync");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Добавить лист",en:"Add sheet"};
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_name, sheet_index, sync];
	
	_embedded("Excel_AddSheet", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
};
function Excel_RenameSheet(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var new_sheet_name = _function_argument("NewSheetName");
	var sync = _function_argument("Sync");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Переименовать лист",en:"Rename sheet"};
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, new_sheet_name, sync];
	
	_embedded("Excel_RenameSheet", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
};
function Excel_MoveSheet(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var new_sheet_index = _function_argument("NewSheetIndex");
	var sync = _function_argument("Sync");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Переместить лист",en:"Move sheet"};
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, new_sheet_index, sync];
	
	_embedded("Excel_MoveSheet", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
};
function Excel_DeleteSheet(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var sync = _function_argument("Sync");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Удалить лист",en:"Delete sheet"};
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, sync];
	
	_embedded("Excel_DeleteSheet", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
};
function Excel_ReadCell(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var cell_address = Excel_FormatAddress(_function_argument("CellAddress"));
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Читать ячейку",en:"Read cell"};
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, cell_address, _XLSX_DATE_FORMAT];
	
	_embedded("Excel_ReadCell", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
	
	var results = VAR_XLSX_NODE_PARAMETERS;
	var value = results[0];
	var is_date = results[1];
	
	_function_return(is_date ? _parse_date(value,"auto") : value);
};
function Excel_WriteToCell(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var cell_address = Excel_FormatAddress(_function_argument("CellAddress"));
	var data = Excel_PreparationOfDates(_function_argument("Data"), _function_argument("Convert"), _function_argument("SetFormat"));
	var sync = _function_argument("Sync");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Запись в ячейку",en:"Write to cell"};
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, cell_address, data, sync];
	
	_embedded("Excel_WriteToCell", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
};
function Excel_ReadSheet(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var data_format = _function_argument("DataFormat");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Читать лист",en:"Read sheet"};
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, data_format, _XLSX_DATE_FORMAT];
	
	_embedded("Excel_ReadSheet", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
	
	_function_return(Excel_ConvertDates(VAR_XLSX_NODE_PARAMETERS));
};
function Excel_WriteToSheet(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var data = Excel_PreparationOfDates(_function_argument("Data"), _function_argument("Convert"), _function_argument("SetFormat"));
	var sync = _function_argument("Sync");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Запись на лист",en:"Write to sheet"};
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, data, sync];
	
	_embedded("Excel_WriteToSheet", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
};
function Excel_CountRows(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Количество строк",en:"Count rows"};
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name];
	
	_embedded("Excel_CountRows", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
	
	_function_return(VAR_XLSX_NODE_PARAMETERS);
};
function Excel_ReadRows(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var from_row = _function_argument("FromRow");
	var to_row = _function_argument("ToRow");
	var data_format = _function_argument("DataFormat");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Читать строки",en:"Read rows"};
	
	from_row = from_row==="" ? "" : from_row+1;
	to_row = to_row==="" ? "" : to_row+1;
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, from_row, to_row, data_format, _XLSX_DATE_FORMAT];
	
	_embedded("Excel_ReadRows", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
	
	_function_return(Excel_ConvertDates(VAR_XLSX_NODE_PARAMETERS));
};
function Excel_InsertRows(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var from_row = _function_argument("FromRow");
	var data = Excel_PreparationOfDates(_function_argument("Data"), _function_argument("Convert"), _function_argument("SetFormat"));
	var sync = _function_argument("Sync");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Вставить строки",en:"Insert rows"};
	
	from_row = from_row==="" ? "" : from_row+1;
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, from_row, data, sync];
	
	_embedded("Excel_InsertRows", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
};
function Excel_DeleteRows(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var from_row = _function_argument("FromRow");
	var to_row = _function_argument("ToRow");
	var sync = _function_argument("Sync");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Удалить строки",en:"Delete rows"};
	
	from_row = from_row==="" ? "" : from_row+1;
	to_row = to_row==="" ? "" : to_row+1;
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, from_row, to_row, sync];
	
	_embedded("Excel_DeleteRows", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
};
function Excel_ReadCellsRange(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var from_cell = Excel_FormatAddress(_function_argument("FromCell"));
	var to_cell = Excel_FormatAddress(_function_argument("ToCell"));
	var data_format = _function_argument("DataFormat");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Читать область ячеек",en:"Read cells range"};
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, from_cell, to_cell, data_format, _XLSX_DATE_FORMAT];
	
	_embedded("Excel_ReadCellsRange", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
	
	_function_return(Excel_ConvertDates(VAR_XLSX_NODE_PARAMETERS));
};
function Excel_WriteToCellsRange(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var from_cell = Excel_FormatAddress(_function_argument("FromCell"));
	var to_cell = Excel_FormatAddress(_function_argument("ToCell"));
	var data = Excel_PreparationOfDates(_function_argument("Data"), _function_argument("Convert"), _function_argument("SetFormat"));
	var sync = _function_argument("Sync");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Запись в область ячеек",en:"Write to cells range"};
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, from_cell, to_cell, data, sync];
	
	_embedded("Excel_WriteToCellsRange", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
};
function Excel_ImportToResources(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_list = _to_arr(_function_argument("SheetList"));
	var success_number = _function_argument("SuccessNumber");
	var fail_number = _function_argument("FailNumber");
	var simultaneous_usage = _function_argument("SimultaneousUsage");
	var interval = _function_argument("Interval");
	var greedy = _function_argument("Greedy");
	var dont_give_up = _function_argument("DontGiveUp");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Импорт из файла в ресурсы",en:"Import from file to resources"};

	_call_function(Excel_GetSheetsList,{"FilePath":file_path,"Timeout":timeout})!
	var sheets_list = _result_function();
	
	if(sheet_list){
		sheets_list = sheets_list.filter(function(sheet_name, sheet_index){return sheet_list.indexOf(sheet_name) > -1 || sheet_list.indexOf(sheet_index) > -1 || sheet_list.indexOf(String(sheet_index)) > -1});
	};
	
	_do_with_params({"foreach_data":sheets_list},function(){
		var sheet_index = _iterator() - 1;
		if(sheet_index > _cycle_param("foreach_data").length - 1){_break()};
		var sheet_name = _cycle_param("foreach_data")[sheet_index];
		
		_call_function(Excel_ReadSheet,{"FilePath":file_path,"SheetIndexOrName":sheet_name,"DataFormat":"CSV list","Timeout":timeout})!
		var sheet_data = _result_function();

		RCreate(sheet_name, success_number, fail_number, simultaneous_usage, interval, greedy, dont_give_up);

		var res = RMap(sheet_name);

		res.clear();
		res.sync();
		sheet_data.forEach(function(ell){
			res.insert(ell);
		});
		res.sync();
	})!
};
function Excel_ExportFromResources(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var resource_list = _to_arr(_function_argument("ResourceList"));
	var sync = _function_argument("Sync");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Экспорт из ресурсов в файл",en:"Export from resources to file"};

	_do_with_params({"foreach_data":resource_list},function(){
		var resource_index = _iterator() - 1;
		if(resource_index > _cycle_param("foreach_data").length - 1){_break()};
		var resource_name = _cycle_param("foreach_data")[resource_index];

		var resource_data = RMap(resource_name).toList();
		
		_call_function(Excel_ClearSheet,{"FilePath":file_path,"SheetIndexOrName":resource_name,"Sync":sync,"Timeout":timeout})!
		_result_function();
		
		_call_function(Excel_WriteToSheet,{"FilePath":file_path,"SheetIndexOrName":resource_name,"Data":resource_data,"Sync":sync,"Timeout":timeout})!
		_result_function();
	})!
};
function Excel_ClearSheet(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var sync = _function_argument("Sync");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Очистить лист",en:"Clear sheet"};
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, sync];
	
	_embedded("Excel_ClearSheet", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
};
function Excel_ClearCell(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var cell_address = Excel_FormatAddress(_function_argument("CellAddress"));
	var sync = _function_argument("Sync");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Очистить ячейку",en:"Clear cell"};
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, cell_address, sync];
	
	_embedded("Excel_ClearCell", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
};
function Excel_ClearCellsRange(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var from_cell = Excel_FormatAddress(_function_argument("FromCell"));
	var to_cell = Excel_FormatAddress(_function_argument("ToCell"));
	var sync = _function_argument("Sync");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Очистить область ячеек",en:"Clear cells range"};
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, from_cell, to_cell, sync];
	
	_embedded("Excel_ClearCellsRange", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
};
function Excel_ConvertToJSON(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Преобразовать в JSON",en:"Convert to JSON"};
	
	var sheets = [];

	_call_function(Excel_GetSheetsList,{"FilePath":file_path,"Timeout":timeout})!
	var sheets_list = _result_function();

	_do_with_params({"foreach_data":sheets_list},function(){
		var sheet_index = _iterator() - 1;
		if(sheet_index > _cycle_param("foreach_data").length - 1){_break()};
		var sheet_name = _cycle_param("foreach_data")[sheet_index];
		
		_call_function(Excel_ReadSheet,{"FilePath":file_path,"SheetIndexOrName":sheet_name,"DataFormat":"2D list","Timeout":timeout})!
		var sheet_data = _result_function();

		sheets.push({name:sheet_name,data:sheet_data});
	})!

	_function_return(JSON.stringify({sheets:sheets}));
};
function Excel_ConvertFromJSON(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var data = _function_argument("Data");
	var sync = _function_argument("Sync");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Преобразовать из JSON",en:"Convert from JSON"};
	
	data = (typeof data=="object") ? data : JSON.parse(data);
	
	var sheets = data.sheets;

	_do_with_params({"foreach_data":sheets},function(){
		var sheet_index = _iterator() - 1;
		if(sheet_index > _cycle_param("foreach_data").length - 1){_break()};
		var sheet = _cycle_param("foreach_data")[sheet_index];

		var sheet_name = sheet.name;
		var sheet_data = sheet.data;
		
		_call_function(Excel_ClearSheet,{"FilePath":file_path,"SheetIndexOrName":sheet_name,"Sync":sync,"Timeout":timeout})!
		_result_function();
		
		_call_function(Excel_WriteToSheet,{"FilePath":file_path,"SheetIndexOrName":sheet_name,"Data":sheet_data,"Sync":sync,"Timeout":timeout})!
		_result_function();
	})!
};
function Excel_FindCell(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var contains = _function_argument("Contains");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Найти все ячейки",en:"Find all cells"};
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, contains];
	
	_embedded("Excel_FindCell", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
	
	_function_return(VAR_XLSX_NODE_PARAMETERS);
};
function Excel_FindCells(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var contains = _function_argument("Contains");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Найти одну ячейку",en:"Find single cell"};
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, contains];
	
	_embedded("Excel_FindCells", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
	
	_function_return(VAR_XLSX_NODE_PARAMETERS);
};
function Excel_GetFormulaFromCell(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var cell_address = Excel_FormatAddress(_function_argument("CellAddress"));
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Получить формулу из ячейки",en:"Get formula from cell"};
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, cell_address];
	
	_embedded("Excel_GetFormulaFromCell", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
	
	_function_return(VAR_XLSX_NODE_PARAMETERS);
};
function Excel_SetFormulaToCell(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var cell_address = Excel_FormatAddress(_function_argument("CellAddress"));
	var formula = _function_argument("Formula");
	var sync = _function_argument("Sync");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Установить формулу в ячейку",en:"Set formula to cell"};
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, cell_address, formula, sync];
	
	_embedded("Excel_SetFormulaToCell", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
};
function Excel_GetCellStyle(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var cell_address = Excel_FormatAddress(_function_argument("CellAddress"));
	var style_name = _function_argument("StyleName");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Получите один стиль для одной ячейки",en:"Get one style for single cell"};
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, cell_address, style_name];
	
	_embedded("Excel_GetCellStyle", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
	
	_function_return(VAR_XLSX_NODE_PARAMETERS);
};
function Excel_GetCellStyles(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var cell_address = Excel_FormatAddress(_function_argument("CellAddress"));
	var styles_name_list = _to_arr(_function_argument("StylesNameList"));
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Получить список стилей для одной ячейки",en:"Get style list for single cell"};
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, cell_address, styles_name_list];
	
	_embedded("Excel_GetCellStyles", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
	
	_function_return(JSON.stringify(VAR_XLSX_NODE_PARAMETERS));
};
function Excel_SetStyleToCell(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var cell_address = Excel_FormatAddress(_function_argument("CellAddress"));
	var style_name = _function_argument("StyleName");
	var style_value = _function_argument("StyleValue");
	var sync = _function_argument("Sync");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Установить один стиль в одну ячейку",en:"Set one style to single cell"};
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, cell_address, style_name, style_value, sync];
	
	_embedded("Excel_SetStyleToCell", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
};
function Excel_SetStylesToCell(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var cell_address = Excel_FormatAddress(_function_argument("CellAddress"));
	var styles = _function_argument("Styles");
	var sync = _function_argument("Sync");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Установить список стилей в одну ячейку",en:"Set list of styles to single cell"};
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, cell_address, styles, sync];
	
	_embedded("Excel_SetStylesToCell", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
};
function Excel_SetStyleToCellsRange(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var from_cell = Excel_FormatAddress(_function_argument("FromCell"));
	var to_cell = Excel_FormatAddress(_function_argument("ToCell"));
	var style_name = _function_argument("StyleName");
	var style_value = _function_argument("StyleValue");
	var sync = _function_argument("Sync");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Установить один стиль в область ячеек",en:"Set one style to cells range"};
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, from_cell, to_cell, style_name, style_value, sync];
	
	_embedded("Excel_SetStyleToCellsRange", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
};
function Excel_SetStylesToCellsRange(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var sheet_index_or_name = _function_argument("SheetIndexOrName");
	var from_cell = Excel_FormatAddress(_function_argument("FromCell"));
	var to_cell = Excel_FormatAddress(_function_argument("ToCell"));
	var styles = _function_argument("Styles");
	var sync = _function_argument("Sync");
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Установить список стилей в область ячеек",en:"Set list of styles to cells range"};
	
	VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, from_cell, to_cell, styles, sync];
	
	_embedded("Excel_SetStylesToCellsRange", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
};
function Excel_GetColumnWidth(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
    var sheet_index_or_name = _function_argument("SheetIndexOrName");
    var column_index_or_name = _function_argument("ColumnIndexOrName");
	var timeout = _function_argument("Timeout");
	
    var column_number_or_name = typeof column_index_or_name=="number" ? column_index_or_name + 1 : column_index_or_name;
	
	_XLSX_LAST_ACTION = {ru:"Получить ширину столбца",en:"Get column width"};
	
    VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, column_number_or_name];
	
	_embedded("Excel_GetColumnWidth", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
	
	_function_return(VAR_XLSX_NODE_PARAMETERS);
};
function Excel_GetRowHeight(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
    var sheet_index_or_name = _function_argument("SheetIndexOrName");
    var row_index = _function_argument("RowIndex");
	var timeout = _function_argument("Timeout");
	
    var row_number = row_index + 1;
	
	_XLSX_LAST_ACTION = {ru:"Получить высоту строки",en:"Get row height"};
	
    VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, row_number];
	
	_embedded("Excel_GetRowHeight", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
	
	_function_return(VAR_XLSX_NODE_PARAMETERS);
};
function Excel_SetColumnWidth(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
    var sheet_index_or_name = _function_argument("SheetIndexOrName");
    var column_index_or_name = _function_argument("ColumnIndexOrName");
    var width = _function_argument("Width");
	var sync = _function_argument("Sync");
	var timeout = _function_argument("Timeout");
	
    var column_number_or_name = typeof column_index_or_name=="number" ? column_index_or_name + 1 : column_index_or_name;
	
	_XLSX_LAST_ACTION = {ru:"Установить ширину столбца",en:"Set column width"};
	
    VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, column_number_or_name, width, sync];
	
	_embedded("Excel_SetColumnWidth", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
};
function Excel_SetRowHeight(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
    var sheet_index_or_name = _function_argument("SheetIndexOrName");
    var row_index = _function_argument("RowIndex");
    var height = _function_argument("Height");
	var sync = _function_argument("Sync");
	var timeout = _function_argument("Timeout");
	
    var row_number = row_index + 1;
	
	_XLSX_LAST_ACTION = {ru:"Установить высоту строки",en:"Set row height"};
	
    VAR_XLSX_NODE_PARAMETERS = [file_path, sheet_index_or_name, row_number, height, sync];
	
	_embedded("Excel_SetRowHeight", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
};
function Excel_CloseFile(){
	var file_path = Excel_FormatPath(_function_argument("FilePath"));
	var timeout = _function_argument("Timeout");
	
	_XLSX_LAST_ACTION = {ru:"Закрыть файл",en:"Close file"};
	
	VAR_XLSX_NODE_PARAMETERS = file_path;
	
	_embedded("Excel_CloseFile", "Node", "18.10.0", "XLSX_NODE_PARAMETERS", timeout)!
};
function Excel_DateToNumber(date){
	date = date instanceof Date ? date : new Date(date);
	var date_base = new Date(1900, 0, 0);
	var incorrect_leap_date = new Date(1900, 1, 28);
	var milliseconds_in_day = 1000 * 60 * 60 * 24;
	
	var date_only = new Date(date.getTime());
	date_only.setHours(0, 0, 0, 0);
	var number = Math.round((date_only - date_base) / milliseconds_in_day);
	number += (date - date_only) / milliseconds_in_day;
	if(date > incorrect_leap_date){number += 1};
	
	return number;
};
function Excel_NumberToDate(number){
	number = typeof number=="number" ? number : Number(number);
	var date_base = new Date(1900, 0, 0);
	var incorrect_leap_date = new Date(1900, 1, 28);
	var milliseconds_in_day = 1000 * 60 * 60 * 24;
	
	if(number > Excel_DateToNumber(incorrect_leap_date)){number--};
	var full_days = Math.floor(number);
	var partial_milliseconds = Math.round((number - full_days) * milliseconds_in_day);
	var date = new Date(date_base.getTime() + partial_milliseconds);
	date.setDate(date.getDate() + full_days);
	
	return date;
};
function Excel_ConvertDates(results){
	var values = results[0];
	var present_date = results[1];
	var date_addresses = results[2];
	if(present_date){
		for(var i = 0; i < date_addresses.length; i++){
			var address = date_addresses[i];
			var ri = address[0];
			var ci = address[1];
			values[ri][ci] = _parse_date(values[ri][ci],"auto");
		};
	};
	return values;
};
function Excel_PreparationOfDates(data, convert, set_format){
	convert = _avoid_nilb(convert, true);
	set_format = _avoid_nilb(set_format, true);
	if(!convert && !set_format){
		return [data, false, null]
	};
	if(typeof data=="object" && Array.isArray(data)){
		var present_date = false;
		var formats = [];
		var new_data = [];
		for(var row_index = 0; row_index < data.length; row_index++){
			formats[row_index] = [];
			new_data[row_index] = [];
			var row = data[row_index];
			if(typeof row=="object" && Array.isArray(row)){
				for(var cell_index = 0; cell_index < row.length; cell_index++){
					var cell = row[cell_index];
					if(cell instanceof Date){
						present_date = true;
						new_data[row_index][cell_index] = convert ? Excel_DateToNumber(cell) : cell;
						formats[row_index][cell_index] = _XLSX_DATE_FORMAT;
					}else{
						new_data[row_index][cell_index] = cell;
						formats[row_index][cell_index] = "General";
					};
				};
			}else{
				new_data[row_index] = row;
			};
		};
		return [new_data, convert, set_format ? (present_date ? formats : "General") : null];
    }else{
		if(data instanceof Date){
			return [convert ? Excel_DateToNumber(data) : data, convert, set_format ? _XLSX_DATE_FORMAT : null];
		}else{
			return [data, convert, set_format ? "General" : null];
		};
	};
};
function Excel_FormatAddress(address){
	if(address.indexOf("*") > -1){
		var split = address.split("*");
		var column_number = parseInt(split[0]) + 1;
		var row_number = parseInt(split[1]) + 1;
		var column_letter = "";
		while(column_number>0){
			var temp = (column_number-1)%26;
			column_letter = String.fromCharCode(temp+65)+column_letter;
			column_number = (column_number-temp-1)/26;
		};
		address = column_letter + row_number;
	};
	return address;
};
function Excel_FormatPath(path){
	return path.split("\\").join("/");
};