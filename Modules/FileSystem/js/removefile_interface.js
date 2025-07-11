<div class="container-fluid">
  <%= _.template($('#input_constructor').html())({id:"Value", description:tr("File/Folder Name"), default_selector: "string", disable_int:true}) %>
</div>
<div class="tooltipinternal">
  <div class="tr tooltip-paragraph-first-fold">Remove file or folder.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>