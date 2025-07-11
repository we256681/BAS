<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"FilePath", description:tr("Remote path to file or folder"), default_selector: "string", disable_int:true, value_string: "", help: {description:tr("The path on the remote server to the file/folder which needs to be moved."),examples:[{code:"/html/test"},{code:"/public_html/test1.zip"},{code:"/domains/site.com/public_html/test2.txt"}]} }) %>
<%= _.template($('#input_constructor').html())({id:"NewFilePath", description:tr("New remote path to file or folder"), default_selector: "string", disable_int:true, value_string: "", help: {description:tr("The path on the remote server to the file/folder after moving."),examples:[{code:"/html/new_test"},{code:"/public_html/new_test1.zip"},{code:"/domains/site.com/public_html/new/test2.txt"}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Move the file/folder to another location on the remote server.</div>
	<div class="tr tooltip-paragraph-fold">In order to execute this action correctly you need to run "Configure FTP/SSH" action first.</div>
	<div class="tr tooltip-paragraph-fold">This action works with both files and folders.</div>
	<div class="tr tooltip-paragraph-fold">Move a folder can take a long time, so it's best to increase the action timeout by clicking on the hourglass next to the cancel button.</div>
	<div class="tr tooltip-paragraph-fold">This action can be used to rename a file/folder on a remote server.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd",use_timeout: true, visible:true}) %>
