<div class="container-fluid randomstring">
	<%= _.template($('#input_constructor').html())({id:"AllowedChars", description:tr("Allowed Chars"), default_selector: "string", disable_int:true, value_string: "abcdefghijklmnopqrstuvwxyz0123456789"}) %>
	<%= _.template($('#input_constructor').html())({id:"Length", description:tr("Result String Length"), default_selector: "int", disable_string:true, value_number:10, min_number:1, max_number:999999}) %>
	<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable To Save Result"), default_variable: "RANDOM_STRING"}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Generate a random string with a given length and allowed characters.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>