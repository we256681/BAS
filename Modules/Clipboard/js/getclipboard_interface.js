<div class="container-fluid">
  <%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable To Save"), default_variable: "CLIPBOARD_CONTENT"}) %>
  <%= _.template($('#input_constructor').html())({id:"MimeType", description:tr("Mime Type"), default_selector: "string", disable_int:true, value_string: "text/plain", variants:["text/plain", "text/html"]}) %>

  <span data-preserve="true" data-preserve-type="check" data-preserve-id="Check">
    <input type="checkbox" id="Check" />
    <label for="Check" class="tr">Save In Base64 Format</label>
  </span>

</div>
<div class="tooltipinternal">
  <div class="tr tooltip-paragraph-first-fold">Read data of the specified type from the clipboard.</div>
  <div class="tr tooltip-paragraph-last-fold">Clipboard is global to whole system and should be locked in multithreaded mode.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>