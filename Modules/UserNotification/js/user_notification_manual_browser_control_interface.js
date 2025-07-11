<div class="container-fluid" >
	<%= _.template($('#input_constructor').html())({id:"Label", description:tr("Label which is shown to user"), default_selector: "string", help: 
		{
			description: tr("You can attach message when giving control to user in order to make him understand, what is a purpose of manual control and what he need to accomplish. Message can contain custom data like variables or resources."), examples:
			[
				{
					code:tr("Empty string")
				},
				{
					code:tr("Please solve captcha"),description:tr("Ask user to solve captcha")
				},
				{
					code:tr("Please input data for [[ACCOUNT]]"),description:tr("Use variable inside message")
				}
			]
		} 
	}) %>
</div>
<div class="tooltipinternal">
      <div class="tr tooltip-paragraph-first-fold">Give control over the browser to user.</div>
      <div class="tr tooltip-paragraph-fold">This action will temporary establish direct control over browser. Every mouse movement and keyboard event will be sent to browser directly.</div>

      <div class="tooltip-paragraph-fold"><span class="tr">When user will finish controlling browser, he can click on link at a page bottom to hand control over browser back to script</span> ( <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=browserautomationstudio_2019-02-24_13-56-59.png');return false">screen</a> ).</div>
      <div class="tooltip-paragraph-fold"><span class="tr">This action is meant to be used in run mode, if you want to control browser in record mode, hit following button</span> ( <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=browserautomationstudio_2019-02-24_14-15-11.png');return false">screen</a> ).
      </div>
      
	  <div class="tr tooltip-paragraph-fold">Developer can attach message when giving control to user in order to make him understand, what is a purpose of manual control and what he need to accomplish. Message can contain custom data like variables or resources.</div>
      <div class="tr tooltip-paragraph-fold">Typical use case would be to control browser in most of the script lifetime, while giving control to user only for a small task, like solving captcha type which is not supported by BAS natively.</div>
      <div class="tr tooltip-paragraph-last-fold">If you want obtain information from user, use 'Ask user input' action or resource system.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>