<div class="container-fluid">
    <%= _.template($('#resource_constructor').html())({id:"ResourceName", description:tr("Resource name"), help: 
{
	description: tr("Resource name in which new item will be inserted.")
}}) %>
    <%= _.template($('#input_constructor').html())({id:"Data", description:tr("Data"), default_selector: "string", disable_int:true, help: 
{
	description: tr("New item value, could be any string.")
}}) %>


    <span data-preserve="true" data-preserve-type="check" data-preserve-id="Check">
         <input type="checkbox" id="Check" checked="checked"/> <label for="Check" class="tr">Add to file/database instantly</label>
	</span>
	<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check2">
         <input type="checkbox" id="Check2"/> <label for="Check2" class="tr">Only write to file/database</label>
	</span>
</div>
<div class="tooltipinternal">
      <div class="tr tooltip-paragraph-first-fold">Add a new item into resource. For example, add new line into file, or new record into database.</div>
      <div class="tr tooltip-paragraph-fold">Each resource holds container for data, for example, lines from file. Unlike lists, resource system helps to distribute data across threads and also handles thread concurrency for that data.</div>
      <div class="tr tooltip-paragraph-fold">Usually resources are created from user interface (with '+ Create New Resource' button), but also may be created with 'Create resource' action.</div>
      <div class="tr tooltip-paragraph-fold">With this action, you can add data into any type of resource. This action doesn't have affect on some resource types, like 'FixedString' or 'FixedInteger'. Normally it is used to add lines into file.</div>
      <div class="tr tooltip-paragraph-fold">By default lines from file adding is postponed, the reason for this is hard drive usage optimisation. But if you check 'Add to file/database instantly', file will be updated instantly.</div>
      <div class="tr tooltip-paragraph-fold">If 'Only write to file/database' is selected, then container with list of data inside resources won't be updated, only actual resource source will be change, like file or database.</div>
      <div class="tooltip-paragraph-last-fold"><span class="tr">If resource is created in writeonly mode, this action can add data inside it. In that case, it would write string from 'Data' parameter into file selected by user during 'Select resources' stage on script start</span> ( 
<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=adddatatofile.png');return false">add data to file</a>, <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=writeonlymode.png');return false">write only mode</a>, <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=resourceselection.png');return false">resource selection</a>).
      </div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true, name: tr("Add Element")}) %>
