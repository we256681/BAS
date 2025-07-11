<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({
		id: "service",
		description: tr("Service"),
		default_selector: "string",
		variants: [
			"sms-activate.org<br/><span style='color:gray'>" + tr("Ex sms-activate.ru") + "</span>",
			"smshub.org",
			"5sim.net",
			"onlinesim.ru",
			"sms-acktiwator.ru",
			"sms-reg.com",
			"vak-sms.com",
			"give-sms.com",
			"getsms.online",
			"sms-man.ru",
			"activation.pw",
			"cheapsms.ru",
			"smsvk.net",
			"smscode.me<br/><span style='color:gray'>" + tr("Ex sms.kopeechka.store") + "</span>",
			"365sms.org<br/><span style='color:gray'>" + tr("Ex 365sms.ru") + "</span>",
			"smspva.com",
			"simsms.org"
		],
		disable_int: true,
		value_string: "sms-activate.org",
		help: {
			description: tr("SMS receiving service for which need to get a balance."),
			examples: [
				{code: "sms-activate.org", description: "https://sms-activate.org"},
				{code: "smshub.org", description: "https://smshub.org"},
				{code: "sms-reg.com", description: "https://sms-reg.com"}
			]
		}
	}) %>
	<%= _.template($('#input_constructor').html())({
		id: "apiKey",
		description: tr("API key"),
		default_selector: "string",
		disable_int: true,
		value_string: "",
		help: {
			description: tr("API key of the SMS receiving service. The key for the service selected in the \"Service\" parameter. Depending on the service, you can get it in your personal account or in the service settings."),
			examples: [
				{code: "8b1a9953c4611296a827abf8c47804d7"},
				{code: "79916U5718g2266a7bff7fad356c6cb280b3ea"},
				{code: "f4d559ba78aa6c4701c1995ae9977c03"}
			]
		}
	}) %>
	<%= _.template($('#block_start').html())({id:"Additional", name: tr("Additional settings"), description: ""}) %>
		<%= _.template($('#input_constructor').html())({
			id: "serverUrl",
			description: tr("Server url"),
			default_selector: "string",
			disable_int: true,
			value_string: "",
			help: {
				description: tr("Optional parameter.") + " " + tr("Url of the SMS receiving service server. Use this parameter to specify the url of the server, if the required service is not in the list of available ones, but it works through an API similar to the selected service."),
				examples: [
					{code: "https://sms.org"},
					{code: "http://receive-sms.com"},
					{code: "http://127.0.0.1:8888"},
					{code: tr("Empty string"), description: tr("Use default server url, https://api.sms-activate.org for sms-activate.org, etc")}
				]
			}
		}) %>
	<%= _.template($('#block_end').html())() %>
	<%= _.template($('#variable_constructor').html())({
		id: "Save",
		description: tr("Variable to save the result"),
		default_variable: "SMS_BALANCE",
		help: {
			description:tr("Variable in which, after successful execution of the action, the balance will be written."),
			examples: [
				{code: 1.32},
				{code: 120},
				{code: 1596},
				{code: "null", description: tr("Failed to get balance")}
			]
		}
	}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get the balance of the SMS receiving service.</div>
	<div class="tr tooltip-paragraph-fold">This action will return a number equal to the balance of the specified SMS receiving service. If failed to get the balance, then the action will return <code>null</code>.</div>
	<div class="tr tooltip-paragraph-fold">If the required service is not in the list of available ones, but it works through an API similar to the selected service, then you can specify its server url in the corresponding parameter located in the additional settings.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", use_timeout:true, visible:true}) %>