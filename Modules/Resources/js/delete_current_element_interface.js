<div class="container-fluid">
    <%= _.template($('#resource_constructor').html())({id:"ResourceName", description:tr("Resource name"), help: 
{
	description: tr("Resource name, for which last acquired item will be deleted.")
}}) %>
    <span data-preserve="true" data-preserve-type="check" data-preserve-id="Check">
         <input type="checkbox" id="Check" checked="checked"/> <label for="Check" class="tr">Delete from file/database instantly</label>
	</span>
</div>

<div class="tooltipinternal">
      <div class="tr tooltip-paragraph-first-fold">Delete resource. If resource type is 'LinesFromFile', current line will be deleted from file, if type is 'Database', current record will be deleted from database, etc.</div>
      <div class="tr tooltip-paragraph-fold">Current line is line which was acquired last by current thread. For example, if you have resource 'Proxy' and that proxy appears banned on target site, then you can use this action with resource name 'Proxy'. Doing that will delete proxy from file and will never use that proxy again during whole script run.</div>
      <div class="tr tooltip-paragraph-fold">By default lines from file deletion is postponed, the reaseon for this is hard drive usage optimisation. But if you check 'Delete from file/database instantly', file will be cleared instantly.</div>
      <div class="tooltip-paragraph-last-fold"><span class="tr">Normally you don't need to call that action manually, resources may be cleared automatically, if 'Read and delete lines after use' setting is selected during resource creation.</span> ( <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=deleteresourceafteruse.png');return false">screen</a> ). <span class="tr">Resource will be cleared automatically if successful or fail number of usage will exceed. For example, if 'Use each file one time' is set, resource item will be deleted automatically after single usage</span> ( <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=singleusageres.png');return false">screen</a> ). </div>
      
</div>


<%= _.template($('#back').html())({action:"executeandadd", visible:true, name: tr("Delete Current Element")}) %>
