<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({id:"Path", description:tr("Profile folder path"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("String with profile folder. Slash type is not important."), examples:[{code:"c:/path/to/profile"},{code:"c:\\path\\to\\profile"},{code:tr("Empty string"), description:tr("Deletes current profile")}]}}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Removes profile.</div>
	<div class="tr tooltip-paragraph-fold">You can delete current profile if you run it with empty profile path. This will stop browser. After deleting current profile, new temporary profile will be used.</div>
	<div class="tr tooltip-paragraph-last-fold">This action won't stop browser, only if you delete current profile.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
