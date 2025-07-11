<div class="container-fluid">
    <%= _.template($('#resource_constructor').html())({id:"ResourceName", description:tr("Resource name"), help: 
{
	description: tr("Resource name which data will be replaced with data from list.")
}}) %>
    
    <%= _.template($('#variable_constructor').html())({id:"Variable", description:tr("Variable with list"), default_variable: "", help: 
{
  description: tr("This list will be converted to resource"), examples:
  [
    {
      code:"[]",description:tr("Take data from empty list i.e. clear resource.")
    },
    {
      code:"[\"line1\",\"line2\",\"line3\"]",description:tr("Set new resource data")
    }
  ]
}}) %>

   
</div>
<div class="tooltipinternal">
      <div class="tr tooltip-paragraph-first-fold">Replace resource data with data from list.</div>
      <div class="tr tooltip-paragraph-fold">All previous data will be lost and all resource item state(successes and failures) will be lost.</div>
      <div class="tr tooltip-paragraph-fold">Using action with empty list will clear resource.</div>
      <div class="tr tooltip-paragraph-fold">Each resource holds container for data, for example, lines from file. Unlike lists, resource system helps to distribute data across threads and also handles thread concurrency for that data.</div>
      <div class="tr tooltip-paragraph-fold">Usually resources are created from user interface (with '+ Create New Resource' button), but also may be created with 'Create resource' action.</div>
      <div class="tr tooltip-paragraph-last-fold">This action doesn't have affect on some resource types, like 'FixedString' or 'FixedInteger'.</div>

      
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true, name: tr("Add Element")}) %>
