<div class="container-fluid">
    <%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "PROJECT_DIRECTORY", help: {description: tr("Variable in which, after successful execution of the action, the path to the project directory will be written."), examples:[{code:"D:/projects",description:tr("Not compiled script")},{code:"D:/Gparser",description:tr("Unprotected compiled script") + "<strong>/</strong>" + tr("Protected compiled script")}]}}) %>
</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Get the full path to the project directory.</div>
	<div class="tr tooltip-paragraph-last-fold">This action will return the full path to the project directory. If the script is not compiled, then this action will return the path to the directory in which the open project file is located. And if the script is compiled, then this action will return the path to the root directory of the script.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>