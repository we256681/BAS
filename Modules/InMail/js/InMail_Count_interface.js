<div class="container-fluid">
	<%= _.template($('#block_start').html())({id:"Filtration", name: tr("Filtration"), description: tr("Using the parameters from this block, you can filter the messages as you need, or leave them without filtering. You can combine the parameters of this block in any order, the specified parameters will be taken into account when filtering, and those not specified will be ignored.")}) %>
		<span class="tr" style="margin-left:15px">Sender of message</span> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title: tr("Sender of message"), description: tr("Sender of message, \"From\" field") + ", " + tr("using the two parameters below, you can make filter by the contents of this field."), examples: [{code: "admin@site.com"}, {code: "no-reply@example.com"}, {code: "Test &lt;info@test.com&gt;"}]})) %>"></i>
		<%= _.template($('#input_constructor').html())({
			id: "from",
			description: tr("Contains"),
			default_selector: "string",
			disable_int: true,
			value_string: "",
			help: {
				description: tr("Optional parameter.") + " " + tr("Part of the message sender address") + ", " + tr("that the searched message should contain") + ".",
				examples:[
					{code: "@twitter.com"},
					{code: "info@twitter.com"},
					{code: tr("Empty string"), description: tr("Don't filter by content in sender")}
				]
			}
		}) %>
		<%= _.template($('#input_constructor').html())({
			id: "notFrom",
			description: tr("Does not contain"),
			default_selector: "string",
			disable_int: true,
			value_string: "",
			help: {
				description: tr("Optional parameter.") + " " + tr("Part of the message sender address") + ", " + tr("that the searched message should not contain") + ".",
				examples:[
					{code: "@twitter.com"},
					{code: "info@twitter.com"},
					{code: tr("Empty string"), description: tr("Don't filter by \"Does not contain\" field in sender")}
				]
			}
		}) %>
		<div class="container-fluid">
			<div class="col-xs-12">
				<form class="form-horizontal">
					<div class="form-group">
						<div class="col-xs-12">
							<hr style="margin-top:0px;margin-bottom:0px"/>
						</div>
					</div>
				</form>
			</div>
		</div>
		<span class="tr" style="margin-left:15px">Recipient of message</span> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title: tr("Recipient of message"), description: tr("Recipient of message, \"To\" field") + ", " + tr("using the two parameters below, you can make filter by the contents of this field."), examples: [{code: "you@site.com"}, {code: "name@example.com"}, {code: "User &lt;user@test.com&gt;"}]})) %>"></i>
		<%= _.template($('#input_constructor').html())({
			id: "to",
			description: tr("Contains"),
			default_selector: "string",
			disable_int: true,
			value_string: "",
			help: {
				description: tr("Optional parameter.") + " " + tr("Part of the message recipient address") + ", " + tr("that the searched message should contain") + ".",
				examples: [
					{code: "you@site.com"},
					{code: "test@yourdomain.com"},
					{code: tr("Empty string"), description: tr("Don't filter by content in recipient")}
				]
			}
		}) %>
		<%= _.template($('#input_constructor').html())({
			id: "notTo",
			description: tr("Does not contain"),
			default_selector: "string",
			disable_int: true,
			value_string: "",
			help: {
				description: tr("Optional parameter.") + " " + tr("Part of the message recipient address") + ", " + tr("that the searched message should not contain") + ".",
				examples: [
					{code: "you@site.com"},
					{code: "test@yourdomain.com"},
					{code: tr("Empty string"), description: tr("Don't filter by \"Does not contain\" field in recipient")}
				]
			}
		}) %>
		<div class="container-fluid">
			<div class="col-xs-12">
				<form class="form-horizontal">
					<div class="form-group">
						<div class="col-xs-12">
							<hr style="margin-top:0px;margin-bottom:0px"/>
						</div>
					</div>
				</form>
			</div>
		</div>
		<span class="tr" style="margin-left:15px">Message subject</span> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title: tr("Message subject"), description: tr("Message subject, \"Subject\" field") + ", " + tr("using the two parameters below, you can make filter by the contents of this field."), examples: [{code: tr("Marketing")}, {code: tr("Business proposal")}, {code: tr("Email confirmation")}]})) %>"></i>
		<%= _.template($('#input_constructor').html())({
			id: "subject",
			description: tr("Contains"),
			default_selector: "string",
			disable_int: true,
			value_string: "",
			help:{
				description: tr("Optional parameter.") + " " + tr("Part of the message subject") + ", " + tr("that the searched message should contain") + ".",
				examples: [
					{code: tr("Business proposal")},
					{code: tr("Email confirmation")},
					{code: tr("Empty string"), description: tr("Don't filter by content in subject")}
				]
			}
		}) %>
		<%= _.template($('#input_constructor').html())({
			id: "notSubject",
			description: tr("Does not contain"),
			default_selector: "string",
			disable_int: true,
			value_string: "",
			help:{
				description: tr("Optional parameter.") + " " + tr("Part of the message subject") + ", " + tr("that the searched message should not contain") + ".",
				examples: [
					{code: tr("Business proposal")},
					{code: tr("Email confirmation")},
					{code: tr("Empty string"), description: tr("Don't filter by \"Does not contain\" field in subject")}
				]
			}
		}) %>
		<div class="container-fluid">
			<div class="col-xs-12">
				<form class="form-horizontal">
					<div class="form-group">
						<div class="col-xs-12">
							<hr style="margin-top:0px;margin-bottom:0px"/>
						</div>
					</div>
				</form>
			</div>
		</div>
		<span class="tr" style="margin-left:15px">Text of message</span> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title: tr("Text of message"), description: tr("Text of message") + ", " + tr("using the two parameters below, you can make filter by the contents of this field."), examples: [{code: "&lt;HTML&gt;&lt;BODY&gt;&lt;div&gt;" + tr("Use code 9779 to confirm") + "&lt;/div&gt;&lt;/BODY&gt;&lt;/HTML&gt;"}, {code: tr("Use code 9779 to confirm")}]})) %>"></i>
		<%= _.template($('#input_constructor').html())({
			id: "text",
			description: tr("Contains"),
			default_selector: "string",
			disable_int: true,
			value_string: "",
			help: {
				description: tr("Optional parameter.") + " " + tr("Part of the message text") + ", " + tr("that the searched message should contain") + ".",
				examples: [
					{code: "Hello"},
					{code: "CODE:"},
					{code: tr("Empty string"), description: tr("Don't filter by content in text")}
				]
			}
		}) %>
		<%= _.template($('#input_constructor').html())({
			id: "notText",
			description: tr("Does not contain"),
			default_selector: "string",
			disable_int: true,
			value_string: "",
			help: {
				description: tr("Optional parameter.") + " " + tr("Part of the message text") + ", " + tr("that the searched message should not contain") + ".",
				examples: [
					{code: "Hello"},
					{code: "CODE:"},
					{code: tr("Empty string"), description: tr("Don't filter by \"Does not contain\" field in text")}
				]
			}
		}) %>
		<div class="container-fluid">
			<div class="col-xs-12">
				<form class="form-horizontal">
					<div class="form-group">
						<div class="col-xs-12">
							<hr style="margin-top:0px;margin-bottom:0px"/>
						</div>
					</div>
				</form>
			</div>
		</div>
		<span class="tr" style="margin-left:15px">Flags of message</span> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title: tr("Flags of message"), description: tr("Flags of message") + ", " + tr("using the two parameters below, you can make filter by the contents of this field."), examples: [{code: "unseen"}, {code: "flagged,recent,unseen"}]})) %>"></i>
		<%= _.template($('#input_constructor').html())({
			id: "flags",
			description: tr("Contains"),
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
			help:{
				description: tr("Optional parameter.") + " " + tr("List of flags that the searched message should contain.") + " " + tr("As a list, you can use a string consisting of flags, separated by commas."),
				examples: [
					{code: "unseen"},
					{code: "flagged,recent,unseen"},
					{code: "flagged, recent, unseen"},
					{code: "[\"flagged\", \"recent\", \"unseen\"]"},
					{code: tr("Empty string"), description: tr("Do not filter by the presence of flags")}
				]
			}
		}) %>
		<%= _.template($('#input_constructor').html())({
			id: "notFlags",
			description: tr("Does not contain"),
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
			help:{
				description: tr("Optional parameter.") + " " + tr("List of flags that the searched message should not contain.") + " " + tr("As a list, you can use a string consisting of flags, separated by commas."),
				examples: [
					{code: "unseen"},
					{code: "flagged,recent,unseen"},
					{code: "flagged, recent, unseen"},
					{code: "[\"flagged\", \"recent\", \"unseen\"]"},
					{code: tr("Empty string"), description: tr("Do not filter by missing flags")}
				]
			}
		}) %>
		<div class="container-fluid">
			<div class="col-xs-12">
				<form class="form-horizontal">
					<div class="form-group">
						<div class="col-xs-12">
							<hr style="margin-top:0px;margin-bottom:0px"/>
						</div>
					</div>
				</form>
			</div>
		</div>
		<span class="tr" style="margin-left:15px">Receiving date</span> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title: tr("Receiving date"), description: tr("Internal date of the message (disregarding time and timezone)") + ", " + tr("using the two parameters below, you can make filter by the contents of this field.") + " " + tr("Date can be created using actions from the \"Date and time\" module."), examples: [{code: '9/14/2003'}, {code: 'May 13 2021'}, {code: '1622214946346'}, {code: new Date('Jan 20 2022 16:57:26')}, {code: new Date()}]})) %>"></i>
		<%= _.template($('#input_constructor').html())({
			id: "since",
			description: tr("From date"),
			default_selector: "string",
			disable_int: true,
			value_string: "",
			help: {
				description: tr("Optional parameter.") + " " + tr("Message received is within or later than the specified date.") + " " + tr("This field must contain a date or a string that can be converted to a date.") + " " + tr("Date can be created using actions from the \"Date and time\" module."),
				examples: [
					{code: '9/14/2003'},
					{code: 'May 13 2021'},
					{code: '1622214946346'},
					{code: new Date('Jan 20 2022 16:57:26')},
					{code: new Date()}
				]
			}
		}) %>
		<%= _.template($('#input_constructor').html())({
			id: "before",
			description: tr("To date"),
			default_selector: "string",
			disable_int: true,
			value_string: "",
			help: {
				description: tr("Optional parameter.") + " " + tr("Message received is earlier than the specified date.") + " " + tr("This field must contain a date or a string that can be converted to a date.") + " " + tr("Date can be created using actions from the \"Date and time\" module."),
				examples: [
					{code: '9/14/2003'},
					{code: 'May 13 2021'},
					{code: '1622214946346'},
					{code: new Date('Jan 20 2022 16:57:26')},
					{code: new Date()}
				]
			}
		}) %>
	<%= _.template($('#block_end').html())() %>
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
		default_variable: "MAIL_COUNT",
		help: {
			description: tr("Variable in which, after successful execution of the action, the number of messages will be written."),
			examples: [
				{code: 2},
				{code: 13},
				{code: 256}
			]
		}
	}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get the number of messages that match the specified criteria.</div>
	<div class="tr tooltip-paragraph-fold">In order to execute this action correctly you need to run "Configure receiving mail" action first.</div>
	<div class="tr tooltip-paragraph-fold">This action will return the number of messages matching the specified filtering criteria, or the count of all messages if no filtering criteria is specified.</div>
	<div class="tr tooltip-paragraph-fold">Using the parameters from the "Filtration" block, you can filter messages at your discretion or leave them unfiltered.</div>
	<div class="tr tooltip-paragraph-fold">Filtering is performed on the server side.</div>
	<div class="tr tooltip-paragraph-fold">Filtering is available only when connecting via imap, if the action is called when connecting via pop3 and filtering is active, then the action will immediately fail.</div>
	<div class="tr tooltip-paragraph-fold">In the additional settings, you can specify the name of the folder in which this action will be performed, otherwise the folder specified in the "Configure receiving mail" action will be used.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", use_timeout:true, visible:true}) %>
