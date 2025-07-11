var FilePath = GetInputConstructorValue("FilePath", loader);
if(FilePath["original"].length == 0){
	Invalid(tr("Remote path to file or folder") + " " + tr("is empty"));
    return;
};
var SaveExists = this.$el.find("#SaveExists").val().toUpperCase();
var SaveSize = this.$el.find("#SaveSize").val().toUpperCase();
var SaveDirectory = this.$el.find("#SaveDirectory").val().toUpperCase();
var SaveIsDirectory = this.$el.find("#SaveIsDirectory").val().toUpperCase();
var SaveIsFile = this.$el.find("#SaveIsFile").val().toUpperCase();
var SaveLastModified = this.$el.find("#SaveLastModified").val().toUpperCase();
if(SaveExists.length==0 || SaveSize.length==0 || SaveDirectory.length==0 || SaveIsDirectory.length==0 || SaveIsFile.length==0 || SaveLastModified.length==0){
	Invalid(tr("Variables not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#FTP_GetInfo_code").html())({
        "FilePath": FilePath["updated"],
		"variable_exists": "VAR_" + SaveExists,
        "variable_size": "VAR_" + SaveSize,
        "variable_directory": "VAR_" + SaveDirectory,
        "variable_is_directory": "VAR_" + SaveIsDirectory,
		"variable_is_file": "VAR_" + SaveIsFile,
        "variable_last_modified": "VAR_" + SaveLastModified
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}