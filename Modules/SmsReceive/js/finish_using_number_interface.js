<div class="container-fluid">
  <%= _.template($('#input_constructor').html())({id:"Number", description:tr("Phone number"), default_selector: "string", disable_int:true}) %>
</div>
<div class="tooltipinternal">
  <div class="tr tooltip-paragraph-first-fold">Finish working with the current phone number.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
