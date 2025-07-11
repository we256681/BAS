var ThreadId = GetInputConstructorValue("ThreadId", loader)
if(ThreadId["original"].length == 0)
{
	Invalid("ThreadId is empty");
	return;
}

var Result = this.$el.find("#Result").val().toUpperCase();
if(Result.length == 0)
{
	Invalid("Result is empty");
	return;
}

var IsRunnig = this.$el.find("#IsRunnig").val().toUpperCase();
if(IsRunnig.length == 0)
{
	Invalid("IsRunnig is empty");
	return;
}

var ErrorString = this.$el.find("#Error").val().toUpperCase();
if(ErrorString.length == 0)
{
	Invalid("Error is empty");
	return;
}

var IsSuccess = this.$el.find("#IsSuccess").val().toUpperCase();
if(IsSuccess.length == 0)
{
	Invalid("IsSuccess is empty");
	return;
}

try{
	var code = loader.GetAdditionalData() + _.template($("#asyncfunction_result_code").html())({
		result: "VAR_" + Result, 
		is_running: "VAR_" + IsRunnig,
		is_success: "VAR_" + IsSuccess,
		error: "VAR_" + ErrorString,
		thread_id: ThreadId["updated"]
	})
	code = Normalize(code,0)
	BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}