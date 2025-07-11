<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({
		id: "name",
		description: tr("Folder name"),
		default_selector: "string",
		disable_int: true, 
		value_string: "",
		help: {
			description: tr("The name of the folder which need to be created.") + " " + tr("You cannot create a folder with the name INBOX or with the same name as an existing folder."),
			examples: [
				{code: "foo"},
				{code: "bar"},
				{code: "zap"},
				{code: "foo/bar", description: tr("Create subfolder bar in folder foo")}
			]
		} 
	}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Create a new mailbox folder.</div>
	<div class="tr tooltip-paragraph-fold">In order to execute this action correctly you need to run "Configure receiving mail" action first.</div>
	<div class="tr tooltip-paragraph-fold">This action will create a new folder with the specified name in the mailbox.</div>
	<div class="tr tooltip-paragraph-fold">This action is only available when connecting via imap, if the action is called when connecting via pop3, it will immediately fail.</div>
	<div class="tr tooltip-paragraph-fold">You cannot create a folder with the name INBOX or with the same name as an existing folder.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", use_timeout:true, visible:true}) %>
