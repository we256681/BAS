<div class="container-fluid">
    <%= _.template($('#input_constructor').html())({id:"String", description:tr("DateTime or string"), default_selector: "string", disable_int:true}) %>
    <%= _.template($('#input_constructor').html())({id:"Seconds", description:tr("Seconds to add. Can be negative."), default_selector: "int", disable_string:true, value_number: 0}) %>
    <%= _.template($('#input_constructor').html())({id:"Minutes", description:tr("Minutes to add. Can be negative."), default_selector: "int", disable_string:true, value_number: 0}) %>
    <%= _.template($('#input_constructor').html())({id:"Hours", description:tr("Hours to add. Can be negative."), default_selector: "int", disable_string:true, value_number: 0}) %>
    <%= _.template($('#input_constructor').html())({id:"Days", description:tr("Days to add. Can be negative."), default_selector: "int", disable_string:true, value_number: 0}) %>
	<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable To Save"), default_variable: "DATE"}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Add the specified number of seconds to an existing date.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>