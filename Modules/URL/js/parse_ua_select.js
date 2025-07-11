var ua = GetInputConstructorValue("ua", loader);
if(ua["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("User-Agent") + tr("\" is not specified"));
    return;
};
var platform_type = this.$el.find("#platform_type").val().toUpperCase();
if(platform_type.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Platform type") + tr("\" is not specified"));
    return;
};
var browser_name = this.$el.find("#browser_name").val().toUpperCase();
if(browser_name.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Browser name") + tr("\" is not specified"));
    return;
};
var browser_version = this.$el.find("#browser_version").val().toUpperCase();
if(browser_version.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Browser version") + tr("\" is not specified"));
    return;
};
var browser_major_version = this.$el.find("#browser_major_version").val().toUpperCase();
if(browser_major_version.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Browser major version") + tr("\" is not specified"));
    return;
};
var engine_name = this.$el.find("#engine_name").val().toUpperCase();
if(engine_name.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Engine name") + tr("\" is not specified"));
    return;
};
var engine_version = this.$el.find("#engine_version").val().toUpperCase();
if(engine_version.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Engine version") + tr("\" is not specified"));
    return;
};
var os_name = this.$el.find("#os_name").val().toUpperCase();
if(os_name.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("OS name") + tr("\" is not specified"));
    return;
};
var os_version = this.$el.find("#os_version").val().toUpperCase();
if(os_version.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("OS version") + tr("\" is not specified"));
    return;
};
var device_vendor = this.$el.find("#device_vendor").val().toUpperCase();
if(device_vendor.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Device vendor") + tr("\" is not specified"));
    return;
};
var device_model = this.$el.find("#device_model").val().toUpperCase();
if(device_model.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Device model") + tr("\" is not specified"));
    return;
};
var device_type = this.$el.find("#device_type").val().toUpperCase();
if(device_type.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Device type") + tr("\" is not specified"));
    return;
};
var cpu_architecture = this.$el.find("#cpu_architecture").val().toUpperCase();
if(cpu_architecture.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Processor architecture") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#parse_ua_code").html())({
        "ua": ua["updated"],
        "platform_type": "VAR_" + platform_type,
        "browser_name": "VAR_" + browser_name,
        "browser_version": "VAR_" + browser_version,
        "browser_major_version": "VAR_" + browser_major_version,
        "engine_name": "VAR_" + engine_name,
        "engine_version": "VAR_" + engine_version,
        "os_name": "VAR_" + os_name,
        "os_version": "VAR_" + os_version,
        "device_vendor": "VAR_" + device_vendor,
        "device_model": "VAR_" + device_model,
        "device_type": "VAR_" + device_type,
        "cpu_architecture": "VAR_" + cpu_architecture
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}