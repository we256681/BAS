<div class="container-fluid">
    <%= _.template($('#input_constructor').html())({id:"String", description:tr("DateTime string"), default_selector: "string", disable_int:true}) %>
    <%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable To Save"), default_variable: "TIMEZONE_IN_MINUTES"}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get the timezone of an existing date.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>