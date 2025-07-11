_call_function(Excel_CountRows,{"FilePath":(<%= FilePath %>), "SheetIndexOrName":(<%= SheetIndexOrName %>), "Timeout":(<%= timeout_value() || 60000 %>)})!
<%= variable %> = _result_function()
