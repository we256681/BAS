var item1 = GetInputConstructorValue("item1", loader);
var item2 = GetInputConstructorValue("item2", loader);
var item3 = GetInputConstructorValue("item3", loader);
var items_list = GetInputConstructorValue("items_list", loader);
if(item1["original"].length==0 && item2["original"].length==0 && item3["original"].length==0 && items_list["original"].length==0){
	Invalid(tr("The parameter \"") + tr("List of items") + tr("\" is not specified"));
    return;
};
var separator = GetInputConstructorValue("separator", loader);
if(separator["original"].length==0){
	Invalid(tr("The parameter \"") + tr("Separator") + tr("\" is not specified"));
    return;
};
var Save = this.$el.find("#Save").val().toUpperCase();
if(Save.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + tr("\" is not specified"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#create_csv_string_code").html())({
        "item1": item1["updated"],
        "item2": item2["updated"],
        "item3": item3["updated"],
        "items_list": items_list["updated"],
        "separator": separator["updated"],
        "variable": "VAR_" + Save
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}