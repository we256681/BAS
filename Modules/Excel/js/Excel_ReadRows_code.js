_call_function(Excel_ReadRows,{"FilePath":(<%= FilePath %>), "SheetIndexOrName":(<%= SheetIndexOrName %>), "FromRow":(<%= FromRow %>), "ToRow":(<%= ToRow %>), "DataFormat":(<%= DataFormat %>), "Timeout":(<%= timeout_value() || 60000 %>)})!
<%= variable %> = _result_function()
