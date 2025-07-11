<div class="container-fluid">
    <%= _.template($('#input_constructor').html())({id:"String", description:tr("Milliseconds"), default_selector: "string", disable_int:true}) %>
	<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable To Save"), default_variable: "DATE_FROM_MILLISECONDS"}) %>
</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Convert number of milliseconds since epoch start to date.</div>
    <div class="tr tooltip-paragraph-last-fold">The number of milliseconds for this conversion is counted from January 1, 1970, 00:00:00 UTC.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>