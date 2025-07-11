var uids = GetInputConstructorValue("uids", loader);
if(uids["original"].length == 0){
    Invalid(tr("The parameter \"") + tr("Message id") + tr("\" is not specified"));
    return;
};
var flags = GetInputConstructorValue("flags", loader);
if(flags["original"].length == 0){
    Invalid(tr("The parameter \"") + tr("Flags") + tr("\" is not specified"));
    return;
};
var box = GetInputConstructorValue("box", loader);
try{
    var code = loader.GetAdditionalData() + _.template($("#InMail_DelFlags_code").html())({
        "uids": uids["updated"],
        "flags": flags["updated"],
        "box": box["updated"]
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}