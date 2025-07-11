_call_function(_InMail.getFlags, {uid: (<%= uid %>)<%if(box!=='""'){%>, box: (<%= box %>)<%}%>, timeout: (<%= timeout_value() || 60000 %>)})!
<%= variable %> = _result_function();
