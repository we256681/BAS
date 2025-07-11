<div class="container-fluid">
    <%= _.template($('#input_constructor').html())({id:"Id", description:tr("Image id"), default_selector: "string", disable_int:true}) %>
    <%= _.template($('#input_constructor').html())({id:"X", description:tr("X"), default_selector: "int", disable_string:true, value_number:0, min_number:0}) %>
    <%= _.template($('#input_constructor').html())({id:"Y", description:tr("Y"), default_selector: "int", disable_string:true, value_number:0, min_number:0}) %>
    <%= _.template($('#variable_constructor').html())({id:"R", description:tr("Color Red"), default_variable: "PIXEL_R"}) %>
    <%= _.template($('#variable_constructor').html())({id:"G", description:tr("Color Green"), default_variable: "PIXEL_G"}) %>
    <%= _.template($('#variable_constructor').html())({id:"B", description:tr("Color Blue"), default_variable: "PIXEL_B"}) %>
    <%= _.template($('#variable_constructor').html())({id:"A", description:tr("Color Transparency"), default_variable: "PIXEL_A"}) %>
</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Get the color of a pixel by its coordinates in the image.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>