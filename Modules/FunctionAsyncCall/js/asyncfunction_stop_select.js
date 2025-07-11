var ThreadId = GetInputConstructorValue("ThreadId", loader)
if(ThreadId["original"].length == 0)
{
	Invalid("ThreadId is empty");
	return;
}



try{
	var code = loader.GetAdditionalData() + _.template($("#asyncfunction_stop_code").html())({
		thread_id: ThreadId["updated"]
	})
	code = Normalize(code,0)
	BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}