<div class="container-fluid">
  <%= _.template($('#input_constructor').html())({id:"Value", description:tr("File path"), default_selector: "string", disable_int:true}) %>
  <%= _.template($('#variable_constructor').html())({id:"Save", description:tr("List To Save"), default_variable: "LIST_WITH_FILE_CONTENT"}) %>
</div>
<div class="tooltipinternal">
  <div class="tr tooltip-paragraph-first-fold">Read the content of the specified file to list. Each element of the list will contain one line of original file.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>