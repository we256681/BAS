var FilePath = GetInputConstructorValue("FilePath", loader);
if(FilePath["original"].length==0){
	Invalid(tr("Remote path to file or folder") + " " + tr("is empty"));
    return;
};
var Save = this.$el.find("#Save").val().toUpperCase();
if(Save.length==0){
	Invalid(tr("Variable not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#FTP_CheckExist_code").html())({
        "FilePath": FilePath["updated"],
        "variable": "VAR_" + Save
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}