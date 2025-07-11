<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"FilePath", description:tr("Remote file path"), default_selector: "string", disable_int:true, value_string: "", help: {description:tr("The path to the file on the remote server which will be read."),examples:[{code:"/html/test/index.html"},{code:"/public_html/test1.js"},{code:"/domains/site.com/public_html/test2.txt"}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "LIST_WITH_REMOTEFILE_CONTENT", help: {description: tr("Variable in which, after the successful execution of the action, the list obtained from the contents of the file will be written."),examples:[{code:tr("[\"line1\", \"line2\", \"line3\"]")}]}}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Read the file on the remote server, convert its contents to a list, and store this list into a variable.</div>
	<div class="tr tooltip-paragraph-fold">In order to execute this action correctly you need to run "Configure FTP/SSH" action first.</div>
	<div class="tr tooltip-paragraph-fold">The resulting list can be processed using actions from the "List" module.</div>
	<div class="tr tooltip-paragraph-fold">If you just need to save the contents of a file to a variable, use the "Read file" action.</div>
	<div class="tr tooltip-paragraph-fold">If you need to save the file directly to computer, use the "Download file/folder" action.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd",use_timeout: true, visible:true}) %>
