function InitEdgeScroll()
{
	var IsInside = false
	var Y = -1
	var X = -1
	var MousePressed = false
	var IsDrag = false
	var Margin1 = 60
	var Margin2 = 40
	var Margin3 = 20

	var Value1 = 10
	var Value2 = 30
	var Value3 = 150
	
	//Move
	document.addEventListener("dragover", function(e){
	    e = e || window.event;
    
	    IsInside = true
		Y = event.clientY
		X = event.clientX
	})

	document.addEventListener("mousemove", function(e){
	    e = e || window.event;

	    if(e.buttons == 0)
	    {
	    	MousePressed = false
	    }
    
	    IsInside = true
		Y = event.clientY
		X = event.clientX
	})

	//Stop
	document.addEventListener("mouseup", function(e){
	    e = e || window.event;
    
	    MousePressed = false
	})

	document.addEventListener("dragstop", function(e){
	    e = e || window.event;
	    MousePressed = false
	})

	//Start
	document.addEventListener("mousedown", function(e){
	    e = e || window.event;

	    if(event.clientY > $(".threads").height() && Y < window.innerHeight - $(".bottompannel").height())
	    {
		    MousePressed = true
		    IsDrag = false
		}

	})

	document.addEventListener("dragstart", function(e){
	    e = e || window.event;

	    if(event.clientY > $(".threads").height() && Y < window.innerHeight - $(".bottompannel").height())
	    {
		    MousePressed = true
		    IsDrag = true
		}

	})


	//Enter/leave

	document.addEventListener("mouseenter", function(e){
	    e = e || window.event;
    
	    IsInside = true
	})

	document.addEventListener("mouseleave", function(e){
	    e = e || window.event;
    
	    IsInside = false
	})


	setInterval(function(){
		var Scroll = 0
		if(IsInside && Y >= 0 && Y <= window.innerHeight && MousePressed && (IsDrag || _MultiSelect.IsActive()))
		{
			if(Y - $(".threads").height() < Margin3)
			{
				Scroll = -Value3
			}else if(Y - $(".threads").height() < Margin2)
			{
				Scroll = -Value2
			}else if(Y - $(".threads").height() < Margin1)
			{
				Scroll = -Value1
			}else if(Y > window.innerHeight - $(".bottompannel").height() - Margin3)
			{
				Scroll = Value3
			}else if(Y > window.innerHeight - $(".bottompannel").height() - Margin2)
			{
				Scroll = Value2
			}else if(Y > window.innerHeight - $(".bottompannel").height() - Margin1)
			{
				Scroll = Value1
			}

		}
		if(Scroll != 0)
		{
			document.documentElement.scrollTop += Scroll
			_MultiSelect.Update(X, Y, true);
		}
	},100)

}