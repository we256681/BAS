<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({
		id: "number",
		description: tr("Number"),
		default_selector: "string",
		disable_int: true,
		value_string: "",
		help: {
			description:tr("The number obtained from the \"Get phone number\" action, for which need to change the activation status."),
			examples: [
				{code: "79001112323"},
				{code: "17039688838"},
				{code: "380048698566"}
			]
		}
	}) %>
	<%= _.template($('#input_constructor').html())({
		id: "status",
		description: tr("Stаtus"),
		default_selector: "string",
		disable_int: true,
		value_string: "", 
		variants: [
			"-1<br/><span style='color:gray'>" + tr("Cancel activation.") + "</span>",
			"1<br/><span style='color:gray'>" + tr("Inform about the availability of the number.") + "</span>",
			"3<br/><span style='color:gray'>" + tr("Request another code.") + "</span>",
			"6<br/><span style='color:gray'>" + tr("Complete activation.") + "</span>",
			"8<br/><span style='color:gray'>" + tr("Notify that the number is used and cancel the activation.") + "</span>",
		],
		help: {
			description: tr("The status to be set for the specified number."),
			examples: [
				{code: "-1", description: tr("Cancel activation.")},
				{code: "1", description: tr("Inform about the availability of the number.")},
				{code: "3", description: tr("Request another code.")},
				{code: "6", description: tr("Complete activation.")},
				{code: "8", description: tr("Notify that the number is used and cancel the activation.")}
			]
		}
	}) %>
	<%= _.template($('#block_start').html())({id:"Additional", name: tr("Additional settings"), description: ""}) %>
		<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check">
			<input type="checkbox" id="Check" checked="checked" style="margin-left:25px"/> <label for="Check" class="tr">Delete information about the number when the status is -1, 6 or 8.</label>
		</span>
	<%= _.template($('#block_end').html())() %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Change the activation status for the specified number.</div>
	<div class="tr tooltip-paragraph-fold">Before performing this action, you first need to get a number using the "Get phone number" action and use it unchanged in the "Number" parameter.</div>
	<div class="tr tooltip-paragraph-fold">This action changes the activation status for the specified number on the SMS receiving service.</div>
	<div class="tr tooltip-paragraph-fold">Possible statuses:</div>
	<div class="tr tooltip-paragraph-fold"><code>-1</code> - cancel activation, if the number did not suit you or was received by accident.</div>
	<div class="tr tooltip-paragraph-fold"><code>1</code> - inform about the readiness of the number, if the SMS has already been sent, the "Get activation code" action sets this status automatically, therefore it is not necessary to set this status.</div>
	<div class="tr tooltip-paragraph-fold"><code>3</code> - request another code, if you need to receive another SMS code to the same number. This status must be set before each reuse of the "Get activation code" action.</div>
	<div class="tooltip-paragraph-fold"><span class="tr"><code>&lt;Attention!&gt;</code></span><span class="tr">Depending on the service, receiving a repeated SMS code may be paid, unavailable or limited in number, information about this should be found in the description of the service or its API.</span><span class="tr"><code>&lt;Attention!&gt;</code></span></div>
	<div class="tr tooltip-paragraph-fold"><code>6</code> - complete activation, if the correct SMS code has been received and you want to complete the work with the specified number.</div>
	<div class="tr tooltip-paragraph-fold"><code>8</code> - inform that the number has been used and cancel activation if the specified number is busy or blocked on the desired site.</div>
	<div class="tr tooltip-paragraph-fold">By default, the information about the number is deleted when the statuses -1, 6 and 8 are successfully set, but this can be disabled in the additional settings.</div>
	<div class="tr tooltip-paragraph-fold">If the service does not support the specified status, then the action will do nothing.</div>
	<div class="tr tooltip-paragraph-last-fold">If the status cannot be changed or an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", use_timeout:true, visible:true}) %>