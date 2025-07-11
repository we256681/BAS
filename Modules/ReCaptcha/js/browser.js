Object.defineProperty(window, 'grecaptcha', {set: function(value){
	if(typeof(value) == "undefined")
		return;

	_BAS_HIDE(BrowserAutomationStudio_GRecaptchaInject) = function(Target)
	{
		Object.defineProperty(Target, 'render', {set: function(value1){
			_BAS_HIDE(BrowserAutomationStudio_GRecaptchaRenderOriginal) = value1;
			_BAS_HIDE(BrowserAutomationStudio_GRecaptchaRender) = function()
			{
				if(typeof(_BAS_HIDE(BrowserAutomationStudio_GRecaptchaCallback)) == "undefined")
					_BAS_HIDE(BrowserAutomationStudio_GRecaptchaCallback) = []
   
				try{
					_BAS_HIDE(BrowserAutomationStudio_GRecaptchaCallback).push(arguments[1]["callback"])
				}catch(e){}
				return _BAS_HIDE(BrowserAutomationStudio_GRecaptchaRenderOriginal).apply(this, arguments);
			}
   
		   }, get: function(){
			return _BAS_HIDE(BrowserAutomationStudio_GRecaptchaRender)
		   }, configurable: true
	   })
   
	   Object.defineProperty(Target, 'getResponse', {set: function(value1){
			_BAS_HIDE(BrowserAutomationStudio_GRecaptchaGetResponseOriginal) = value1;
			_BAS_HIDE(BrowserAutomationStudio_GRecaptchaGetResponse) = function()
			{
				if(typeof(_BAS_HIDE(BrowserAutomationStudio_RecaptchaResult)) == "string")
				{
					return _BAS_HIDE(BrowserAutomationStudio_RecaptchaResult)
				}
				return _BAS_HIDE(BrowserAutomationStudio_GRecaptchaGetResponseOriginal).apply(this, arguments);
			}
   
		   }, get: function(){
			return _BAS_HIDE(BrowserAutomationStudio_GRecaptchaGetResponse)
		   }, configurable: true
	   })
   
	   Object.defineProperty(Target, 'execute', {set: function(value1){
		   _BAS_HIDE(BrowserAutomationStudio_GRecaptchaExecuteOriginal) = value1;
		   _BAS_HIDE(BrowserAutomationStudio_GRecaptchaExecute) = function()
		   {
			   
			   var InterceptId = ""
			   try
			   {
				   var ActionName = arguments[1].action
				   if(typeof(_BAS_HIDE(BrowserAutomationStudio_RecaptchaV3ActionList)) != "undefined")
				   {
					   var ActionList = _BAS_HIDE(BrowserAutomationStudio_RecaptchaV3ActionList).split(",")
					   if(ActionList.indexOf("*") >= 0 || ActionList.indexOf(ActionName) >= 0)
					   {
						   InterceptId = "v3_" + (Math.floor(Math.random() * 1000000) + 100).toString()
					   }
				   }
				   _BAS_HIDE(BrowserAutomationStudio_RecaptchaV3Call) = JSON.stringify({"ActionName": ActionName, "Url": document.location.href, "SiteKey": arguments[0], "InspectId": InterceptId})
			   }catch(e)
			   {
   
			   }
			   if(InterceptId)
			   {
				   if(typeof(_BAS_HIDE(BrowserAutomationStudio_RecaptchaV3Data)) == "undefined")
						_BAS_HIDE(BrowserAutomationStudio_RecaptchaV3Data) = {}
				   return new Promise(function(resolve, reject){
					   _BAS_HIDE(BrowserAutomationStudio_RecaptchaV3Data)[InterceptId] = {resolve: resolve, reject: reject}
				   })
			   }else
			   {
				   return _BAS_HIDE(BrowserAutomationStudio_GRecaptchaExecuteOriginal).apply(this, arguments)
			   }
		   }
   
		  }, get: function(){
		   return _BAS_HIDE(BrowserAutomationStudio_GRecaptchaExecute)
		  }, configurable: true
	  	})
	};

	_BAS_HIDE(BrowserAutomationStudio_GRecaptchaOriginal) = value;

	Object.defineProperty(_BAS_HIDE(BrowserAutomationStudio_GRecaptchaOriginal), 'enterprise', {set: function(value1){
		_BAS_HIDE(BrowserAutomationStudio_GRecaptchaEnterprise) = value1;

		(_BAS_HIDE(BrowserAutomationStudio_GRecaptchaInject))(_BAS_HIDE(BrowserAutomationStudio_GRecaptchaEnterprise));
		
	   }, get: function(){
		return _BAS_HIDE(BrowserAutomationStudio_GRecaptchaEnterprise)
	   }, configurable: true
   });

   (_BAS_HIDE(BrowserAutomationStudio_GRecaptchaInject))(_BAS_HIDE(BrowserAutomationStudio_GRecaptchaOriginal));


}, get: function()
{
	if(typeof(_BAS_HIDE(BrowserAutomationStudio_GRecaptchaOriginal)) != "undefined")
		return _BAS_HIDE(BrowserAutomationStudio_GRecaptchaOriginal);
}});

_BAS_HIDE(BrowserAutomationStudio_RecaptchaV3Solved) = function(Id, Result)
{
	try
	{
		_BAS_HIDE(BrowserAutomationStudio_RecaptchaV3Data)[Id]["resolve"](Result)
	}catch(e)
	{

	}
};

_BAS_HIDE(BrowserAutomationStudio_RecaptchaSolved) = function(result)
{
	try{
		_BAS_HIDE(BrowserAutomationStudio_RecaptchaResult) = result
	}catch(e){}

	try{
		if(typeof(_BAS_HIDE(BrowserAutomationStudio_GRecaptchaCallback)) == "object")
		{
			_BAS_HIDE(BrowserAutomationStudio_GRecaptchaCallback).forEach(function(resp){
				if(typeof(resp) == "string")
				{
					try{
						(window[resp])(result)
					}catch(e){}						
				}else
				{
					try{
						resp(result)
					}catch(e){}						
				}
			})
		}
	}catch(e){}		

};
