<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"path", description: tr("Path"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("The path that need to check out whether it is absolute."),examples:[{code:"/foo/bar", description: tr("Absolute path")},{code:"C:/foo/..", description: tr("Absolute path")},{code:"bar/baz", description: tr("Not absolute path")},{code:"qux/", description: tr("Not absolute path")}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "IS_ABSOLUTE_PATH", help: {description: tr("This variable will be true or false depending on whether the specified path is absolute."), examples:[{code:"true",description:tr("The path is absolute.")},{code:"false",description:tr("The path is not absolute.")}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Check if the specified path is absolute.</div>
	<div class="tr tooltip-paragraph-fold">This action will return true or false depending on whether the specified path is absolute. This result may be used with "If" action.</div>
	<div class="tr tooltip-paragraph-fold">For example, if the path is <code>"/foo/bar"</code>, then the action will return true, and if the path is specified <code>"bar/baz"</code> or <code>"qux/"</code>, then the action will return false.</div>
	<div class="tr tooltip-paragraph-last-fold">If the "Path" parameter is not a string, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
