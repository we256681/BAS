<div class="container-fluid">
    <%= _.template($('#input_constructor').html())({id:"Id", description:tr("Image id"), default_selector: "string", disable_int:true}) %>
    <%= _.template($('#input_constructor').html())({id:"Text", description:tr("Text"), default_selector: "string", disable_int:true}) %>
    <%= _.template($('#input_constructor').html())({id:"X", description:tr("X"), default_selector: "int", disable_string:true, value_number:0, min_number:0}) %>
    <%= _.template($('#input_constructor').html())({id:"Y", description:tr("Y"), default_selector: "int", disable_string:true, value_number:0, min_number:0}) %>
    <%= _.template($('#input_constructor').html())({id:"Width", description:tr("Width"), default_selector: "int", disable_string:true, value_number:800, min_number:0}) %>
    <%= _.template($('#input_constructor').html())({id:"Height", description:tr("Height"), default_selector: "int", disable_string:true, value_number:800, min_number:0}) %>
    <%= _.template($('#input_constructor').html())({id:"FontFamily", description:tr("Font Family"), default_selector: "string", disable_int:true, value_string:"Arial"}) %>
    <%= _.template($('#input_constructor').html())({id:"FontSize", description:tr("Font Size"), default_selector: "int", disable_string:true, value_number:15, min_number:1}) %>
    <%= _.template($('#input_constructor').html())({id:"ColorR", description:tr("Font Color Red"), default_selector: "int", disable_string:true, value_number:0, min_number:0, max_number:255}) %>
    <%= _.template($('#input_constructor').html())({id:"ColorG", description:tr("Font Color Green"), default_selector: "int", disable_string:true, value_number:0, min_number:0, max_number:255}) %>
    <%= _.template($('#input_constructor').html())({id:"ColorB", description:tr("Font Color Blue"), default_selector: "int", disable_string:true, value_number:0, min_number:0, max_number:255}) %>
    <%= _.template($('#input_constructor').html())({id:"ColorA", description:tr("Font Color Transparency"), default_selector: "int", disable_string:true, value_number:255, min_number:0, max_number:255}) %>
    <%= _.template($('#input_constructor').html())({id:"VerticalAlign", description:tr("Vertical align"), default_selector: "string", disable_int:true, value_string: "top", variants: ["top","bottom","center"]}) %>
    <%= _.template($('#input_constructor').html())({id:"HorizontalAlign", description:tr("Horizontal align"), default_selector: "string", disable_int:true, value_string: "center", variants: ["left","right","center"]}) %>
</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Add text to the image using specified parameters.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>