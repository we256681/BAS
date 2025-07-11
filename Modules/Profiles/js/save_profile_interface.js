<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({id:"Path", description:tr("Profile folder path"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("String with profile folder. Slash type is not important. It is better to use empty or non existing folder here."), examples:[{code:"c:/path/to/profile"},{code:"c:\\path\\to\\profile"}]}}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Copy your current profile to other folder.</div>
	<div class="tr tooltip-paragraph-fold">This action doesn't close browser and doesn't switch profile path to new location.</div> 
	<div class="tr tooltip-paragraph-last-fold">Best place to use this action is thread end, if you want to preserve your cookies and don't login to sites that you visited when running BAS later.</div>
</div>

<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
