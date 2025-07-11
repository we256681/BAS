<div class="container-fluid">
	<%= _.template($('#variable_constructor').html())({id:"Variable", description:tr("Variable With List"), default_variable: ""}) %>
</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Shuffle all list items in the random order.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
