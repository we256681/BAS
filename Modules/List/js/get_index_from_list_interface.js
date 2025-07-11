<div class="container-fluid">
	<%= _.template($('#variable_constructor').html())({id:"Variable", description:tr("Variable With List"), default_variable: ""}) %>
	<%= _.template($('#input_constructor').html())({id:"Value", description:tr("Value"), default_selector: "string"}) %>
    <%= _.template($('#variable_constructor').html())({id:"VariableSave", description:tr("Variable To Save Result"), default_variable: "VALUE_INDEX"}) %>
</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Get the item index with a specific value from the list.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
