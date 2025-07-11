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
		default_variable: "MAIL_ID",
		help: {
			description: tr("Variable in which, after successful execution of the action, the id of the found message will be written."),
			examples: [
				{code: 159},
				{code: 273},
				{code: 587}
			]
		}
	}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Find and save the message id by the specified criteria.</div>
	<div class="tr tooltip-paragraph-fold">In order to execute this action correctly you need to run "Configure receiving mail" action first.</div>
	<div class="tr tooltip-paragraph-fold">This action will return the id of the message found by the specified criteria.</div>
	<div class="tr tooltip-paragraph-fold">The received id can be used in other module actions, for example, to receive a message, set flags and delete.</div>
	<div class="tr tooltip-paragraph-fold">Using the parameters from the "Filtration" block, you can filter messages at your discretion or leave them unfiltered.</div>
	<div class="tr tooltip-paragraph-fold">In the "Sorting" block, you can set the order and field for sorting.</div>
	<div class="tr tooltip-paragraph-fold">Filtering and sorting is performed on the side of the mail server.</div>
	<div class="tr tooltip-paragraph-fold">Sorting is not supported by all mail services, if sorting is enabled, but the service does not support it, the action will fail.</div>
	<div class="tr tooltip-paragraph-fold">Filtering and sorting are only available when connecting via imap, if the action is called when connecting via pop3 and filtering or sorting is active, then the action will immediately fail.</div>
	<div class="tr tooltip-paragraph-fold">This action can wait for an message for the specified time if you activate the corresponding parameter in the additional settings.</div>
	<div class="tr tooltip-paragraph-fold">In the additional settings, you can specify the name of the folder in which this action will be performed, otherwise the folder specified in the "Configure receiving mail" action will be used.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", use_timeout:true, visible:true}) %>
<script>
		
		function setVisibleWait(){
			if($('#wait').is(':checked')){
				$('#waitSettings').show();
			}else{
				$('#waitSettings').hide();
			};
		};
		
		$(document).ready(function(){
			setTimeout(setVisibleWait, 0);
        });
		
        $('#wait').on('change', setVisibleWait);

</script>
