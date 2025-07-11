var uids = GetInputConstructorValue("uids", loader);
if(uids["original"].length == 0){
    Invalid(tr("The parameter \"") + tr("Message id") + tr("\" is not specified"));
    return;
};
var toBox = GetInputConstructorValue("toBox", loader);
if(toBox["original"].length == 0){
    Invalid(tr("The parameter \"") + tr("To folder") + tr("\" is not specified"));
    return;
};
var box = GetInputConstructorValue("box", loader);
try{
    var code = loader.GetAdditionalData() + _.template($("#InMail_MoveMessages_code").html())({
        "uids": uids["updated"],
		"toBox": toBox["updated"],
        "box": box["updated"]
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}
