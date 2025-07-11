<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"string", description: tr("String"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("The string from which to get the substring."),examples:[{code:tr("Just sample text")},{code:"Test text"},{code:"string"}]} }) %>
<%= _.template($('#input_constructor').html())({id:"left", description: tr("Left substring") + ". " + tr("Can be blank"), default_selector: "string", value_string: "", help: {description: tr("The substring to the left of the substring to get."),examples:[{code:"("},{code:"&lt;a&gt;"},{code:"&lt;p&gt;"},{code:"&lt;span&gt;"},{code:tr("Empty string"), description:tr("From the beginning of the string")}]} }) %>
<%= _.template($('#input_constructor').html())({id:"right", description: tr("Right substring") + ". " + tr("Can be blank"), default_selector: "string", value_string: "", help: {description: tr("The substring to the right of the substring to get."),examples:[{code:")"},{code:"&lt;/a&gt;"},{code:"&lt;/p&gt;"},{code:"&lt;/span&gt;"},{code:tr("Empty string"), description:tr("To the end of the string")}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "SUBSTRING", help: {description: tr("Variable in which, after successful execution of the action, the substring will be written."),examples:[{code:tr("sample")},{code:"text"},{code:"str"}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get a substring between two substrings.</div>
	<div class="tr tooltip-paragraph-fold">This action will return the received substring.</div>
	<div class="tr tooltip-paragraph-fold">For example, to get the substring <code>"Text title"</code> from the string <code>"&lt;h1&gt;Text title&lt;/h1&gt;"</code>, you need to specify <code>"&lt;h1&gt;"</code> in the "Left substring" parameter and specify <code>"&lt;/h1&gt;"</code> in the "Right substring" parameter.</div>
	<div class="tr tooltip-paragraph-fold">If the "Left substring" and "Right substring" parameters are not specified, the entire string will be received.</div>
	<div class="tr tooltip-paragraph-fold">If the "Left substring" parameter is specified and the "Right substring" parameter is not specified, all characters from the specified substring to the end of the string will be received.</div>
	<div class="tr tooltip-paragraph-fold">If the "Left substring" parameter is not specified and the "Right substring" parameter is specified, all characters from the first to the specified substring will be received.</div>
	<div class="tr tooltip-paragraph-last-fold">If both parameters "Left substring" and "Right substring" are specified, all characters from the specified substring to the specified substring will be received.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
