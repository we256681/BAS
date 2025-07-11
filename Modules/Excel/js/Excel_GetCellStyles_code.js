_call_function(Excel_GetCellStyles,{"FilePath":(<%= FilePath %>), "SheetIndexOrName":(<%= SheetIndexOrName %>), "CellAddress":(<%= CellAddress %>), "StylesNameList":(<%= StylesNameList %>), "Timeout":(<%= timeout_value() || 60000 %>)})!
<%= variable %> = _result_function()
