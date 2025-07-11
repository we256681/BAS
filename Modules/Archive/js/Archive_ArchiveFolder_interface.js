<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"FolderPath", description:tr("Folder path"), default_selector: "string", disable_int:true, value_string: "", help: {description:tr("The path to the folder which needs to be archived."),examples:[{code:"C:/test/ImapCustom"},{code:"C:/Program Files/test"}]} }) %>
<%= _.template($('#input_constructor').html())({id:"ArchiveType", description:tr("Archive type"), default_selector: "string", variants: ["auto","zip","7z"], disable_int:true, value_string: "auto", help: {description:tr("The archive type is usually written in the file extension. But there are exceptions, such as Chrome extensions files have the extension .crx, but are zip archive with additional headers."),examples:[{code:"auto", description: tr("The archive type will be determined by the file extension") + " " + tr("in the destination path")}]} }) %>
<%= _.template($('#input_constructor').html())({id:"DestinationPath", description:tr("Destination path") + ". " + tr("Can be blank"), default_selector: "string", disable_int:true, value_string: "", help: {description:tr("The path to the location where the archive will be saved."),examples:[{code:"C:/test.zip"},{code:"C:/Program Files/test.7z"},{code:tr("Empty string"), description: tr("Place archive next to the original folder")}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Archive folder and save the archive to the specified location.</div>
	<div class="tr tooltip-paragraph-fold">If the destination path is not specified, the archive will be saved to the location where the folder is located and will have the folder name and archive type in the extension.</div>
	<div class="tr tooltip-paragraph-fold">If a non-existent destination path is specified, it will be created.</div>
	<div class="tr tooltip-paragraph-fold">If with same name archive already exists along the destination path, it will be overwritten.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd",use_timeout: true, visible:true}) %>