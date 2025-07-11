_call_function(FTP_CheckExist,{ "FilePath": (<%= FilePath %>),"Timeout": (<%= timeout_value() || 60000 %>) })!
<%= variable %> = _result_function()
