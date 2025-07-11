<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({
		id: "uids",
		description: tr("Message id"),
		default_selector: "string",
		value_string: "",
		help: {
			description: tr("The id of the message which need to be copied from one mailbox folder to another. If a list is specified instead of one message id, then all messages from the specified list will be copied") + ", " + tr("as a list, you can use a string consisting of id, separated by commas."),
			examples: [
				{code: 134},
				{code: "263,339"},
				{code: "145, 187, 422"},
				{code: "[254, 356, 593, 694, 947]"}
			]
		}
	}) %>
	<%= _.template($('#input_constructor').html())({
		id: "toBox",
		description: tr("To folder"),
		default_selector: "string",
		variants: [
			{value: "INBOX", description: tr("Default folder incoming messages")}
		],
		disable_int: true,
		value_string: "",
		help: {
			description: tr("The name of the folder where the message or several messages will be copied."),
			examples: [
				{code: "INBOX", description: tr("Default folder incoming messages")},
				{code: "Spam", description: tr("Spam folder, on some mails")},
				{code: "Trash", description: tr("Trash folder, on some mails")}
			]
		}
	}) %>
	<%= _.template($('#block_start').html())({id:"Additional", name: tr("Additional settings"), description: ""}) %>
		<%= _.template($('#input_constructor').html())({
			id: "box",
			description: tr("From folder"),
			default_selector: "string",
			variants: [
				{value: "INBOX", description: tr("Default folder incoming messages")}
			],
			disable_int: true,
			value_string: "",
			help: {
				description: tr("Optional parameter.") + " " + tr("The name of the folder from which the message will be copied, if not specified, the folder specified in the \"Configure receiving mail\" action will be used.") + " " + tr("You can get a list of mailbox folders using the \"Get list of folders\" action."),
				examples: [
					{code: "INBOX", description: tr("Default folder incoming messages")},
					{code: "Spam", description: tr("Spam folder, on some mails")},
					{code: "Trash", description: tr("Trash folder, on some mails")},
					{code: tr("Empty string"), description: tr("Use the folder specified in the \"Configure receiving mail\" action")}
				]
			}
		}) %>
	<%= _.template($('#block_end').html())() %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Copy the specified message from one mailbox folder to another.</div>
	<div class="tr tooltip-paragraph-fold">In order to execute this action correctly you need to run "Configure receiving mail" action first.</div>
	<div class="tr tooltip-paragraph-fold">This action will copy the message from one mailbox folder to another. The copy of the message will have a different id.</div>
	<div class="tr tooltip-paragraph-fold">This action is only available when connecting via imap, if the action is called when connecting via pop3, it will immediately fail.</div>
	<div class="tr tooltip-paragraph-fold">In the additional settings, you can specify the name of the folder from which the message will be copied, otherwise the folder specified in the "Configure receiving mail" action will be used.</div>
	<div class="tr tooltip-paragraph-fold">To perform this action, you need to get the id of the message, you can do this using the actions "Find id of the last message", "Find id of one message by criteria".</div>
	<div class="tooltip-paragraph-fold"><span class="tr">This action can copy several messages at once</span>, <span class="tr">for this, instead of one id, a list of id must be specified</span>, <span class="tr">as a list, you can use a string consisting of id, separated by commas.</span></div>
	<div class="tr tooltip-paragraph-fold">You can get a list of message ids using the "Find id of multiple messages by criteria" action.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", use_timeout:true, visible:true}) %>