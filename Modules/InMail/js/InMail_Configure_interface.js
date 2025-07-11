<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({
		id: "protocol",
		description: tr("Protocol"),
		default_selector: "string",
		variants: [
			{value: "imap", description: "Internet Message Access Protocol"},
			{value: "pop3", description: "Post Office Protocol Version 3"}
			
		],
		disable_int: true,
		value_string: "imap",
		help: {
			description: tr("The protocol by which the connection to the mail server will be made."),
			examples: [
				{code: "imap", description: "Internet Message Access Protocol"},
				{code: "pop3", description: "Post Office Protocol Version 3"}
			]
		}
	}) %>
	<%= _.template($('#input_constructor').html())({
		id: "autoConfig",
		description: tr("Auto configuration"),
		default_selector: "string",
		variants: [
			{value: "true", description: tr("Automatically configure connection")},
			{value: "false", description: tr("Specify connection settings manually")}
		],
		disable_int: true,
		value_string: "false",
		help: {
			description: tr("If the value is <code>true</code> and the username is an email address along with a domain, then the action will try to automatically determine the server address, port and encryption type.") + " " + tr("To automatically determine the settings, a pre-prepared list is used, if the settings cannot be determined, the action will fail."),
			examples: [
				{code: "true", description: tr("Automatically configure connection")},
				{code: "false", description: tr("Specify connection settings manually")}
			]
		}
	}) %>
	<span id="advancedSettings">
		<%= _.template($('#input_constructor').html())({
			id: "host",
			description: tr("Server address"),
			default_selector: "string",
			disable_int: true,
			value_string: "",
			help: {
				description: tr("Address (URL or IP) of the mail server.") + " " + tr("The value of this parameter is ignored if auto configuration is enabled."),
				examples: [
					{code: "imap.site.com"},
					{code: "pop.example.com"},
					{code: "96.256.27.26"}
				]
			}
		}) %>
		<%= _.template($('#input_constructor').html())({
			id: "port",
			description: tr("Port"),
			variants: [
				{value: "auto", description: tr("Automatically detect port based on protocol and encryption type")},
				{value: "993", description: tr("Default port for imap protocol with SSL encryption")},
				{value: "995", description: tr("Default port for pop3 protocol with SSL encryption")},
				{value: "143", description: tr("Default port for imap protocol without encryption or with STARTTLS encryption")},
				{value: "110", description: tr("Default port for pop3 protocol without encryption or with STARTTLS encryption")}
			],
			default_selector: "string",
			value_string: "auto",
			help: {
				description: tr("Port number of the mail server.") + " " + tr("The value of this parameter is ignored if auto configuration is enabled."),
				examples: [
					{code: "auto", description: tr("Automatically detect port based on protocol and encryption type")},
					{code: "993", description: tr("Default port for imap protocol with SSL encryption")},
					{code: "143", description: tr("Default port for imap protocol without encryption or with STARTTLS encryption")},
					{code: "995", description: tr("Default port for pop3 protocol with SSL encryption")},
					{code: "110", description: tr("Default port for pop3 protocol without encryption or with STARTTLS encryption")}
				]
			}
		}) %>
		<%= _.template($('#input_constructor').html())({
			id: "encrypt",
			description: tr("Encryption"),
			default_selector: "string",
			variants: [
				{value: "none", description: tr("Without using encryption")},
				"SSL",
				"STARTTLS"
			],
			disable_int: true,
			value_string: "SSL",
			help: {
				description: tr("Encryption type of connection to the mail server.") + " " + tr("The value of this parameter is ignored if auto configuration is enabled."),
				examples: [
					{code: "none", description: tr("Without using encryption")},
					{code: "SSL"},
					{code: "STARTTLS"}
				]
			}
		}) %>
	</span>
	<%= _.template($('#input_constructor').html())({
		id: "username",
		description: tr("Username") + ". " + tr("Can be blank"),
		default_selector: "string",
		disable_int: true,
		value_string: "",
		help: {
			description: tr("The username of the mail server, this is almost always the same as the email address, but may not include the mail domain.") + " " + tr("See the settings of the used protocol for your mail.") + " " + tr("With auto configuration enabled, this field should contain an email address along with the domain."),
			examples: [
				{code: "test@gmail.com"},
				{code: "test", description: tr("The username may also not include the mail domain.") + " " + tr("See the settings of the used protocol for your mail.")}
          ]

		}
	}) %>
	<%= _.template($('#input_constructor').html())({
		id: "password",
		description: tr("Password") + ". " + tr("Can be blank"),
		default_selector: "string",
		disable_int: true,
		value_string: "",
		help: {
			description: tr("The password of the mail server, it is almost always the same as the email password, but the mail service may have a separate password for access via imap and pop3 protocols.") + " " + tr("See the settings of the used protocol for your mail.")
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
			value_string: "INBOX",
			help: {
				description: tr("The name of the folder in which all actions will be performed if no other folder is specified in their additional settings.") + " " + tr("You can get a list of mailbox folders using the \"Get list of folders\" action."),
				examples: [
					{code: "INBOX", description: tr("Default folder incoming messages")},
					{code: "Spam", description: tr("Spam folder, on some mails")},
					{code: "Trash", description: tr("Trash folder, on some mails")}
				]
			}
		}) %>
		<%= _.template($('#input_constructor').html())({
			id: "connectTimeout",
			description: tr("Connection timeout (seconds)"),
			default_selector: "int",
			disable_string: true,
			value_number: 300,
			min_number: 0,
			max_number: 999999,
			help: {
				description: tr("The number of seconds after which the connection to the mail server will be considered unsuccessful.")
			}
		}) %>
		<%= _.template($('#input_constructor').html())({
			id: "resetTimeout",
			description: tr("Reset timeout (seconds)"),
			default_selector: "int",
			disable_string: true,
			value_number: 300,
			min_number: 0,
			max_number: 999999,
			help: {
				description: tr("This action won't start connection, it only sets configuration. Connection will be established after first attempt to access server. When action, which triggers connection will be finished, connection won't be closed immediately.") + " " + tr("It will be preserved for a time specified in this parameter.") + " " + tr("If another action will require access to server, connection will be reused.")
			}
		}) %>
	<%= _.template($('#block_end').html())() %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Configure access to the mail server to read email.</div>
	<div class="tr tooltip-paragraph-fold">This action must be called once before performing other actions that work with the mail server.</div>
	<div class="tr tooltip-paragraph-fold">Specify the protocol, imap or pop3, by which the connection to the mail server will be made in the "Protocol" parameter, please note that when using the pop3, most of the module functions will not be available. If the action is not available on pop3, then it will end with an appropriate error.</div>
	<div class="tr tooltip-paragraph-fold">Depending on the mail service, the connection may be available only via imap or only via pop3, or it may not be available at all via these protocols.</div>
	<div class="tr tooltip-paragraph-fold">You also need to fill server address, port, encryption type, your username and password. These can usually be found by searching for the imap/pop3 settings for your mail service, like this: "gmail imap settings".</div>
	<div class="tr tooltip-paragraph-fold">This action can automatically determine the server address, port and encryption type, for this you need to specify <code>true</code> in the "Auto configuration" parameter, and specify the email address along with the domain as the username.</div>
	<div class="tr tooltip-paragraph-fold">To automatically determine the settings, a pre-prepared list is used, if the settings cannot be determined, the action will fail.</div>
	<div class="tooltip-paragraph-fold"><span class="tr">For most mail services, you also need to allow imap/pop3 access in your account settings. Here are <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://support.google.com/mail/answer/7126229?hl=en');return false">settings</a> for gmail.</span> <span class="tr">Also, the mail service may have a separate password for connecting via imap/pop3.</span></div>
	<div class="tooltip-paragraph-fold"><span class="tr">Gmail also requires to</span> <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://myaccount.google.com/lesssecureapps');return false">allow insecure applications</a> <span class="tr">in order to work with BAS</span>.</div>
	<div class="tr tooltip-paragraph-fold">Mail.ru doesn't allow to make search in mailbox.</div>
	<div class="tr tooltip-paragraph-fold">Mailbox can contain several folders, and you can work with messages from a specific folder by changing "Folder name" parameter located in the additional settings. By default it is set to "INBOX", this folder contains incoming messages. Each mail service provides its own default names for different folders. For example, this is default name of gmail trash folder "[Gmail]/Trash" and spam folder is "[Gmail]/Spam". You can also create your own folders and use them in this action.</div>
	<div class="tr tooltip-paragraph-fold">You can get a list of mailbox folders using the "Get list of folders" action.</div>
	<div class="tooltip-paragraph-fold"><span class="tr">This action won't start connection, it only sets configuration. Connection will be established after first attempt to access server. When action, which triggers connection will be finished, connection won't be closed immediately.</span> <span class="tr">It will be preserved for a time specified in "Reset timeout" parameter located in the additional settings.</span> <span class="tr">If another action will require access to server, connection will be reused.</span></div>
	<div class="tr tooltip-paragraph-fold">You don't need to close connection explicitly, it will be closed automatically after timeout or when the script will stop.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
<script>
		
		$(document).ready(function(){
			setTimeout(function(){
				let setVisibleAdvanced = function(e){
					if($('#autoConfig').val() != "true"){
						$('#advancedSettings').show();
					}else{
						$('#advancedSettings').hide();
					};
				};
				
				$('#autoConfig').on('blur', setVisibleAdvanced);
				
				setVisibleAdvanced();
			}, 0);
		});

</script>
