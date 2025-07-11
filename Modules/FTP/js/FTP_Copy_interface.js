<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"FilePath", description:tr("Remote path to file or folder"), default_selector: "string", disable_int:true, value_string: "", help: {description:tr("The path on the remote server to the file/folder which needs to be copied."),examples:[{code:"/html/test"},{code:"/public_html/test1.zip"},{code:"/domains/site.com/public_html/test2.txt"}]} }) %>
<%= _.template($('#input_constructor').html())({id:"CopyPath", description:tr("Remote destination path"), default_selector: "string", disable_int:true, value_string: "", help: {description:tr("The path on the remote server to the location where the file will be copied."),examples:[{code:"/html/new_test"},{code:"/public_html/new_test1"},{code:"/domains/site.com/public_html/new/test"}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Copy file/folder to another location on the remote server.</div>
	<div class="tr tooltip-paragraph-fold">The file/folder will be copied to a new location on the remote server, and the original file/folder will remain unchanged.</div>
	<div class="tr tooltip-paragraph-fold">In order to execute this action correctly you need to run "Configure FTP/SSH" action first.</div>
	<div class="tr tooltip-paragraph-fold">This action works with both files and folders.</div>
	<div class="tr tooltip-paragraph-fold">Copy a folder can take a long time, so it is better to increase the action timeout by clicking on the hourglass next to the cancel button.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd",use_timeout: true, visible:true}) %>
