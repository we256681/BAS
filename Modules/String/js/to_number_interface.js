<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"string", description: tr("String"), default_selector: "string", value_string: "", help: {description: tr("The string to convert to a number."),examples:[{code:"123"},{code:"100,000"},{code:"1,456.578"}]} }) %>
<%= _.template($('#block_start').html())({id:"Additional", name: tr("Additional settings"), description: ""}) %>
<%= _.template($('#input_constructor').html())({id:"decimals", description:tr("Number of decimal places"), default_selector: "int", disable_string:true, value_number: "", min_number:-1, max_number:20, help: {description: tr("Optional parameter.") + " " + tr("The number of decimal places to which the number will be rounded."),examples:[{code:4, description:156.6439},{code:2, description:21.78},{code:0, description:tr("Round to whole number")},{code:-1, description:tr("Do not round the number")},{code:tr("Empty string"), description:tr("Do not round the number")}]} }) %>
<%= _.template($('#input_constructor').html())({id:"dec_point", description: tr("Decimal separator"), default_selector: "string", value_string: ".", help: {description: tr("Optional parameter.") + " " + tr("Separator between integer and decimal parts of a number."),examples:[{code:'"."', description:156.6439},{code:'","', description:"156,6439"},{code:tr("Empty string"), description:tr('"." - 156.6439')}]} }) %>
<%= _.template($('#input_constructor').html())({id:"thousands_sep", description: tr("Thousandth separator"), default_selector: "string", value_string: ",", help: {description: tr("Optional parameter.") + " " + tr("Separator between thousandths."),examples:[{code:'" "', description:"1 000 000.01"},{code:'","', description:"1,000,000.01"},{code:tr("Empty string"), description:tr('"," - 1,000,000.01')}]} }) %>
<%= _.template($('#block_end').html())() %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "RESULT_NUMBER", help: {description: tr("Variable in which, after successful execution of the action, the numeric representation of the specified string will be written."),examples:[{code:123},{code:100000},{code:1456.578}]}}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Convert the specified string to a number.</div>
	<div class="tr tooltip-paragraph-fold">This action will return the numeric representation of the specified string.</div>
	<div class="tr tooltip-paragraph-fold">For example, if <code>"123"</code> is specified, then the action will return <code>123</code>, and if <code>"123.45"</code> is specified, then the action will return <code>123.45</code>.</div>
	<div class="tr tooltip-paragraph-fold">The final number can be rounded off by specifying the "Number of decimal places" parameter located in the additional settings.</div>
	<div class="tr tooltip-paragraph-fold">This action can convert numbers in a non-standard format for this, you need to specify the "Decimal separator" and "Thousandth separator" parameters located in the additional settings.</div>
	<div class="tr tooltip-paragraph-last-fold">For example, the string <code>"1 967 567.3245"</code> will be converted to the number <code>1967567.3245</code> if a comma is specified in the "Decimal separator" parameter, and a space is specified in the "Thousandth separator" parameter.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
