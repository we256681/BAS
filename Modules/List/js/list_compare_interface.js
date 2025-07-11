<div class="container-fluid">
	<%= _.template($('#variable_constructor').html())({id:"Variable", description:tr("Variable With List"), default_variable: ""}) %>
	<%= _.template($('#variable_constructor').html())({id:"VariableSecond", description:tr("List To Compare"), default_variable: ""}) %>
	<%= _.template($('#variable_constructor').html())({id:"VariableResult", description:tr("Result"), default_variable: "LIST_COMPARE_RESULT"}) %>
</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Compare the two specified lists item by item. Returns true if both lists contain same values, even if they are situated in different order.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
