<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"FilePath", description:tr("Remote file path"), default_selector: "string", disable_int:true, value_string: "", help: {description:tr("The path to the file on the remote server which will be read."),examples:[{code:"/html/test/index.html"},{code:"/public_html/test1.js"},{code:"/domains/site.com/public_html/test2.txt"}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "REMOTEFILE_CONTENT", help: {description: tr("Variable in which, after the successful execution of the action, the contents of the read file will be written.")}}) %>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check">
	<input type="checkbox" id="Check"/> <label for="Check" class="tr">Save in base64 format</label>
</span>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Read a file on a remote server and store its contents into a variable.</div>
	<div class="tr tooltip-paragraph-fold">In order to execute this action correctly you need to run "Configure FTP/SSH" action first.</div>
	<div class="tr tooltip-paragraph-fold">For binary data use "Save in base64 format" option.</div>
	<div class="tr tooltip-paragraph-fold">Images obtained with the "Save in base64 format" option can be used in the "Solve Captcha" action or in the "Image processing" module.</div>
	<div class="tr tooltip-paragraph-fold">If you need to get a list from a file use the "Read file to list" action.</div>
	<div class="tr tooltip-paragraph-fold">If you need to save the file directly to computer, use the "Download file/folder" action.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd",use_timeout: true, visible:true}) %>
