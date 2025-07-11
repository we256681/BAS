<div class="container-fluid">
	<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable To Save"), default_variable: "DATE_NOW"}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get the current date and time.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>