<div class="container-fluid">
    <%= _.template($('#input_constructor').html())({id:"Data", description:tr("Image data in base64 format"), default_selector: "string", disable_int:true}) %>
    <%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save image id"), default_variable: "LOADED_IMAGE_ID"}) %>
</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Load image from binary data (base64 format). Obtain image id, which will help to manipulate image.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>