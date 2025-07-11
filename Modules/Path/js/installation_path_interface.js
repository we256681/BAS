<div class="container-fluid">
    <%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "INSTALLATION_PATH", help: {description: tr("Variable in which, after successful execution of the action, the installation path will be written."), examples:[{code:"C:/BrowserAutomationStudio/apps/24.0.8",description:tr("Not compiled script")},{code:"D:/Gparser/appslocal/ibq8y9qo/SID/engine",description:tr("Unprotected compiled script")},{code:"D:/Gparser/appsremote/Gparser/SID/engine",description:tr("Protected compiled script")}]}}) %>
</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Get the full installation path.</div>
	<div class="tr tooltip-paragraph-fold">This action will return the full path to the directory where the executable file is located.</div>
	<div class="tooltip-paragraph-fold"><span class="tr">For example, if the script is not compiled and is running BAS version <code>24.0.8</code> installed in the <code>"C:/BrowserAutomationStudio"</code> folder, then the action will return</span> <code>"C:/BrowserAutomationStudio/apps/24.0.8"</code>.</div>
	<div class="tr tooltip-paragraph-fold">For compiled unprotected and protected scripts, the paths are slightly different, since the unprotected script saves the engine to the appslocal folder, and the protected one to the appsremote folder.</div>
	<div class="tooltip-paragraph-fold"><span class="tr">For example, if the script is compiled without protection and is located in the <code>"D:/Gparser"</code> folder, then the action will return</span> <code>"D:/Gparser/appslocal/ibq8y9qo/SIDbfzc5bdv/engine"</code>.</div>
	<div class="tooltip-paragraph-last-fold"><span class="tr">For example, if the script is compiled with protection and is located in the <code>"D:/Gparser"</code> folder, then the action will return</span> <code>"D:/Gparser/appsremote/Gparser/SIDic9cmk8n/engine"</code>.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>