var MailFrom = GetInputConstructorValue("MailFrom",loader)
var MailTo = GetInputConstructorValue("MailTo",loader)
var MailCC = GetInputConstructorValue("CC",loader)
var MailSubject = GetInputConstructorValue("Subject",loader)
var MailBody = GetInputConstructorValue("MailBody", loader)
var IsHtml = $("#Check").is(':checked');
var Attachments = GetInputConstructorValue("Attachments",loader)

if(MailFrom["original"].length <= 0)
{
    Invalid("Mail From is empty");
    return;
}

if(MailTo["original"].length <= 0)
{
    Invalid("Mail To is empty");
    return;
}



try{
	var code = loader.GetAdditionalData() + _.template($("#smtp_send_mail_code").html())({mailfrom:MailFrom["updated"], mailto:MailTo["updated"], mailcc:MailCC["updated"], mailsubject:MailSubject["updated"], mailbody:MailBody["updated"], attachments:Attachments["updated"], is_html:IsHtml})
    code = Normalize(code,0)
    BrowserAutomationStudio_Append("" , BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}