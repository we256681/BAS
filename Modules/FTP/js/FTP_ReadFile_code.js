_call_function(FTP_ReadFile,{ "FilePath": (<%= FilePath %>),"base64": (<%= base64 %>),"Timeout": (<%= timeout_value() || 60000 %>) })!
<%= variable %> = _result_function()
