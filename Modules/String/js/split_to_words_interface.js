<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"string", description: tr("String"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("The string containing a sentence to be split into words."),examples:[{code:tr("Just sample text")},{code:tr("Test text")},{code:"string"}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "WORDS_LIST", help: {description: tr("Variable in which, after successful execution of the action, the list of words will be written."), examples:[{code:tr("[\"Just\", \"sample\", \"text\"]")},{code:tr("[\"Test\", \"text\"]")},{code:"[\"string\"]"}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Split a string into words.</div>
	<div class="tr tooltip-paragraph-fold">This action will return a list containing the words obtained from the string.</div>
	<div class="tr tooltip-paragraph-fold">For example, if the string <code>"Test text"</code> is specified, then the action will return <code>["Test","text"]</code>, and if the string <code>"Just example string."</code> is specified, then the action will return <code>["Just","example","string"]</code>.</div>
	<div class="tr tooltip-paragraph-fold">This action ignores any characters other than letters and numbers.</div>
	<div class="tr tooltip-paragraph-fold">For example, if the string <code>"Delicious 100% juice."</code> is specified, then the action will return <code>["Delicious", "100", "juice"]</code>.</div>
	<div class="tr tooltip-paragraph-last-fold">The resulting list can be processed using actions from the "List" module.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
