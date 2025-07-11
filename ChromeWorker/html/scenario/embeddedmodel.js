function EmbeddedModel()
{
	var CacheNames = []
	var CacheNamesAndIds = []
	var Deleted = []


	this.GetCache = function()
	{
		return [CacheNames,CacheNamesAndIds];
	}

	this.Init = function()
	{
		CacheNames = []
		CacheNamesAndIds = []
		Deleted = []
		var models = _TaskCollection.models
		var len = models.length

		for(var i = 0;i<len;i++)
		{
			var task = models[i]
			var code = task.get("code")
			if(code.indexOf("_embedded(") >= 0)
			{
				code = code.split("_embedded(")[1]
		        code = code.split(")!")[0]
   		        var name = JSON.parse(code.split(",")[0])
   		        var id = task.get("id")
   		        CacheNames.push(name)
   		        CacheNamesAndIds.push(name + "," + id)
		    }
		}
	}

	this.Update = function()
	{
		var models = _TaskCollection.models
		var len = models.length

		var CacheNamesNew = []
		var CacheNamesAndIdsNew = []

		for(var i = 0;i<len;i++)
		{
			var task = models[i]
			var code = task.get("code")
			if(code.indexOf("_embedded(") >= 0)
			{
				var name = code.split("_embedded(")[1]
		        name = name.split(")!")[0]
   		        var name = JSON.parse(name.split(",")[0])
   		        var name_old = name
   		        var id = task.get("id")
   		        var nameandid = name + "," + id
   		        var isupdated = false

   		        if(CacheNamesAndIds.indexOf(nameandid) < 0 && typeof(Deleted.find((el) => {return name == el.split(",")[0]})) != "undefined" )
   		        {
   		        	Deleted = Deleted.filter((el) => {return name != el.split(",")[0]})
   		        	BrowserAutomationStudio_UpdateEmbeddedData("restore," + name)
   		        }else if(CacheNamesAndIds.indexOf(nameandid) < 0 && CacheNames.indexOf(name) >= 0)
   		        {
   		        	isupdated = true
   		        	name = Math.random().toString(36).substring(2)
   		        	nameandid = name + "," + id

   		        	code = code.replace(new RegExp("\\_embedded\\(s*\\\"[^\\\"]+\\\"","g"),"_embedded(\"" + name + "\"")

					var match = code.match("Dat:([^\*]+)")
					if(match)
					{
						var dat = JSON.parse(b64_to_utf8(match[1]))
						var dat_name = dat["d"].find((el) => {
							return el["id"] == "Name"
						}) || null
						if(dat_name)
						{
							dat_name["data"] = name
							code = code.replace(new RegExp("Dat:([^\*]+)"), "Dat:" + utf8_to_b64(JSON.stringify(dat)))
							task.set("code",code)
						}

					}
					task.attributes["code"] = code
					task.attributes["code_precomputed"] = null
   		        }


				CacheNamesNew.push(name)
   		        CacheNamesAndIdsNew.push(nameandid)

   		        if(isupdated)
   		        {
   		        	BrowserAutomationStudio_UpdateEmbeddedData("duplicate," + name_old + "," + name)
   		        }
				
		    }
		}

		len = CacheNamesAndIds.length
		for(var i = 0;i<len;i++)
		{
			var nameandid = CacheNamesAndIds[i]
			if(CacheNamesAndIdsNew.indexOf(nameandid) < 0)
	        {
	        	var name = nameandid.split(",")[0]
	        	BrowserAutomationStudio_UpdateEmbeddedData("delete," + name)
	        	Deleted.push(nameandid)
	        }
		}


		CacheNames = CacheNamesNew
		CacheNamesAndIds = CacheNamesAndIdsNew

	}
}
