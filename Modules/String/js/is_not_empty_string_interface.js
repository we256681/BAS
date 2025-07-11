<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"data", description: tr("Data"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("The data that need to check out whether they is not empty string."),examples:[{code:tr("Just sample text"), description: tr("Not empty string")},{code:'""', description: tr("Empty string")}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "IS_NOT_EMPTY", help: {description: tr("This variable will be true or false depending on whether the specified data is not an empty string."), examples:[{code:"true",description:tr("Not empty string") + "."},{code:"false",description:tr("Empty string or not string.")}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Check if the specified data is not an empty string.</div>
	<div class="tr tooltip-paragraph-fold">This action will return true or false depending on whether the specified data is not an empty string. This result may be used with "If" action.</div>
	<div class="tr tooltip-paragraph-last-fold">For example, if <code>"Test text"</code> is specified, then the action will return true, and if <code>""</code> or <code>123</code> is specified, then the action will return false.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
