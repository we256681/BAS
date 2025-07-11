<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({
		id: "oldName",
		description: tr("Old folder name"),
		default_selector: "string",
		variants: [
			{value: "INBOX", description: tr("Default folder incoming messages")}
		],
		disable_int: true,
		value_string: "",
		help: {
			description: tr("The name of the folder which need to be renamed.") + " " + tr("You can get a list of mailbox folders using the \"Get list of folders\" action."),
			examples: [
				{code: "foo"},
				{code: "bar"},
				{code: "zap"}
			]
		}
	}) %>
	<%= _.template($('#input_constructor').html())({
		id: "newName",
		description: tr("New folder name"),
		default_selector: "string",
		disable_int: true,
		value_string: "",
		help: {
			description: tr("The folder name to which the specified folder will be renamed."),
			examples: [
				{code: "foo"},
				{code: "bar"},
				{code: "zap"}
			]
		} 
	}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Rename the specified mailbox folder.</div>
	<div class="tr tooltip-paragraph-fold">In order to execute this action correctly you need to run "Configure receiving mail" action first.</div>
	<div class="tr tooltip-paragraph-fold">This action will rename the specified folder in the mailbox.</div>
	<div class="tr tooltip-paragraph-fold">This action is only available when connecting via imap, if the action is called when connecting via pop3, it will immediately fail.</div>
	<div class="tr tooltip-paragraph-fold">Renaming INBOX is allowed and has special behavior. It moves all messages from INBOX to a new folder with the specified name, while INBOX remains empty.</div>
	<div class="tr tooltip-paragraph-fold">You can get a list of mailbox folders using the "Get list of folders" action.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", use_timeout:true, visible:true}) %>
