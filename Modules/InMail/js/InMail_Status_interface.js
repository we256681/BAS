<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({
		id: "name",
		description: tr("Folder name"),
		default_selector: "string",
		variants: [
			{value: "INBOX", description: tr("Default folder incoming messages")}
		],
		disable_int: true, 
		value_string: "",
		help: {
			description: tr("The name of the folder which need to get information about.") + " " + tr("You can get a list of mailbox folders using the \"Get list of folders\" action."),
			examples: [
				{code: "INBOX", description: tr("Default folder incoming messages")},
				{code: "Spam", description: tr("Spam folder, on some mails")},
				{code: "Trash", description: tr("Trash folder, on some mails")}
			]
		} 
	}) %>
	<%= _.template($('#variable_constructor').html())({
		id: "total",
		description: tr("Total messages"),
		default_variable: "MAIL_TOTAL_COUNT",
		help: {
			description: tr("Variable in which, after successful execution of the action, the number of all messages in the specified folder will be written."),
			examples: [
				{code: 14},
				{code: 5},
				{code: 0, description: tr("The specified folder is empty")}
			]
		}
	}) %>
	<%= _.template($('#variable_constructor').html())({
		id: "recent",
		description: tr("Recent messages"),
		default_variable: "MAIL_RECENT_COUNT",
		help: {
			description: tr("Variable in which, after successful execution of the action, the number of recent messages in the specified folder will be written.") + " " + tr("Recent messages are messages received during the current session and have the <code>\\Recent</code> flag."),
			examples: [
				{code: 5},
				{code: 2},
				{code: 0, description: tr("There are no recent messages in the specified folder")}
			]
		}
	}) %>
	<%= _.template($('#variable_constructor').html())({
		id: "unseen",
		description: tr("Unseen messages"),
		default_variable: "MAIL_UNSEEN_COUNT",
		help: {
			description: tr("Variable in which, after successful execution of the action, the number of unseen messages in the specified folder will be written.") + " " + tr("Unseen messages are messages that do not have the <code>\\Seen</code> flag."),
			examples: [
				{code: 37},
				{code: 16},
				{code: 0, description: tr("There are no unseen messages in the specified folder")}
			]
		}
	}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get information about the folder in the specified mailbox.</div>
	<div class="tr tooltip-paragraph-fold">In order to execute this action correctly you need to run "Configure receiving mail" action first.</div>
	<div class="tr tooltip-paragraph-fold">This action will get information about the specified folder, namely the number of all, recent and unseen messages.</div>
	<div class="tr tooltip-paragraph-fold">This action is only available when connecting via imap, if the action is called when connecting via pop3, it will immediately fail.</div>
	<div class="tr tooltip-paragraph-fold">Recent messages are messages received during the current session and have the <code>\Recent</code> flag.</div>
	<div class="tr tooltip-paragraph-fold">Unseen messages are messages that do not have the <code>\Seen</code> flag.</div>
	<div class="tr tooltip-paragraph-fold">You can get a list of mailbox folders using the "Get list of folders" action.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", use_timeout:true, visible:true}) %>
