<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"enable", description:tr("Enable debug"), default_selector: "string", disable_int:true, value_string: "true", variants: ["true","false"], help: {description: tr("Enable phone verification module debug."), examples:[{code:"true",description:tr("Enable debug")},{code:"false",description:tr("Disable debug")}]}}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Enable or disable phone verification module debug.</div>
	<div class="tr tooltip-paragraph-last-fold">If debug is enabled, all queries and their results will be displayed in the log.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
