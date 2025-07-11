<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"string", description: tr("String"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("The string from which to get the substring."),examples:[{code:tr("Just sample text")},{code:"Test text"},{code:"string"}]} }) %>
<%= _.template($('#input_constructor').html())({id:"from", description:tr("From index") + ". " + tr("Can be blank"), default_selector: "int", disable_string:true, value_number: "", min_number:-999999, max_number:999999, help: {description: tr("The index of the character at which to start getting the substring.") + " " + tr("If negative, indicates an offset from the end of the string."),examples:[{code:1, description:tr("From the second character")},{code:0, description:tr("From the first character")},{code:-1, description:tr("From the last character")},{code:tr("Empty string"), description:tr("From the beginning of the string")}]} }) %>
<%= _.template($('#input_constructor').html())({id:"to", description:tr("To index") + ". " + tr("Can be blank"), default_selector: "int", disable_string:true, value_number: "", min_number:-999999, max_number:999999, help: {description: tr("The index of the character at which the retrieval of the substring ends.") + " " + tr("If negative, indicates an offset from the end of the string."),examples:[{code:1, description:tr("To the second character")},{code:0, description:tr("To the first character")},{code:-1, description:tr("To the last character")},{code:tr("Empty string"), description:tr("To the end of the string")}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "SUBSTRING", help: {description: tr("Variable in which, after successful execution of the action, the substring will be written."),examples:[{code:tr("sample")},{code:"text"},{code:"str"}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get a substring from the string.</div>
	<div class="tr tooltip-paragraph-fold">This action will return the received substring.</div>
	<div class="tr tooltip-paragraph-fold">For example, to get the substring <code>"example"</code> from the string <code>"Just example string"</code>, you need to specify 5 in the "From index" parameter and specify 12 in the "To index" parameter.</div>
	<div class="tr tooltip-paragraph-fold">If the "From index" and "To index" parameters are not specified, the entire string will be received.</div>
	<div class="tr tooltip-paragraph-fold">If the "From index" parameter is specified and the "To index" parameter is not specified, all characters from the specified to the end of the string will be received.</div>
	<div class="tr tooltip-paragraph-fold">If the "From index" parameter is not specified and the "To index" parameter is specified, all characters from the first to the specified one will be received.</div>
	<div class="tr tooltip-paragraph-fold">If both parameters "From Index" and "To Index" are specified, all characters from specified to specified will be received.</div>
	<div class="tr tooltip-paragraph-last-fold">If the index is a negative number, then it is interpreted as the number of characters from the end of the string.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
