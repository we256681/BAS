<div class="container-fluid">
  <%= _.template($('#input_constructor').html())({id:"Value", description:tr("Ip address"), default_selector: "string", disable_int:true, help: 
{
	description: tr("A string which contains ip address. Ipv6 string should be enclosed with square brackets. Timezone and geolocation will be extracted from selected ip and browser settings will be updated accordingly."), examples:
	[
		{
			code:"140.97.75.15",description:tr("Ipv4 example")
		},
		{
			code:"<s>140.97.75.15:8080</s>",description:tr("Not correct, ip string can't contain port.")
		},
		{
			code:"[2a03:2880:f11c:8083:face:b00c:0:25de]",description:tr("Ipv6 example")
		}
	]
}
}) %>
</div>

<div class="container-fluid">
	<%= _.template($('#block_start').html())({id: "Advanced", name: tr("Security settings"), description: tr("Options below will help you to adjust different browser settings to match specified IP, for example: timezone and geolocation. Default settings will work fine.")}) %>


		<%= _.template($('#input_constructor').html())({id:"ChangeTimezone", description:tr("Change timezone"), default_selector: "string", disable_int:true, value_string: "true", variants: ["true","false"], help: {description: tr("Change browser timezone according to specified ip. For example, if ip is located in United Kingdom, then browser timezone offset will be equal to 0(UTC+00:00). You can set timezone to custom value with 'Timezone' module."),examples:[{code:"true",description:tr("Change timezone.")},{code:"false",description:tr("Don't change timezone.")}]}}) %>


		<%= _.template($('#input_constructor').html())({id:"ChangeGeolocation", description:tr("Change geolocation"), default_selector: "string", disable_int:true, value_string: "false", variants: ["true","false"], help: {description: tr("Change browser geolocation according to specified ip. Location will be set to a point which is close to ip longitude/latitude. You can set geolocation to custom value with 'Timezone' module."),examples:[{code:"true",description:tr("A browser request to access your geolocation will be accepted. Browser location will be set to ip location.")},{code:"false",description:tr("Don't change geolocation. A browser request to access your geolocation will be rejected.")}]}}) %>

		<%= _.template($('#input_constructor').html())({id:"ChangeWebrtcIp", description:tr("Change WebRTC ip"), default_selector: "string", disable_int:true, value_string: "true", variants: ["true","false"], help: {description: tr("Replace ip exposed by WebRTC with specified ip. If you want WebRTC to return custom ip, please use 'Browser Settings' action."),examples:[{code:"true",description:tr("Enable WebRTC, replace ip exposed by WebRTC with specified ip.")},{code:"false",description:tr("Don't change WebRTC state.")}]}}) %>

		<%= _.template($('#input_constructor').html())({id:"ChangeBrowserLanguage", description:tr("Change browser language"), default_selector: "string", disable_int:true, value_string: "true", variants: ["true","false"], help: {description: tr("Change browser language according to country of ip. This setting will change 'Accept-Language' header as well as navigator.language and navigator.languages javascript properties. By default language value will be equal to language and country code separated with dash, for example 'de-DE' for Germany. This value is valid, but most browsers uses more complicated strings. If you want to make BAS look like real browser, use FingerprintSwitcher service, it will set language to more natural value, for example for iPhone from Germany it will equal 'de, en;q=0.8, *;q=0.01'. You can also set this value explicitly by using 'Set Header' action"),examples:[{code:"true",description:tr("Change browser language according to country of specified ip.")},{code:"false",description:tr("Don't change browser language, default will be 'en-US,en;q=0.9'.")}]}}) %>

	<%= _.template($('#block_end').html())() %>
</div>

<div class="container-fluid">
	<%= _.template($('#ip_information').html())() %>
</div>
<div class="tooltipinternal">
      <div class="tr tooltip-paragraph-first-fold">Set browser timezone and position the same as timezone and position of selected ip.</div>
      <div class="tooltip-paragraph-fold"><span class="tr">Site can obtain information about system timezone and location using internal apis (</span><a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date/getTimezoneOffset');return false">example 1</a>, <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://developer.mozilla.org/en-US/docs/Web/API/Geolocation_API');return false">example 2</a><span class="tr">). This action allows to spoof timezone and location values returned by browser.</span></div>
      <div class="tooltip-paragraph-fold"><span class="tr">Full list of timezones can be found</span> <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://en.wikipedia.org/wiki/List_of_UTC_time_offsets');return false">here</a>.</div>
      <div class="tr tooltip-paragraph-fold">To obtain ip information internal database or external service is used, check "IP information" section for more details.</div>
      <div class="tr tooltip-paragraph-last-fold">It is recommended to use 'Proxy' action instead of this one, because it will set timezone and geolocation automatically according to current proxy.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>

 