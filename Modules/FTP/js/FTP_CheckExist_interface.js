<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"FilePath", description:tr("Remote path to file or folder"), default_selector: "string", disable_int:true, value_string: "", help: {description:tr("The path on the remote server to the file/folder whose existence needs to be checked."),examples:[{code:"/html/test"},{code:"/public_html/test1.zip"},{code:"/domains/site.com/public_html/test2.txt"}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "REMOTEFILE_EXISTS", help: {description: tr("This variable will be to true or false depending on the existence of the file/folder."), examples:[{code:"true",description:tr("File/Folder exists.")},{code:"false",description:tr("File/Folder does not exist.")}]}}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Check if a file/folder exists at the specified path on the remote server.</div>
	<div class="tr tooltip-paragraph-fold">This action will return true or false depending on the existence of the file/folder. This result may be used with "If" action.</div>
	<div class="tr tooltip-paragraph-fold">In order to execute this action correctly you need to run "Configure FTP/SSH" action first.</div>
	<div class="tr tooltip-paragraph-fold">This action works with both files and folders.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd",use_timeout: true, visible:true}) %>
