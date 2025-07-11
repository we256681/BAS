<div class="container-fluid">
    <%= _.template($('#resource_constructor').html())({id:"ResourceName", description:tr("Resource name"), help: 
{
	description: tr("Resource name, which will be copied to list")
}}) %>
    <%= _.template($('#variable_constructor').html())({id:"Save", description:tr("List To Save"), default_variable: "RESOURCE_AS_LIST", help: 
{
	description: tr("This variable will contain list with all data from selected resource."), examples:
	[
		{
			code:"[\"line1\",\"line2\",\"line3\"]"
		}
	]
}}) %>
    
</div>

<div class="tooltipinternal">
      <div class="tr tooltip-paragraph-first-fold">Copy resource data into list.</div>
      <div class="tr tooltip-paragraph-fold">Result has a list type, so it can be manipulated with 'List' module or with javascript.</div>
      <div class="tr tooltip-paragraph-fold">It is very important to understand, that changing resulting list won't affect resource itself.</div>
      <div class="tooltip-paragraph-last-fold"><span class="tr">Typical example of usage would be to copy resource into list and to iterate over each list element</span> ( <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=iterateresource.png');return false">screen</a> ).</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
