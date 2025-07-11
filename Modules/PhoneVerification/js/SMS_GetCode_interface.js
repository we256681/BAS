<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({
		id: "number",
		description: tr("Number"),
		default_selector: "string",
		disable_int: true,
		value_string: "",
		help: {
			description:tr("The number obtained from the \"Get phone number\" action, for which need to receive an SMS code."),
			examples: [
				{code: "79001112323"},
				{code: "17039688838"},
				{code: "380048698566"}
			]
		}
	}) %>
	<%= _.template($('#block_start').html())({id:"Additional", name: tr("Additional settings"), description: ""}) %>
		<%= _.template($('#input_constructor').html())({
			id: "interval",
			description: tr("Interval (seconds)"),
			default_selector: "int",
			disable_string: true,
			value_number: 5,
			min_number: 1,
			max_number: 999999,
			help: {
				description: tr("The code readiness check interval in seconds."),
				examples: [
					{code: 2, description: tr("Check every 2 seconds")},
					{code: 5, description: tr("Check every 5 seconds")},
					{code: 10, description: tr("Check every 10 seconds")}
				]
			}
		}) %>
		<%= _.template($('#input_constructor').html())({
			id: "timeout",
			description: tr("Timeout (minutes)"),
			default_selector: "int",
			disable_string: true,
			value_number: 10,
			min_number: 1,
			max_number: 999999,
			help: {
				description: tr("Maximum waiting time for SMS code in minutes. If the specified time expires and the SMS is not received, then the action will end with an error."),
				examples: [
					{code: 10, description: tr("Wait 10 minutes")},
					{code: 15, description: tr("Wait 15 minutes")},
					{code: 20, description: tr("Wait 20 minutes")}
				]
			}
		}) %>
	<%= _.template($('#block_end').html())() %>
	<%= _.template($('#variable_constructor').html())({
		id: "Save",
		description: tr("Variable to save the result"),
		default_variable: "SMS_CODE",
		help: {
			description:tr("Variable in which, after successful execution of the action, he SMS code will be written."),
			examples: [
				{code: "75588"},
				{code: "98292"},
				{code: "558925"},
				{code: tr("Activation code: 75588"), description: tr("The service failed to get the code from the SMS, a full message was returned")}
			]
		}
	}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get an activation code from the specified number.</div>
	<div class="tr tooltip-paragraph-fold">Before performing this action, you first need to get a number using the "Get phone number" action and use it unchanged in the "Number" parameter.</div>
	<div class="tr tooltip-paragraph-fold">This action will return a string containing the code from the SMS message or the full message if the service was unable to get the code from it.</div>
	<div class="tr tooltip-paragraph-fold">For example, if the service managed to get the code from SMS, then the action will return <code>"75588"</code>, and if the service failed to get the code from SMS, then the action will return <code>"Activation code: 75588"</code>.</div>
	<div class="tr tooltip-paragraph-fold">To get another code to the same number, set status <code>3</code> by the action "Change activation status" and call this action again. You must set the status <code>3</code> before each new receipt of the code to the same number.</div>
	<div class="tooltip-paragraph-fold"><span class="tr"><code>&lt;Attention!&gt;</code></span><span class="tr">Depending on the service, receiving a repeated SMS code may be paid, unavailable or limited in number, information about this should be found in the description of the service or its API.</span><span class="tr"><code>&lt;Attention!&gt;</code></span></div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action or code is not received within timeout specified in additional settings, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>