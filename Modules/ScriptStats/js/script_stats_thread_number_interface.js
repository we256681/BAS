<div class="container-fluid">
    <%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save thread index"), default_variable: "THREAD_INDEX"}) %>
</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Get the number of the current working thread.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>