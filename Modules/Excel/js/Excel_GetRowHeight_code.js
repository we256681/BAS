_call_function(Excel_GetRowHeight,{"FilePath":(<%= FilePath %>), "SheetIndexOrName":(<%= SheetIndexOrName %>), "RowIndex":(<%= RowIndex %>), "Timeout":(<%= timeout_value() || 60000 %>)})!
<%= variable %> = _result_function()
