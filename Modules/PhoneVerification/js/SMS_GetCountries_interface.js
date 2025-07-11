<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({
		id: "service",
		description: tr("Service"),
		default_selector: "string",
		variants: [
			"sms-activate.org<br/><span style='color:gray'>" + tr("Ex sms-activate.ru") + "</span>",
			"sms-acktiwator.ru",
			"sms-man.ru"
		],
		disable_int: true,
		value_string: "sms-activate.org",
		help: {
			description: tr("SMS receiving service for which need to get a list of countries."),
			examples: [
				{code: "sms-activate.org", description: "https://sms-activate.org"},
				{code: "sms-acktiwator.ru", description: "https://sms-acktiwator.ru"},
				{code: "sms-man.ru", description: "https://sms-man.ru"}
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
		default_variable: "SMS_COUNTRIES_LIST",
		help: {
			description:tr("Variable in which, after successful execution of the action, the list of countries will be written."),
			examples: [
				{code: "[{\"id\":\"0\",\"name\":\"Россия\",\"name_en\":\"Russia\"},{\"id\":\"1\",\"name\":\"Украина\",\"name_en\":\"Ukraine\"}]"}
			]
		}
	}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get list of countries of the SMS receiving service.</div>
	<div class="tr tooltip-paragraph-fold">This action will return a list consisting of objects containing identifiers and names of countries supported by the specified SMS receiving service. The country identifier is contained in the <code>id</code> property of the object, the country name in Russian is in the <code>name</code> property, and the country name in English is in the <code>name_en</code> property.</div>
	<div class="tooltip-paragraph-fold"><span class="tr">Example</span>: <code>[{"id":"0","name":"Россия","name_en":"Russia"},{"id":"1","name":"Украина","name_en":"Ukraine"}]</code></div>
	<div class="tr tooltip-paragraph-fold">Some services may not have a country name in English or have a name in English in the <code>name</code> property and have no name in Russian at all.</div>
	<div class="tr tooltip-paragraph-fold">The resulting list can be processed using actions from the "JSON" module.</div>
	<div class="tr tooltip-paragraph-fold">To get the Russian name of the first country, use the JPath query <span style="color:black">$.[0].name</span> in the "Get value" action from the "JSON" module.</div>
	<div class="tr tooltip-paragraph-fold">To get the English name of the first country, use the JPath query <span style="color:black">$.[0].name_en</span> in the "Get value" action from the "JSON" module.</div>
	<div class="tr tooltip-paragraph-fold">To get a list of Russian names of all countries, use the JPath query <span style="color:black">$.[*].name</span> in the "Get all values" action from the "JSON" module.</div>
	<div class="tr tooltip-paragraph-fold">To get a list of English names of all countries, use the JPath query <span style="color:black">$.[*].name_en</span> in the "Get all values" action from the "JSON" module.</div>
	<div class="tr tooltip-paragraph-fold">To get the id of the first country, use the JPath query <span style="color:black">$.[0].id</span> in the "Get value" action from the "JSON" module.</div>
	<div class="tr tooltip-paragraph-fold">To get the id of the country with the Russian name from the [[NAME]] variable, use the JPath query <span style="color:black">$.[?(@.name=="[[NAME]]")].id</span> in the "Get value" action from the "JSON" module.</div>
	<div class="tr tooltip-paragraph-fold">To get the id of the country with the English name from the [[NAME]] variable, use the JPath query <span style="color:black">$.[?(@.name_en=="[[NAME]]")].id</span> in the "Get value" action from the "JSON" module.</div>
	<div class="tr tooltip-paragraph-fold">The resulting country id can be used in the "Custom country" parameter of the "Get the count of available numbers" and "Get phone number" actions.</div>
	<div class="tr tooltip-paragraph-fold">If the required service is not in the list of available ones, but it works through an API similar to the selected service, then you can specify its server url in the corresponding parameter located in the additional settings.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", use_timeout:true, visible:true}) %>