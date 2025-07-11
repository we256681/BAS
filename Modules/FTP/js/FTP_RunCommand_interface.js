<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"Command", description:tr("Command"), default_selector: "string", disable_int:true, disable_editor:true, disable_string:true, use_textarea:true, size: 8, disable_type_chooser:true,textarea_height:80, help: {description: tr("The command which needs to be executed on the remote server."), examples:[{code:"tail -100 /home/nginx/error.log",description:tr("Show last lines of log")},{code:"df -h",description:tr("Show disk usage")},{code:"top -n 1 -b",description:tr("Show CPU usage")},{code:"mysqldump -p --all-databases --result-file=dump.sql",description:tr("Dump mysql databases")}]}}) %>
<%= _.template($('#input_constructor').html())({id:"WorkingFolder", description:tr("Working folder"), default_selector: "string", disable_int:true, help: {description: tr("Working folder in which bash script will be executed."), examples:[{code:"/home"},{code:"/root/test"},{code:tr("Empty string"),description:tr("Run in current working directory")}]}}) %>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check">
	<input type="checkbox" id="Check" checked="checked"/> <label for="Check" class="tr">Wait while process will finish</label>
</span>
<%= _.template($('#variable_constructor').html())({id:"Standart", description:tr("Standard output"), default_variable: "STANDART_OUTPUT", help: {description: tr("This variable will contain the standard output of the command.")}}) %>
<%= _.template($('#variable_constructor').html())({id:"Error", description:tr("Error output"), default_variable: "ERROR_OUTPUT", help: {description: tr("This variable will contain the result of the command with error.")}}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Execute bash command on a remote server.</div>
	<div class="tr tooltip-paragraph-fold">In order to execute this action correctly you need to run "Configure FTP/SSH" action first.</div>
	<div class="tr tooltip-paragraph-fold">This action only works with the SSH protocol.</div>
	<div class="tooltip-paragraph-fold"><span class="tr">The command  can have multiple lines, you can learn more about bash in</span> <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://www.gnu.org/savannah-checkouts/gnu/bash/manual/bash.html');return false">manual</a><span>.</span></div>
	<div class="tr tooltip-paragraph-fold">Working folder by default is the home folder of current user, you may want to change it with "Working folder" parameter or by running "cd" command.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd",use_timeout: true, visible:true}) %>
