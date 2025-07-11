<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"string", description: tr("String"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("The string in which to convert words."),examples:[{code:tr("just sample text")},{code:"test text"},{code:"string"}]} }) %>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check">
	<div><input type="checkbox" id="Check"/> <label for="Check" class="tr">Capitalize all words</label></div>
</span>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check2">
	<div><input type="checkbox" id="Check2"/> <label for="Check2" class="tr">All other letters in lowercase</label></div>
</span>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "RESULT_STRING", help: {description: tr("Variable in which, after successful execution of the action, the converted string will be written."), examples:[{code:tr("Just sample text")},{code:"Test Text"},{code:"String"}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Convert the first letter of a word to uppercase.</div>
	<div class="tr tooltip-paragraph-fold">This action will return a string in which the first letter will be uppercase and the rest will remain unchanged.</div>
	<div class="tr tooltip-paragraph-fold">For example, if the string <code>"test text"</code> is specified, then the action will return <code>"Test text"</code>, and if the string <code>"just example string"</code> is specified, then the action will return <code>"Just example string"</code>.</div>
	<div class="tr tooltip-paragraph-fold">If the "Capitalize all words" parameter is enabled, the first letters in all words will be uppercase.</div>
	<div class="tr tooltip-paragraph-fold">For example, if the string <code>"test text"</code> is specified, then the action will return <code>"Test Text"</code>, and if the string <code>"just example string"</code> is specified, then the action will return <code>"Just Example String"</code>.</div>
	<div class="tr tooltip-paragraph-fold">If the "All other letters in lowercase" parameter is enabled, then all letters that were not converted to uppercase will be converted to lowercase.</div>
	<div class="tr tooltip-paragraph-fold">For example, if the string <code>"TEST TEXT"</code> is specified, then the action will return <code>"Test text"</code>, and if the string <code>"JUST EXAMPLE STRING"</code> is specified, then the action will return <code>"Just example string"</code>.</div>
	<div class="tr tooltip-paragraph-fold">If both "Capitalize all words" and "All other letters in lowercase" are both enabled, the first letters in all words will be uppercase and all other lowercase.</div>
	<div class="tr tooltip-paragraph-last-fold">For example, if the string <code>"TEST TEXT"</code> is specified, then the action will return <code>"Test Text"</code>, and if the string <code>"JUST EXAMPLE STRING"</code> is specified, then the action will return <code>"Just Example String"</code>.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
