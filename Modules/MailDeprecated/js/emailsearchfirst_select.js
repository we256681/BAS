var Sender = GetInputConstructorValue("Sender",loader)
var Subject = GetInputConstructorValue("Subject",loader)
var Body = GetInputConstructorValue("Body",loader)
var Save = this.$el.find("#Save").val().toUpperCase();
var To = GetInputConstructorValue("To",loader)
var MailId = this.$el.find("#MailId").val().toUpperCase();

var List = $("#VariablesList").val().toUpperCase()
List = List.replace(/\s/g,"")
List = List.split(",")
List = _.compact(List);


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
    var code = loader.GetAdditionalData() + _.template($("#emailsearchfirst_code").html())({link_list:List,variable:"VAR_" + Save,mailid: "VAR_" + MailId,to:To["updated"],sender:Sender["updated"],subject:Subject["updated"],body:Body["updated"]})
    code = Normalize(code,0)
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}
          