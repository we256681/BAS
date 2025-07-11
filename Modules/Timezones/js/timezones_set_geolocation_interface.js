<div class="container-fluid">
  <%= _.template($('#input_constructor').html())({id:"Latitude", description:tr("Latitude. Can be blank"), default_selector: "expression", disable_int:true, value_string: "", help: 
		{
			description: tr("Exact coordinates, which identifies your position. This value will be returned by browser as a response on location request from site."), examples:
			[
				{
					code:tr("Empty string"),description:tr("Don't set latitude")
				},
				{
					code:"10.433"
				}
			]
		}
 	}) %>
  <%= _.template($('#input_constructor').html())({id:"Longitude", description:tr("Longitude. Can be blank"), default_selector: "expression", disable_int:true, value_string: "", help: 
		{
			description: tr("Exact coordinates, which identifies your position. This value will be returned by browser as a response on location request from site."), examples:
			[
				{
					code:tr("Empty string"),description:tr("Don't set longitude")
				},
				{
					code:"10.433"
				}
			]
		} }) %>
  

   <%= _.template($('#input_constructor').html())({id:"GeolocationObject", description:tr("Geolocation object"), default_selector: "expression", disable_int:true, disable_editor:true, disable_string:true, use_textarea:true, size: 8, disable_type_chooser:true,textarea_height:180, value_string: "{\n   accuracy: 4413,\n   altitude: null,\n   altitudeAccuracy: null,\n   heading: null,\n  speed: null\n}", help: {description: tr("This field contains properties of returned gelocation object. By changing this object, you can tweak, for example, accuracy property, or others. This object should not include latitude and longitude, they are set through separate settings.")}}) %>

</div>

<div class="tooltipinternal">
      <div class="tr tooltip-paragraph-first-fold">Set browser location. Geolocation can be changed by "Proxy" action as well.</div>
      <div class="tooltip-paragraph-fold"><span class="tr">Site can obtain information about your position using </span><a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://developer.mozilla.org/en-US/docs/Web/API/Geolocation_API');return false">following api</a><span class="tr">. This action allows to spoof position returned by browser.</span></div>
      <div class="tr tooltip-paragraph-fold">Using this action is not necessary to hide your location, because coordintes can be obtained only after user accepts request from site in a special popup window. By default, if you don't use this action, this popup is rejected by BAS.</div>
	  <div class="tr tooltip-paragraph-fold">Leave any field blank if you don't want to change it.</div>
	  <div class="tooltip-paragraph-fold"><span class="tr">Use "Geolocation object" property to tweak resulting object </span><a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://developer.mozilla.org/en-US/docs/Web/API/GeolocationCoordinates');return false">GeolocationCoordinates</a></div>


      <div class="tr tooltip-paragraph-last-fold">It is recommended to use 'Proxy' action instead of this one, because it will set position automatically according to current proxy.</div>
</div>


<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>