<div class="container-fluid">
	<%= _.template($('#variable_constructor').html())({id:"Variable", description:tr("Variable With List"), default_variable: ""}) %>
    <%= _.template($('#variable_constructor').html())({id:"VariableLength", description:tr("Variable To Save Length"), default_variable: "LIST_LENGTH"}) %>
</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Get number of items in the list.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
