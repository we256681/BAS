<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"string", description: tr("String"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("The string from which to get the word count."),examples:[{code:tr("Just sample text")},{code:"Test text"},{code:"string"}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "WORDS_COUNT", help: {description: tr("Variable in which, after successful execution of the action, the number of words contained in the string will be written."), examples:[{code:5},{code:2},{code:0,description: tr("The string does not contain the words")}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get the number of words from a string.</div>
	<div class="tr tooltip-paragraph-fold">This action will return a number equal to the number of words in the string.</div>
	<div class="tr tooltip-paragraph-last-fold">For example, if the string <code>"Test text"</code> is specified, then the action will return 2, and if the string <code>"Just example string"</code> is specified, then the action will return 3.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
