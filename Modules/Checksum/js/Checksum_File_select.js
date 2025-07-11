var filePath = GetInputConstructorValueFilename("filePath", loader);
if(filePath["original"].length == 0){
    Invalid(tr("The parameter \"") + tr("File path") + tr("\" is not specified"));
    return;
};
var algorithm = GetInputConstructorValue("algorithm", loader);
if(algorithm["original"].length == 0){
    Invalid(tr("The parameter \"") + tr("Algorithm") + tr("\" is not specified"));
    return;
};
var outputLength = GetInputConstructorValue("outputLength", loader);
var outputEncoding = GetInputConstructorValue("outputEncoding", loader);
if(outputEncoding["original"].length == 0){
    Invalid(tr("The parameter \"") + tr("Output format") + tr("\" is not specified"));
    return;
};
var Save = this.$el.find("#Save").val().toUpperCase();
try{
    var code = loader.GetAdditionalData() + _.template($("#Checksum_File_code").html())({
        "filePath": filePath["updated"],
        "algorithm": algorithm["updated"],
        "outputLength": outputLength["updated"],
        "outputEncoding": outputEncoding["updated"],
        "variable": "VAR_" + Save
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}