_call_function(Excel_ReadSheet,{"FilePath":(<%= FilePath %>), "SheetIndexOrName":(<%= SheetIndexOrName %>), "DataFormat":(<%= DataFormat %>), "Timeout":(<%= timeout_value() || 60000 %>)})!
<%= variable %> = _result_function()
