_call_function(_InMail.getBoxes, {format: (<%= format %>), timeout: (<%= timeout_value() || 60000 %>)})!
<%= variable %> = _result_function();
