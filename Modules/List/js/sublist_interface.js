<div class="container-fluid">
	<%= _.template($('#variable_constructor').html())({id:"Variable", description:tr("Variable With List"), default_variable: ""}) %>
    <%= _.template($('#input_constructor').html())({id:"IndexStart", description:tr("Start Index"), default_selector: "int", disable_string:true, min_number: -999999, value_number: 0}) %>
    <%= _.template($('#input_constructor').html())({id:"IndexEnd", description:tr("Length"), default_selector: "int", disable_string:true, min_number: -999999, value_number: 10}) %>
    <%= _.template($('#variable_constructor').html())({id:"VariableNewList", description:tr("New List"), default_variable: "NEW_SUBLIST"}) %>
</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Get a sublist of a certain number of items from a list.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
