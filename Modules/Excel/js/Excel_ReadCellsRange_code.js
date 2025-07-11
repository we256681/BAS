_call_function(Excel_ReadCellsRange,{"FilePath":(<%= FilePath %>), "SheetIndexOrName":(<%= SheetIndexOrName %>), "FromCell":(<%= FromCell %>), "ToCell":(<%= ToCell %>), "DataFormat":(<%= DataFormat %>), "Timeout":(<%= timeout_value() || 60000 %>)})!
<%= variable %> = _result_function()
