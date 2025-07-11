<div class="container-fluid">
	<%= _.template($('#variable_constructor').html())({
		id: "Save",
		description: tr("Variable to save the result"),
		default_variable: "MAIL_BOXES_LIST",
		help: {
			description: tr("Variable in which, after successful execution of the action, the list of folders will be written.") + " " + tr("The folder list format depends on the \"Data format\" setting.")
		}
	}) %>
	<%= _.template($('#input_constructor').html())({
		id: "format",
		description: tr("Data format"),
		default_selector: "string",
		variants: [
			{value: "Objects list", description: "[{name:\"INBOX\",attribs:[],delimiter:\"/\"},{name:\"Spam\",attribs:[\"\\Marked\",\"\\Junk\"],delimiter:\"/\"}]"},
			{value: "CSV list", description: "[\"INBOX::/\",\"Spam:\"\\Marked,\\Junk\":/\"]"},
			{value: "CSV string", description: "INBOX::/<br/>Spam:\"\\Marked,\\Junk\":/"}
		],
		disable_int: true,
		value_string: "Objects list",
		help: {
			description: tr("The format in which the list of folders will be saved to a variable."),
			examples: [
				{code: "Objects list", description: "[{name:\"INBOX\",attribs:[],delimiter:\"/\"},{name:\"Spam\",attribs:[\"\\Marked\",\"\\Junk\"],delimiter:\"/\"}]"},
				{code: "<br style=\"display:block;content:'';margin:6px;\"/>CSV list", description: "[\"INBOX::/\",\"Spam:\"\\Marked,\\Junk\":/\"]"},
				{code: "<br style=\"display:block;content:'';margin:6px;\"/>CSV string", description: "INBOX::/<br/>Spam:\"\\Marked,\\Junk\":/"}
			]
		} 
	}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get a list of folders in the specified mailbox.</div>
	<div class="tr tooltip-paragraph-fold">In order to execute this action correctly you need to run "Configure receiving mail" action first.</div>
	<div class="tr tooltip-paragraph-fold">This action will return the folder list of the specified mailbox in the specified format.</div>
	<div class="tr tooltip-paragraph-fold">This action is only available when connecting via imap, if the action is called when connecting via pop3, it will immediately fail.</div>
	<div class="tooltip-paragraph-fold"><span class="tr">CSV list - a list that consists of CSV strings and CSV string consists of name, attributes and delimiter of folder separated with ":" symbol.</span> <span class="tr">Example:</span> <code>["INBOX::/","Spam:\"\\Marked,\\Junk\":/","Trash:\"\\Marked,\\Trash\":/"]</code></div>
	<div class="tooltip-paragraph-fold"><span class="tr">CSV string - this CSV strings separated by a line break, and consisting of name, attributes and delimiter of folder separated with ":" symbol.</span> <span class="tr">Example:</span> <code><br/>INBOX::/<br/>Spam:"\\Marked,\\Junk":/<br/>Trash:"\\Marked,\\Trash":/</code></div>
	<div class="tr tooltip-paragraph-fold">Object list - a list that consists of objects, in an object, <span style="color:black">name</span> property contains the name, <span style="color:black">attribs</span> contains a list of attributes, and <span style="color:black">delimiter</span> contains the delimiter of folder.</div>
	<div class="tooltip-paragraph-fold"><span class="tr">Example:</span> <code>[{name:"INBOX",attribs:[],delimiter:"/"},{name:"Spam",attribs:["\\Marked","\\Junk"],delimiter:"/"},{name:"Trash",attribs:["\\Marked","\\Trash"],delimiter:"/"}]</code></div>
	<div class="tr tooltip-paragraph-fold">CSV strings can be parsed using the "Parse CSV string" action from the "String" module.</div>
	<div class="tr tooltip-paragraph-fold">Lists can be processed using actions from the "List" module.</div>
	<div class="tr tooltip-paragraph-fold">Objects can be processed using actions from the "JSON" module.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", use_timeout:true, visible:true}) %>
