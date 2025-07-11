class HelpManager
{
	constructor()
	{
	}

	GenearateId()
	{
		return Math.floor(Math.random() * (1000000000 - 100)) + 100
	}

	SetPositionToParent(Id)
	{
		_MainView.model.attributes["insert_index"] = IndexOf(Id)
        _MainView.model.attributes["insert_parent"] = Id

        _MainView.UpdateInsertDataInterface()
        UpdateInsertPlacesTooltips();
	}

	/*SetPositionNextTo(Id)
	{
		_MainView.model.attributes["insert_index"] = IndexOf(Id) + 1
        _MainView.model.attributes["insert_parent"] = parseInt(FindTaskById(Id).get("parentid"))

        _MainView.UpdateInsertDataInterface()
        UpdateInsertPlacesTooltips();
	}*/

	AddTask(JsonDat,Code,Id,Name)
	{
		BrowserAutomationStudio_AddTask(Name,`/*Dat:${utf8_to_b64(JSON.stringify(JsonDat))}*/
			${Code}`,Id)
		return Id
	}

	ChangeStart()
	{
        BrowserAutomationStudio_StopSaveToUndoManager = true
	}

	ChangeEnd()
	{
        BrowserAutomationStudio_StopSaveToUndoManager = false
		BrowserAutomationStudio_SaveToUndoManager()
		BrowserAutomationStudio_AskForVariablesUpdateIfNeeded()
	}

	If(Expression,Code,Description)
	{
		if(_GobalModel.get("isscriptexecuting"))
		{
			return;
		}
		var IdIf = this.GenearateId()
		var IdElse = this.GenearateId()
		var IdLog = this.GenearateId()
		var IdLog2 = this.GenearateId()

		var VariableName = Math.floor(Math.random() * (999999 - 100000) + 100000) 

		var IfData = {"s":"if","v":1,"role":"master","slave":IdElse,"master":IdIf,"if_var":"_cycle_params().if_else","f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"IfExpression","type":"constr","data":Expression,"class":"expression"},{"id":"IfElse","type":"check","data":true}]}
		var IfCode = `_cycle_params().if_else = ${Code};
		_set_if_expression("${utf8_to_b64(Expression)}");
		_if(_cycle_params().if_else,function(){
			section_insert()
		})!`


		var LogData = {"s":"log","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LogText","type":"constr","data":tr("Condition is met"),"class":"string"}]}
		var LogCode = `log("${tr("Condition is met")}")`

		var ElseData = {"s":"if","v":1,"role":"slave","slave":IdElse,"master":IdIf,"if_var":"_cycle_params().if_else","f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"IfExpression","type":"constr","data":Expression,"class":"expression"},{"id":"IfElse","type":"check","data":true}]}
		var ElseCode = `_if(!_cycle_params().if_else,function(){
			section_insert()
			})!
			delete _cycle_params().if_else;`

		var Log2Data = {"s":"log","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LogText","type":"constr","data":tr("Condition is not met"),"class":"string"}]}
		var Log2Code = `log("${tr("Condition is not met")}")`

		
		this.ChangeStart()
			var Name = "";
			if(Description)
				Name = JSON.stringify({n:Description})
			this.AddTask(IfData,IfCode,IdIf,Name)
			this.AddTask(ElseData,ElseCode,IdElse,JSON.stringify({n:"Else"}))
			this.SetPositionToParent(IdIf)
			this.AddTask(LogData,LogCode,IdLog,JSON.stringify({c:"1"}))
			this.SetPositionToParent(IdElse)
			this.AddTask(Log2Data,Log2Code,IdLog2,JSON.stringify({c:"5"}))
		this.ChangeEnd()

		BrowserAutomationStudio_FocusAction(IdIf,true)
		BrowserAutomationStudio_FocusAction(IdLog,false)
		BrowserAutomationStudio_FocusAction(IdLog2,false)
	}

	Repeat10Times()
	{
		if(_GobalModel.get("isscriptexecuting"))
		{
			return;
		}

		var IdFor = this.GenearateId()
		var IdLog = this.GenearateId()

		var ForData = {"s":"for","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"ForFrom","type":"constr","data":"1","class":"expression"},{"id":"ForTo","type":"constr","data":"10","class":"expression"}]}
		var ForCode = `_do(function(){
			_set_action_info({ name: "For" });
			VAR_CYCLE_INDEX = _iterator() - 1 + parseInt(1)
			if(VAR_CYCLE_INDEX > parseInt(10))_break();
			section_insert()
		})!`

		var LogData = {"s":"log","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LogText","type":"constr","data":(tr("Current loop repetition") + " : [[CYCLE_INDEX]]"),"class":"string"}]}
		var LogCode = `log("${tr("Current loop repetition")} : " + VAR_CYCLE_INDEX)`

		this.ChangeStart()

			this.AddTask(ForData,ForCode,IdFor,"")
			this.SetPositionToParent(IdFor)
			this.AddTask(LogData,LogCode,IdLog,"")

		this.ChangeEnd()

		BrowserAutomationStudio_FocusAction(IdFor,true)
        BrowserAutomationStudio_FocusAction(IdLog,false)
	}

	RepeatInfinite()
	{
		if(_GobalModel.get("isscriptexecuting"))
		{
			return;
		}

		var IdWhile = this.GenearateId()
		var IdLog = this.GenearateId()
		var IdSleep = this.GenearateId()

		var WhileData = {"s":"while","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"WhileExpression","type":"constr","data":"true","class":"expression"}]}
		var WhileCode = `_do(function(){
			_set_action_info({ name: "While" });
			VAR_CYCLE_INDEX = _iterator() - 1
			BREAK_CONDITION = true;
			if(!BREAK_CONDITION)_break();
			section_insert()
		})!`

		var LogData = {"s":"log","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LogText","type":"constr","data":(tr("Current loop repetition") + " : [[CYCLE_INDEX]]"),"class":"string"}]}
		var LogCode = `log("${tr("Current loop repetition")} : " + VAR_CYCLE_INDEX)`

		var	SleepData =	{"s":"sleep","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"sleepfrom","type":"constr","data":"1000","class":"int"},{"id":"sleepto","type":"constr","data":"1000","class":"int"},{"id":"sleepfromto","type":"constr","data":"1000","class":"int"},{"id":"sleeprandom","type":"check","data":false}]}
		var SleepCode = `sleep(1000)!`		

		this.ChangeStart()

			this.AddTask(WhileData,WhileCode,IdWhile,"")
			this.SetPositionToParent(IdWhile)
			this.AddTask(LogData,LogCode,IdLog,"")
			this.AddTask(SleepData,SleepCode,IdSleep,"")

		this.ChangeEnd()

		BrowserAutomationStudio_FocusAction(IdWhile,true)
        BrowserAutomationStudio_FocusAction(IdLog,false)
        BrowserAutomationStudio_FocusAction(IdSleep,false)
	}

	RepeatUntil()
	{
		if(_GobalModel.get("isscriptexecuting"))
		{
			return;
		}

		var IdWhile = this.GenearateId()
		var IdLog = this.GenearateId()
		var IdSleep = this.GenearateId()
		var IdIf = this.GenearateId()
		var IdLabelFrom = this.GenearateId()
		var IdLabelTo = this.GenearateId()

		var	LabelName = Math.floor(Math.random() * (999999 - 100000) + 100000)


		var WhileData = {"s":"while","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"WhileExpression","type":"constr","data":"true","class":"expression"}]}
		var WhileCode = `_do(function(){
			_set_action_info({ name: "While" });
			VAR_CYCLE_INDEX = _iterator() - 1
			BREAK_CONDITION = true;
			if(!BREAK_CONDITION)_break();
			section_insert()
		})!`

		var LogData = {"s":"log","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LogText","type":"constr","data":(tr("Current loop repetition") + " : [[CYCLE_INDEX]]"),"class":"string"}]}
		var LogCode = `log("${tr("Current loop repetition")} : " + VAR_CYCLE_INDEX)`

		var	SleepData =	{"s":"sleep","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"sleepfrom","type":"constr","data":"1000","class":"int"},{"id":"sleepto","type":"constr","data":"1000","class":"int"},{"id":"sleepfromto","type":"constr","data":"1000","class":"int"},{"id":"sleeprandom","type":"check","data":false}]}
		var SleepCode = `sleep(1000)!`		

		var	IfData = {"s":"if","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"IfExpression","type":"constr","data":"[[TEST]] == \"test\"","class":"expression"},{"id":"IfElse","type":"check","data":false}]}
		var IfCode = `_set_if_expression("${utf8_to_b64('[[TEST]] == "test"')}");
		_if(VAR_TEST == "test",function(){
			section_insert()
		})!`		

		var	LabelFromData = {"s":"goto","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LabelName","type":"text","data":"End loop " + LabelName}]}
		var LabelFromCode = `_goto("End loop ${LabelName}")!`		

		var	LabelToData = {"s":"label","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"Label","type":"constr","data":"End loop " + LabelName,"class":"string"}]}
		var LabelToCode = `_set_goto_label("End loop ${LabelName}")!`		

		this.ChangeStart()

			var save_insert_index = _MainView.model.attributes["insert_index"]
			if(save_insert_index < 0)
				save_insert_index = -save_insert_index
			else
				save_insert_index += 1


        	var save_insert_parent = _MainView.model.attributes["insert_parent"]

			this.AddTask(WhileData,WhileCode,IdWhile,"")

			_MainView.model.attributes["insert_index"] = save_insert_index
        	_MainView.model.attributes["insert_parent"] = save_insert_parent

			_MainView.UpdateInsertDataInterface()
        	UpdateInsertPlacesTooltips();


			this.AddTask(LabelToData,LabelToCode,IdLabelTo,"")
			this.SetPositionToParent(IdWhile)
			
			save_insert_index = _MainView.model.attributes["insert_index"]
			if(save_insert_index < 0)
				save_insert_index = -save_insert_index
			else
				save_insert_index += 1

        	save_insert_parent = _MainView.model.attributes["insert_parent"]

			this.AddTask(IfData,IfCode,IdIf,JSON.stringify({n:tr("Replace condition to end loop")}))

			
			_MainView.model.attributes["insert_index"] = save_insert_index
        	_MainView.model.attributes["insert_parent"] = save_insert_parent

			_MainView.UpdateInsertDataInterface()
        	UpdateInsertPlacesTooltips();

    		this.AddTask(LogData,LogCode,IdLog,"")
			this.AddTask(SleepData,SleepCode,IdSleep,"")
	
			this.SetPositionToParent(IdIf)

			this.AddTask(LabelFromData,LabelFromCode,IdLabelFrom,"")


		this.ChangeEnd()

		BrowserAutomationStudio_FocusAction(IdWhile,true)
        BrowserAutomationStudio_FocusAction(IdLabelTo,false)
        BrowserAutomationStudio_FocusAction(IdIf,false)
        BrowserAutomationStudio_FocusAction(IdLog,false)
        BrowserAutomationStudio_FocusAction(IdSleep,false)
        BrowserAutomationStudio_FocusAction(IdLabelFrom,false)
    }


	RepeatForeachList()
	{
		if(_GobalModel.get("isscriptexecuting"))
		{
			return;
		}

		var IdForEach = this.GenearateId()
		var IdLog1 = this.GenearateId()
		var IdLog2 = this.GenearateId()

		var ForEachData = {"s":"foreach","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"ForArray","type":"constr","data":"[[LIST]]","class":"expression"}]}
		var	RandomId = Math.floor(Math.random() * (999999 - 100000) + 100000)
		var ForEachCode = `
			_do_with_params({"foreach_data":(VAR_LIST)},function(){
			_set_action_info({ name: "Foreach" });
			VAR_CYCLE_INDEX = _iterator() - 1
			if(VAR_CYCLE_INDEX > _cycle_param("foreach_data").length - 1)_break();
			VAR_FOREACH_DATA = _cycle_param("foreach_data")[VAR_CYCLE_INDEX]
			section_insert()
			})!`

		var Log1Data = {"s":"log","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LogText","type":"constr","data":(tr("Current data") + " : [[FOREACH_DATA]]"),"class":"string"}]}
		var Log1Code = `log("${tr("Current data")} : " + VAR_FOREACH_DATA)`

		var Log2Data = {"s":"log","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LogText","type":"constr","data":(tr("Current loop repetition") + " : [[CYCLE_INDEX]]"),"class":"string"}]}
		var Log2Code = `log("${tr("Current loop repetition")} : " + VAR_CYCLE_INDEX)`

		this.ChangeStart()

			this.AddTask(ForEachData,ForEachCode,IdForEach,JSON.stringify({n:tr("Replace [[LIST]] with your list")}))
			this.SetPositionToParent(IdForEach)
			this.AddTask(Log1Data,Log1Code,IdLog1,"")
			this.AddTask(Log2Data,Log2Code,IdLog2,"")

		this.ChangeEnd()

		BrowserAutomationStudio_FocusAction(IdForEach,true)
        BrowserAutomationStudio_FocusAction(IdLog1,false)
        BrowserAutomationStudio_FocusAction(IdLog2,false)
	}

	RepeatForeachResource()
	{
		bootbox.alert(tr("In order to iterate over resource, set 'Run type' to 'Repeat' and user resource anywhere in a script, like ") + 
			"<a href=\"#\" class=\"tr\" onclick=\"BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=browserautomationstudio_2019-08-10_13-55-07.png');return false\">" + tr("[[this]]") + "</a>.<br/>" +
			tr("You also need to use 'Run' mode because in 'Record' mode only first line will be processed,") + 
			" <a href=\"#\" class=\"tr\" onclick=\"BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=browserautomationstudio_2019-08-03_16-56-55.png');return false\">" + tr("[[screenshot]]") + "</a>.<br/>"
		)

	}

	RepeatForeachUserDefinedList()
	{
		if(_GobalModel.get("isscriptexecuting"))
		{
			return;
		}

		var IdCreateList = this.GenearateId()
		var IdForEach = this.GenearateId()
		var IdLog1 = this.GenearateId()
		var IdLog2 = this.GenearateId()


		var CreateListData = {"s":"CreateList","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"Variable","type":"var","data":"NEW_LIST"},{"id":"Value1","type":"constr","data":"a","class":"string"},{"id":"Value2","type":"constr","data":"b","class":"string"},{"id":"Value3","type":"constr","data":"c","class":"string"}]}
		var CreateListCode = `VAR_NEW_LIST = []
   VAR_NEW_LIST.push("a")
   VAR_NEW_LIST.push("b")
   VAR_NEW_LIST.push("c")`

		var ForEachData = {"s":"foreach","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"ForArray","type":"constr","data":"[[NEW_LIST]]","class":"expression"}]}
		var	RandomId = Math.floor(Math.random() * (999999 - 100000) + 100000)
		var ForEachCode = `_do_with_params({"foreach_data":(VAR_NEW_LIST)},function(){
			_set_action_info({ name: "Foreach" });
			VAR_CYCLE_INDEX = _iterator() - 1
			if(VAR_CYCLE_INDEX > _cycle_param("foreach_data").length - 1)_break();
			VAR_FOREACH_DATA = _cycle_param("foreach_data")[VAR_CYCLE_INDEX]
			section_insert()
			})!`

		var Log1Data = {"s":"log","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LogText","type":"constr","data":(tr("Current data") + " : [[FOREACH_DATA]]"),"class":"string"}]}
		var Log1Code = `log("${tr("Current data")} : " + VAR_FOREACH_DATA)`

		var Log2Data = {"s":"log","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LogText","type":"constr","data":(tr("Current loop repetition") + " : [[CYCLE_INDEX]]"),"class":"string"}]}
		var Log2Code = `log("${tr("Current loop repetition")} : " + VAR_CYCLE_INDEX)`

		this.ChangeStart()
			
			this.AddTask(CreateListData,CreateListCode,IdCreateList,JSON.stringify({n:tr("Create new list with 3 values: a, b, c.")}))
			this.AddTask(ForEachData,ForEachCode,IdForEach,"")
			this.SetPositionToParent(IdForEach)
			this.AddTask(Log1Data,Log1Code,IdLog1,"")
			this.AddTask(Log2Data,Log2Code,IdLog2,"")

		this.ChangeEnd()

		BrowserAutomationStudio_FocusAction(IdCreateList,true)
		BrowserAutomationStudio_FocusAction(IdForEach,true)
        BrowserAutomationStudio_FocusAction(IdLog1,false)
        BrowserAutomationStudio_FocusAction(IdLog2,false)
	}

	ExecuteIfTrue()
	{
		this.If("[[YOUR_VARIABLE]]","VAR_YOUR_VARIABLE",tr("Replace [[YOUR_VARIABLE]]"))
	}

	ExecuteIfEqual()
	{
		this.If("[[YOUR_VARIABLE]] == \"Your value\"","VAR_YOUR_VARIABLE == \"Your value\"",tr("Replace [[YOUR_VARIABLE]] and \"Your value\""))
	}

	ExecuteIfGreaterThan()
	{
		this.If("[[YOUR_VARIABLE]] > 5","VAR_YOUR_VARIABLE > 5",tr("Replace [[YOUR_VARIABLE]] and 5"))
	}

	ExecuteAlways()
	{
		this.If("true","true","")
	}

	NeverExecute()
	{
		this.If("false","false","")
	}

	ExecuteIfVariableContains()
	{
		this.If("[[YOUR_VARIABLE]].indexOf(\"Your value\") >= 0","VAR_YOUR_VARIABLE.indexOf(\"Your value\") >= 0",tr("Replace [[YOUR_VARIABLE]] and \"Your value\""))
	}

	LoadPageGetContent()
	{
		if(_GobalModel.get("isscriptexecuting"))
		{
			return;
		}

		var IdLoad = this.GenearateId()
		var IdContent = this.GenearateId()
		var IdLog = this.GenearateId()

		var LoadData = {"s":"load","v":1,"f":[],"uw":"1","ut":"0","uto":"0","um":"0","d":[{"id":"LoadUrl","type":"constr","data":"ip.bablosoft.com","class":"string"}]}
		var LoadCode = `load("ip.bablosoft.com")!`

		var LogData = {"s":"log","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LogText","type":"constr","data":(tr("Page content is ") + "[[SAVED_PAGE_HTML]]"),"class":"string"}]}
		var LogCode = `log("${tr("Page content is ")}" + VAR_SAVED_PAGE_HTML)`

		var	ContentData =	{"s":"html","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"Save","type":"var","data":"SAVED_PAGE_HTML"}]}
		var ContentCode = `/*Browser*/
   page().script("_BAS_HIDE(BrowserAutomationStudio_GetPageContent)()")!
   VAR_SAVED_PAGE_HTML = _result()`		

		this.ChangeStart()

			this.AddTask(LoadData,LoadCode,IdLoad,"")
			this.AddTask(ContentData,ContentCode,IdContent,"")
			this.AddTask(LogData,LogCode,IdLog,"")


		this.ChangeEnd()

		BrowserAutomationStudio_FocusAction(IdLoad,true)
        BrowserAutomationStudio_FocusAction(IdContent,false)
        BrowserAutomationStudio_FocusAction(IdLog,false)
	}

	LoadPageDoXpath()
	{
		if(_GobalModel.get("isscriptexecuting"))
		{
			return;
		}

		var IdLoad = this.GenearateId()
		var IdContent = this.GenearateId()
		var IdXpath = this.GenearateId()
		var IdLog = this.GenearateId()

		var LoadData = {"s":"load","v":1,"f":[],"uw":"1","ut":"0","uto":"0","um":"0","d":[{"id":"LoadUrl","type":"constr","data":"google.com","class":"string"}]}
		var LoadCode = `load("google.com")!`

		var LogData = {"s":"log","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LogText","type":"constr","data":(tr("Page title is ") + "[[XPATH_TEXT]]"),"class":"string"}]}
		var LogCode = `log("${tr("Page title is ")}" + VAR_XPATH_TEXT)`

		var	ContentData =	{"s":"html","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"Save","type":"var","data":"SAVED_PAGE_HTML"}]}
		var ContentCode = `/*Browser*/
   page().script("_BAS_HIDE(BrowserAutomationStudio_GetPageContent)()")!
   VAR_SAVED_PAGE_HTML = _result()`		

	    var XpathData = {"s":"XpathText","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"Text","type":"constr","data":"[[SAVED_PAGE_HTML]]","class":"string"},{"id":"Value","type":"constr","data":"//title","class":"string"},{"id":"Save","type":"var","data":"XPATH_TEXT"},{"id":"Check","type":"check","data":true}]}
		var XpathCode = `html_parser_xpath_parse(VAR_SAVED_PAGE_HTML)
   if((true) && !html_parser_xpath_exist("//title"))
   fail("Can't resolve query " + "//title");
   VAR_XPATH_TEXT = html_parser_xpath_text("//title")`


		this.ChangeStart()

			this.AddTask(LoadData,LoadCode,IdLoad,"")
			this.AddTask(ContentData,ContentCode,IdContent,"")
			this.AddTask(XpathData,XpathCode,IdXpath,"")
			this.AddTask(LogData,LogCode,IdLog,"")


		this.ChangeEnd()

		BrowserAutomationStudio_FocusAction(IdLoad,true)
        BrowserAutomationStudio_FocusAction(IdContent,false)
        BrowserAutomationStudio_FocusAction(IdXpath,false)
        BrowserAutomationStudio_FocusAction(IdLog,false)
	}

	CreateCycles()
	{
		if(_GobalModel.get("isscriptexecuting"))
		{
			return;
		}

		var IdSetLabelStart = this.GenearateId()
		var IdMoveToLabelStart = this.GenearateId()
		var IdSetLabelEnd = this.GenearateId()
		var IdMoveToLabelEnd = this.GenearateId()

		var IdIncrease = this.GenearateId()
		var IdIf = this.GenearateId()
		var IdLog = this.GenearateId()
		var IdSetVariable = this.GenearateId()

		var	LabelName = Math.floor(Math.random() * (999999 - 100000) + 100000)


		var SetLabelStartData = {"s":"label","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"Label","type":"constr","data":"Start loop " + LabelName,"class":"string"}]}
		var SetLabelStartCode = `_set_goto_label("Start loop ${LabelName}")!`
		var MoveToLabelStartData = {"s":"goto","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LabelName","type":"text","data":"Start loop " + LabelName}]}
		var MoveToLabelStartCode = `_goto("Start loop ${LabelName}")!`		
		var SetLabelEndData = {"s":"label","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"Label","type":"constr","data":"End loop " + LabelName,"class":"string"}]}
		var SetLabelEndCode = `_set_goto_label("End loop ${LabelName}")!`
		var MoveToLabelEndData = {"s":"goto","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LabelName","type":"text","data":"End loop " + LabelName}]}
		var MoveToLabelEndCode = `_goto("End loop ${LabelName}")!`		

		var IncreaseData = {"s":"incvariable","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"SetVariableName","type":"var","data":"INDEX"},{"id":"IncVariableValue","type":"constr","data":"1","class":"int"}]}
		var IncreaseCode = `VAR_INDEX = parseInt(VAR_INDEX) + parseInt(1)`
		var IfData = {"s":"if","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"IfExpression","type":"constr","data":"[[INDEX]] > 5","class":"expression"},{"id":"IfElse","type":"check","data":false}]}
		var IfCode = `_set_if_expression("${utf8_to_b64('[[INDEX]] > 5')}");
		_if(VAR_INDEX > 5,function(){
			section_insert()
		})!`
		var LogData = {"s":"log","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LogText","type":"constr","data":(tr("Current loop repetition") + " : [[INDEX]]"),"class":"string"}]}
		var LogCode = `log("${tr("Current loop repetition")} : " + VAR_INDEX)`

		var SetVariableData = {"s":"setvariable","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"SetVariableName","type":"var","data":"INDEX"},{"id":"SetVariableValue","type":"constr","data":"0","class":"expression"}]}
		var SetVariableCode = `VAR_INDEX = 0`
		
		this.ChangeStart()

			this.AddTask(SetVariableData,SetVariableCode,IdSetVariable,"")
			this.AddTask(SetLabelStartData,SetLabelStartCode,IdSetLabelStart,"")
			

			var save_insert_index = _MainView.model.attributes["insert_index"]
			if(save_insert_index < 0)
				save_insert_index = -save_insert_index
			else
				save_insert_index += 1


        	var save_insert_parent = _MainView.model.attributes["insert_parent"]

			this.AddTask(IfData,IfCode,IdIf,"")

			_MainView.model.attributes["insert_index"] = save_insert_index
        	_MainView.model.attributes["insert_parent"] = save_insert_parent

			_MainView.UpdateInsertDataInterface()
        	UpdateInsertPlacesTooltips();


			this.AddTask(LogData,LogCode,IdLog,"")
			this.AddTask(IncreaseData,IncreaseCode,IdIncrease,"")
			this.AddTask(MoveToLabelStartData,MoveToLabelStartCode,IdMoveToLabelStart,"")
			this.AddTask(SetLabelEndData,SetLabelEndCode,IdSetLabelEnd,"")

			this.SetPositionToParent(IdIf)
			this.AddTask(MoveToLabelEndData,MoveToLabelEndCode,IdMoveToLabelEnd,"")

			

		this.ChangeEnd()


		BrowserAutomationStudio_FocusAction(IdSetLabelStart,true)
		BrowserAutomationStudio_FocusAction(IdMoveToLabelStart,false)
		BrowserAutomationStudio_FocusAction(IdSetLabelEnd,false)
		BrowserAutomationStudio_FocusAction(IdMoveToLabelEnd,false)
		BrowserAutomationStudio_FocusAction(IdIncrease,false)
		BrowserAutomationStudio_FocusAction(IdIf,false)
		BrowserAutomationStudio_FocusAction(IdLog,false)
		BrowserAutomationStudio_FocusAction(IdSetVariable,false)
	}

	ReloadPage()
	{
		if(_GobalModel.get("isscriptexecuting"))
		{
			return;
		}

		var IdSetLabel = this.GenearateId()
		var IdMoveToLabel = this.GenearateId()
		
		var IdIgnore = this.GenearateId()
		var IdIf = this.GenearateId()
		var IdLoad = this.GenearateId()
		
		var	LabelName = Math.floor(Math.random() * (999999 - 100000) + 100000)


		var SetLabelData = {"s":"label","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"Label","type":"constr","data":"Retry loading " + LabelName,"class":"string"}]}
		var SetLabelCode = `_set_goto_label("Retry loading ${LabelName}")!`
		var MoveToLabelData = {"s":"goto","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LabelName","type":"text","data":"Retry loading " + LabelName}]}
		var MoveToLabelCode = `_goto("Retry loading ${LabelName}")!`		
		
		var IgnoreData = {"s":"ignoreerrors","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[]}
		var IgnoreCode = `_call(function()
		   {
		   _on_fail(function(){
		   VAR_LAST_ERROR = _result()
		   VAR_ERROR_ID = ScriptWorker.GetCurrentAction()
		   VAR_WAS_ERROR = false
		   _break(1,true)
		   })
		   CYCLES.Current().RemoveLabel("function")
		   section_insert()
		   },null)!`

		var LoadData = {"s":"load","v":1,"f":[],"uw":"1","ut":"0","uto":"0","um":"0","d":[{"id":"LoadUrl","type":"constr","data":"google.com","class":"string"}]}
		var LoadCode = `/*Browser*/
      load("google.com")!`

		var IfData = {"s":"if","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"IfExpression","type":"constr","data":"[[WAS_ERROR]]","class":"expression"},{"id":"IfElse","type":"check","data":false}]}
		var IfCode = `_set_if_expression("${utf8_to_b64('[[WAS_ERROR]]')}");
		_if(VAR_WAS_ERROR,function(){
			section_insert()
		})!`

		
		
		this.ChangeStart()
			this.AddTask(SetLabelData,SetLabelCode,IdSetLabel,"")
			

			var save_insert_index = _MainView.model.attributes["insert_index"]
			if(save_insert_index < 0)
				save_insert_index = -save_insert_index
			else
				save_insert_index += 1


        	var save_insert_parent = _MainView.model.attributes["insert_parent"]

			this.AddTask(IgnoreData,IgnoreCode,IdIgnore,"")

			_MainView.model.attributes["insert_index"] = save_insert_index
        	_MainView.model.attributes["insert_parent"] = save_insert_parent

			_MainView.UpdateInsertDataInterface()
        	UpdateInsertPlacesTooltips();
			this.AddTask(IfData,IfCode,IdIf,"")
			this.SetPositionToParent(IdIf)
			this.AddTask(MoveToLabelData,MoveToLabelCode,IdMoveToLabel,"")

			this.SetPositionToParent(IdIgnore)
			this.AddTask(LoadData,LoadCode,IdLoad,"")			

		this.ChangeEnd()


		BrowserAutomationStudio_FocusAction(IdSetLabel,true)
		BrowserAutomationStudio_FocusAction(IdMoveToLabel,false)
		BrowserAutomationStudio_FocusAction(IdIgnore,false)
		BrowserAutomationStudio_FocusAction(IdIf,false)
		BrowserAutomationStudio_FocusAction(IdLoad,false)
		
	}

	ExtractTitle()
	{
		if(_GobalModel.get("isscriptexecuting"))
		{
			return;
		}

		var IdLoad = this.GenearateId()
		var IdContent = this.GenearateId()
		var IdMatch = this.GenearateId()
		var IdLog = this.GenearateId()

		var LoadData = {"s":"load","v":1,"f":[],"uw":"1","ut":"0","uto":"0","um":"0","d":[{"id":"LoadUrl","type":"constr","data":"gmail.com","class":"string"}]}
		var LoadCode = `load("gmail.com")!`

		var LogData = {"s":"log","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LogText","type":"constr","data":(tr("Page title") + " : [[TITLE]]"),"class":"string"}]}
		var LogCode = `log("${tr("Page title")} : " + VAR_TITLE)`

		var MatchData = {"s":"RegexpFirst","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"Value","type":"constr","data":"[[SAVED_PAGE_HTML]]","class":"string"},{"id":"Regexp","type":"constr","data":"\\<title\\>(.*)\\<\\/title\\>","class":"string"},{"id":"Result","type":"var","data":"GROUP1,GROUP2,GROUP3"},{"id":"ResultAll","type":"var","data":"TITLE"}]}
		var MatchCode = `var regexp_result = native("regexp", "first", JSON.stringify({text: VAR_SAVED_PAGE_HTML,regexp:"\u005c\u003ctitle\u005c\u003e(.*)\u005c\u003c\u005c/title\u005c\u003e"}))
				   if(regexp_result.length == 0)
				   regexp_result = []
				   else
				   regexp_result = JSON.parse(regexp_result)
				   VAR_TITLE = regexp_result.pop()
				   if(typeof(VAR_TITLE) == 'undefined' || !VAR_TITLE)
				   VAR_TITLE = ""
				   VAR_GROUP1 = regexp_result[0]
				   if(typeof(VAR_GROUP1) == 'undefined' || !VAR_GROUP1)
				   VAR_GROUP1 = ""
				   VAR_GROUP2 = regexp_result[1]
				   if(typeof(VAR_GROUP2) == 'undefined' || !VAR_GROUP2)
				   VAR_GROUP2 = ""
				   VAR_GROUP3 = regexp_result[2]
				   if(typeof(VAR_GROUP3) == 'undefined' || !VAR_GROUP3)
				   VAR_GROUP3 = ""
				   if(regexp_result.length == 0)
				   {
				   VAR_GROUP1 = VAR_TITLE
				   }`
		
		var	ContentData =	{"s":"html","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"Save","type":"var","data":"SAVED_PAGE_HTML"}]}
		var ContentCode = `/*Browser*/
   page().script("_BAS_HIDE(BrowserAutomationStudio_GetPageContent)()")!
   VAR_SAVED_PAGE_HTML = _result()`	

   		this.ChangeStart()

			this.AddTask(LoadData,LoadCode,IdLoad,"")
			this.AddTask(ContentData,ContentCode,IdContent,"")
			this.AddTask(MatchData,MatchCode,IdMatch,"")
			this.AddTask(LogData,LogCode,IdLog,"")			

		this.ChangeEnd()


		BrowserAutomationStudio_FocusAction(IdLoad,true)
		BrowserAutomationStudio_FocusAction(IdContent,false)
		BrowserAutomationStudio_FocusAction(IdMatch,false)
		BrowserAutomationStudio_FocusAction(IdLog,false)

	}

	ExtractLinks()
	{
		if(_GobalModel.get("isscriptexecuting"))
		{
			return;
		}

		var IdLoad = this.GenearateId()
		var IdContent = this.GenearateId()
		var IdMatch = this.GenearateId()
		var IdLog = this.GenearateId()

		var LoadData = {"s":"load","v":1,"f":[],"uw":"1","ut":"0","uto":"0","um":"0","d":[{"id":"LoadUrl","type":"constr","data":"gmail.com","class":"string"}]}
		var LoadCode = `load("gmail.com")!`

		var LogData = {"s":"log","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LogText","type":"constr","data":(tr("List of links") + " : [[XPATH_TEXT_LIST]]"),"class":"string"}]}
		var LogCode = `log("${tr("List of links")} : " + VAR_XPATH_TEXT_LIST)`

		var MatchData = {"s":"XpathTextList","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"Text","type":"constr","data":"[[SAVED_PAGE_HTML]]","class":"string"},{"id":"Value","type":"constr","data":"//a/@href","class":"string"},{"id":"Save","type":"var","data":"XPATH_TEXT_LIST"}]}
		var MatchCode = `html_parser_xpath_parse(VAR_SAVED_PAGE_HTML)
   VAR_XPATH_TEXT_LIST = html_parser_xpath_text_list("//a/@href")`
		
		var	ContentData =	{"s":"html","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"Save","type":"var","data":"SAVED_PAGE_HTML"}]}
		var ContentCode = `/*Browser*/
   page().script("_BAS_HIDE(BrowserAutomationStudio_GetPageContent)()")!
   VAR_SAVED_PAGE_HTML = _result()`	

   		this.ChangeStart()

			this.AddTask(LoadData,LoadCode,IdLoad,"")
			this.AddTask(ContentData,ContentCode,IdContent,"")
			this.AddTask(MatchData,MatchCode,IdMatch,"")
			this.AddTask(LogData,LogCode,IdLog,"")			

		this.ChangeEnd()


		BrowserAutomationStudio_FocusAction(IdLoad,true)
		BrowserAutomationStudio_FocusAction(IdContent,false)
		BrowserAutomationStudio_FocusAction(IdMatch,false)
		BrowserAutomationStudio_FocusAction(IdLog,false)
	}

	IterateLinks()
	{
		if(_GobalModel.get("isscriptexecuting"))
		{
			return;
		}

		var IdLoad = this.GenearateId()
		var IdLoop = this.GenearateId()
		var IdGetText = this.GenearateId()
		var IdLog = this.GenearateId()

		var	Rand = Math.floor(Math.random() * (999999 - 100000) + 100000)


		var LoadData = {"s":"load","v":1,"f":[],"uw":"1","ut":"0","uto":"0","um":"0","d":[{"id":"LoadUrl","type":"constr","data":"gmail.com","class":"string"}]}
		var LoadCode = `load("gmail.com")!`

		var LogData = {"s":"log","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LogText","type":"constr","data":(tr("Current link text") + " : [[SAVED_TEXT]]"),"class":"string"}]}
		var LogCode = `log("${tr("Current link text")} : " + VAR_SAVED_TEXT)`

		var LoopData = {"s":"loopelement","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[],"p":{"is_image":false,"css":" >CSS> a","version":"1.0","css1":"","css2":"","css3":"","current":"css","match":">MATCH><a","xpath":" >XPATH> //a","at":"197, 482","we":true,"fa":true}}
		var LoopCode = `/*Browser*/
   ;_SELECTOR=" \u003eCSS\u003e a";
   get_element_selector(_SELECTOR, true).length()!
   
   _do_with_params({"element_length":_result(),"querycss":" \u003eCSS\u003e a","querymatch":"\u003eMATCH\u003e\u003ca", "queryxpath": " \u003eXPATH\u003e //a"},function(){
	 VAR_CYCLE_INDEX = _iterator() - 1
	 VAR_FOR_EACH_CSS = _cycle_param("querycss")
	 VAR_FOR_EACH_MATCH = _cycle_param("querymatch")
     VAR_FOR_EACH_XPATH = _cycle_param("queryxpath")

	 if(VAR_CYCLE_INDEX >= parseInt(_cycle_param("element_length")))_break();
	 section_insert()
   })!
   VAR_CYCLE_INDEX = 0;`
		
		var	GetTextData =	{"s":"text","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"SaveText","type":"var","data":"SAVED_TEXT"}],"p":{"is_image":false,"css":"[[FOR_EACH_CSS]]>AT>[[CYCLE_INDEX]]","version":"1.0","css1":"[[FOR_EACH_CSS]]>AT>[[CYCLE_INDEX]]","css2":"","css3":"","current":"css","match":"[[FOR_EACH_MATCH]]>AT>[[CYCLE_INDEX]]","xpath":"[[FOR_EACH_XPATH]]>AT>[[CYCLE_INDEX]]","at":"298, 353","we":true,"fa":true}}
		var GetTextCode = `/*Browser*/
      _SELECTOR = VAR_FOR_EACH_CSS + "\u003eAT\u003e" + VAR_CYCLE_INDEX;
      wait_element(_SELECTOR)!
      get_element_selector(_SELECTOR, false).text()!
      VAR_SAVED_TEXT = _result()`	

   		this.ChangeStart()

			this.AddTask(LoadData,LoadCode,IdLoad,"")
			this.AddTask(LoopData,LoopCode,IdLoop,"")
			this.SetPositionToParent(IdLoop)

			this.AddTask(GetTextData,GetTextCode,IdGetText,"")
			this.AddTask(LogData,LogCode,IdLog,"")			

		this.ChangeEnd()


		BrowserAutomationStudio_FocusAction(IdLoad,true)
		BrowserAutomationStudio_FocusAction(IdLoop,false)
		BrowserAutomationStudio_FocusAction(IdGetText,false)
		BrowserAutomationStudio_FocusAction(IdLog,false)
	}

	SplitCsv()
	{
		if(_GobalModel.get("isscriptexecuting"))
		{
			return;
		}

		var IdSetVariable = this.GenearateId()
		var IdParse = this.GenearateId()
		var IdLog1 = this.GenearateId()
		var IdLog2 = this.GenearateId()
		var IdLog3 = this.GenearateId()

		var SetVariableData = {"s":"setvariable","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"SetVariableName","type":"var","data":"LINE_FROM_FILE"},{"id":"SetVariableValue","type":"constr","data":"login:password:email","class":"string"}]}
		var SetVariableCode = `VAR_LINE_FROM_FILE = "login:password:email"`

		var Log1Data = {"s":"log","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LogText","type":"constr","data":(tr("Login") + " : [[LOGIN]]"),"class":"string"}]}
		var Log1Code = `log("${tr("Login")} : " + VAR_LOGIN)`

		var Log2Data = {"s":"log","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LogText","type":"constr","data":(tr("Password") + " : [[PASSWORD]]"),"class":"string"}]}
		var Log2Code = `log("${tr("Password")} : " + VAR_PASSWORD)`

		var Log3Data = {"s":"log","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LogText","type":"constr","data":(tr("Email") + " : [[EMAIL]]"),"class":"string"}]}
		var Log3Code = `log("${tr("Email")} : " + VAR_EMAIL)`

		var ParseData = {"s":"parseline","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"Value","type":"constr","data":"[[LINE_FROM_FILE]]","class":"string"},{"id":"VariablesList","type":"var","data":"LOGIN,PASSWORD,EMAIL"}]}
		var ParseCode = `var csv_parse_result = csv_parse(VAR_LINE_FROM_FILE)
   VAR_LOGIN = csv_parse_result[0]
   if(typeof(VAR_LOGIN) == 'undefined' || !VAR_LOGIN)
   {
   VAR_LOGIN = ""
   }
   VAR_PASSWORD = csv_parse_result[1]
   if(typeof(VAR_PASSWORD) == 'undefined' || !VAR_PASSWORD)
   {
   VAR_PASSWORD = ""
   }
   VAR_EMAIL = csv_parse_result[2]
   if(typeof(VAR_EMAIL) == 'undefined' || !VAR_EMAIL)
   {
   VAR_EMAIL = ""
   }`
		
   		this.ChangeStart()

			this.AddTask(SetVariableData,SetVariableCode,IdSetVariable,"")
			this.AddTask(ParseData,ParseCode,IdParse,"")
			this.AddTask(Log1Data,Log1Code,IdLog1,"")			
			this.AddTask(Log2Data,Log2Code,IdLog2,"")			
			this.AddTask(Log3Data,Log3Code,IdLog3,"")			

		this.ChangeEnd()


		BrowserAutomationStudio_FocusAction(IdSetVariable,true)
		BrowserAutomationStudio_FocusAction(IdParse,false)
		BrowserAutomationStudio_FocusAction(IdLog1,false)
		BrowserAutomationStudio_FocusAction(IdLog2,false)
		BrowserAutomationStudio_FocusAction(IdLog3,false)
	}

	SplitMultiline()
	{
		if(_GobalModel.get("isscriptexecuting"))
		{
			return;
		}

		var IdSetVariable = this.GenearateId()
		var IdParse = this.GenearateId()
		var IdForEach = this.GenearateId()
		var IdLog1 = this.GenearateId()
		var IdLog2 = this.GenearateId()

		var SetVariableData = {"s":"template","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"Template","type":"constr","data":"line1\nline2\nline3","class":"expression"},{"id":"Save","type":"var","data":"MULTILINE_STRING"},{"id":"Check2","type":"check","data":false},{"id":"Check3","type":"check","data":false},{"id":"Check","type":"check","data":false}]}
		var SetVariableCode = `VAR_MULTILINE_STRING = "line1\\nline2\\nline3"`

		var ParseData = {"s":"RegexpSplit","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"Value","type":"constr","data":"[[MULTILINE_STRING]]","class":"string"},{"id":"Regexp","type":"constr","data":"\\r?\\n","class":"string"},{"id":"Result","type":"var","data":"LIST_FROM_STRING"}]}
		var ParseCode = `VAR_LIST_FROM_STRING = native("regexp", "split", JSON.stringify({text: VAR_MULTILINE_STRING,regexp:"\u005cr?\u005cn"}))
   if(VAR_LIST_FROM_STRING.length == 0)
   VAR_LIST_FROM_STRING = []
   else
   VAR_LIST_FROM_STRING = JSON.parse(VAR_LIST_FROM_STRING)`

		var ForEachData = {"s":"foreach","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"ForArray","type":"constr","data":"[[LIST_FROM_STRING]]","class":"expression"}]}
		var	RandomId = Math.floor(Math.random() * (999999 - 100000) + 100000)
		var ForEachCode = `
			_do_with_params({"foreach_data":(VAR_LIST_FROM_STRING)},function(){
			_set_action_info({ name: "Foreach" });
			VAR_CYCLE_INDEX = _iterator() - 1
			if(VAR_CYCLE_INDEX > _cycle_param("foreach_data").length - 1)_break();
			VAR_FOREACH_DATA = _cycle_param("foreach_data")[VAR_CYCLE_INDEX]
			section_insert()
			})!`

		var Log1Data = {"s":"log","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LogText","type":"constr","data":(tr("Current data") + " : [[FOREACH_DATA]]"),"class":"string"}]}
		var Log1Code = `log("${tr("Current data")} : " + VAR_FOREACH_DATA)`

		var Log2Data = {"s":"log","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LogText","type":"constr","data":(tr("Current loop repetition") + " : [[CYCLE_INDEX]]"),"class":"string"}]}
		var Log2Code = `log("${tr("Current loop repetition")} : " + VAR_CYCLE_INDEX)`

		this.ChangeStart()

			this.AddTask(SetVariableData,SetVariableCode,IdSetVariable,JSON.stringify({n:tr("Create multiline variables")}))
			this.AddTask(ParseData,ParseCode,IdParse,JSON.stringify({n:tr("Split string into lines")}))
			this.AddTask(ForEachData,ForEachCode,IdForEach,JSON.stringify({n:tr("Iterate on list")}))
			this.SetPositionToParent(IdForEach)
			this.AddTask(Log1Data,Log1Code,IdLog1,"")
			this.AddTask(Log2Data,Log2Code,IdLog2,"")

		this.ChangeEnd()

		BrowserAutomationStudio_FocusAction(IdSetVariable,true)
        BrowserAutomationStudio_FocusAction(IdParse,false)
        BrowserAutomationStudio_FocusAction(IdForEach,false)
        BrowserAutomationStudio_FocusAction(IdLog1,false)
        BrowserAutomationStudio_FocusAction(IdLog2,false)
	}

	WriteTestMessage()
	{
		if(_GobalModel.get("isscriptexecuting"))
		{
			return;
		}

		var IdLog = this.GenearateId()
		
		var LogData = {"s":"log","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LogText","type":"constr","data":tr("Test message"),"class":"string"}]}
		var LogCode = `log("${tr("Test message")}")`

		
		this.ChangeStart()

			this.AddTask(LogData,LogCode,IdLog,"")

		this.ChangeEnd()

		BrowserAutomationStudio_FocusAction(IdLog,true)
	}

	WriteToUserSelectedFile()
	{
		if(_GobalModel.get("isscriptexecuting"))
		{
			return;
		}

		var IdWrite = this.GenearateId()
		
		var WriteData = {"s":"filesystem_writefile","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"File","type":"constr","data":"{{Result File}}","class":"string"},{"id":"Value","type":"constr","data":"Test Message","class":"string"},{"id":"Check3","type":"check","data":true},{"id":"Check","type":"check","data":true},{"id":"Check2","type":"check","data":false}]}
		var WriteCode = `native("filesystem", "writefile", JSON.stringify({path: RInfo("Result File","Location"),value: ("Test Message").toString() + "\\r\\n",base64:false,append:true}))`

		
		this.ChangeStart()

			this.AddTask(WriteData,WriteCode,IdWrite,JSON.stringify({n:tr("Replace 'Result File' to actual resource name")}))

		this.ChangeEnd()

		BrowserAutomationStudio_FocusAction(IdWrite,true)
	}
}
