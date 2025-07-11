<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"ua", description: "User-Agent", default_selector: "string", disable_int:true, value_string: "", help: {description: tr("String containing the User-Agent with want to parse."),examples:[{code:"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.198 Safari/537.36"},{code:"</br>Mozilla/5.0 (Android 11; Mobile; rv:68.0) Gecko/68.0 Firefox/82.0"},{code:"</br>Mozilla/5.0 (iPhone; CPU iPhone OS 14_2 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/14.0 Mobile/15E148 Safari/604.1"}]} }) %>
<div class="container-fluid">
	<div class="col-xs-12">
		<form class="form-horizontal">
			<div class="form-group">
				<div class="col-xs-12">
					<hr style="margin-top:1px;margin-bottom:1px"/>
				</div>
			</div>
		</form>
	</div>
</div>
<%= _.template($('#variable_constructor').html())({id:"platform_type", description:tr("Platform type"), default_variable: "PARSED_UA_PLATFORM_TYPE", help: {description: tr("This variable will contain the platform type."), examples:[{code:"desktop"},{code:"mobile"},{code:"tablet"},{code:"tv"},{code:"car"},{code:"bot"}]} }) %>
<div class="container-fluid">
	<div class="col-xs-12">
		<form class="form-horizontal">
			<div class="form-group">
				<div class="col-xs-12">
					<hr style="margin-top:1px;margin-bottom:1px"/>
				</div>
			</div>
		</form>
	</div>
</div>
<%= _.template($('#variable_constructor').html())({id:"browser_name", description:tr("Browser name"), default_variable: "PARSED_UA_BROWSER_NAME", help: {description: tr("This variable will contain the name of the browser."), examples:[{code:"Chrome"},{code:"Firefox"},{code:"Safari"}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"browser_version", description:tr("Browser version"), default_variable: "PARSED_UA_BROWSER_VERSION", help: {description: tr("This variable will contain the version of the browser."), examples:[{code:"86.0.4240.198"},{code:"82.0"},{code:"14.0"}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"browser_major_version", description:tr("Browser major version"), default_variable: "PARSED_UA_BROWSER_MAJOR_VERSION", help: {description: tr("This variable will contain the major version of the browser."), examples:[{code:86},{code:82},{code:14}]} }) %>
<div class="container-fluid">
	<div class="col-xs-12">
		<form class="form-horizontal">
			<div class="form-group">
				<div class="col-xs-12">
					<hr style="margin-top:1px;margin-bottom:1px"/>
				</div>
			</div>
		</form>
	</div>
</div>
<%= _.template($('#variable_constructor').html())({id:"engine_name", description:tr("Engine name"), default_variable: "PARSED_UA_ENGINE_NAME", help: {description: tr("This variable will contain the name of the browser engine."), examples:[{code:"Blink"},{code:"Gecko"},{code:"WebKit"}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"engine_version", description:tr("Engine version"), default_variable: "PARSED_UA_ENGINE_VERSION", help: {description: tr("This variable will contain the version of the browser engine."), examples:[{code:"86.0.4240.198"},{code:"82.0"},{code:"605.1.15"}]} }) %>
<div class="container-fluid">
	<div class="col-xs-12">
		<form class="form-horizontal">
			<div class="form-group">
				<div class="col-xs-12">
					<hr style="margin-top:1px;margin-bottom:1px"/>
				</div>
			</div>
		</form>
	</div>
</div>
<%= _.template($('#variable_constructor').html())({id:"os_name", description:tr("OS name"), default_variable: "PARSED_UA_OS_NAME", help: {description: tr("This variable will contain the name of the operating system."), examples:[{code:tr("Android")},{code:"Windows"},{code:"Mac OS"}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"os_version", description:tr("OS version"), default_variable: "PARSED_UA_OS_VERSION", help: {description: tr("This variable will contain the version of the operating system."), examples:[{code:tr("11")},{code:"10"},{code:"11.0.1"}]} }) %>
<div class="container-fluid">
	<div class="col-xs-12">
		<form class="form-horizontal">
			<div class="form-group">
				<div class="col-xs-12">
					<hr style="margin-top:1px;margin-bottom:1px"/>
				</div>
			</div>
		</form>
	</div>
</div>
<%= _.template($('#variable_constructor').html())({id:"device_vendor", description:tr("Device vendor"), default_variable: "PARSED_UA_DEVICE_VENDOR", help: {description: tr("This variable will contain the name of the device vendor."), examples:[{code:"Samsung"},{code:"Asus"},{code:"Apple"}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"device_model", description:tr("Device model"), default_variable: "PARSED_UA_DEVICE_MODEL", help: {description: tr("This variable will contain the device model."), examples:[{code:"SM-T865"},{code:"ASU2JS"},{code:"iPhone"}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"device_type", description:tr("Device type"), default_variable: "PARSED_UA_DEVICE_TYPE", help: {description: tr("This variable will contain the device type."), examples:[{code:"tablet"},{code:"desktop"},{code:"mobile"}]} }) %>
<div class="container-fluid">
	<div class="col-xs-12">
		<form class="form-horizontal">
			<div class="form-group">
				<div class="col-xs-12">
					<hr style="margin-top:1px;margin-bottom:1px"/>
				</div>
			</div>
		</form>
	</div>
</div>
<%= _.template($('#variable_constructor').html())({id:"cpu_architecture", description:tr("Processor architecture"), default_variable: "PARSED_UA_CPU_ARCHITECTURE", help: {description: tr("This variable will contain the processor architecture."), examples:[{code:tr("amd64")},{code:"ia[32/64]"},{code:"arm[64]"}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Parse the User-Agent string into items.</div>
	<div class="tr tooltip-paragraph-fold">This action will save each User-Agent item to its own variable.</div>
	<div class="tr tooltip-paragraph-last-fold">User-agent is a string containing information about the browser used, operating system, device. You can learn more about User-agent from <a href="#" onclick="BrowserAutomationStudio_OpenUrl('https://en.wikipedia.org/wiki/User_agent');return false">Wiki</a>.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
