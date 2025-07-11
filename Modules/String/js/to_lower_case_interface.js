<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"string", description: tr("String"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("The string to be converted to lowercase."),examples:[{code:tr("Just sample text")},{code:"Test Text"},{code:"STRING"}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "LOWERCASE_STRING", help: {description: tr("Variable in which, after successful execution of the action, the converted string will be written."), examples:[{code:tr("just sample text")},{code:"test text"},{code:"string"}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Convert all letters of the string to lowercase.</div>
	<div class="tr tooltip-paragraph-fold">This action will return a string in which all letters will be lowercase, and the rest of the characters will remain unchanged.</div>
	<div class="tr tooltip-paragraph-fold">For example, the string <code>"TEST"</code> will be converted to the string <code>"test"</code>, and the string <code>"Example String"</code> will be converted to the string <code>"example string"</code>.</div>
	<div class="tr tooltip-paragraph-last-fold">If you need to convert the first letter to uppercase, and the rest to lowercase, then use the action "Capitalize word".</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
