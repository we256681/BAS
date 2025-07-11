_call_function(Excel_GetFormulaFromCell,{"FilePath":(<%= FilePath %>), "SheetIndexOrName":(<%= SheetIndexOrName %>), "CellAddress":(<%= CellAddress %>), "Timeout":(<%= timeout_value() || 60000 %>)})!
<%= variable %> = _result_function()
