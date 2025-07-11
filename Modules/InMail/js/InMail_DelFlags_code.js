_call_function(_InMail.delFlags, {uids: (<%= uids %>), flags: (<%= flags %>)<%if(box!=='""'){%>, box: (<%= box %>)<%}%>, timeout: (<%= timeout_value() || 60000 %>)})!
