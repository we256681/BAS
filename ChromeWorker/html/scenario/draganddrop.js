function DragAndDrop()
{
	var DeleteAfterInsert = []
	var DragType = ""

	var _DragStartTaskCotainer = function(event) { 

		var target = $(event.currentTarget)
		if(target.hasClass("taskcontainer"))
		{
			DragType = "action"

			var id = parseInt(target.attr("task-id"))
			var task = FindTaskById(id)

			if(task && id != 0)
			{
				var label = "\"" + target.find(".tooltitle").text().trim() + "\"";
				var selectedtext = "";
				

				if(!task.get("is_selected"))
				{
					//Single
					var Ids = [id]
					var dat = task.dat()
					if(dat)
					{
						if(dat["role"] == "master")
						{
							Ids.push(dat["slave"])
						}else if(dat["role"] == "slave")
						{
							Ids.push(dat["master"])
						}
					}
					DeleteAfterInsert = Ids.slice(0);

					selectedtext = _MainView.CopyToString(Ids)

				}else
				{
					//All selection

					var selection = _MainView.GetSelection()
					DeleteAfterInsert = selection.slice(0);


					if(selection.length > 1)
					{
						label += tr(" and ") + (selection.length - 1).toString() + tr(" more")
					}

					selectedtext = _MainView.CopyToString()
				}

					event.originalEvent.dataTransfer.setDragImage(target[0], 0, 0);
					event.originalEvent.dataTransfer.setData('bas/draganddrop', selectedtext);


					$("#container").addClass("dragactive")

			}
			
			
		}
		
	}

	var _DragStartToolLabelCreate = function(event) { 

		var target = $(event.currentTarget)

		if(target.hasClass("toollabelcreate"))
		{
			var toolinsertdata = target.parents(".toolinsertdata")
			var selectedtext = TaskIdToDataInsertIndex(toolinsertdata.attr("task-id")) + ";" + toolinsertdata.attr("data-insert-parent")

			DragType = "label"
			event.originalEvent.dataTransfer.setDragImage(target[0], -10, -5);
			event.originalEvent.dataTransfer.setData('bas/draganddrop', selectedtext);
			$("#container").addClass("dragactivelabel")
			$('.toollabelcreate').tooltip('hide')

		}

	};

	var _DragEndTaskContainer = function(event) {
		if(DragType == "action")
		{
			$(".toolinsert").removeClass("toolinsertdrop")
			$("#container").removeClass("dragactive")
		}
		if(DragType == "label")
		{
			$(".toolinsert").removeClass("toolinsertdrop")
			$(".tool").removeClass("tooldrop")
			$("#container").removeClass("dragactivelabel")
		}
	}

	var _DragLeaveToolInsertData = function(event) { 
		if(DragType == "action")
		{
			var target = $(event.currentTarget)
			if(target.hasClass("toolinsertdata"))
			{
				target.find(".toolinsert").removeClass("toolinsertdrop")
			}
		}
		if(DragType == "label")
		{
			var target = $(event.currentTarget)
			if(target.hasClass("toolinsertdata"))
			{
				target.find(".toolinsert").removeClass("toolinsertdrop")
			}
		}


	}

	var _DragLeaveTool = function(event) { 

		if(DragType == "label")
		{
			var target = $(event.currentTarget)
			if(target.hasClass("tool"))
			{
				target.removeClass("tooldrop")
			}
		}
	}

	var _DragOverToolInsertData = function(event) { 

		if(DragType == "action")
		{
			var target = $(event.currentTarget)
			$(".toolinsert").removeClass("toolinsertdrop")
			if(target.hasClass("toolinsertdata"))
			{
				target.find(".toolinsert").addClass("toolinsertdrop")
				event.preventDefault();
			}
		}
		if(DragType == "label")
		{
			var target = $(event.currentTarget)
			$(".toolinsert").removeClass("toolinsertdrop")
			$(".tool").removeClass("tooldrop")
			if(target.hasClass("toolinsertdata"))
			{
				target.find(".toolinsert").addClass("toolinsertdrop")
				event.preventDefault();
			}
		}
	}

	var _DragOverTool = function(event) { 
		if(DragType == "label")
		{
			var target = $(event.currentTarget)
			$(".toolinsert").removeClass("toolinsertdrop")
			$(".tool").removeClass("tooldrop")
			if(target.hasClass('tool') && target.attr('data-is-label-to') == "true")
			{
				target.addClass("tooldrop")
				event.preventDefault();
			}
		}
	}

	var _DropToolInsertData = function(event) { 
		if(DragType == "action")
		{
			var target = $(event.currentTarget)
			if(target.hasClass("toolinsertdata"))
			{
				BrowserAutomationStudio_StopSaveToUndoManager = true

				var datainsertindex = TaskIdToDataInsertIndex(parseInt(target.attr("task-id")))
				var datainsertparent = parseInt(target.attr("data-insert-parent"))
				var data = event.originalEvent.dataTransfer.getData('bas/draganddrop')

				_MainView.model.attributes["insert_index"] = datainsertindex
				_MainView.model.attributes["insert_parent"] = datainsertparent
				_MainView.UpdateInsertDataInterface()
				
				var new_ids = _MainView.PasteFinal(data, true)
				var needclear = DeleteAfterInsert.length > 0
				if(needclear && new_ids.length > 0)
				{
					var new_id = new_ids[0]
					needclear = !IsAnsectorOfAny(DeleteAfterInsert,new_id)
				}

				if(needclear)
				{
					_MainView.model.set("dontsend",true)
					_CacheLoader.SetDontUpdate(true)

					var stillexists = true

					while(stillexists)
					{
						stillexists = false
						_.every(_MainView.collection.models, function(task,index){
							if(DeleteAfterInsert.indexOf(parseInt(task.get("id"))) >= 0)
							{
								stillexists = true
								_MainView.currentTargetId = index
								_MainView.Delete()
								return false
							}
							return true
						})
					}
					_CacheLoader.SetDontUpdate(false)
					_CacheLoader.Start()


					_MainView.model.set("dontsend",false)
					BrowserAutomationStudio_StopSaveToUndoManager = false
					BrowserAutomationStudio_SaveToUndoManager()
					BrowserAutomationStudio_AskForVariablesUpdateIfNeeded()
					_MainView.send()
					_MainView.render()
				}else
				{
					BrowserAutomationStudio_StopSaveToUndoManager = false
					BrowserAutomationStudio_SaveToUndoManager()
				}

				DeleteAfterInsert = []

				$(".toolinsert").removeClass("toolinsertdrop")
				$("#container").removeClass("dragactive")

				new_ids.forEach(function(id){
					BrowserAutomationStudio_FocusActionFast(parseInt(id))
				})

			}
		}
		if(DragType == "label")
		{
			var target = $(event.currentTarget)
			if(target.hasClass("toolinsertdata"))
			{
				if(!_GobalModel.get("isscriptexecuting"))
				{
					var data = event.originalEvent.dataTransfer.getData('bas/draganddrop')

					bootbox.prompt({
						title: ("<h4>" + tr("Label Name") + "</h4> <span id='ValidationInsertLabel' style='font-size:small;color:red'></span>"),
						message: '<p style="color: gray; font-size:small">' + tr("Please give a name to a new label. Labels helps to transfer execution point between two arbitrary places inside script.") + '</p><hr/>',
						value: "label name " + (Math.floor(Math.random() * (10000 - 100)) + 100).toString(),
						callback: function (Name) {
							if(Name)
							{
								if(!Name.match(/^[a-zA-Z0-9а-яА-Я_ ]+$/))
								{
									$("#ValidationInsertLabel").text(tr("Label can contain only letters, digits, underscore and spaces"));
									return false;
								}
								var IdLabelTo = Math.floor(Math.random() * (1000000000 - 100)) + 100
								var IdLabelFrom = Math.floor(Math.random() * (1000000000 - 100)) + 100

								var datainsertindefrom = parseInt(data.split(";")[0])
								var datainsertparentfrom = parseInt(data.split(";")[1])
								
								var datainsertindexto = TaskIdToDataInsertIndex(parseInt(target.attr("task-id")))
								var datainsertparentto = parseInt(target.attr("data-insert-parent"))

								var AddOrder = ["to","from"]

								
								if(Math.abs(datainsertindefrom) < Math.abs(datainsertindexto))
								{
									AddOrder = ["to","from"]
								}else if(Math.abs(datainsertindexto) < Math.abs(datainsertindefrom))
								{
									AddOrder = ["from","to"]
								}else if(datainsertindexto == datainsertindefrom)
								{
									var depthto = datainsertparentto
									var depthfrom = datainsertparentfrom
									
									if(depthto == 0)
										depthto = -1
									else
										depthto = GetTaskDepth(depthto)

									if(depthfrom == 0)
										depthfrom = -1
									else
										depthfrom = GetTaskDepth(depthfrom)

									

									if(depthto < depthfrom)
									{
										AddOrder = ["to","from"]
									}else
									{
										AddOrder = ["from","to"]
									}
								}else if(datainsertindexto < 0)
								{
									AddOrder = ["from","to"]
								}else if(datainsertindefrom < 0)
								{
									AddOrder = ["to","from"]
								}

								BrowserAutomationStudio_StopSaveToUndoManager = true

								for(var i = 0;i<2;i++)
								{
									var CurrentAction = AddOrder[i]
									if(CurrentAction == "from")
									{
										_MainView.model.attributes["insert_index"] = datainsertindefrom
										_MainView.model.attributes["insert_parent"] = datainsertparentfrom
										_MainView.UpdateInsertDataInterface()

										UpdateInsertPlacesTooltips();


										var DataJsonFrom = {"s":"goto","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LabelName","type":"text","data":Name}]}
										
										BrowserAutomationStudio_AddTask("",`/*Dat:${utf8_to_b64(JSON.stringify(DataJsonFrom))}*/
		_goto(${JSON.stringify(Name)})!`,IdLabelFrom)

									}else
									{
										_MainView.model.attributes["insert_index"] = datainsertindexto
										_MainView.model.attributes["insert_parent"] = datainsertparentto
										_MainView.UpdateInsertDataInterface()

										UpdateInsertPlacesTooltips();

										var DataJsonTo = {"s":"label","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"Label","type":"constr","data":Name,"class":"string"}]}
										
										BrowserAutomationStudio_AddTask("",`/*Dat:${utf8_to_b64(JSON.stringify(DataJsonTo))}*/
		_set_goto_label(${JSON.stringify(Name)})!`,IdLabelTo)
									}
								}

								BrowserAutomationStudio_StopSaveToUndoManager = false
								BrowserAutomationStudio_SaveToUndoManager()
								BrowserAutomationStudio_AskForVariablesUpdateIfNeeded()

								   BrowserAutomationStudio_FocusAction(IdLabelFrom,false)
								BrowserAutomationStudio_FocusAction(IdLabelTo,true)
							}
						}
					});

					

				}

				$(".toolinsert").removeClass("toolinsertdrop")
				$(".tool").removeClass("tooldrop")
				$("#container").removeClass("dragactivelabel")
			}
		}
	

	}

	var _DropTool = function(event) { 
			
		if(DragType == "label")
		{
			var target = $(event.currentTarget)
			if(target.hasClass("tool"))
			{
				if(!_GobalModel.get("isscriptexecuting"))
				{
					var data = event.originalEvent.dataTransfer.getData('bas/draganddrop')
					
					var IdLabelFrom = Math.floor(Math.random() * (1000000000 - 100)) + 100

					var datainsertindefrom = parseInt(data.split(";")[0])
					var datainsertparentfrom = parseInt(data.split(";")[1])
					
					var Name = target.attr("data-label-name")

					BrowserAutomationStudio_StopSaveToUndoManager = true

					_MainView.model.attributes["insert_index"] = datainsertindefrom
					_MainView.model.attributes["insert_parent"] = datainsertparentfrom
					_MainView.UpdateInsertDataInterface()

					UpdateInsertPlacesTooltips();


					var DataJsonFrom = {"s":"goto","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"LabelName","type":"text","data":Name}]}
					
					BrowserAutomationStudio_AddTask("",`/*Dat:${utf8_to_b64(JSON.stringify(DataJsonFrom))}*/
_goto(${JSON.stringify(Name)})!`,IdLabelFrom)


					BrowserAutomationStudio_StopSaveToUndoManager = false
					BrowserAutomationStudio_SaveToUndoManager()
					BrowserAutomationStudio_AskForVariablesUpdateIfNeeded()

					   BrowserAutomationStudio_FocusAction(IdLabelFrom,false)
							
				}

				$(".toolinsert").removeClass("toolinsertdrop")
				$(".tool").removeClass("tooldrop")
				$("#container").removeClass("dragactivelabel")
			}
		}
	

	}
	

	this.AddElement = function(element)
	{
		var el = $(element)
		
		if(el.hasClass("taskcontainer"))
		{
			el.on('dragstart', _DragStartTaskCotainer);
			el.on('dragend', _DragEndTaskContainer);
		}else if(el.hasClass("toollabelcreate"))
		{
			el.on('dragstart', _DragStartToolLabelCreate);
			el.on('dragend', _DragEndTaskContainer);
		}else if(el.hasClass("toolinsertdata"))
		{
			el.on('dragleave', _DragLeaveToolInsertData);
			el.on('dragover', _DragOverToolInsertData);
			el.on('drop', _DropToolInsertData);
		}
		
		if(el.hasClass("tool"))
		{
			el.on('dragleave', _DragLeaveTool);
			el.on('dragover', _DragOverTool);
			el.on('drop', _DropTool);
		}
		
		
	}

	this.Init = function()
	{
		
			//taskcontainer, toollabelcreate
			/*$('.taskcontainer').off('dragstart').on('dragstart', _DragStartTaskCotainer);

			$('.taskcontainer, .toollabelcreate').off('dragend').on('dragend', _DragEndTaskContainer);

			$('.toollabelcreate').off('dragstart').on('dragstart', _DragStartToolLabelCreate);
		
			//toolinsertdata
			$('.toolinsertdata').off('dragleave').on('dragleave', _DragLeaveToolInsertData);

			$('.toolinsertdata').off('dragover').on('dragover', _DragOverToolInsertData);
			
			$('.toolinsertdata').off('drop').on('drop', _DropToolInsertData);

			//tool
			$('.tool').off('dragleave').on('dragleave', _DragLeaveTool);

			$('.tool').off('dragover').on('dragover', _DragOverTool)

			$('.tool').off('drop').on('drop', _DropTool);*/
		

	}

}