var path = GetInputConstructorValue("path", loader);
if(path["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Path") + tr("\" is not specified"));
    return;
};
var root = this.$el.find("#root").val().toUpperCase();
if(root.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Root") + tr("\" is not specified"));
    return;
};
var dir = this.$el.find("#dir").val().toUpperCase();
if(dir.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Directory") + tr("\" is not specified"));
    return;
};
var base = this.$el.find("#base").val().toUpperCase();
if(base.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Last part") + tr("\" is not specified"));
    return;
};
var ext = this.$el.find("#ext").val().toUpperCase();
if(ext.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("File extension") + tr("\" is not specified"));
    return;
};
var name = this.$el.find("#name").val().toUpperCase();
if(name.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("File name") + tr("\" is not specified"));
    return;
};
var items = this.$el.find("#items").val().toUpperCase();
if(items.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("List of path items") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#parse_path_code").html())({
        "path": path["updated"],
        "root": "VAR_" + root,
        "dir": "VAR_" + dir,
        "base": "VAR_" + base,
        "ext": "VAR_" + ext,
        "name": "VAR_" + name,
        "items": "VAR_" + items
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}