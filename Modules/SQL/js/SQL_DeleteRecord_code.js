_call_function(SQL_DeleteRecords,{"table":(<%= table %>), "where":(<%= where %>), "where_parameterize":(<%= parameterize %>), "limit":(1), "timeout":(<%= timeout_value() || 60000 %>)})!
