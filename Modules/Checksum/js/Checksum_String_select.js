var input = GetInputConstructorValue("input", loader);
if(input["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("String") + tr("\" is not specified"));
    return;
};
var algorithm = GetInputConstructorValue("algorithm", loader);
if(algorithm["original"].length == 0){
    Invalid(tr("The parameter \"") + tr("Algorithm") + tr("\" is not specified"));
    return;
};
var base64 = $("#Check").is(':checked');
var outputLength = GetInputConstructorValue("outputLength", loader);
var outputEncoding = GetInputConstructorValue("outputEncoding", loader);
if(outputEncoding["original"].length == 0){
    Invalid(tr("The parameter \"") + tr("Output format") + tr("\" is not specified"));
    return;
};
var Save = this.$el.find("#Save").val().toUpperCase();
try{
    var code = loader.GetAdditionalData() + _.template($("#Checksum_String_code").html())({
        "input": input["updated"],
        "algorithm": algorithm["updated"],
        "base64": base64,
        "outputLength": outputLength["updated"],
        "outputEncoding": outputEncoding["updated"],
        "variable": "VAR_" + Save
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}