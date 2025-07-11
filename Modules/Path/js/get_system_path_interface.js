<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"name", description: tr("Path name"), default_selector: "string", variants:["Program Files","Program Files (x86)","Desktop","Downloads","Documents","Pictures","Videos","Music","Favorites","App Data","Local App Data","User Profile","Public","Program Data","System Drive","System Root","Windows Directory","Temp","User Name","Computer Name"], disable_int:true, value_string: "Program Files", help: {description: tr("The name of the path that you want to receive."),examples:[{code:"Program Files"},{code:"AppData"},{code:"Desktop"}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "RECEIVED_PATH", help: {description: tr("Variable in which, after successful execution of the action, the resulting path will be written."), examples:[{code:"C:/Program Files",description:tr("Path name") + ": <code style=\"font-size:85%\">Program Files</code>"},{code:"C:/Users/Admin/AppData/Roaming",description:tr("Path name") + ": <code style=\"font-size:85%\">AppData</code>"},{code:"C:/Users/Admin/Desktop",description:tr("Path name") + ": <code style=\"font-size:85%\">Desktop</code>"}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get the system path with the specified name.</div>
	<div class="tr tooltip-paragraph-fold"><b>Program Files</b> - Path to the "Program Files" directory. Example: <code>"C:/Program Files"</code>.</div>
	<div class="tr tooltip-paragraph-fold"><b>Program Files (x86)</b> - The path to the "Program Files (x86)" directory on 64-bit systems for x86 architecture applications. Example: <code>"C:/Program Files (x86)"</code>.</div>
	<div class="tr tooltip-paragraph-fold"><b>Desktop</b> - Path to the "Desktop" directory. Example: <code>"C:/Users/Admin/Desktop"</code>.</div>
	<div class="tr tooltip-paragraph-fold"><b>Downloads</b> - Path to the "Downloads" directory. Example: <code>"C:/Users/Admin/Downloads"</code>.</div>
	<div class="tr tooltip-paragraph-fold"><b>Documents</b> - Path to the "Documents" directory. Example: <code>"C:/Users/Admin/Documents"</code>.</div>
	<div class="tr tooltip-paragraph-fold"><b>Pictures</b> - Path to the "Pictures" directory. Example: <code>"C:/Users/Admin/Pictures"</code>.</div>
	<div class="tr tooltip-paragraph-fold"><b>Videos</b> - Path to the "Videos" directory. Example: <code>"C:/Users/Admin/Videos"</code>.</div>
	<div class="tr tooltip-paragraph-fold"><b>Music</b> - Path to the "Music" directory. Example: <code>"C:/Users/Admin/Music"</code>.</div>
	<div class="tr tooltip-paragraph-fold"><b>Favorites</b> - Path to the "Favorites" directory. Example: <code>"C:/Users/Admin/Favorites"</code>.</div>
	<div class="tr tooltip-paragraph-fold"><b>App Data</b> - The default location for app data. Example: <code>"C:/Users/Admin/AppData/Roaming"</code>.</div>
	<div class="tr tooltip-paragraph-fold"><b>Local App Data</b> - The default local location for application data. Example: <code>"C:/Users/Admin/AppData/Local"</code>.</div>
	<div class="tr tooltip-paragraph-fold"><b>User Profile</b> - The path to the profile of the current user. Example: <code>"C:/Users/Admin"</code>.</div>
	<div class="tr tooltip-paragraph-fold"><b>System Drive</b> - The drive where the Windows root directory is located. Example: <code>"C:"</code>.</div>
	<div class="tr tooltip-paragraph-fold"><b>System Root</b> - The path to the Windows root directory. Example: <code>"C:/Windows"</code>.</div>
	<div class="tr tooltip-paragraph-fold"><b>Windows Directory</b> - The directory where Windows is installed. Example: <code>"C:/Windows"</code>.</div>
	<div class="tr tooltip-paragraph-fold"><b>Temp</b> - Path to the temporary directory. Example: <code>"C:/Users/Admin/AppData/Local/Temp"</code>.</div>
	<div class="tr tooltip-paragraph-fold"><b>User Name</b> - The name of the current user. Example: <code>"Admin"</code>.</div>
	<div class="tr tooltip-paragraph-last-fold"><b>Computer Name</b> - Computer name. Example: <code>"ADMIN"</code>.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
