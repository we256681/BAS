function UpdateGotos()
{
	try
	{
		var cache = {}
		var models = _TaskCollection.models
		var len = models.length
		for(var i = 0;i<len;i++)
		{

			var task = models[i]
			var code = task.get("code")
			if(task.attributes["internal_label_id"] != "")
			{
				task.attributes["code_precomputed"] = null
			}
			task.attributes["internal_label_id"] = ""


			if(code.indexOf("_goto(") >= 0)
			{
				code = code.split("_goto(")[1]
		        code = code.split(")!")[0]
		        var label = JSON.parse(code.split(",")[0])

		        var label_hash = {set: null, get: []}

				if(cache[label])
					label_hash = cache[label]
				else
					cache[label] = label_hash

				var TasksParents = []
				var Task = task
				while(true)
				{
					var parentid = parseInt(Task.get("parentid"))
					if(parentid == 0)
						break

					
					Task = FindTaskById(parentid)
					if(!Task)
						break

					TasksParents.push(Task)

					if(IsFunctionNode(parentid))
						break

				}


				label_hash["get"].push({task: task, parents: TasksParents})

			}else if(code.indexOf("_set_goto_label(") >= 0)
			{
				code = code.split("_set_goto_label(")[1]
		        code = code.split(")!")[0]

		        var label = JSON.parse(code)

		        var label_hash = {set: null, get: []}

				if(cache[label])
					label_hash = cache[label]
				else
					cache[label] = label_hash

				var TasksParents = []
				var Task = task
				while(true)
				{
					var parentid = parseInt(Task.get("parentid"))
					if(parentid == 0)
						break
					
					Task = FindTaskById(parentid)
					if(!Task)
						break

					TasksParents.push(Task)

					if(IsFunctionNode(parentid))
						break

				}


				label_hash["set"] = {task: task, parents: TasksParents}
		        

			}
		}


		Object.keys(cache).forEach(function(label){	
			
			var label_hash = cache[label]
			if(label_hash["set"])
			{
				var SetLabelTask = label_hash["set"]["task"]
				var SetLabelParents = label_hash["set"]["parents"]

				label_hash["get"].forEach(function(Get){
					var GetLabelTask = Get["task"]
					var GetLabelParents = Get["parents"]					



					var Offset = 0;
					var Reverse = []
					var Found = false

					for(var i = 0;i<GetLabelParents.length;i++)
					{
						var GetLabelParent = GetLabelParents[i]

						var SetLabelParent = SetLabelParents.find(function(el){return parseInt(el.get("id")) == parseInt(GetLabelParent.get("id")) })
						if(SetLabelParent)
						{


							var index = SetLabelParents.indexOf(SetLabelParent)
							Reverse = SetLabelParents.slice(0,index)
							Found = true
							Offset = i
							break;
						}

					}

					if(!Found)
					{
						Reverse = SetLabelParents.slice(0);
						Offset = -1
					}

					var ReverseFinal = []

					Reverse.forEach(function(Rev){
						var n = FindNextSibling(parseInt(Rev.get("id")))
						ReverseFinal.push(n)
					})

					var RevLabels = []
					ReverseFinal.forEach(function(Rev){

						if(Rev)
						{
							if(!IsFunctionNode(parseInt(Rev.get("id"))) && parseInt(GetFirstNotFunctionId()) != parseInt(Rev.get("id")) )
							{
								var code = Rev.get("code")
								var split = code.split("_set_goto_label(\"_internal_")
								var id
								if(Rev.attributes["internal_label_id"].length > 0)
								{
									id = Rev.attributes["internal_label_id"]
								}else
								{
									id = Math.abs(label.split("").reduce(function(a,b){a=((a<<5)-a)+b.charCodeAt(0);return a&a},0)).toString() + "_" + Rev.get("id")
									//id = (Math.floor(Math.random() * (10000000 - 1000000) + 1000000)).toString()
									if(Rev.attributes["internal_label_id"] != id)
									{
										Rev.attributes["code_precomputed"] = null
									}
									Rev.attributes["internal_label_id"] = id
								}
									
								RevLabels.push("_internal_" + id)
							}
							
						}else
						{
							RevLabels.push(null)
						}
					})



					var code = GetLabelTask.get("code")
					code = code.replace(new RegExp("(\_fast|\_long)?\_goto\(.*?\)\!","g"),"_long_goto(" + JSON.stringify(label) + ", " + Offset + ", " + JSON.stringify(RevLabels) + ")!")
					if(GetLabelTask.attributes["code"] != code)
					{
						GetLabelTask.attributes["code_precomputed"] = null
					}
					GetLabelTask.attributes["code"] = code


				})
			}
		})


		
	}catch(e){}
	
}