var path1 = GetInputConstructorValue("path1", loader);
var path2 = GetInputConstructorValue("path2", loader);
var path3 = GetInputConstructorValue("path3", loader);
var paths_list = GetInputConstructorValue("paths_list", loader);
if(path1["original"].length==0 && path2["original"].length==0 && path3["original"].length==0 && paths_list["original"].length==0){
	Invalid(tr("The parameter \"") + tr("List of paths") + tr("\" is not specified"));
    Invalid(tr("List of paths") + " " + tr("is empty"));
    return;
};
var Save = this.$el.find("#Save").val().toUpperCase();
if(Save.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#join_paths_code").html())({
        "path1": path1["updated"],
        "path2": path2["updated"],
        "path3": path3["updated"],
        "paths_list": paths_list["updated"],
        "variable": "VAR_" + Save
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}