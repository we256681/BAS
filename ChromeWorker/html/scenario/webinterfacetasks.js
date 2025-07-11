function WebInterfaceTasks()
{

	var CurrentResultId = -1;
	var IsReady = false;

	var HasLast = false;
	var LastResultId = null;
	var LastFunctionName = null;
	var LastParams = null;

	this.CodeReady = function()
	{
		IsReady = true
		if(HasLast)
		{
			this.RunTask(LastResultId, LastFunctionName, LastParams)
			
			HasLast = false;
			LastResultId = null;
			LastFunctionName = null;
			LastParams = null;
		}
	}

	this.MovedExecutionPoint = function(IdFrom,IdTo)
	{
		//if(this.GetFunctionNameByTaskId(IdFrom) != this.GetFunctionNameByTaskId(IdTo))
		//{
			this.SendResultSuccess(null)
		//}

	}

	this.GetFunctionNameByTaskId = function(TaskId)
	{

		var FunctionName = "Main"

		var FunctionList = GetFunctionList()

		for(var i = 0;i<FunctionList.length;i++)
		{
			var Func = FunctionList[i]
			if(Func.id == 0)
			{
				continue;
			}

			
			if(IsAnsectorOf(Func.id,TaskId))
			{
				FunctionName = Func.name

				break
				
			}
		}

		return FunctionName

	}


	this.CurrentIdChanged = function(CurrentId)
	{
		if(CurrentResultId == -1)
			return

		if(CurrentId == 1)
			this.SendResultSuccess(null)

	}

	
	this.OnStartCustomThread = function()
	{
		CurrentResultId = -2
	}



	this.RunTask = function(ResultId, FunctionName, Params)
	{
		if(!IsReady)
		{
			HasLast = true;
			LastResultId = ResultId;
			LastFunctionName = FunctionName;
			LastParams = Params;
			return;
		}
		CurrentResultId = ResultId

		if(CurrentResultId == -1)
		{
			return
		}

		if(_GobalModel.get("isscriptexecuting"))
		{
			this.SendResultFail("Script is executing, can't start new task")
			return
		}



		var code = 
			`
			${GetFunctionBody(FunctionName)}
			_call_task(${FunctionName},${Params})!
			section_start("test", 1)!`

		BrowserAutomationStudio_Execute(code)

	}

	this.SendResultSuccess = function(Result)
	{
		if(CurrentResultId == -1)
			return

		BrowserAutomationStudio_SetWebInterfaceTaskResult(CurrentResultId, JSON.stringify({Message: "", Result: Result,Success:true}))
		CurrentResultId = -1
	}

	this.SendResultFail = function(Message)
	{
		if(CurrentResultId == -1)
			return

		BrowserAutomationStudio_SetWebInterfaceTaskResult(CurrentResultId, JSON.stringify({Message: Message, Result: null,Success:false}))
		CurrentResultId = -1
	}

}