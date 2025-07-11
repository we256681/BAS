<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"string", description: tr("String"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("CSV string with want to parse into items."),examples:[{code:"login:password"},{code:"id,name,login"},{code:tr("Just sample text") + ";Test text;string"}]} }) %>
<%= _.template($('#input_constructor').html())({id:"separators", description:tr("List of separators"), default_selector: "string", disable_int:true, value_string: ":;,", help: {description: tr("The list of delimiters by which the string can be split.") + " " + tr("As a list, you can use a string consisting only of the separators."),examples:[{code:":;,"},{code:"[\":\", \";\", \",\"]"},{code:tr("Empty string"), description: "[\":\", \";\", \",\"]"}]} }) %>
<%= _.template($('#block_start').html())({id:"Additional", name: tr("Additional settings"), description: ""}) %>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check">
	<input type="checkbox" id="Check" style="margin-left:25px"/> <label for="Check" class="tr">Convert types</label>
</span>
<%= _.template($('#block_end').html())() %>
<%= _.template($('#variable_constructor').html())({id:"VariablesList", description:tr("Variables To Save Result"), default_variable: "USERNAME,PASSWORD", append_mode:true, help: {description: tr("List of variables separated by commas. Results will be written into that variables.")} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Parse CSV string into items.</div>
	<div class="tr tooltip-paragraph-fold">This action will parse the string by the specified separators and save the parsed data by the specified variables.</div>
	<div class="tr tooltip-paragraph-fold">For example, if the string <code>"email@gmail.com:mypass1"</code> and the variables <code>USERNAME,PASSWORD</code> are specified, then the action will save <code>"email@gmail.com"</code> to the variable <code>[[USERNAME]]</code> and <code>"mypass1"</code> to the variable <code>[[PASSWORD]]</code>.</div>
	<div class="tr tooltip-paragraph-fold">By default, the action returns strings, but this can be changed by enabling the "Convert types" parameter located in the additional settings, then strings will automatically be converted to numbers, true or false, objects, null and undefined.</div>
	<div class="tr tooltip-paragraph-fold">For example, if the string <code>"test:123:true"</code> is specified, then the string <code>"test"</code> will be saved in the first variable, and the number <code>123</code> in the second variable and in the third variable the boolean value <code>true</code>.</div>
	<div class="tr tooltip-paragraph-fold">If the number of variables is greater than the number of items in the string, then an empty string will be written to the excess variables.</div>
	<div class="tr tooltip-paragraph-last-fold">Use "Parse String" action if you want to get list as a result.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
