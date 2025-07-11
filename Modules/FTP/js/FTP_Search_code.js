_call_function(FTP_Search,{ "FolderPath": (<%= FolderPath %>),"Mask": (<%= Mask %>),"Recursive": (<%= Recursive %>),"SearchFiles": (<%= SearchFiles %>),"SearchFolders": (<%= SearchFolders %>),"Timeout": (<%= timeout_value() || 60000 %>) })!
<%= variable %> = _result_function()
