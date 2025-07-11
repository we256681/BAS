<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({
		id: "uid",
		description: tr("Message id"),
		default_selector: "string",
		value_string: "",
		help: {
			description: tr("The id of the message which flags needs to be retrieved."),
			examples: [
				{code: 134},
				{code: 370},
				{code: 458}
			]
		}
	}) %>
	<%= _.template($('#block_start').html())({id:"Additional", name: tr("Additional settings"), description: ""}) %>
		<%= _.template($('#input_constructor').html())({
			id: "box",
			description: tr("Folder name"),
			default_selector: "string",
			variants: [
				{value: "INBOX", description: tr("Default folder incoming messages")}
			],
			disable_int: true,
			value_string: "",
			help: {
				description: tr("Optional parameter.") + " " + tr("The name of the folder in which this action will be performed, if not specified, the folder specified in the \"Configure receiving mail\" action will be used.") + " " + tr("You can get a list of mailbox folders using the \"Get list of folders\" action."),
				examples: [
					{code: "INBOX", description: tr("Default folder incoming messages")},
					{code: "Spam", description: tr("Spam folder, on some mails")},
					{code: "Trash", description: tr("Trash folder, on some mails")},
					{code: tr("Empty string"), description: tr("Use the folder specified in the \"Configure receiving mail\" action")}
				]
			}
		}) %>
	<%= _.template($('#block_end').html())() %>
	<%= _.template($('#variable_constructor').html())({
		id: "Save",
		description: tr("Variable to save the result"),
		default_variable: "MAIL_FLAGS_LIST",
		help: {
			description: tr("Variable in which, after successful execution of the action, the list of flags will be written.") + " " + tr("The possible flags may differ depending on the server implementation."),
			examples: [
				{code: "[\"\\Seen\"]"},
				{code: "[\"\\Seen\", \"\\Flagged\"]"},
				{code: "[\"\\Seen\", \"\\Answered\", \"\\Deleted\"]"}
			]
		}
	}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get a list of flags for the specified message.</div>
	<div class="tr tooltip-paragraph-fold">In order to execute this action correctly you need to run "Configure receiving mail" action first.</div>
	<div class="tr tooltip-paragraph-fold">This action will return a list of flags set for the specified message.</div>
	<div class="tr tooltip-paragraph-fold">This action is only available when connecting via imap, if the action is called when connecting via pop3, it will immediately fail.</div>
	<div class="tr tooltip-paragraph-fold">The resulting list can be processed using actions from the "List" module.</div>
	<div class="tr tooltip-paragraph-fold">Flags are system message tags starting with <code>\</code>. Possible flags:</div>
	<div class="tooltip-paragraph-fold"><code>\Seen</code> - <span class="tr">Message has been read</span>.</div>
	<div class="tooltip-paragraph-fold"><code>\Answered</code> - <span class="tr">Message has been answered</span>.</div>
	<div class="tooltip-paragraph-fold"><code>\Flagged</code> - <span class="tr">Message is "flagged" for urgent/special attention</span>.</div>
	<div class="tooltip-paragraph-fold"><code>\Deleted</code> - <span class="tr">Message is marked for removal</span>.</div>
	<div class="tooltip-paragraph-fold"><code>\Draft</code> - <span class="tr">Message has not completed composition (marked as a draft)</span>.</div>
	<div class="tr tooltip-paragraph-fold">The possible flags may differ depending on the server implementation.</div>
	<div class="tr tooltip-paragraph-fold">To perform this action, you need to get the id of the message, you can do this using the actions "Find id of the last message", "Find id of one message by criteria".</div>
	<div class="tr tooltip-paragraph-fold">You can set message flags using the "Set message flags by id" action.</div>
	<div class="tr tooltip-paragraph-fold">You can remove message flags using the "Remove message flags by id" action.</div>
	<div class="tr tooltip-paragraph-fold">In the additional settings, you can specify the name of the folder in which this action will be performed, otherwise the folder specified in the "Configure receiving mail" action will be used.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", use_timeout:true, visible:true}) %>
