<div class="container-fluid">
    <%= _.template($('#input_constructor').html())({id:"String", description:tr("DateTime string"), default_selector: "string", disable_int:true}) %>
    <%= _.template($('#input_constructor').html())({id:"Timezone", description:tr("Timezone as offset from UTC in minutes"), default_selector: "int", disable_string:true, value_number: 0 , variants: ["-840","-780","-765","-720","-660","-630","-600","-570","-540","-525","-510","-480","-420","-390","-360","-345","-330","-300","-270","-240","-210","-180","-120","-60","0","60","120","180","210","240","300","360","420","480","540","570","600","660","720"]}) %>
	<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable To Save"), default_variable: "DATE_WITH_CHANGED_TIMEZONE"}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Change the timezone of an existing date.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>