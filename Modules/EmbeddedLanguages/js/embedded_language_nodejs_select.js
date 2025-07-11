var Code = ""
if(window.NodeJsGlobal.Selected == "current")
{
  Code = window.NodeJsGlobal.Editor.getValue()
}else
{
  Code = window.NodeJsGlobal.InitialValue
}

var NameRaw = GetInputConstructorValue("Name", loader)["original"];
var Name = '"' + NameRaw + '"';
var VersionRaw = this.$el.find("#NodeVersionList").val();
var Version = '"' + VersionRaw + '"';

var Variables = $.unique((Code.match(/\[\[([^\]]+)\]\]/g) || []));
var VariablesString = '"' + Variables.join(",").replace(/[\]\[]/g,"")+ '"';

Variables = Variables.map((el) => {return el.replace("[[","VAR_").replace("]]","")})

//Code = Code.replace(/\[\[([^\]]+)\]\]/g, "BAS_VARS[\"$1\"]")

try{
  var code = loader.GetAdditionalData() + _.template($('#embedded_language_nodejs_code').html())(
  	{
  		name_raw: NameRaw,
  		name: Name,
  		version_raw: VersionRaw,
  		version: Version,
      variables: Variables.join(" "),
      variables_string: VariablesString
  	})
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}