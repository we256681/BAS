<div class="container-fluid">
    <%= _.template($('#input_constructor').html())({id:"DecreasingString", description:tr("Decreasing DateTime or string"), default_selector: "string", disable_int:true}) %>
    <%= _.template($('#input_constructor').html())({id:"SubtractedString", description:tr("Subtracted DateTime or string"), default_selector: "string", disable_int:true}) %>  
    <%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable To Save"), default_variable: "DATE_DIFFERENCE_IN_SECONDS"}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get the difference in seconds between the specified dates.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>