var Save = this.$el.find("#Save").val().toUpperCase();
var SaveError = this.$el.find("#SaveError").val().toUpperCase();
var Command = GetInputConstructorValue("Command", loader);
var waitfinish = $("#Check").is(':checked')
var WorkingFolder = GetInputConstructorValue("WorkingFolder", loader);

if(Save.length == 0)
{
  Invalid("Variable is empty");
  return;
}
if(Command["original"].length == 0)
{
  Invalid("Command is empty");
  return;
}


 try{
  var code = loader.GetAdditionalData() + _.template($('#execute_process_code').html())(
  	{
  		variable:"VAR_" + Save,
  		command: Command["updated"], 
  		waitfinish:waitfinish,
  		working_folder: WorkingFolder["updated"],
  		variable_fail:"VAR_" + SaveError,
  	}
  )
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}