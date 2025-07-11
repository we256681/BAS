<div class="container-fluid">
	<%= _.template($('#variable_constructor').html())({id:"Variable", description:tr("Variable With List"), default_variable: ""}) %>
    <%= _.template($('#input_constructor').html())({id:"Separator", description:tr("Separator"), default_selector: "string", variants: [{value:"&#034;\\n&#034;",type:"expression",description:tr("Line break, join into multiline text")},{value:"&#034;\\r\\n&#034;",type:"expression",description:tr("Carriage return and line break, join into multiline text")}], disable_int:true, value_string: ","}) %>
    <%= _.template($('#variable_constructor').html())({id:"VariableResult", description:tr("New List"), default_variable: "JOIN_RESULT_STRING"}) %>
</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Join all elements of the list into a string using a specific separator.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
