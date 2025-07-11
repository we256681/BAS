var Save = this.$el.find("#Save").val().toUpperCase();
var Value = GetInputConstructorValue("Value", loader);

var List = $("#VariablesList").val().toUpperCase()
List = List.replace(/\s/g,"")
List = List.split(",")
List = _.compact(List);

if(Save.length == 0)
{
    Invalid("Variable is empty");
    return;
}
if(Value["original"].length == 0)
{
    Invalid("Eamil id is empty");
    return;
}
    try{
    var code = loader.GetAdditionalData() + _.template($("#emailpull_code").html())({link_list:List,variable:"VAR_" + Save,value: Value["updated"]})
    code = Normalize(code,0)
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}