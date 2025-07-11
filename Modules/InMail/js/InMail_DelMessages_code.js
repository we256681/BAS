_call_function(_InMail.delMessages, {uids: (<%= uids %>)<%if(box!=='""'){%>, box: (<%= box %>)<%}%>, timeout: (<%= timeout_value() || 60000 %>)})!
