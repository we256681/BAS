<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"FilePath", description:tr("Remote path to file or folder"), default_selector: "string", disable_int:true, value_string: "", help: {description:tr("The path on the remote server to the file/folder about which information will be obtained."),examples:[{code:"/html/test"},{code:"/public_html/test1.zip"},{code:"/domains/site.com/public_html/test2.txt"}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"SaveExists", description:tr("File/Folder existence"), default_variable: "REMOTEFILE_EXISTS", help: {description: tr("This variable will be set to true or false depending on the existence of the file/folder."),examples:[{code:"true",description:tr("File/Folder exists.")},{code:"false",description:tr("File/Folder does not exist.")}]}}) %>
<%= _.template($('#variable_constructor').html())({id:"SaveSize", description:tr("File/Folder size"), default_variable: "REMOTEFILE_SIZE", help: {description: tr("This variable will contain the size of the file/folder.")}}) %>
<%= _.template($('#variable_constructor').html())({id:"SaveDirectory", description:tr("File/Folder directory"), default_variable: "REMOTEFILE_BASE_DIRECTORY", help: {description: tr("This variable will contain the path to the directory where the file/folder is located."),examples:[{code:"/html"},{code:"/public_html"},{code:"/domains/site.com/public_html"}]}}) %>
<%= _.template($('#variable_constructor').html())({id:"SaveIsDirectory", description:tr("Is directory"), default_variable: "REMOTEFILE_IS_DIRECTORY", help: {description: tr("This variable will be true or false depending on whether the specified path leads to the directory is or not."),examples:[{code:"true",description:tr("Is directory")},{code:"false",description:tr("Is not directory")}]}}) %>
<%= _.template($('#variable_constructor').html())({id:"SaveIsFile", description:tr("Is file"), default_variable: "REMOTEFILE_IS_FILE", help: {description: tr("This variable will be true or false depending on whether the specified path leads to the file is or not."),examples:[{code:"true",description:tr("Is file")},{code:"false",description:tr("Is not file")}]}}) %>
<%= _.template($('#variable_constructor').html())({id:"SaveLastModified", description:tr("Last modified date"), default_variable: "REMOTEFILE_LAST_MODIFIED", help: {description: tr("This variable will contain the date the file/folder was last modified.")}}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get information about a file located on a remote server.</div>
	<div class="tr tooltip-paragraph-fold">In order to execute this action correctly you need to run "Configure FTP/SSH" action first.</div>
	<div class="tr tooltip-paragraph-fold">This action works with both files and folders.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd",use_timeout: true, visible:true}) %>
