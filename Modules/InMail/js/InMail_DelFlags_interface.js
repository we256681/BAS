<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({
		id: "uids",
		description: tr("Message id"),
		default_selector: "string",
		value_string: "",
		help: {
			description: tr("The id of the message from which need to remove the flags. If a list is specified instead of one message id, then flags will be removed from all messages from the specified list") + ", " + tr("as a list, you can use a string consisting of id, separated by commas."),
			examples: [
				{code: 134},
				{code: "263,339"},
				{code: "145, 187, 422"},
				{code: "[254, 356, 593, 694, 947]"}
			]
		}
	}) %>
	<%= _.template($('#input_constructor').html())({
		id: "flags",
		description: tr("Flags"),
		default_selector: "string",
		variants: [
			{value: "\\Seen", description: tr("Message has been read")},
			{value: "\\Answered", description: tr("Message has been answered")},
			{value: "\\Flagged", description: tr("Message is \"flagged\" for urgent/special attention")},
			{value: "\\Deleted", description: tr("Message is marked for removal")},
			{value: "\\Draft", description: tr("Message has not completed composition (marked as a draft)")}
		],
		disable_int: true,
		value_string: "",
		help: {
			description: tr("List or one flag which needs to remove from the message.") + " " + tr("As a list, you can use a string consisting of flags, separated by commas.") + " " + tr("The possible flags may differ depending on the server implementation.") + "<br/><strong>\\Seen</strong> - " + tr("Message has been read") + "<br/><strong>\\Answered</strong> - " + tr("Message has been answered") + "<br/><strong>\\Flagged</strong> - " + tr("Message is \"flagged\" for urgent/special attention") + "<br/><strong>\\Deleted</strong> - " + tr("Message is marked for removal") + "<br/><strong>\\Draft</strong> - " + tr("Message has not completed composition (marked as a draft)"),
			examples: [
				{code: "\\Seen,\\Flagged"},
				{code: "\\Seen, \\Flagged"},
				{code: "[\"\\Seen\", \"\\Flagged\"]"}
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
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Remove a flag or several flags from the specified message.</div>
	<div class="tr tooltip-paragraph-fold">In order to execute this action correctly you need to run "Configure receiving mail" action first.</div>
	<div class="tr tooltip-paragraph-fold">This action will remove the flags from the specified message.</div>
	<div class="tr tooltip-paragraph-fold">This action is only available when connecting via imap, if the action is called when connecting via pop3, it will immediately fail.</div>
	<div class="tr tooltip-paragraph-fold">Flags are system message tags starting with <code>\</code>. Possible flags:</div>
	<div class="tooltip-paragraph-fold"><code>\Seen</code> - <span class="tr">Message has been read</span>.</div>
	<div class="tooltip-paragraph-fold"><code>\Answered</code> - <span class="tr">Message has been answered</span>.</div>
	<div class="tooltip-paragraph-fold"><code>\Flagged</code> - <span class="tr">Message is "flagged" for urgent/special attention</span>.</div>
	<div class="tooltip-paragraph-fold"><code>\Deleted</code> - <span class="tr">Message is marked for removal</span>.</div>
	<div class="tooltip-paragraph-fold"><code>\Draft</code> - <span class="tr">Message has not completed composition (marked as a draft)</span>.</div>
	<div class="tr tooltip-paragraph-fold">The possible flags may differ depending on the server implementation.</div>
	<div class="tr tooltip-paragraph-fold">To perform this action, you need to get the id of the message, you can do this using the actions "Find id of the last message", "Find id of one message by criteria".</div>
	<div class="tooltip-paragraph-fold"><span class="tr">This action can remove flags for several messages at once</span>, <span class="tr">for this, instead of one id, a list of id must be specified</span>, <span class="tr">as a list, you can use a string consisting of id, separated by commas.</span></div>
	<div class="tr tooltip-paragraph-fold">You can get a list of message ids using the "Find id of multiple messages by criteria" action.</div>
	<div class="tr tooltip-paragraph-fold">You can get message flags using the "Get message flags by id" action.</div>
	<div class="tr tooltip-paragraph-fold">You can set message flags using the "Set message flags by id" action.</div>
	<div class="tr tooltip-paragraph-fold">In the additional settings, you can specify the name of the folder in which this action will be performed, otherwise the folder specified in the "Configure receiving mail" action will be used.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", use_timeout:true, visible:true}) %>
