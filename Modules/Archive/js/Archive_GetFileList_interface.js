<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"ArchivePath", description:tr("Archive path"), default_selector: "string", disable_int:true, value_string: "", help: {description:tr("The path to the archive from which needs to get the list of files."),examples:[{code:"C:/test/ImapCustom.zip"},{code:"C:/Program Files/test.rar"}]} }) %>
<%= _.template($('#input_constructor').html())({id:"ArchiveType", description:tr("Archive type"), default_selector: "string", variants: ["auto","zip","7z","rar"], disable_int:true, value_string: "auto", help: {description:tr("The archive type is usually written in the file extension. But there are exceptions, such as Chrome extensions files have the extension .crx, but are zip archive with additional headers."),examples:[{code:"auto", description: tr("The archive type will be determined by the file extension")}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("List of files"), default_variable: "ARCHIVE_FILES", help: {description: tr("Variable in which, after successful completion of the action, will be recorded list of files contained in the archive. The list will contain not only the names of the files but their path in the archive."),examples:[{code:tr("[\"file1.txt\", \"pathto/file2.txt\", \"pathto2/file3.txt\"]")}]}}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get a list of files contained in the archive.</div>
	<div class="tr tooltip-paragraph-fold">The resulting list will contain not only the names of the files, but their path in the archive.</div>
	<div class="tr tooltip-paragraph-fold">The resulting list can be used in the "Unpack archive" action.</div>
	<div class="tr tooltip-paragraph-fold">The resulting list can be processed using actions from the "List" module.</div>
	<div class="tr tooltip-paragraph-last-fold">If the archive has incorrect format or an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd",use_timeout: true, visible:true}) %>