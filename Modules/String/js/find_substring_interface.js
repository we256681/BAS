<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"string", description: tr("String"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("The string in which to find the substring."),examples:[{code:tr("Just sample text")},{code:"Test text"},{code:"string"}]} }) %>
<%= _.template($('#input_constructor').html())({id:"substring", description: tr("Substring"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("The substring to find in the string."),examples:[{code:tr("sample")},{code:"text"},{code:"str"}]} }) %>
<%= _.template($('#block_start').html())({id:"Additional", name: tr("Additional settings"), description: ""}) %>
<%= _.template($('#input_constructor').html())({id:"from", description:tr("From index"), default_selector: "int", disable_string:true, value_number: "", min_number:0, max_number:999999, help: {description: tr("Optional parameter.") + " " + tr("The character index at which to start the search."),examples:[{code:1, description:tr("From the second character")},{code:0, description:tr("From the first character")},{code:tr("Empty string"), description:tr("From the beginning of the string")}]} }) %>
<%= _.template($('#block_end').html())() %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "SUBSTRING_INDEX", help: {description: tr("Variable in which, after successful execution of the action, the index of the beginning of the substring will be written."), examples:[{code:0},{code:3},{code:5},{code:-1,description: tr("String does not contain the specified substring")}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Find a substring in a string.</div>
	<div class="tr tooltip-paragraph-fold">This action will return a number equal to the index of the beginning of the first substring found, or -1 if the substring was not found.</div>
	<div class="tr tooltip-paragraph-fold">For example, if the string <code>"Test text"</code> is specified and the substring <code>"text"</code> is specified, then the action will return 5, and if the string is <code>"Just example string"</code> and the substring <code>"test"</code>, then the action will return -1.</div>
	<div class="tr tooltip-paragraph-last-fold">By default, the search is performed from the beginning of the string, but this can be changed by specifying the index from which to start the search in the "From index" parameter located in the additional settings.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
