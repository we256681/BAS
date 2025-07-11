<div class="container-fluid">
    <%= _.template($('#resource_constructor').html())({id:"ResourceName", description:tr("Resource name"), help: 
{
	description: tr("Resource name for which location will be obtained.")
}}) %>
    <%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable To Save"), default_variable: "RESOURCE_LOCATION", help: 
{
	description: tr("String which will hold resource location. Resource location is filename for 'LinesFromFile' resource, url for 'LinesFromUrl' and directory location for 'FilesFromDirectory' resource type.")
}}) %>
    
</div>

<div class="tooltipinternal">
      <div class="tr tooltip-paragraph-first-fold">Get resource loaction. Resource location is filename for LinesFromFile resource, url for LinesFromUrl and directory location for FilesFromDirectory resource type.</div>
      <div class="tr tooltip-paragraph-fold">Use that action to obtain path to file/folder selected by user.</div>
      <div class="tr tooltip-paragraph-fold">That file or folder can be manipulated later with 'Filesystem' module.</div>
      <div class="tooltip-paragraph-last-fold"><span class="tr">Typical example of using this action would be to delete file on script startup. To do that call this action in 'OnApplicationStart' function and send result into 'Remove File/Folder' action</span>
 ( <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=deletefilefromresource.png');return false">screen</a> ).
		
      </div>
</div>


<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
