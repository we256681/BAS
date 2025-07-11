function ResourcesToHash(ResourcesXml)
{
	var XmlDoc = $.parseXML(ResourcesXml);
	var Xml = $(XmlDoc);
	var Result = {}
	Xml.find("Model").each(function(Index,Model){
		var ModelName = $(Model).find("Name").text()
		var ModelType = $(Model).find("Type").text()
		var Value = null
		if(ModelType == "FixedString")
		{
			Value = $(Model).find("Value").text()
		}
		if(ModelType == "FixedInteger")
		{
			Value = $(Model).find("Value").text()
		}	
		if(ModelType == "RandomInteger")
		{
			Value = $(Model).find("MinUserValue").text() + "," + $(Model).find("MaxUserValue").text()
		}
		if(ModelType == "RandomString")
        {
			Value = $(Model).find("Value").text()
		}
		if(ModelType == "LinesFromFile")
		{
			Value = $(Model).find("Filename").text()
		}
		if(ModelType == "LinesFromUrl")
		{
			Value = $(Model).find("LinesFromUrl").text()
		}
		if(ModelType == "FilesFromDirectory")
		{
			Value = $(Model).find("FilesFromDirectory").text()
		}
		if(ModelType == "Select")
		{
			var Values = $(Model).find("Values").text().match(/[^\r\n]+/g)
			try
			{
				Value = $(Model).find("Selected").text().split(",").map(index => Values[parseInt(index)]).join(",")
			}catch(e){}
        }   
		if(ModelType == "Database")
		{
			Value = $(Model).find("GroupList").text()
		} 
		Result[ModelName] = Value    	
	})
	return Result
}

function HashToResources(ResourcesXml, Hash)
{
	var XmlDoc = $.parseXML(ResourcesXml);
	var Xml = $(XmlDoc);

	Xml.find("Script").text("")
	Xml.find("EmbeddedData").text("")
	Xml.find("DatabaseId").text("")
	Xml.find("Schema").text("")
	Xml.find("ConnectionServer").text("")
	Xml.find("ConnectionPort").text("")
	Xml.find("ConnectionLogin").text("")
	Xml.find("ConnectionPassword").text("")
	Xml.find("ScriptIcon").text("")
	Xml.find("SettingsWorker").text("")
	Xml.find("ChromeCommandLine").text("")
	Xml.find("ModulesMetaJson").text("")
	

	Xml.find("Model").each(function(Index,Model){
		try
		{
			var ModelName = $(Model).find("Name").text()
			var ModelType = $(Model).find("Type").text()
			var Value = Hash[ModelName]
			if(ModelType == "FixedString")
			{
				$(Model).find("Value").text(Value)
			}
			if(ModelType == "FixedInteger")
			{
				$(Model).find("Value").text(Value)
			}	
			if(ModelType == "RandomInteger")
			{
				var Split = Value.split(",")
				if(Split.length == 2)
				{
					$(Model).find("MinUserValue").text(Split[0])
					$(Model).find("MaxUserValue").text(Split[1])
				}
			}
			if(ModelType == "RandomString")
	        {
				$(Model).find("Value").text(Value)
			}
			if(ModelType == "LinesFromFile")
			{
				$(Model).find("Filename").text(Value)
			}
			if(ModelType == "LinesFromUrl")
			{
				$(Model).find("LinesFromUrl").text(Value)
			}
			if(ModelType == "FilesFromDirectory")
			{
				$(Model).find("FilesFromDirectory").text(Value)
			}
			if(ModelType == "Select")
			{
				
				var Values = $(Model).find("Values").text().match(/[^\r\n]+/g)
				var ValuesConverted = {}
				for(var i = 0;i<Values.length;i++)
				{
					ValuesConverted[Values[i]] = i
				}
				try
				{
					var ValuesUpdated = Value.split(",").map(value => ValuesConverted[value]).join(",")
					$(Model).find("Selected").text(ValuesUpdated)
				}catch(e){}
	        }   
			if(ModelType == "Database")
			{
				$(Model).find("GroupList").text(Value);
			} 
		}catch(e)
		{

		}
	})

	return XmlDoc.children[0].outerHTML
	
}