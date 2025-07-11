<div class="container-fluid">
  <%= _.template($('#input_constructor').html())({id:"Number", description:tr("Phone number"), default_selector: "string", disable_int:true}) %>
  <%= _.template($('#input_constructor').html())({id:"MaxWait", description:tr("Max waiting time in minutes"), default_selector: "int", disable_string:true, value_number:10}) %>
  <%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable To Save"), default_variable: "ACTIVATION_CODE"}) %>
</div>
<div class="tooltipinternal">
  <div class="tr tooltip-paragraph-first-fold">Get an activation code from the specified SMS receiving service.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
