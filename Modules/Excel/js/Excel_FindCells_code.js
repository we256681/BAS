_call_function(Excel_FindCells,{"FilePath":(<%= FilePath %>), "SheetIndexOrName":(<%= SheetIndexOrName %>), "Contains":(<%= Contains %>), "Timeout":(<%= timeout_value() || 60000 %>)})!
<%= variable %> = _result_function()
