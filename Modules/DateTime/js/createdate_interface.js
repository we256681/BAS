<div class="container-fluid">
    <%= _.template($('#input_constructor').html())({id:"Year", description:tr("Year"), default_selector: "int", disable_string:true, value_number: 2010}) %>
	<%= _.template($('#input_constructor').html())({id:"Month", description:tr("Month. From 1 to 12"), default_selector: "int", disable_string:true, value_number: 1}) %>
	<%= _.template($('#input_constructor').html())({id:"Day", description:tr("Day. From 1 to 31"), default_selector: "int", disable_string:true, value_number: 1}) %>
	<%= _.template($('#input_constructor').html())({id:"Hour", description:tr("Hour. From 0 to 23"), default_selector: "int", disable_string:true, value_number: 12}) %>
	<%= _.template($('#input_constructor').html())({id:"Minutes", description:tr("Minutes. From 0 to 59"), default_selector: "int", disable_string:true, value_number: 0}) %>
	<%= _.template($('#input_constructor').html())({id:"Seconds", description:tr("Seconds. From 0 to 59"), default_selector: "int", disable_string:true, value_number: 0}) %>
	<%= _.template($('#input_constructor').html())({id:"Milliseconds", description:tr("Milliseconds. From 0 to 999"), default_selector: "int", disable_string:true, value_number: 0}) %>
    <%= _.template($('#input_constructor').html())({id:"Timezone", description:tr("Timezone as offset from UTC in minutes"), default_selector: "int", disable_string:true, value_number: 0 , variants: ["-840","-780","-765","-720","-660","-630","-600","-570","-540","-525","-510","-480","-420","-390","-360","-345","-330","-300","-270","-240","-210","-180","-120","-60","0","60","120","180","210","240","300","360","420","480","540","570","600","660","720"]}) %>
	<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable To Save"), default_variable: "DATE"}) %>
</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Create a new date and time object using the specified parameters.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>