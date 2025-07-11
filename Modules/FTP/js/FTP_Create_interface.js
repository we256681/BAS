<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"FilePath", description:tr("Remote path to new file/folder"), default_selector: "string", disable_int:true, value_string: "", help: {description:tr("The path on the remote server to the new file/folder which will be available after creation."),examples:[{code:"/html/new_test"},{code:"/public_html/new_test1.zip"},{code:"/domains/site.com/public_html/new_test2.txt"}]} }) %>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check">
	<div><input type="radio" id="Check"  name="filetype" checked/> <label for="Check" class="tr">File</label></div>
</span>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check2">
	<div><input type="radio" id="Check2"  name="filetype"/> <label for="Check2" class="tr">Folder</label></div>
</span>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Create file/folder on remote server.</div>
	<div class="tr tooltip-paragraph-fold">In order to execute this action correctly you need to run "Configure FTP/SSH" action first.</div>
	<div class="tr tooltip-paragraph-fold">If the specified path is missing it will be created.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd",use_timeout: true, visible:true}) %>
