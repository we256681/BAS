<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"string", description: tr("String"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("The string from which to get the number of substrings."),examples:[{code:tr("Just sample text")},{code:"Test text"},{code:"string"}]} }) %>
<%= _.template($('#input_constructor').html())({id:"substring", description: tr("Substring"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("Contiguous sequence of characters within a string."),examples:[{code:tr("sample")},{code:"text"},{code:"str"}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "SUBSTRINGS_COUNT", help: {description: tr("Variable in which, after successful execution of the action, the number of substrings in the string will be written."), examples:[{code:5},{code:2},{code:0,description:tr("The substring is missing from the string.")}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get the number of substrings in the string.</div>
	<div class="tr tooltip-paragraph-fold">This action will return a number equal to the number of substrings in the string.</div>
	<div class="tr tooltip-paragraph-last-fold">For example, if the string <code>"test text"</code> is specified and the substring <code>"te"</code> is specified, then the action will return 2, and if the string is <code>"Just example string"</code> and the substring <code>"example"</code>, then the action will return 1.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
