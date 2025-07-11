<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"FilePath", description:tr("Remote file path"), default_selector: "string", disable_int:true, value_string: "", help: {description:tr("The path to the file on the remote server, the checksum for which needs to be obtained."),examples:[{code:"/html/test/index.html"},{code:"/public_html/test1.js"},{code:"/domains/site.com/public_html/test2.txt"}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "REMOTEFILE_CHECKSUM_SHA1", help: {description: tr("Variable in which, after the successful execution of the action, the checksum of the specified file will be written."),examples:[{code:"867269acd8fc6d05b308557be825f18c4baa4503"},{code:"177c85ed0bbe05be06108aaf749b6f6f249feee8"},{code:"4bca5e9713553e1438d2e8000144566bcafffc5c"}]}}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get the checksum of the file located on the remote server.</div>
	<div class="tr tooltip-paragraph-fold">In order to execute this action correctly you need to run "Configure FTP/SSH" action first.</div>
	<div class="tr tooltip-paragraph-fold">This action only works with the SSH protocol.</div>
	<div class="tooltip-paragraph-fold"><span class="tr">sha1sum must be installed on the server. Learn more about sha1sum</span> <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://en.wikipedia.org/wiki/Sha1sum');return false">here</a><span>.</span></div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd",use_timeout: true, visible:true}) %>
