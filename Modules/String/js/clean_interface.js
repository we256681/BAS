<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"string", description: tr("String"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("The string which needs to be cleaned."),examples:[{code:"\"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" + tr("Just&nbsp;&nbsp;&nbsp;&nbsp;sample&nbsp;&nbsp;text") + "&nbsp;&nbsp;&nbsp;&nbsp;\""},{code:"\"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Test&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;text&nbsp;&nbsp;&nbsp;\""},{code:"\"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;string&nbsp;&nbsp;&nbsp;&nbsp;\""}]} }) %>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check">
	<div><input type="checkbox" id="Check" checked="checked" style="margin-left:25px"/> <label for="Check" class="tr">Replace multiple spaces with single ones</label></div>
</span>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check2">
	<div><input type="checkbox" id="Check2" checked="checked" style="margin-left:25px"/> <label for="Check2" class="tr">Replace line breaks with space</label></div>
</span>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check3">
	<div><input type="checkbox" id="Check3" checked="checked" style="margin-left:25px"/> <label for="Check3" class="tr">Remove tabs</label></div>
</span>
<%= _.template($('#block_start').html())({id:"Additional", name: tr("Additional settings"), description: ""}) %>
<%= _.template($('#input_constructor').html())({id:"characters_to_delete", description: tr("Remove characters"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("Characters which needs to be removed."),examples:[{code:"_-|"},{code:"_-"},{code:"%&"}]} }) %>
<%= _.template($('#input_constructor').html())({id:"characters_to_space", description: tr("Replace characters with space"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("Characters which needs to be replaced with a space."),examples:[{code:"_-|"},{code:"_-"},{code:"%&"}]} }) %>
<%= _.template($('#block_end').html())() %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "CLEANED_STRING", help: {description: tr("Variable in which, after successful execution of the action, the final string will be written."), examples:[{code:tr("Just sample text")},{code:"Test text"},{code:"string"}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Clean string from unnecessary characters.</div>
	<div class="tr tooltip-paragraph-fold">This action will return a string from which all unnecessary characters will be removed, such as multiple spaces, line breaks, tabs.</div>
	<div class="tr tooltip-paragraph-fold">For example, if the string <code>"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Test&nbsp;&nbsp;&nbsp;text&nbsp;&nbsp;&nbsp;"</code> is specified, then the action will return <code>"Test text"</code>, and if the string <code>"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Just&nbsp;&nbsp;example&nbsp;&nbsp;&nbsp;string&nbsp;&nbsp;&nbsp;&nbsp;"</code> is specified, then the action will return <code>"Just example string"</code>.</div>
	<div class="tr tooltip-paragraph-fold">In addition to deleting tabs, activated by the corresponding parameter, you can specify your own characters that will be deleted in the "Remove characters" parameter located in the additional settings.</div>
	<div class="tr tooltip-paragraph-last-fold">The characters that will be replaced by space are determined by the "Replace multiple spaces with single ones", "Replace line breaks with space" parameters, but you can also specify your own characters in the "Replace characters with space" parameter located in the additional settings.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
