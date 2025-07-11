<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({
		id: "name",
		description: tr("Folder name"),
		default_selector: "string",
		disable_int: true, 
		value_string: "",
		help: {
			description: tr("The name of the folder which need to be deleted.") + " " + tr("You cannot delete a folder with the name INBOX.") + " " + tr("You can get a list of mailbox folders using the \"Get list of folders\" action."),
			examples: [
				{code: "foo"},
				{code: "bar"},
				{code: "zap"},
				{code: "foo/bar", description: tr("Delete subfolder bar from foo folder")}
			]
		} 
	}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Delete the specified mailbox folder.</div>
	<div class="tr tooltip-paragraph-fold">In order to execute this action correctly you need to run "Configure receiving mail" action first.</div>
	<div class="tr tooltip-paragraph-fold">This action will completely and permanently delete the folder with the specified name and all messages it contains from the mailbox.</div>
	<div class="tr tooltip-paragraph-fold">This action is only available when connecting via imap, if the action is called when connecting via pop3, it will immediately fail.</div>
	<div class="tr tooltip-paragraph-fold">You cannot delete a folder with the name INBOX.</div>
	<div class="tr tooltip-paragraph-fold">You can get a list of mailbox folders using the "Get list of folders" action.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", use_timeout:true, visible:true}) %>
