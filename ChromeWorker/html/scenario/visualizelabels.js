class VisualizeLabels
{
	constructor()
	{
		this.TagHeight = 22;
		this.TagMargin = 10;
		this.LineWidth = 3
		this.SpaceBetweenLines = 30
		this.MinimumOffset = 25
		this.Styles = 
		[
			{Color: "#bbe2ab", Index: 0},
			{Color: "#d9d4a1", Index: 1},
			{Color: "#a8d6d4", Index: 2},
			{Color: "#a7bed9", Index: 3},
			{Color: "#e6bbbb", Index: 4}
		];

		this.LinesDrawn = []
		this.ScenarioData = []
		this.PlaceTaken = new Set([]);


		var self = this

		$(window).scroll(function (event) {
    		self.OnScroll()
		});

	}

	PlaceTag(Element, Min, Max, Y, Scroll)
	{
		var self = this
		var TagTotal = this.TagHeight + this.TagMargin
		if(Y < Min)
			return false
		if(Y + TagTotal > Max)
			return false
		for (let Place of self.PlaceTaken.keys())
		{
			if(Math.abs(Place - Y) < TagTotal)
			{
				return false
			}
		}
		Element.css("top",(Y + Scroll) + "px")
		Element.show()
		self.PlaceTaken.add(Y)
		return true
	}

	OnScroll()
	{
		$(".tag").hide()
		this.PlaceTaken = new Set([]);

		var self = this
		var Scroll = $(window).scrollTop();
		//$(".tag").css("top",(scroll) + "px")
		var Top = parseInt($(".main").css("padding-top"))
		if($("#paddinfind").css("display") == "block")
		{
			Top += parseInt($("#paddinfind").css("padding-top"))
		}
		var Bottom = parseInt($(".main").css("padding-bottom"))
		var Height = document.documentElement.clientHeight - Top - Bottom
		var Tags = $(".tag").toArray();
		Tags = Tags.sort(function(a, b){return parseInt($(a).attr("data-x")) - parseInt($(b).attr("data-x"))})
		
		var TagTotal = this.TagHeight + this.TagMargin
		Tags.forEach(function(TagRaw){
			var Tag = $(TagRaw)
			var Min = parseInt(Tag.attr("data-min")) - Scroll
			var Max = parseInt(Tag.attr("data-max")) - Scroll
			if(Min < 0)
				Min = 0
			if(Max > Height)
				Max = Height



			if(!self.PlaceTag(Tag,Min,Max,Min,Scroll))
			{
				for (let Place of self.PlaceTaken.keys())
				{
					if(self.PlaceTag(Tag,Min,Max,Place + TagTotal,Scroll))
					{
						break
					}
				}
			}
			
		})
		
	}


	CalculateScenarioData()
	{
		var self = this
		var AllElement = $(".tool").toArray()
		var Index = 0
		var AdditionalMarginTop = 0
		if($("#paddinfind").css("display") == "block")
		{
			AdditionalMarginTop = parseInt($("#paddinfind").css("padding-top"))
		}
		var MainPaddingTop = parseInt($(".main").css("padding-top")) + AdditionalMarginTop
		var MainPaddingLeft = parseInt($(".main").css("padding-left"))
		AllElement.forEach(function(ElementRaw){
			var Element = $(ElementRaw)
			if(Element.parents('.folded').length == 0)
			{
				var AddData = {}

				var offset = Element.offset()
				var position = Element.position()
				var outerWidth = Element.outerWidth()
				var outerHeight = Element.outerHeight()


				AddData.x = offset.left + outerWidth
				AddData.y = position.top - MainPaddingTop + outerHeight / 2
				AddData.ymin = position.top - MainPaddingTop
				AddData.ymax = position.top - MainPaddingTop + outerHeight

				AddData.is_label_from = Element[0].hasAttribute("data-is-label-from")
				AddData.is_label_to = Element[0].hasAttribute("data-is-label-to")
				if(AddData.is_label_to)
				{
					AddData.label_id = parseInt(Element.attr("task-id"))
				}
				AddData.label_name = ""
				AddData.index = Index
				Index++
				if(Element[0].hasAttribute("data-label-name"))
					AddData.label_name = Element.attr("data-label-name")

				self.ScenarioData.push(AddData)
			}
		})
	}

	ConvertLabelNameToStyle(LabelName)
	{
		var Max = this.Styles.length
		var Length = LabelName.length
		var Res = 0
		for(var i = 0;i<Length;i++)
		{
			Res += LabelName.charCodeAt(i)
			Res %= Max;
		}

		return this.Styles[Res]
	}

	Clear()
	{
		$(".tag").remove()
		$("#visualizelabels line").remove()
		this.LinesDrawn = []
		this.ScenarioData = []
	}

	GroupScenarioData()
	{
		var Res = {}
		var Length = this.ScenarioData.length
		for(var i = 0;i<Length;i++)
		{
			var Element = this.ScenarioData[i]
			if(Element.is_label_from || Element.is_label_to)
			{
				if(Res.hasOwnProperty(Element.label_name))
				{
					Res[Element.label_name].push(Element)
				}else
				{
					Res[Element.label_name] = [Element]
				}
			}
			
		}
		return Res
	}

	Visualize()
	{
		
		var self = this
		this.Clear()
		this.CalculateScenarioData()
		var ScenarioDataGrouped = this.GroupScenarioData();

		var Labels = Object.keys(ScenarioDataGrouped)
		for(var i = 0; i < Labels.length; i++)
		{
			var Label = Labels[i]
			var Style = this.ConvertLabelNameToStyle(Label)
			var ToElement = null
			var HasFrom = false
			var Elements = ScenarioDataGrouped[Label]

			ToElement = Elements.find(function(Element){
				return Element.is_label_to
			})

			if(typeof(ToElement) == "undefined")
				ToElement = null

			HasFrom = Elements.find(function(Element){
				return Element.is_label_from
			})

			if(typeof(HasFrom) == "undefined")
				HasFrom = false

			if(ToElement && !HasFrom)
			{
				if(Element.length > 0)
					self.DrawLine(-1,Element[0].index,1,0,Style)
			}else if(!ToElement && HasFrom)
			{
				Elements.forEach(function(Element){
					self.DrawLine(Element.index,-1,1,0,Style)
				})
			}else
			{
				var Index = 0
				Elements = Elements.filter((Element) => Element.is_label_from)
				Elements.forEach(function(Element){
					self.DrawLine(Element.index,ToElement.index,Elements.length,Index,Style)
					Index++
					
				})
			}


		}

		this.OnScroll()

	}

	AllowToDrawLine(Y1, Y2, Offset)
	{

		for(var i = 0;i<this.LinesDrawn.length;i++)
		{
			var LineDrawn = this.LinesDrawn[i]
			if(LineDrawn.offset == Offset)
			{
				var a1,a2,b1,b2;
				if(Y1 > Y2)
				{
					a1 = Y2
					a2 = Y1
				}else
				{
					a1 = Y1
					a2 = Y2
				}

				if(LineDrawn.y1 > LineDrawn.y2)
				{
					b1 = LineDrawn.y2
					b2 = LineDrawn.y1
				}else
				{
					b1 = LineDrawn.y1
					b2 = LineDrawn.y2
				}

				if(b2 >= a1 && b1 <= a2)
				{
					return false;
				}
			}

		}
		this.LinesDrawn.push({y1: Y1, y2: Y2, offset: Offset})
		return true
	}

	DrawLine(IndexFrom, IndexTo, TotalNumberTo, IndexNumberTo, Style)
	{
		if(IndexFrom < 0)
			return
		if(IndexTo < 0)
			return
		/*this.LineWidth = 1
		this.SpaceBetweenLines = 1
		this.MinimumOffset = 10*/

		var ElementFrom = this.ScenarioData[IndexFrom]
		var ElementTo = this.ScenarioData[IndexTo]


		var IndexMin;
		var IndexMax;
		if(IndexFrom > IndexTo)
		{
			IndexMin = IndexTo
			IndexMax = IndexFrom
		}else
		{
			IndexMin = IndexFrom
			IndexMax = IndexTo
		}
		var Offset = 0;
		for(var i = IndexMin;i<=IndexMax;i++)
		{
			var Element = this.ScenarioData[i]
			var CurrentOffset = Math.ceil((Element.x + this.MinimumOffset) / (this.LineWidth + this.SpaceBetweenLines))
			if(CurrentOffset > Offset)
				Offset = CurrentOffset
		}

		var ToY = (ElementTo.ymax * (TotalNumberTo - IndexNumberTo) + ElementTo.ymin * (IndexNumberTo + 1)) / (TotalNumberTo + 1)

		while(true)
		{
			if(this.AllowToDrawLine(ElementFrom.y, ToY, Offset))
				break
			Offset++
		}

		this.DrawLineRaw(ElementFrom.x,ElementFrom.y,ElementTo.x,ToY,Offset,Style,ElementFrom.label_name,ElementTo.label_id)
	}

	DrawLineRaw(X1, Y1, X2, Y2, Offset, Style, Text, Id)
	{
		Offset = Offset * (this.LineWidth + this.SpaceBetweenLines)
		this.AddSvgLine(X1, Y1, Offset, Y1, Style, false)
		this.AddSvgLine(Offset, Y1, Offset, Y2, Style, false)
		this.AddSvgLine(Offset, Y2, X2, Y2, Style, true)
		var min,max;
		if(Y1 > Y2)
		{
			min = Y2
			max = Y1
		}else
		{
			min = Y1
			max = Y2
		}
		this.DrawText(Offset + 2,0.5*(Y1 + Y2),Text,Style,Id,min,max)
	}

	DrawText(X, Y, Text, Style, Id, Min, Max)
	{
		var Element = $("<a href='#' data-min='" + Min + "' data-max='" + Max + "' data-x='" + X + "' onclick='BrowserAutomationStudio_FocusAction(" + Id + ",false);return false' class='tag' style='position:absolute;top:" + Y + "px;left:" + X + "px;'></a>").text(Text)
		$(".main").append(Element)
	}


	AddSvgLine(X1, Y1, X2, Y2, Style, Marker, Mid)
	{
		var newLine = document.createElementNS('http://www.w3.org/2000/svg','line');
		newLine.setAttribute('x1',X1.toString());
		newLine.setAttribute('y1',Y1.toString());
		newLine.setAttribute('x2',X2.toString());
		newLine.setAttribute('y2',Y2.toString());
		if(Marker)
		{
			newLine.setAttribute('marker-end',"url(#Arrow" + Style.Index + ")");
		}
		
		newLine.setAttribute("style", "stroke:" + Style.Color + ";stroke-width:" + this.LineWidth)
		$("#visualizelabels").append(newLine);
	}


}