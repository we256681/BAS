_call_function(Excel_GetCellStyle,{"FilePath":(<%= FilePath %>), "SheetIndexOrName":(<%= SheetIndexOrName %>), "CellAddress":(<%= CellAddress %>), "StyleName":(<%= StyleName %>), "Timeout":(<%= timeout_value() || 60000 %>)})!
<%= variable %> = _result_function()
