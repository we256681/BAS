function MultiSelectManager()
{
	this.url = "main";
	this.multiselect_mode = false;
	
	this.BeforUrlChanged = function(url)
	{

		if(url == "loopelement" && this.url != "loopelement" || url == "length" && this.url != "length")
		{
			this.multiselect_mode = true;
			var Data = ""
			if(_SelectorModel.get("is_editing"))
			{
				Data = JSON.stringify(CurrentDat)
			}
			BrowserAutomationStudio_MultiselectStateChanged(1, Data);
		}

		if(url != "loopelement" && this.url == "loopelement" || url != "length" && this.url == "length")
		{
			this.multiselect_mode = false;
			BrowserAutomationStudio_MultiselectStateChanged(0, "");
		}
	}

	this.AfterUrlChanged = function(url)
	{
		this.url = url;
	}

	this.PathManualyEdited = function()
	{
		if(this.multiselect_mode)
		{
			BrowserAutomationStudio_MultiselectReset();
		}
	}

	this.PathManuallyRest = function()
	{
		BrowserAutomationStudio_MultiselectReset();
		$('#multiselectincludenumber').text('0');
		$('#multiselectexcludenumber').text('0');

		_SelectorModel.set("css",">CSS> empty-path")
		_SelectorModel.set("xpath",">XPATH> .//empty-path")

		_MainView.updatepath();
	}

	this.Report = function(data_string)
	{
		
		var index = 0;
		var items = $(".dropdown-menu-css-item");
		for(var i = 0;i<items.length;i++)
		{
			var item = $(items[i])
			if(item.attr("data-selected") == "true")
			{
				index = i;
			}
		}
		var parsed = JSON.parse(data_string)
		
		var include_number = parsed.include_number
		var exclude_number = parsed.exclude_number
		$("#multiselectincludenumber").text(include_number)
		$("#multiselectexcludenumber").text(exclude_number)


		var data = parsed.data
		if(typeof(data) == 'object')
		{
			_SelectorModel.set("is_image",false)
			_SelectorModel.set("css",(data.css.length > 0) ? data.css[index] : "")
			_SelectorModel.set("css1",(data.css.length > 0) ? data.css[0] : "")
			_SelectorModel.set("css2",(data.css.length > 1) ? data.css[1] : "")
			_SelectorModel.set("css3",(data.css.length > 2) ? data.css[2] : "")
			_SelectorModel.set("xpath",(data.xpath.length > 0) ? data.xpath[0] : "")
			_MainView.updatepath();
			$(".selector-menu").replaceWith(_.template($('#selector_menu').html())({selector:_SelectorModel.toJSON()}))
			try
			{
				$($(".dropdown-menu-css-item")[index]).attr("data-selected","true")
			}catch(e)
			{

			}
		}
	}


	
}