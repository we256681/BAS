var FolderPath = GetInputConstructorValue("FolderPath", loader);
if(FolderPath["original"].length==0){
    Invalid(tr("Remote directory to search") + " " + tr("is empty"));
    return;
};
var Mask = GetInputConstructorValue("Mask", loader);
if(Mask["original"].length==0){
	Invalid(tr("File/Folder name mask") + " " + tr("is empty"));
    return;
};
var SearchFolders = $("#Check").is(':checked');
var SearchFiles = $("#Check2").is(':checked');
var Recursive = $("#Check3").is(':checked');
var Save = this.$el.find("#Save").val().toUpperCase();
try{
    var code = loader.GetAdditionalData() + _.template($("#FTP_Search_code").html())({
        "FolderPath": FolderPath["updated"],
        "Mask": Mask["updated"],
        "Recursive": Recursive,
        "SearchFiles": SearchFiles,
        "SearchFolders": SearchFolders,
        "variable": "VAR_" + Save
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}