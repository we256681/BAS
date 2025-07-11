var Sender = GetInputConstructorValue("Sender",loader)
var Subject = GetInputConstructorValue("Subject",loader)
var Body = GetInputConstructorValue("Body",loader)
var To = GetInputConstructorValue("To",loader)
var Save = this.$el.find("#Save").val().toUpperCase();


if(Sender["original"].length <= 0)
{
    Sender["updated"] = '""';
}
if(Subject["original"].length <= 0)
{
    Subject["updated"] = '""';
}
if(Body["original"].length <= 0)
{
    Body["updated"] = '""';
}
if(To["original"].length <= 0)
{
    To["updated"] = '""';
}

    try{
    var code = loader.GetAdditionalData() + _.template($("#emailsearchall_code").html())({variable:"VAR_" + Save,sender:Sender["updated"],to:To["updated"],subject:Subject["updated"],body:Body["updated"]})
    code = Normalize(code,0)
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}
