<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"FilePath", description:tr("Remote path to file or folder"), default_selector: "string", disable_int:true, value_string: "", help: {description:tr("The path on the remote server to the file/folder which needs to be downloaded."),examples:[{code:"/html/test"},{code:"/public_html/test1.zip"},{code:"/domains/site.com/public_html/test2.txt"}]} }) %>
<%= _.template($('#input_constructor').html())({id:"DestinationPath", description:tr("Destination path"), default_selector: "string", disable_int:true, value_string: "", help: {description:tr("The path to the location where the file downloaded from the remote server or the contents of the downloaded folder will be saved."),examples:[{code:"C:/test"},{code:"C:/Program Files"}]} }) %>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check">
	<input type="checkbox" id="Check"/> <label for="Check" class="tr">Verify checksums (SSH only)</label>
</span>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Download file/folder from a remote server and save to the specified location.</div>
	<div class="tr tooltip-paragraph-fold">The file/folder will be copied to your computer, and the original file/folder on the remote server will remain unchanged.</div>
	<div class="tr tooltip-paragraph-fold">In order to execute this action correctly you need to run "Configure FTP/SSH" action first.</div>
	<div class="tr tooltip-paragraph-fold">This action works with both files and folders.</div>
	<div class="tr tooltip-paragraph-fold">Downloading a folder can take a long time, so it is better to increase the action timeout by clicking on the hourglass next to the cancel button.</div>
	<div class="tr tooltip-paragraph-fold">If a non-existent destination path is specified, it will be created.</div>
	<div class="tr tooltip-paragraph-fold">If downloaded files already exist along the destination path, they will be overwritten.</div>
	<div class="tr tooltip-paragraph-fold">If the "Verify checksums" parameter is activated, downloaded file existence and its compliance with the original file will be checked. And if the file was not downloaded or downloaded corrupted, the action will fail.</div>
	<div class="tr tooltip-paragraph-fold">Checksum verification only works with SHH protocol, but checking for file existence works with any protocol.</div>
	<div class="tr tooltip-paragraph-fold">If a resource is specified in the "Destination path" parameter, resource location will be used.</div>
	<div class="tr tooltip-paragraph-fold">If you need to get a list from a file use the "Read file to list" action.</div>
	<div class="tr tooltip-paragraph-fold">If you just need to save the contents of a file to a variable, use the "Read file" action.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd",use_timeout: true, visible:true}) %>
