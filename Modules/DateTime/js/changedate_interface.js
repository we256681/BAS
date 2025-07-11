<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({id:"String", description:tr("DateTime string"), default_selector: "string", disable_int:true}) %>
    <%= _.template($('#input_constructor').html())({id:"Year", description:tr("Year"), default_selector: "int", disable_string:true, value_number: -1}) %>
	<%= _.template($('#input_constructor').html())({id:"Month", description:tr("Month. From 1 to 12"), default_selector: "int", disable_string:true, value_number: -1}) %>
	<%= _.template($('#input_constructor').html())({id:"Day", description:tr("Day. From 1 to 31"), default_selector: "int", disable_string:true, value_number: -1}) %>
	<%= _.template($('#input_constructor').html())({id:"Hour", description:tr("Hour. From 0 to 23"), default_selector: "int", disable_string:true, value_number: -1}) %>
	<%= _.template($('#input_constructor').html())({id:"Minutes", description:tr("Minutes. From 0 to 59"), default_selector: "int", disable_string:true, value_number: -1}) %>
	<%= _.template($('#input_constructor').html())({id:"Seconds", description:tr("Seconds. From 0 to 59"), default_selector: "int", disable_string:true, value_number: -1}) %>
	<%= _.template($('#input_constructor').html())({id:"Milliseconds", description:tr("Milliseconds. From 0 to 999"), default_selector: "int", disable_string:true, value_number: -1}) %>
	<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable To Save"), default_variable: "CHANGED_DATE"}) %>
</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Change an existing date and time object using the specified parameters.</div>
    <div class="tr tooltip-paragraph-last-fold">This action can change year or month or other properties of date. Leave parameter to -1 if you dont want to change it.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>