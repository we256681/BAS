<div class="container-fluid">
    <%= _.template($('#input_constructor').html())({id:"Id", description:tr("Image id"), default_selector: "string", disable_int:true}) %>
    <%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save image data"), default_variable: "IMAGE_DATA"}) %>
</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Get image data with specific id in base64 format. Can be used to save to file.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>