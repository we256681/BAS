<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"string", description: tr("String"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("The string which needs to be trimmed."),examples:[{code:"\"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" + tr("Just sample text") + "&nbsp;&nbsp;&nbsp;&nbsp;\""},{code:"\"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Test text&nbsp;&nbsp;&nbsp;\""},{code:"\"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;string&nbsp;&nbsp;&nbsp;&nbsp;\""}]} }) %>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check">
	<div><input type="checkbox" id="Check" checked="checked" style="margin-left:25px"/> <label for="Check" class="tr">Trim spaces</label></div>
</span>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check2">
	<div><input type="checkbox" id="Check2" checked="checked" style="margin-left:25px"/> <label for="Check2" class="tr">Trim line breaks</label></div>
</span>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check3">
	<div><input type="checkbox" id="Check3" checked="checked" style="margin-left:25px"/> <label for="Check3" class="tr">Trim tabs</label></div>
</span>
<%= _.template($('#block_start').html())({id:"Additional", name: tr("Additional settings"), description: ""}) %>
<%= _.template($('#input_constructor').html())({id:"characters", description: tr("Trim characters"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("Characters to be trimmed at the edges of the string."),examples:[{code:"_-|,"},{code:"_-"},{code:"%&"}]} }) %>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check4">
	<div><input type="radio" id="Check4" name="side" checked="checked" style="margin-left:25px"/> <label for="Check4" class="tr">Trim on both sides</label></div>
</span>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check5">
	<div><input type="radio" id="Check5" name="side" style="margin-left:25px"/> <label for="Check5" class="tr">Trim left</label></div>
</span>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check6">
	<div><input type="radio" id="Check6" name="side" style="margin-left:25px"/> <label for="Check6" class="tr">Trim right</label></div>
</span>
<%= _.template($('#block_end').html())() %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "TRIMMED_STRING", help: {description: tr("Variable in which, after successful execution of the action, the final string will be written."), examples:[{code:tr("Just sample text")},{code:"Test text"},{code:"string"}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Trim string along the edges.</div>
	<div class="tr tooltip-paragraph-fold">This action will return a string, from the beginning and end of which unnecessary characters such as spaces, line breaks, tabs will be trimmed.</div>
	<div class="tr tooltip-paragraph-fold">For example, if the string <code>"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Test text&nbsp;&nbsp;&nbsp;"</code> is specified, then the action will return <code>"Test text"</code>, and if the string <code>"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Just example string&nbsp;&nbsp;&nbsp;&nbsp;"</code> is specified, then the action will return <code>"Just example string"</code>.</div>
	<div class="tr tooltip-paragraph-fold">The characters to be trimmed are determined by the "Trim spaces", "Trim line breaks", "Trim tabs" parameters, but you can also specify your own characters in the "Trim characters" parameter located in the additional settings.</div>
	<div class="tr tooltip-paragraph-last-fold">By default, the action trim the string on both sides, but this can be changed using the radio buttons located in the additional settings.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
