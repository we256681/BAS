_call_function(Excel_FindCell,{"FilePath":(<%= FilePath %>), "SheetIndexOrName":(<%= SheetIndexOrName %>), "Contains":(<%= Contains %>), "Timeout":(<%= timeout_value() || 60000 %>)})!
var res = _result_function();
<%= Column %> = res[0];
<%= Row %> = res[1];
<%= Sheet %> = res[2];
