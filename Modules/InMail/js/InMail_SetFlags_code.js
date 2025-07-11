_call_function(_InMail.<%if(add){%>add<%}else{%>set<%}%>Flags, {uids: (<%= uids %>), flags: (<%= flags %>)<%if(box!=='""'){%>, box: (<%= box %>)<%}%>, timeout: (<%= timeout_value() || 60000 %>)})!
