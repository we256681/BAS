<div class="container-fluid">
	<%= _.template($('#variable_constructor').html())({id:"Variable", description:tr("Variable With List"), default_variable: ""}) %>
    <%= _.template($('#input_constructor').html())({id:"Value", description:tr("Value to add"), default_selector: "string"}) %>
</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Add an item to the end of the list.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
