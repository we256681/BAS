<div class="container-fluid">
    <%= _.template($('#resource_constructor').html())({id:"ResourceName", description:tr("Resource name"), help: 
{
	description: tr("Resource name to count items")
}}) %>
    <%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable To Save"), default_variable: "RESOURCE_LENGTH", help: 
{
	description: tr("This variable will contain number of items in selected resource")
}}) %>
    
</div>

<div class="tooltipinternal">
      <div class="tr tooltip-paragraph-first-fold">Get number of items in resource.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
