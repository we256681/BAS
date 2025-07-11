_call_function(FTP_FileToList,{ "FilePath": (<%= FilePath %>),"Timeout": (<%= timeout_value() || 60000 %>) })!
<%= variable %> = _result_function()
