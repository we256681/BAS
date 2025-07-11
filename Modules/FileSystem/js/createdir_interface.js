<div class="container-fluid">
  <%= _.template($('#input_constructor').html())({id:"Value", description:tr("Directory Name"), default_selector: "string", disable_int:true}) %>
</div>
<div class="tooltipinternal">
  <div class="tr tooltip-paragraph-first-fold">Create a new folder at the specified path.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>