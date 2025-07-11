<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"data", description: tr("Data"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("The data that need to check out whether they is a string."),examples:[{code:tr("Just sample text"), description: tr("String")},{code:tr("Empty string"), description:tr("String")},{code:123, description: tr("Not string")},{code:"null", description: tr("Not string")}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "IS_STRING", help: {description: tr("This variable will be true or false depending on whether the specified data is a string."), examples:[{code:"true",description:tr("The data is a string.")},{code:"false",description:tr("The data is not a string.")}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Check if the specified data is a string.</div>
	<div class="tr tooltip-paragraph-fold">This action will return true or false depending on whether the specified data is a string. This result may be used with "If" action.</div>
	<div class="tr tooltip-paragraph-last-fold">For example, if <code>"Test text"</code> is specified, then the action will return true, and if <code>123</code> or <code>true</code> is specified, then the action will return false.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
