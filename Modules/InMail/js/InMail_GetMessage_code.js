<%if((getTextHtml && getLinksTextHtml) || (getTextPlain && getLinksTextPlain) || (getTextRaw && getLinksTextRaw)){%>/*Dat:eyJzIjoiZXh0cmFjdF91cmxzIn0=*/<%}%>
_call_function(_InMail.getMessage, {uid: (<%= uid %>), body: [<%= [['html', getTextHtml], ['plain', getTextPlain], ['raw', getTextRaw]].filter(function(el){return el[1]}).map(function(el){return "'" + el[0] + "'"}).join(', ') %>], headers: [<%= [['from', getFrom], ['to', getTo], ['subject', getSubject], ['raw', getRawHeader]].filter(function(el){return el[1]}).map(function(el){return "'" + el[0] + "'"}).join(', ') %>]<%if(getSize){%>, size: <%= getSize %><%}%><%if(getFlags){%>, flags: <%= getFlags %><%}%><%if(getDate){%>, date: <%= getDate %><%}%><%if(getAttachNames){%>, attachnames: <%= getAttachNames %><%}%><%if(getAttachments){%>, attachments: (<%= attachmentsMask %>)<%}%><%if(box!=='""'){%>, box: (<%= box %>)<%}%>, timeout: (<%= timeout_value() || 60000 %>)})!
var messageData = _result_function();
<%if(getFrom){%><%= saveFrom %> = messageData["headers"]["from"];<%}%>
<%if(getTo){%><%= saveTo %> = messageData["headers"]["to"];<%}%>
<%if(getSubject){%><%= saveSubject %> = messageData["headers"]["subject"];<%}%>
<%if(getRawHeader){%><%= saveRawHeader %> = messageData["headers"]["raw"];<%}%>
<%if(getTextHtml){%>
	<%= saveTextHtml %> = messageData["body"]["html"];
	<%if(getLinksTextHtml){%>
		<%= saveLinksTextHtml %> = _extract_urls(messageData["body"]["html"]);
	<%}%>
<%}%>
<%if(getTextPlain){%>
	<%= saveTextPlain %> = messageData["body"]["plain"];
	<%if(getLinksTextPlain){%>
		<%= saveLinksTextPlain %> = _extract_urls(messageData["body"]["plain"]);
	<%}%>
<%}%>
<%if(getTextRaw){%>
	<%= saveTextRaw %> = messageData["body"]["raw"];
	<%if(getLinksTextRaw){%>
		<%= saveLinksTextRaw %> = _extract_urls(messageData["body"]["raw"]);
	<%}%>
<%}%>
<%if(getSize){%><%= saveSize %> = messageData["size"];<%}%>
<%if(getFlags){%><%= saveFlags %> = messageData["flags"];<%}%>
<%if(getDate){%><%= saveDate %> = messageData["date"];<%}%>
<%if(getAttachNames){%><%= saveAttachnames %> = messageData["attachnames"];<%}%>
<%if(getAttachments){%><%= saveAttachments %> = messageData["attachments"];<%}%>

<%if(delAfter){%>
	_call_function(_InMail.delMessages, {uids: messageData["uid"]<%if(box!=='""'){%>, box: (<%= box %>)<%}%>})!
<%}else if(setFlagsAfter){%>
	_call_function(_InMail.setFlags, {uids: messageData["uid"], flags: (<%= setFlags %>)<%if(box!=='""'){%>, box: (<%= box %>)<%}%>})!
<%}%>
