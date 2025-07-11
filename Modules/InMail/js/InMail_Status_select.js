var name = GetInputConstructorValue("name", loader);
if(name["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Folder name") + tr("\" is not specified"));
    return;
};
var total = this.$el.find("#total").val().toUpperCase();
if(total.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Total messages") + tr("\" is not specified"));
    return;
};
var recent = this.$el.find("#recent").val().toUpperCase();
if(recent.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Recent messages") + tr("\" is not specified"));
    return;
};
var unseen = this.$el.find("#unseen").val().toUpperCase();
if(unseen.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Unseen messages") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#InMail_Status_code").html())({
        "name": name["updated"],
        "total": "VAR_" + total,
        "recent": "VAR_" + recent,
        "unseen": "VAR_" + unseen
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}