<div class="container-fluid">
    <%= _.template($('#input_constructor').html())({id:"Id", description:tr("Image id"), default_selector: "string", disable_int:true}) %>
    <%= _.template($('#variable_constructor').html())({id:"Width", description:tr("Variable to save width"), default_variable: "IMAGE_WIDTH"}) %>
    <%= _.template($('#variable_constructor').html())({id:"Height", description:tr("Variable to save height"), default_variable: "IMAGE_HEIGHT"}) %>
</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Get image width and height.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>