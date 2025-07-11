<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"string", description: tr("String"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("The string containing text to be split into lines."),examples:[{code:tr("Just<br>sample<br>text")},{code:"<br>Test:text"},{code:"<br>string"}]} }) %>
<%= _.template($('#input_constructor').html())({id:"separator", description: tr("Separator"), default_selector: "expression", value_string: "/\\r?\\n/", variants: [{value:"&#034;\\n&#034;",type:"expression",description:tr("Line break, split multiline text")},{value:"&#034;\\r\\n&#034;",type:"expression",description:tr("Carriage return and line break, split multiline text")},{value:"/\\r?\\n/",type:"expression",description:tr("Line break with or without carriage return, split multiline text")}], disable_int:true, help: {description: tr("Optional parameter.") + " " + tr("String or regular expression used as the delimiter for the original string.") + " " + tr("If the separator does not occur in the original string, then the resulting list will contain one element, the original string."),examples:[{code:":",description:"[\"Test\", \"text\"]"},{code:'"\\n"',description:tr("[\"Just\", \"sample\", \"text\"]")},{code:tr("Empty string"), description: tr("Split the original string character by character, each character will be a separate item in the list.")}]} }) %>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check">
	<input type="checkbox" checked="checked" id="Check" style="margin-left:25px"/> <label for="Check" class="tr">Remove empty lines</label>
</span>
<%= _.template($('#block_start').html())({id:"Additional", name: tr("Additional settings"), description: ""}) %>
<%= _.template($('#input_constructor').html())({id:"limit", description:tr("Limit"), default_selector: "int", disable_string:true, value_number: "", min_number:0, max_number:999999, help: {description: tr("Optional parameter.") + " " + tr("An integer specifying the maximum number of items in the resulting list. The string will still be split completely, but the final list will be truncated so that its length does not exceed the specified limit."),examples:[{code:10, description:tr("The resulting list can contain a maximum of 10 items.")},{code:tr("Empty string"), description:tr("Don't limit the number of items on the resulting list.")}]} }) %>
<%= _.template($('#block_end').html())() %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "LINES_LIST", help: {description: tr("Variable in which, after successful execution of the action, the list of strings will be written."), examples:[{code:tr("[\"Just\", \"sample\", \"text\"]")},{code:"[\"Test\", \"text\"]"},{code:"[\"string\"]"}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Split string into lines.</div>
	<div class="tr tooltip-paragraph-fold">This action will return a list containing the lines obtained from the original string as a result of splitting at the specified separator.</div>
	<div class="tr tooltip-paragraph-fold">For example, if the specified string is <code>"Test:text"</code> and the separator is <code>":"</code>, then the action will return <code>["Test","text"]</code>.</div>
	<div class="tr tooltip-paragraph-fold">For example, if the specified string is <br><code>"Just<br>example<br>string."</code> and the separator is <code>"\n"</code>, then the action will return <code>["Just","example","string"]</code>.</div>
	<div class="tr tooltip-paragraph-fold">If the separator does not occur in the original string, then the resulting list will contain one element, the original string.</div>
	<div class="tr tooltip-paragraph-fold">For example, if the specified string is <code>"string"</code> and the separator is <code>";"</code>, then the action will return <code>["string"]</code>.</div>
	<div class="tr tooltip-paragraph-fold">If separator is not specified, the string will be split character by character, each character will be a separate item of the list.</div>
	<div class="tr tooltip-paragraph-fold">For example, if the specified string is <code>"string"</code> and the separator is not specified, then the action will return <code>["s","t","r","i","n","g"]</code>.</div>
	<div class="tr tooltip-paragraph-fold">If the "Remove empty lines" parameter is activated, then all empty lines will be removed from the resulting list.</div>
	<div class="tr tooltip-paragraph-last-fold">The resulting list can be processed using actions from the "List" module.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
