_call_function(_SMS.getSites,{service:(<%= service %>),apiKey:(<%= apiKey %>)<%if(serverUrl!=='""'){%>,serverUrl:(<%= serverUrl %>)<%}%>,timeout:(<%= timeout_value() || 60000 %>)})!
<%= variable %> = _result_function();
