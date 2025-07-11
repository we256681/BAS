_call_function(FTP_GetInfo,{ "FilePath": (<%= FilePath %>),"Timeout": (<%= timeout_value() || 60000 %>) })!
var json = _result_function();
<%= variable_exists %> = json["exists"];
<%= variable_size %> = json["size"];
<%= variable_directory %> = json["directory"];
<%= variable_is_directory %> = json["is_directory"];
<%= variable_is_file %> = json["is_file"];
<%= variable_last_modified %> = new Date(json["last_modified"]);
