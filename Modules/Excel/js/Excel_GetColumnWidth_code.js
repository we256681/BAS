_call_function(Excel_GetColumnWidth,{"FilePath":(<%= FilePath %>), "SheetIndexOrName":(<%= SheetIndexOrName %>), "ColumnIndexOrName":(<%= ColumnIndexOrName %>), "Timeout":(<%= timeout_value() || 60000 %>)})!
<%= variable %> = _result_function()
