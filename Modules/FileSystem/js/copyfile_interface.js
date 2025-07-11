<div class="container-fluid">
  <%= _.template($('#input_constructor').html())({id:"Value", description:tr("File/Folder Name"), default_selector: "string", disable_int:true}) %>
  <%= _.template($('#input_constructor').html())({id:"ValueDest", description:tr("File/Folder Destination"), default_selector: "string", disable_int:true}) %>
</div>
<div class="tooltipinternal">
  <div class="tr tooltip-paragraph-first-fold">Copy file or folder to a new location.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>