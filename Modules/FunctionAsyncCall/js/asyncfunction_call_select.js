var FunctionName = $("#FunctionName").val();
if(FunctionName.length == 0)
{
	Invalid("FunctionName is empty");
	return;
}

var Arguments = {}

_.each(this.$el.find("#FunctionParametersNoReturn").find("[data-preserve-type=constr]"), function(Element) {
	var IdHex = $(Element).attr("data-preserve-id")
	var Id = fromhex(IdHex.replace("ParamId_",""))
	Arguments[Id] = GetInputConstructorValue(IdHex, loader)["updated"]
})

var ResultThreadId = this.$el.find("#ResultThreadId").val().toUpperCase();
if(ResultThreadId.length == 0)
{
	Invalid("ResultThreadId is empty");
	return;
}

var ThreadList = this.$el.find("#ThreadList").val().toUpperCase();
if(ThreadList.length == 0)
{
	Invalid("ThreadList is empty");
	return;
}

var DesiredThreadId = GetInputConstructorValue("DesiredThreadId", loader)
var CloseThreadAfterFunctionEnds = GetInputConstructorValue("CloseThreadAfterFunctionEnds", loader)
var MaxThreads = GetInputConstructorValue("MaxThreads", loader)
var OnIdle = GetInputConstructorValue("OnIdle", loader)


if(DesiredThreadId["original"].length == 0)
{
	Invalid("DesiredThreadId is empty");
	return;
}

if(CloseThreadAfterFunctionEnds["original"].length == 0)
{
	Invalid("CloseThreadAfterFunctionEnds is empty");
	return;
}


if(MaxThreads["original"].length == 0)
{
	Invalid("MaxThreads is empty");
	return;
}

if(OnIdle["original"].length == 0)
{
	Invalid("OnIdle is empty");
	return;
}



try{
	var code = loader.GetAdditionalData() + _.template($("#asyncfunction_call_code").html())({
		name:FunctionName, 
		args: Arguments,
		result_thread_id: "VAR_" + ResultThreadId,
		thread_list: "VAR_" + ThreadList,
		desired_thread_id: DesiredThreadId["updated"],
		on_iddle: OnIdle["updated"],
		close_thread_after_function_ends: CloseThreadAfterFunctionEnds["updated"],
		max_threads: MaxThreads["updated"],
	})
	code = Normalize(code,0)
	BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, 1, false);
}catch(e)
{}