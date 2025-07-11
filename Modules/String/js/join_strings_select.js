var string1 = GetInputConstructorValue("string1", loader);
var string2 = GetInputConstructorValue("string2", loader);
var string3 = GetInputConstructorValue("string3", loader);
var strings_list = GetInputConstructorValue("strings_list", loader);
if(string1["original"].length==0 && string2["original"].length==0 && string3["original"].length==0 && strings_list["original"].length==0){
	Invalid(tr("The parameter \"") + tr("List of strings") + tr("\" is not specified"));
    return;
};
var separator = GetInputConstructorValue("separator", loader);
var Save = this.$el.find("#Save").val().toUpperCase();
if(Save.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#join_strings_code").html())({
        "string1": string1["updated"],
        "string2": string2["updated"],
        "string3": string3["updated"],
        "strings_list": strings_list["updated"],
        "separator": separator["updated"],
        "variable": "VAR_" + Save
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}