<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({id:"Value", description:tr("Original String"), default_selector: "string", disable_int:true, help: {description: tr("The string in which the replacement will be made")}}) %>
	<%= _.template($('#input_constructor').html())({id:"ReplaceFrom", description:tr("Replace From"), default_selector: "string", disable_int:true, help: {description: tr("The string to be searched for")}}) %>
	<%= _.template($('#input_constructor').html())({id:"ReplaceTo", description:tr("Replace To"), default_selector: "string", disable_int:true, help: {description: tr("New value")}}) %>
	<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable To Save Result"), default_variable: "REPLACE_RESULT", help: {description: tr("Variable, which will hold copy of the original string with the replacement")}}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Searches a string defined with "Original String" for a specified value defined with "Replace From", and returns a new string where the specified values are replaced to value defined with "Replace To" parameter.</div>
	<div class="tr tooltip-paragraph-fold">This action replaces all occurrences.</div>
	<div class="tr tooltip-paragraph-last-fold">If you want to replace with regexp, use "Replace text with regexp" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>