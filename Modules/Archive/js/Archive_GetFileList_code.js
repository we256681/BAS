_call_function(Archive_GetFileList,{"ArchivePath":(<%= ArchivePath %>), "ArchiveType":(<%= ArchiveType %>)," Timeout":(<%= timeout_value() || 60000 %>)})!
<%= variable %> = _result_function()
