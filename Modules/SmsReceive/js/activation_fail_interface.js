<div class="container-fluid">
  <%= _.template($('#input_constructor').html())({id:"Number", description:tr("Phone number"), default_selector: "string", disable_int:true}) %>
</div>
<div class="tooltipinternal">
  <div class="tr tooltip-paragraph-first-fold">Send a notification of the incorrect SMS code to the service.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
