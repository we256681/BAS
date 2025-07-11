<div class="container-fluid">
	<%= _.template($('#variable_constructor').html())({id:"Variable", description:tr("Variable With List"), default_variable: ""}) %>
	<%= _.template($('#variable_constructor').html())({id:"VariableSecond", description:tr("List To Merge"), default_variable: ""}) %>
</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Add all items from one list to another.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
