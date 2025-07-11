var FilePath = GetInputConstructorValue("FilePath", loader);
if(FilePath["original"].length == 0){
	Invalid(tr("Remote file path") + " " + tr("is empty"));
    return;
};
var base64 = $("#Check").is(':checked');
var Save = this.$el.find("#Save").val().toUpperCase();
try{
    var code = loader.GetAdditionalData() + _.template($("#FTP_ReadFile_code").html())({
        "FilePath": FilePath["updated"],
        "base64": base64,
        "variable": "VAR_" + Save
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}