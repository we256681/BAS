<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({id:"Path", description:tr("Profile folder path"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("String with profile folder. Slash type is not important. If folder doesn't exist, it will be created. If folder already exists, BAS will use it as profile and restore all data from it like cookies, localstorage, etc"), examples:[{code:"c:/path/to/profile"},{code:"c:\\path\\to\\profile"}]} }) %>
	<%= _.template($('#input_constructor').html())({id:"LoadFPFromFolder", description:tr("Always load fingerprint from profile folder"), default_selector: "string", disable_int:true, value_string: "true", variants: ["true","false"], help: {description: tr("In case if profile folder already exists and has fingerprint data, tells BAS to apply fingerpint used latest for that profile.")}}) %>
	
	<%= _.template($('#input_constructor').html())({id:"LoadProxyFromFolder", description:tr("Always load proxy from profile folder"), default_selector: "string", disable_int:true, value_string: "true", variants: ["true","false"], help: {description: tr("In case if profile folder already exists and has proxy data, tells BAS to apply proxy used latest for that profile.")}}) %>
</div>

<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Tells browser to use specified folder as a place to store cookies, cache, localstorage, etc.</div>
	<div class="tr tooltip-paragraph-fold">If specified folder doesn't exist it will be created.</div>
	<div class="tr tooltip-paragraph-fold">If specified folder already exists, than browser will load all data from it.</div> 
	<div class="tr tooltip-paragraph-fold">Switching profile requires browser restart, so this action may restart browser in case if it exists. Restarting browser will also reset all browser settings.</div>
	<div class="tr tooltip-paragraph-last-fold">You can implement autologin with this action, first you need to specify some folder and do login with browser, then you may restart and specify same folder again. Browser will load all data from previous session and you don't need to login second time.</div>
</div>

<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>