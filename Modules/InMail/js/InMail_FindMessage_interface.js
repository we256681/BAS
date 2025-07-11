<div class="container-fluid">
	<%= _.template($('#block_start').html())({id:"Filtration", name: tr("Filtration"), description: tr("Using the parameters from this block, you can filter the results as you need, or leave them without filtering. You can combine the parameters of this block in any order, the specified parameters will be taken into account when filtering, and those not specified will be ignored.")}) %>
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
	<%= _.template($('#block_start').html())({id:"Sorting", name: tr("Sorting"), description: tr("Using the parameters from this block, you can sort the results in the order you need, or leave them without sorting.")}) %>
		<%= _.template($('#input_constructor').html())({
			id: "sortType",
			description: tr("Sorting type"),
			default_selector: "string",
			variants: [
				{value: "no sorting", description: tr("No sorting")},
				{value: "ascending", description: tr("Sort ascending")},
				{value: "descending", description: tr("Sort descending")}
			],
			disable_int: true,
			value_string: "no sorting",
			help: {
				description: tr("By default, sorting is disabled, but you can enable it by changing the value of this parameter and specifying the \"Sorting field\" parameter.") + " " + tr("Sorting is not supported by all mail services, if sorting is enabled, but the service does not support it, the action will fail."),
				examples: [
					{code: "no sorting", description: tr("No sorting")},
					{code: "ascending", description: tr("Sort ascending")},
					{code: "descending", description: tr("Sort descending")}
				]
			}
		}) %>
		<%= _.template($('#input_constructor').html())({
			id: "sortField",
			description: tr("Sorting field"),
			default_selector: "string",
			variants: [
				{value: "from", description: tr("Sender of message") + ", " + tr("first \"From\" address") + ", " + tr("sort alphabetically")},
				{value: "to", description: tr("Recipient of message") + ", " + tr("first \"To\" address") + ", " + tr("sort alphabetically")},
				{value: "subject", description: tr("Message subject") + ", " + tr("sort alphabetically")},
				{value: "size", description: tr("Message size")},
				{value: "date", description: tr("Receiving date")}
			],
			disable_int: true,
			value_string: "",
			help: {
				description: tr("Part of the message by which sorting will be performed."),
				examples: [
					{code: "from", description: tr("Sender of message") + ", " + tr("first \"From\" address") + ", " + tr("sort alphabetically")},
					{code: "to", description: tr("Recipient of message") + ", " + tr("first \"To\" address") + ", " + tr("sort alphabetically")},
					{code: "subject", description: tr("Message subject") + ", " + tr("sort alphabetically")},
					{code: "size", description: tr("Message size")},
					{code: "date", description: tr("Receiving date")}
				]
			}
		}) %>
	<%= _.template($('#block_end').html())() %>
	<%= _.template($('#block_start').html())({id:"Parsing", name: tr("Parsing"), description: tr("Using the parameters from this block, you can select which parts of the message will be fetched and in which variables will be saved.")}) %>
		<span data-preserve="true" data-preserve-type="check" data-preserve-id="getUid">
			<input type="checkbox" checked="checked" id="getUid" style="margin-left:25px"/> <label for="getUid" class="tr">Get message id</label> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title: tr("Get message id"), description: tr("If enabled, then the message id will be stored in the variable below."), examples: [{code: tr("Activated"), description: tr("Save message id")}, {code: tr("Deactivated"), description: tr("Don't save message id")}]})) %>"></i>
		</span>
		<%= _.template($('#variable_constructor').html())({
			id: "saveUid",
			description: tr("Message id"),
			default_variable: "MAIL_ID",
			help: {
				description: tr("Variable in which, after successful execution of the action, the id of the retrieved message will be written."),
				examples: [
					{code: 134},
					{code: 370},
					{code: 458}
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
		<span data-preserve="true" data-preserve-type="check" data-preserve-id="getFrom">
			<input type="checkbox" id="getFrom" style="margin-left:25px"/> <label for="getFrom" class="tr">Get sender of message</label> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title: tr("Get sender of message"), description: tr("If enabled, then the sender of the message will be retrieved from the server and stored in the variable below."), examples: [{code: tr("Activated"), description: tr("Get message sender from server and save")}, {code: tr("Deactivated"), description: tr("Don't get message sender from server")}]})) %>"></i>
		</span>
		<span id="advancedFrom">
			<%= _.template($('#variable_constructor').html())({
				id: "saveFrom",
				description: tr("Sender of message"),
				default_variable: "MAIL_SENDER",
				help: {
					description: tr("Variable in which, after successful execution of the action, the sender of the retrieved message will be written."),
					examples: [
						{code: "admin@site.com"},
						{code: "no-reply@example.com"},
						{code: "Test &lt;info@test.com&gt;"}
					]
				}
			}) %>
		</span>
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
		<span data-preserve="true" data-preserve-type="check" data-preserve-id="getTo">
			<input type="checkbox" id="getTo" style="margin-left:25px"/> <label for="getTo" class="tr">Get recipient of message</label> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title: tr("Get recipient of message"), description: tr("If enabled, then the recipient of the message will be retrieved from the server and stored in the variable below."), examples: [{code: tr("Activated"), description: tr("Get message recipient from server and save")}, {code: tr("Deactivated"), description: tr("Don't get message recipient from server")}]})) %>"></i>
		</span>
		<span id="advancedTo">
			<%= _.template($('#variable_constructor').html())({
				id: "saveTo",
				description: tr("Recipient of message"),
				default_variable: "MAIL_RECIPIENT",
				help: {
					description: tr("Variable in which, after successful execution of the action, the recipient of the retrieved message will be written."),
					examples: [
						{code: "you@site.com"},
						{code: "name@example.com"},
						{code: "User &lt;user@test.com&gt;"}
					]
				}
			}) %>
		</span>
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
		<span data-preserve="true" data-preserve-type="check" data-preserve-id="getSubject">
			<input type="checkbox" id="getSubject" style="margin-left:25px"/> <label for="getSubject" class="tr">Get message subject</label> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title: tr("Get message subject"), description: tr("If enabled, then the subject of the message will be retrieved from the server and stored in the variable below."), examples: [{code: tr("Activated"), description: tr("Get message subject from server and save")}, {code: tr("Deactivated"), description: tr("Don't get message subject from server")}]})) %>"></i>
		</span>
		<span id="advancedSubject">
			<%= _.template($('#variable_constructor').html())({
				id: "saveSubject",
				description: tr("Message subject"),
				default_variable: "MAIL_SUBJECT",
				help: {
					description: tr("Variable in which, after successful execution of the action, the subject of the retrieved message will be written."),
					examples: [
						{code: tr("Marketing")},
						{code: tr("Business proposal")},
						{code: tr("Email confirmation")}
					]
				}
			}) %>
		</span>
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
		<span data-preserve="true" data-preserve-type="check" data-preserve-id="getTextHtml">
			<input type="checkbox" checked="checked" id="getTextHtml" style="margin-left:25px"/> <label for="getTextHtml"><span class="tr">Get body of message</span> (text/html)</label> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title: tr("Get body of message") + " (text/html)", description: tr("If enabled, then the body of the message in html format (text/html) will be retrieved from the server and stored in the variable below.") + " " + tr("text/html is the MIME type of data represented in html format, html is the markup language for web pages.") + " " + tr("If the message does not contain a body with this type, then an empty string will be stored."), examples: [{code: tr("Activated"), description: tr("Get message body (text/html) from server and save")}, {code: tr("Deactivated"), description: tr("Don't get message body (text/html) from server")}]})) %>"></i>
		</span>
		<span id="advancedTextHtml">
			<%= _.template($('#variable_constructor').html())({
				id: "saveTextHtml",
				description: tr("Body of message") + " (text/html)",
				default_variable: "MAIL_TEXT_HTML",
				help: {
					description: tr("Variable in which, after successful execution of the action, the body of the retrieved message in html format (text/html) will be written.") + " " + tr("text/html is the MIME type of data represented in html format, html is the markup language for web pages.") + " " + tr("If the message does not contain a body with this type, then an empty string will be stored."),
					examples: [
						{code: "<p style=\"margin-bottom:6px\">&lt;HTML&gt;&lt;BODY&gt;&lt;div&gt;" + tr("Use code 9779 to confirm") + "&lt;/div&gt;&lt;/BODY&gt;&lt;/HTML&gt;</p>"},
						{code: tr("Empty string"), description: tr("The message does not contain a body in html format")}
					]
				}
			}) %>
			<div>
				<span data-preserve="true" data-preserve-type="check" data-preserve-id="getLinksTextHtml">
					<input type="checkbox" id="getLinksTextHtml" style="margin-left:25px"/> <label for="getLinksTextHtml"><span class="tr">Parse links from body of message</span> (text/html)</label> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title: tr("Parse links from body of message") + " (text/html)", description: tr("If enabled, then all links will be extracted from the retrieved message body (text/html) and stored in the variable below.") + " " + tr("The resulting list can be processed using actions from the \"List\" module."), examples: [{code: tr("Activated"), description: tr("Extract links from message body") + " (text/html)"}, {code: tr("Deactivated"), description: tr("Don't extract links from message body") + " (text/html)"}]})) %>"></i>
				</span>
				<span>
					<%= _.template($('#variable_constructor').html())({
						id: "saveLinksTextHtml",
						description: tr("Links from body of message") + " (text/html)",
						default_variable: "MAIL_TEXT_HTML_LINKS_LIST",
						help: {
							description: tr("Variable in which, after successful execution of the action, the list of links from the body of the message (text/html) will be written.") + " " + tr("The resulting list can be processed using actions from the \"List\" module."),
							examples: [
								{code: "[\"https://marketplace.biz/section_3/product_213234.php\"]"},
								{code: "[\"http://test.com\", \"example.org\"]"},
								{code: "[\"http://www.ad.by\", \"www.feedback.io\", \"https://support.co/new/ticket.php\"]"}
							]
						}
					}) %>
				</span>
			</div>
		</span>
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
		<span data-preserve="true" data-preserve-type="check" data-preserve-id="getTextPlain">
			<input type="checkbox" checked="checked" id="getTextPlain" style="margin-left:25px"/> <label for="getTextPlain"><span class="tr">Get body of message</span> (text/plain)</label> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title: tr("Get body of message") + " (text/plain)", description: tr("If enabled, then the body of the message in text format (text/plain) will be retrieved from the server and stored in the variable below.") + " " + tr("text/plain is a MIME type that is the base type for text files.") + " " + tr("If the message does not contain a body with this type, then an empty string will be stored."), examples: [{code: tr("Activated"), description: tr("Get message body (text/plain) from server and save")}, {code: tr("Deactivated"), description: tr("Don't get message body (text/plain) from server")}]})) %>"></i>
		</span>
		<span id="advancedTextPlain">
			<%= _.template($('#variable_constructor').html())({
				id: "saveTextPlain",
				description: tr("Body of message") + " (text/plain)",
				default_variable: "MAIL_TEXT_PLAIN",
				help: {
					description: tr("Variable in which, after successful execution of the action, the body of the retrieved message in text format (text/plain) will be written.") + " " + tr("text/plain is a MIME type that is the base type for text files.") + " " + tr("If the message does not contain a body with this type, then an empty string will be stored."),
					examples: [
						{code: "<p style=\"margin-bottom:6px\">" + tr("Use code 9779 to confirm") + "</p>"},
						{code: tr("Empty string"), description: tr("The message does not contain a body in text format")}
					]
				}
			}) %>
			<div>
				<span data-preserve="true" data-preserve-type="check" data-preserve-id="getLinksTextPlain">
					<input type="checkbox" id="getLinksTextPlain" style="margin-left:25px"/> <label for="getLinksTextPlain"><span class="tr">Parse links from body of message</span> (text/plain)</label> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title: tr("Parse links from body of message") + " (text/plain)", description: tr("If enabled, then all links will be extracted from the retrieved message body (text/plain) and stored in the variable below.") + " " + tr("The resulting list can be processed using actions from the \"List\" module."), examples: [{code: tr("Activated"), description: tr("Extract links from message body") + " (text/plain)"}, {code: tr("Deactivated"), description: tr("Don't extract links from message body") + " (text/plain)"}]})) %>"></i>
				</span>
				<span>
					<%= _.template($('#variable_constructor').html())({
						id: "saveLinksTextPlain",
						description: tr("Links from body of message") + " (text/plain)",
						default_variable: "MAIL_TEXT_PLAIN_LINKS_LIST",
						help: {
							description: tr("Variable in which, after successful execution of the action, the list of links from the body of the message (text/plain) will be written.") + " " + tr("The resulting list can be processed using actions from the \"List\" module."),
							examples: [
								{code: "[\"https://marketplace.biz/section_3/product_213234.php\"]"},
								{code: "[\"http://test.com\", \"example.org\"]"},
								{code: "[\"http://www.ad.by\", \"www.feedback.io\", \"https://support.co/new/ticket.php\"]"}
							]
						}
					}) %>
				</span>
			</div>
		</span>
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
		<span data-preserve="true" data-preserve-type="check" data-preserve-id="getTextRaw">
			<input type="checkbox" id="getTextRaw" style="margin-left:25px"/> <label for="getTextRaw"><span class="tr">Get body of message</span> (raw)</label> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title: tr("Get body of message") + " (raw)", description: tr("If enabled, then the body of the message in raw form (raw) will be retrieved from the server and stored in the variable below.") + " " + tr("raw - this means that the body of the message will be retrieved in its raw form, without any processing."), examples: [{code: tr("Activated"), description: tr("Get message body (raw) from server and save")}, {code: tr("Deactivated"), description: tr("Don't get message body (raw) from server")}]})) %>"></i>
		</span>
		<span id="advancedTextRaw">
			<%= _.template($('#variable_constructor').html())({
				id: "saveTextRaw",
				description: tr("Body of message") + " (raw)",
				default_variable: "MAIL_TEXT_RAW",
				help: {
					description: tr("Variable in which, after successful execution of the action, the body of the retrieved message in raw form (raw) will be written.") + " " + tr("raw - this means that the body of the message will be retrieved in its raw form, without any processing."),
					examples: [
						{code: "----ALT--d449E7c1653598173380270778C1f6B21643480035<br/>Content-Type: text/plain; charset=utf-8<br/>Content-Transfer-Encoding: base64<p style=\"margin-top:6px;margin-bottom:6px\">ClVzZSBjb2RlIDk3NzkgdG8gY29uZmlybQrCoArCoA==</p><p style=\"margin-bottom:6px\">----ALT--d449E7c1653598173380270778C1f6B21643480035</p>"},
						{code: tr("Empty string"), description: tr("The message does not contain a body")}
					]
				}
			}) %>
			<div>
				<span data-preserve="true" data-preserve-type="check" data-preserve-id="getLinksTextRaw">
					<input type="checkbox" id="getLinksTextRaw" style="margin-left:25px"/> <label for="getLinksTextRaw"><span class="tr">Parse links from body of message</span> (raw)</label> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title: tr("Parse links from body of message") + " (raw)", description: tr("If enabled, then all links will be extracted from the retrieved message body (raw) and stored in the variable below.") + " " + tr("The resulting list can be processed using actions from the \"List\" module."), examples: [{code: tr("Activated"), description: tr("Extract links from message body") + " (raw)"}, {code: tr("Deactivated"), description: tr("Don't extract links from message body") + " (raw)"}]})) %>"></i>
				</span>
				<span>
					<%= _.template($('#variable_constructor').html())({
						id: "saveLinksTextRaw",
						description: tr("Links from body of message") + " (raw)",
						default_variable: "MAIL_TEXT_RAW_LINKS_LIST",
						help: {
							description: tr("Variable in which, after successful execution of the action, the list of links from the body of the message (raw) will be written.") + " " + tr("The resulting list can be processed using actions from the \"List\" module."),
							examples: [
								{code: "[\"https://marketplace.biz/section_3/product_213234.php\"]"},
								{code: "[\"http://test.com\", \"example.org\"]"},
								{code: "[\"http://www.ad.by\", \"www.feedback.io\", \"https://support.co/new/ticket.php\"]"}
							]
						}
					}) %>
				</span>
			</div>
		</span>
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
		<span data-preserve="true" data-preserve-type="check" data-preserve-id="getSize">
			<input type="checkbox" id="getSize" style="margin-left:25px"/> <label for="getSize" class="tr">Get message size</label> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title: tr("Get message size"), description: tr("If enabled, then the size of the message will be retrieved from the server and stored in the variable below.") + " " + tr("The size of the message will be retrieved in octets, one octet is equal to 8 bits."), examples: [{code: tr("Activated"), description: tr("Get message size from server and save")}, {code: tr("Deactivated"), description: tr("Don't get message size from server")}]})) %>"></i>
		</span>
		<span id="advancedSize">
			<%= _.template($('#variable_constructor').html())({
				id: "saveSize",
				description: tr("Message size"),
				default_variable: "MAIL_SIZE",
				help: {
					description: tr("Variable in which, after successful execution of the action, the size of the retrieved message will be written.") + " " + tr("The size of the message will be retrieved in octets, one octet is equal to 8 bits."),
					examples: [
						{code: 175},
						{code: 2345},
						{code: 11546}
					]
				}
			}) %>
		</span>
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
		<span data-preserve="true" data-preserve-type="check" data-preserve-id="getFlags">
			<input type="checkbox" id="getFlags" style="margin-left:25px"/> <label for="getFlags" class="tr">Get flags of message</label> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title: tr("Get flags of message"), description: tr("If enabled, then the list of message flags will be retrieved from the server and stored in the variable below.") + " " + tr("The resulting list can be processed using actions from the \"List\" module."), examples: [{code: tr("Activated"), description: tr("Get message flags from server and save")}, {code: tr("Deactivated"), description: tr("Don't get message flags from server")}]})) %>"></i>
		</span>
		<span id="advancedFlags">
			<%= _.template($('#variable_constructor').html())({
				id: "saveFlags",
				description: tr("Flags of message"),
				default_variable: "MAIL_FLAGS_LIST",
				help: {
					description: tr("Variable in which, after successful execution of the action, the list of flags of the retrieved message will be written.") + " " + tr("The resulting list can be processed using actions from the \"List\" module."),
					examples: [
						{code: "[\"\\Seen\"]"},
						{code: "[\"\\Seen\", \"\\Flagged\"]"},
						{code: "[\"\\Seen\", \"\\Answered\", \"\\Deleted\"]"}
					]
				}
			}) %>
		</span>
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
		<span data-preserve="true" data-preserve-type="check" data-preserve-id="getDate">
			<input type="checkbox" id="getDate" style="margin-left:25px"/> <label for="getDate" class="tr">Get receiving date</label> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title: tr("Get receiving date"), description: tr("If enabled, then the date of the message will be retrieved from the server and stored in the variable below.") + " " + tr("The resulting date can be processed using actions from the \"Date and time\" module."), examples: [{code: tr("Activated"), description: tr("Get message date from server and save")}, {code: tr("Deactivated"), description: tr("Don't get message date from server")}]})) %>"></i>
		</span>
		<span id="advancedDate">
			<%= _.template($('#variable_constructor').html())({
				id: "saveDate",
				description: tr("Receiving date"),
				default_variable: "MAIL_DATE",
				help: {
					description: tr("Variable in which, after successful execution of the action, the date of the retrieved message will be written.") + " " + tr("The resulting date can be processed using actions from the \"Date and time\" module."),
					examples: [
						{code: new Date('May 13 2021')},
						{code: new Date('Jan 20 2022 16:57:26')},
						{code: new Date()}
					]
				}
			}) %>
		</span>
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
		<span data-preserve="true" data-preserve-type="check" data-preserve-id="getAttachNames">
			<input type="checkbox" id="getAttachNames" style="margin-left:25px"/> <label for="getAttachNames" class="tr">Get list of attached file names</label> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title: tr("Get list of attached file names"), description: tr("If enabled, then the list of filenames attached to the message will be retrieved from the server and stored in the variable below.") + " " + tr("The resulting list can be processed using actions from the \"List\" module."), examples: [{code: tr("Activated"), description: tr("Get list of file attachment names of the message from server and save")}, {code: tr("Deactivated"), description: tr("Don't get list of file attachment names of the message from server")}]})) %>"></i>
		</span>
		<span id="advancedAttachnames">
			<%= _.template($('#variable_constructor').html())({
				id: "saveAttachnames",
				description: tr("Attachment names"),
				default_variable: "MAIL_ATTACHMENT_NAMES_LIST",
				help: {
					description: tr("Variable in which, after successful execution of the action, the list of filenames attached to the retrieved message will be written.") + " " + tr("The resulting list can be processed using actions from the \"List\" module."),
					examples: [
						{code: "[\"photo.png\"]"},
						{code: "[\"document.pdf\", \"text.txt\"]"},
						{code: "[\"index.html\", \"script.js\", \"icon.ico\"]"}
					]
				}
			}) %>
		</span>
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
		<span data-preserve="true" data-preserve-type="check" data-preserve-id="getAttachments">
			<input type="checkbox" id="getAttachments" style="margin-left:25px"/> <label for="getAttachments" class="tr">Save attached files</label> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title: tr("Save attached files"), description: tr("If enabled, then the attachments of the message will be retrieved from the server and saved to the computer as files with a random name, a list of objects with information about attachments will be written to the variable specified below the mask.") + " " + tr("The resulting list can be processed using actions from the \"JSON\" module.") + " " + tr("You can choose which attachments will be saved by specifying the filename mask in the parameter below."), examples: [{code: tr("Activated"), description: tr("Get message attachments from server and save")}, {code: tr("Deactivated"), description: tr("Don't get message attachments from server")}]})) %>"></i>
		</span>
		<span id="advancedAttachments">
			<%= _.template($('#input_constructor').html())({
				id: "attachmentsMask",
				description: tr("File name mask"),
				default_selector: "string",
				disable_int: true,
				value_string: "*",
				help: {
					description: tr("The file name mask by which attachments will be filtered. Only files whose names match the mask will be retrieved and saved. In the mask, the character <code>*</code> is perceived as a sequence of any characters, and the character <code>?</code> treated as one any character. If the mask begins with the character <code>!</code>, then all attachments whose names do not match this mask will be retrieved and saved.") + " " + tr("You can specify several masks by separating them with the character <code>;</code>, the filtering results of all specified masks will be combined."),
					examples: [
						{code: tr("*.js"), description: tr("All JavaScript files")},
						{code: tr("*.txt"), description: tr("All text files")},
						{code: tr("!*.txt"), description: tr("All files except text")},
						{code: tr("*.js;*.txt"), description: tr("All JavaScript and all text files")},
						{code: tr("*"), description: tr("All files")}
					]
				}
			}) %>
			<%= _.template($('#variable_constructor').html())({
				id: "saveAttachments",
				description: tr("Attachments"),
				default_variable: "MAIL_ATTACHMENTS_LIST",
				help: {
					description: tr("Variable in which, after successful execution of the action, the list of objects containing information about the saved attachments of the message will be written.") + " " + tr("The resulting list can be processed using actions from the \"JSON\" module.") + " " + tr("The \"name\" property of the object will contain the original file name, the \"path\" property will contain the path to the saved file, and the \"type\" property will contain the mime type of the file. Having obtained the path to the file and its original name from the object, you can rename it using the \"Move file/folder\" action from the \"Filesystem\" module."),
					examples: [
						{code: "[{<br/>name: \"photo.png\",<br/>path: \"1VVDZIew0K.file\",<br/>type: \"image/png\"<br/>}, {<br/>name: \"document.pdf\",<br/>path: \"D0rThm7KHp.file\",<br/>type: \"application/pdf\"<br/>}, {<br/>name: \"text.txt\",<br/>path: \"uBhN59Tokt.file\",<br/>type: \"text/plain\"<br/>}]"}
					]
				}
			}) %>
		</span>
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
		<span data-preserve="true" data-preserve-type="check" data-preserve-id="getRawHeader">
			<input type="checkbox" id="getRawHeader" style="margin-left:25px"/> <label for="getRawHeader" class="tr">Get technical headers of message</label> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title: tr("Get technical headers of message"), description: tr("If enabled, then all the headers of message will be retrieved from the server and stored in the variable below as an object.") + " " + tr("The resulting object can be processed using actions from the \"JSON\" module."), examples: [{code: tr("Activated"), description: tr("Get message technical headers from server and save")}, {code: tr("Deactivated"), description: tr("Don't get message technical headers from server")}]})) %>"></i>
		</span>
		<span id="advancedRawHeader">
			<%= _.template($('#variable_constructor').html())({
				id: "saveRawHeader",
				description: tr("Technical headers of message"),
				default_variable: "MAIL_RAW_HEADERS",
				help: {
					description: tr("Variable in which, after successful execution of the action, an object with all the headers of the retrieved message will be written.") + " " + tr("The resulting object can be processed using actions from the \"JSON\" module.") + " " + tr("The headers are stored as an object where the key is the header name and the value is the header value, the header value can be a string or a list of strings."),
					examples: [
						{code: "{<br/>content-type: \"multipart/alternative\",<br/>date: \"Thu, 18 Jun 2020 11:26:19 +0000 (UTC)\",<br/>delivered-to: \"you@site.com\",<br/>mime-version: \"1.0\",<br/>message-id: \"5eeb4f5be870.6c531352947a@example.com\",<br/>x-info: \"info\"<br/>...<br/>}"}
					]
				}
			}) %>
		</span>
	<%= _.template($('#block_end').html())() %>
	<%= _.template($('#block_start').html())({id:"Additional", name: tr("Additional settings"), description: ""}) %>
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
		<span data-preserve="true" data-preserve-type="check" data-preserve-id="wait">
			<input type="checkbox" id="wait" style="margin-left:25px"/> <label for="wait" class="tr">Wait message</label> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title: tr("Wait message"), description: tr("Optional parameter.") + " " + tr("If enabled, then the action will not fail if the message is not found according to the specified criteria, but will wait for the specified time, and only if the message is not found within the specified time, the action will fail."), examples: [{code: tr("Activated"), description: tr("Wait message")}, {code: tr("Deactivated"), description: tr("Don't wait message")}]})) %>"></i>
		</span>
		<span id="waitSettings">
			<%= _.template($('#input_constructor').html())({
				id: "minResults",
				description: tr("Number of messages"),
				default_selector: "int",
				disable_string: true,
				value_number: 1,
				min_number: 1,
				max_number: 999999,
				help: {
					description: tr("Wait for the appearance of the specified number of messages matching the specified criteria. The action will be completed successfully when the number of messages matching the specified criteria is equal to or greater than the number specified in this parameter."),
					examples: [
						{code: 1, description: tr("Wait for one message matching the specified criteria")},
						{code: 5, description: tr("Wait for five messages matching the specified criteria")},
						{code: 10, description: tr("Wait for ten messages matching the specified criteria")}
					]
				}
			}) %>
			<%= _.template($('#input_constructor').html())({
				id: "interval",
				description: tr("Interval (seconds)"),
				default_selector: "int",
				disable_string: true,
				value_number: 5,
				min_number: 1,
				max_number: 999999,
				help: {
					description: tr("Interval in seconds to check existence messages matching the specified criteria."),
					examples: [
						{code: 2, description: tr("Check every 2 seconds")},
						{code: 5, description: tr("Check every 5 seconds")},
						{code: 10, description: tr("Check every 10 seconds")}
					]
				}
			}) %>
			<%= _.template($('#input_constructor').html())({
				id: "timeout",
				description: tr("Timeout (seconds)"),
				default_selector: "int",
				disable_string: true,
				value_number: 300,
				min_number: 1,
				max_number: 999999,
				help: {
					description: tr("Maximum waiting time for messages in seconds. If the specified time expires and the messages are not found, then the action will end with an error."),
					examples: [
						{code: 300, description: tr("Wait 5 minutes")},
						{code: 600, description: tr("Wait 10 minutes")},
						{code: 900, description: tr("Wait 15 minutes")},
						{code: 1200, description: tr("Wait 20 minutes")}
					]
				}
			}) %>
		</span>
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
		<span data-preserve="true" data-preserve-type="check" data-preserve-id="delAfter">
			<input type="checkbox" id="delAfter" style="margin-left:25px"/> <label for="delAfter" class="tr">Delete message after receiving</label> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title: tr("Delete message after receiving"), description: tr("Optional parameter.") + " " + tr("If activated, after successfully receiving the message will be deleted from the mailbox. When this parameter is enabled, the \"Set flags after receiving\" parameter is ignored."), examples: [{code: tr("Activated"), description: tr("Delete message after receiving")}, {code: tr("Deactivated"), description: tr("Don't delete message after receiving")}]})) %>"></i>
		</span>
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
		<span data-preserve="true" data-preserve-type="check" data-preserve-id="setFlagsAfter">
			<input type="checkbox" id="setFlagsAfter" style="margin-left:25px"/> <label for="setFlagsAfter" class="tr">Set flags after receiving</label> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title: tr("Set flags after receiving"), description: tr("Optional parameter.") + " " + tr("If activated, after successfully receiving the message, one or more of the specified flags will be set for it. This parameter is ignored if the \"Delete message after receiving\" parameter is enabled."), examples: [{code: tr("Activated"), description: tr("Set message flags after receiving")}, {code: tr("Deactivated"), description: tr("Don't set message flags after receiving")}]})) %>"></i>
		</span>
		<span id="setFlagsSettings">
			<%= _.template($('#input_constructor').html())({
				id: "setFlags",
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
					description: tr("List or one flag which needs to set for the message.") + " " + tr("As a list, you can use a string consisting of flags, separated by commas.") + " " + tr("The possible flags may differ depending on the server implementation.") + "<br/><strong>\\Seen</strong> - " + tr("Message has been read") + "<br/><strong>\\Answered</strong> - " + tr("Message has been answered") + "<br/><strong>\\Flagged</strong> - " + tr("Message is \"flagged\" for urgent/special attention") + "<br/><strong>\\Deleted</strong> - " + tr("Message is marked for removal") + "<br/><strong>\\Draft</strong> - " + tr("Message has not completed composition (marked as a draft)"),
					examples: [
						{code: "\\Seen,\\Flagged"},
						{code: "\\Seen, \\Flagged"},
						{code: "[\"\\Seen\", \"\\Flagged\"]"}
					]
				}
			}) %>
		</span>
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
	<div class="tr tooltip-paragraph-first-fold">Find and get the content of the message by the specified criteria.</div>
	<div class="tr tooltip-paragraph-fold">In order to execute this action correctly you need to run "Configure receiving mail" action first.</div>
	<div class="tr tooltip-paragraph-fold">This action will get the last message that matches the specified criteria and store parts of it in the specified variables.</div>
	<div class="tr tooltip-paragraph-fold">Using the parameters from the "Filtration" block, you can filter messages at your discretion or leave them unfiltered.</div>
	<div class="tr tooltip-paragraph-fold">In the "Sorting" block, you can set the order and field for sorting.</div>
	<div class="tr tooltip-paragraph-fold">Filtering and sorting is performed on the side of the mail server.</div>
	<div class="tr tooltip-paragraph-fold">Sorting is not supported by all mail services, if sorting is enabled, but the service does not support it, the action will fail.</div>
	<div class="tr tooltip-paragraph-fold">Filtering and sorting are only available when connecting via imap, if the action is called when connecting via pop3 and filtering or sorting is active, then the action will immediately fail.</div>
	<div class="tr tooltip-paragraph-fold">Using the parameters from the "Parsing" block, you can choose which parts of the message will be retrieved and saved, and which ones will be skipped, thereby saving traffic and time by getting only what you need.</div>
	<div class="tr tooltip-paragraph-fold">This action can wait for an message for the specified time if you activate the corresponding parameter in the additional settings.</div>
	<div class="tr tooltip-paragraph-fold">This action can delete the message or set flags for it, after successful receipt, for this you need to use the corresponding parameters in the additional settings.</div>
	<div class="tr tooltip-paragraph-fold">In the additional settings, you can specify the name of the folder in which this action will be performed, otherwise the folder specified in the "Configure receiving mail" action will be used.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", use_timeout:true, visible:true}) %>
<script>
		
		$(document).ready(function(){
			setTimeout(function(){
				let switchField = function(e){
					let target = e.target ? $(e.target) : e;
					if(target.length){
						let id = target.prop("id");
						let disable = !target.is(':checked');
						let next = target.closest('span').next();
						if(id.startsWith('getLinks') || id.startsWith('setFlags') || id === 'wait'){
							if(disable){
								next.hide();
							}else{
								next.show();
							};
						}else{
							next.find('input').prop('disabled', disable);
						};
						if(id.startsWith('getText')){
							let children = next.children('div');
							if(disable){
								children.hide();
							}else{
								children.show();
							};
						};
					};
				};
				let ids = ['getUid','getFrom','getTo','getSubject','getTextHtml','getLinksTextHtml','getTextPlain','getLinksTextPlain','getTextRaw','getLinksTextRaw','getSize','getFlags','getDate','getAttachNames','getAttachments','getRawHeader','wait','setFlagsAfter'];
				ids.forEach(function(id){
					let ell = $('#' + id);
					if(ell.length){
						ell.on('change', switchField);
						switchField(ell);
					};
				});
			}, 0);
		});

</script>
