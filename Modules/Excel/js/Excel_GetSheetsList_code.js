_call_function(Excel_GetSheetsList,{"FilePath":(<%= FilePath %>), "Timeout":(<%= timeout_value() || 60000 %>)})!
<%= variable %> = _result_function()
