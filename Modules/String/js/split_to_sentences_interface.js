<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"string", description: tr("String"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("The string containing text to be split into sentences."),examples:[{code:tr("Just sample text. Test text, string")},{code:"Test text! Test text! Test text!"},{code:"string"}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "SENTENCES_LIST", help: {description: tr("Variable in which, after successful execution of the action, the list of sentences will be written."), examples:[{code:tr("[\"Just sample text.\", \"Test text, string\"]")},{code:"[\"Test text!\", \"Test text!\", \"Test text!\"]"},{code:"[\"string\"]"}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Split the string into sentences.</div>
	<div class="tr tooltip-paragraph-fold">This action will return a list containing the sentences obtained from the string.</div>
	<div class="tr tooltip-paragraph-fold">For example, if the string <code>"Just sample text. Test text, string"</code> is specified, then the action will return <code>["Just sample text.", "Test text, string"]</code>, and if the string <code>"Test text! Test text! Test text!"</code> is specified, then the action will return <code>["Test text!", "Test text!", "Test text!"]</code>.</div>
	<div class="tr tooltip-paragraph-last-fold">The resulting list can be processed using actions from the "List" module.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
