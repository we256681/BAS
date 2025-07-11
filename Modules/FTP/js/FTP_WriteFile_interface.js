<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"FilePath", description:tr("Remote file path"), default_selector: "string", disable_int:true, value_string: "", help: {description:tr("The path to the file on the remote server in which data will be written."),examples:[{code:"/html/test/index.html"},{code:"/public_html/test1.js"},{code:"/domains/site.com/public_html/test2.txt"}]} }) %>
<%= _.template($('#input_constructor').html())({id:"Value", description:tr("Data"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("Data that will be written to the specified file on the remote server.")} }) %>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check">
	<div><input type="checkbox" id="Check"/> <label for="Check" class="tr">Add line ending symbol</label></div>
</span>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check2">
	<div><input type="checkbox" id="Check2"/> <label for="Check2" class="tr">Append file</label></div>
</span>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check3">
	<div><input type="checkbox" id="Check3"/> <label for="Check3" class="tr">Data is in base64 format</label></div>
</span>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Write data to a file on a remote server.</div>
	<div class="tr tooltip-paragraph-fold">In order to execute this action correctly you need to run "Configure FTP/SSH" action first.</div>
	<div class="tr tooltip-paragraph-fold">If a non-existent file path is specified, it will be created.</div>
	<div class="tr tooltip-paragraph-fold">If you want the data to be written on more than one line when adding new data to the file, use "Add line ending symbol" option.</div>
	<div class="tr tooltip-paragraph-fold">If the "Append file" option is enabled, the new data will be added to the data already in the file, or will be written to a new file if the file does not already exist.</div>
	<div class="tr tooltip-paragraph-fold">If the "Append file" option is disabled, the new data will overwrite the data already in the file, or will be written to a new file if the file does not already exist.</div>
	<div class="tr tooltip-paragraph-fold">If the data is in base64 format, use the "Data is in base64 format" parameter for correct writing to the file.</div>
	<div class="tr tooltip-paragraph-fold">To write the list to a file correctly, use the "Write list to file" action.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd",use_timeout: true, visible:true}) %>