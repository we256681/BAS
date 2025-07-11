_call_function(_InMail.copyMessages, {uids: (<%= uids %>)<%if(box!=='""'){%>, box: (<%= box %>)<%}%>, toBox: (<%= toBox %>), timeout: (<%= timeout_value() || 60000 %>)})!
