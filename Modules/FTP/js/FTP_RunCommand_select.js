var Command = GetInputConstructorValue("Command", loader);
if(Command["original"].length == 0){
	Invalid(tr("Command") + " " + tr("is empty"));
    return;
};
var Standart = this.$el.find("#Standart").val().toUpperCase();
var Error = this.$el.find("#Error").val().toUpperCase();
if(Standart.length==0 || Error.length==0){
	Invalid(tr("Variables not specified"));
    return;
};
var WaitFinish = $("#Check").is(':checked')
var WorkingFolder = GetInputConstructorValue("WorkingFolder", loader);
try{
    var code = loader.GetAdditionalData() + _.template($("#FTP_RunCommand_code").html())({
        "Command": Command["updated"],
        "Standart": "VAR_" + Standart,
		"WaitFinish": WaitFinish,
  		"WorkingFolder": WorkingFolder["updated"],
		"Error": "VAR_" + Error
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}
