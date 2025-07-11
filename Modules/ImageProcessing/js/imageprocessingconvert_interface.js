<div class="container-fluid">
    <%= _.template($('#input_constructor').html())({id:"Id", description:tr("Image id"), default_selector: "string", disable_int:true}) %>
    <%= _.template($('#input_constructor').html())({id:"Format", description:tr("Image format"), default_selector: "string", disable_int:true, variants: ["png","jpeg"]}) %>
</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Change image format: png, jpeg.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>