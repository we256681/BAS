<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"FolderPath", description:tr("Remote directory to search"), default_selector: "string", disable_int:true, value_string: "", help: {description:tr("The path to the directory on the remote server in which the search will be performed."),examples:[{code:"/html/test"},{code:"/public_html"},{code:"/domains/site.com/public_html"}]} }) %>
<%= _.template($('#input_constructor').html())({id:"Mask", description:tr("File/Folder name mask"), default_selector: "string", disable_int:true, value_string: "*", help: {description: tr("File/Folder name mask for filtering. The final list will include only files/folders whose names match the mask."),examples:[{code:tr("*.js"), description: tr("All JavaScript files")},{code:tr("*.txt"), description: tr("All text files")},{code:tr("*.js;*.txt"), description: tr("All JavaScript and text files")},{code:tr("*"), description: tr("All files")}]} }) %>
<div>
	<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check">
	  <input type="checkbox" id="Check" checked/> <label for="Check" class="tr">Search folders</label>  
	</span>
	<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check2">
		<input type="checkbox" id="Check2" checked/> <label for="Check2" class="tr">Search files</label>
	</span>
	<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check3">
		<input type="checkbox" id="Check3" checked/> <label for="Check3" class="tr">Search in subfolders</label> 
	</span>
</div>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "REMOTEFILE_SEARCH_RESULT", help: {description: tr("Variable in which, after successful execution of the action, the list of found files/folders will be written. The list will contain not only the names of files/folders, but their full path."),examples:[{code:tr("[\"/file1.txt\", \"/pathto\", \"/pathto/file2.txt\"]")}]}}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Search for files/folders in the specified directory on the remote server.</div>
	<div class="tr tooltip-paragraph-fold">In order to execute this action correctly you need to run "Configure FTP/SSH" action first.</div>
	<div class="tr tooltip-paragraph-fold">Using a mask, you can search for files/folders with a specific name or extension.</div>
	<div class="tr tooltip-paragraph-fold">You can specify several masks by separating them with ";".</div>
	<div class="tr tooltip-paragraph-fold">If "Search files" option is enabled, the final list will include files.</div>
	<div class="tr tooltip-paragraph-fold">If "Search folders" option is enabled, the final list will include folders.</div>
	<div class="tr tooltip-paragraph-fold">If "Search in subfolders" option is enabled, the search will be performed not only in the specified directory, but also recursively in all folders that are in it.</div>
	<div class="tr tooltip-paragraph-fold">If "Search in subfolders" option is enabled, the search can take a long time, so it is better to increase the action timeout by clicking on the hourglass next to the cancel button.</div>
	<div class="tr tooltip-paragraph-fold">The resulting list will contain not only the names of the files/folders, but their full path.</div>
	<div class="tr tooltip-paragraph-fold">The resulting list can be processed using actions from the "List" module.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd",use_timeout: true, visible:true}) %>
