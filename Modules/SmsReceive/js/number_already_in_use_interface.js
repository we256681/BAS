<div class="container-fluid">
  <%= _.template($('#input_constructor').html())({id:"Number", description:tr("Phone number"), default_selector: "string", disable_int:true}) %>
</div>
<div class="tooltipinternal">
  <div class="tr tooltip-paragraph-first-fold">Send a notification that phone number was already in use on target site.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
