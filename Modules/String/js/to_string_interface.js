<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"data", description: tr("Data"), default_selector: "string", value_string: "", help: {description: tr("The data to be converted to a string."),examples:[{code:123,description:tr("Numbеr")},{code:true,description:tr("Boolean")},{code:"{\"text\":\"Test text\",\"num\":123}",description:tr("Object")},{code:new Date(),description:tr("Date")}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "RESULT_STRING", help: {description: tr("Variable in which, after successful execution of the action, the string representation of the specified data will be written."),examples:[{code:"123"},{code:"true"},{code:"{\"text\":\"Test text\",\"num\":123}"},{code:new Date()}]}}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Convert the specified data to a string.</div>
	<div class="tr tooltip-paragraph-fold">This action will return a string representation of the specified data.</div>
	<div class="tr tooltip-paragraph-fold">This action works with any type of data.</div>
	<div class="tr tooltip-paragraph-last-fold">For example, if <code>123</code> is specified, then the action will return <code>"123"</code>, and if <code>true</code> is specified, then the action will return <code>"true"</code>.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
