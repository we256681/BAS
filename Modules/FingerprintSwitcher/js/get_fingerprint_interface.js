<div class="container-fluid">
  <%= _.template($('#input_constructor').html())({id:"Tags", description:tr("Tags"), default_selector: "string", disable_int:true, value_string:"Microsoft Windows,Chrome", variants: ["*", 'Desktop', 'Mobile', 'Microsoft Windows', 'Microsoft Windows Phone', 'Apple Mac', 'Android', 'Linux', 'iPad', 'iPhone', 'Edge', 'Chrome', 'Safari', 'Firefox', 'IE', 'YaBrowser', 'Windows 10', 'Windows 7', 'Windows 8'], help: 
		{
			description: tr("Select system and device for which you want to obtain fingerprint. Several tags may be combined together, in that case service will return only fingerprints, which matches all tags."), examples:
			[
				{
					code:"Chrome",description:tr("Select Chrome fingerprint for any platform")
				},
				{
					code:"Microsoft Windows,Chrome",description:tr("Select Chrome fingerprint for Windows. This is the only type of fingerprint which can be used for free.")
				},
				{
					code:tr("Empty string"),description:tr("Select any fingerprint")
				}
			]
		} }) %>
  <%= _.template($('#input_constructor').html())({id:"Key", description:tr("Service key. Can be empty"), default_selector: "string", disable_int:true, help: 
	{
		description: tr("Fingerprints are fetched from FingerprintSwitcher service, in order to use that service, you need to buy a key(see the link below). However, you may use it for free with Microsoft Windows,Chrome tags and with requests limits. In order to use free version, leave this field blank."), examples:
		[
			{
				code:"pGGeNdza2e0gIw48oa",description:tr("Key example")
			},
			{
				code:tr("Empty string"),description:tr("Use a free version")
			}
		]
	} }) %>
  <div style="margin-bottom:5px;margin-left:20px;font-size:small">
  	<span class="tr">This action works with</span> 
  	<a href="#"  onclick="BrowserAutomationStudio_OpenUrl('https://fingerprints.bablosoft.com'); return false;">
  		<span class="tr">Fingerprint switcher service</span></a>. 
  	<span class="tr">You can get a key there</span>. 
  </div>

  <%= _.template($('#block_start').html())({id: "PerfectCanvasBlock", name: "PerfectCanvas", description: 
  `
	<span>${tr('With the help of PerfectCanvas technology, canvas data is replaced in the most accurate way possible. Data is drawn on real devices.')}</span> 
	<a href="#" class="tr-en"  onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/doku.php?id=perfectcanvas'); return false;">
		<span>${tr('Learn more.')}</span>
	</a>
	<a href="#" class="tr-ru"  onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/doku.php?id=ru:perfectcanvas'); return false;">
		<span>${tr('Learn more.')}</span>
	</a>
	<span>${tr('If you want to use PerfectCanvas, you only need to fill this field:')}</span> 
  `}) %>


  
		 <%= _.template($('#input_constructor').html())({id:"PerfectCanvasRequest", description:tr("PerfectCanvas request. Can be blank."), default_selector: "string", disable_int:true, disable_expression:true, value_string:"", use_textarea:true, disable_type_chooser:true,textarea_height:80,
		 help: 
		 {
			 description: tr("PerfectCanvas request contains all data required to render canvas on remote machine. In order to obtain request, use CanvasInspector application, see wiki for more information and download link. \"PerfectCanvas request\" data is obtained one time for site, not for each fingerprint request."), examples:
			 [
				 {
					code:tr("Empty string"),description:tr("Use standard fingerprint. Canvas data will be replaced, by adding noise.")
				 },
				 {
					code:tr("PerfectCanvas request"),description:tr("Use fingerprint with PerfectCanvas. All canvas data, which matches request will be replaced with images from real devices. Any other canvas data will be replaced by adding noise.")
				 }
			 ]
		 }
		  }) %>
		 <div style="margin-bottom:5px;margin-left:20px;font-size:small">
			<a href="#" class="tr-en"  onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/doku.php?id=perfectcanvas#how_to_obtain_perfectcanvas_request'); return false;">
				<span class="tr">How to get PerfectCanvas request?</span>
			</a>
			<a href="#" class="tr-ru"  onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/doku.php?id=ru:perfectcanvas#%D0%BA%D0%B0%D0%BA_%D0%BF%D0%BE%D0%BB%D1%83%D1%87%D0%B8%D1%82%D1%8C_%D0%B7%D0%B0%D0%BF%D1%80%D0%BE%D1%81_perfectcanvas'); return false;">
				<span class="tr">How to get PerfectCanvas request?</span>
			</a>

			
		</div>

		<%= _.template($('#input_constructor').html())({id:"PerfectCanvasDebug", description:tr("PerfectCanvas logs"), default_selector: "string", disable_int:true, value_string: "false", variants: ["true", "false"],
		 help: 
		 {
			 description: tr("Is it necessary to add logs while obtaining fingerprint with PerfectCanvas."), examples:
			 [
				 {
					code:"true",description:tr("Add logs, may be useful for diagnostic.")
				 },
				 {
					code:"false",description:tr("Disable logs, fingerprint will be obtained without any messages.")
				 }
			 ]
		 } }) %>

		<%= _.template($('#input_constructor').html())({id:"EnablePrecomputedFingerprints", description:tr("Enable precomputed fingerprints with PerfectCanvas"), default_selector: "string", disable_int:true, value_string: "true", variants: ["true", "false"],
		 help: 
		 {
			 description: tr("In case if fingerprint with specified PerfectCanvas request is not present in static database, canvas data will be rendered in real time from machines which are currently connected. This is default behavior, but sometimes you may avoid quering static database and use dynamic rendering instantly. In order to do that, set this value to false. This settings has no effect if PerfectCanvas request is not set or if CustomServers are used."), examples:
			 [
				 {
					code:"true",description:tr("Fingerprints with specified PerfectCanvas request will be searched inside static database, if not found, canvas data will be rendered in real time from machines which are currently connected.")
				 },
				 {
					code:"false",description:tr("Fingerprints with specified PerfectCanvas request will be rendered in real time from machines which are currently connected.")
				 }
			 ]
		 } }) %>
		
	<%= _.template($('#block_end').html())() %>

	<%= _.template($('#block_start').html())({id: "Advanced", name: tr("Options"), description: tr("")}) %>

  		<%= _.template($('#input_constructor').html())({id:"AddedDate", description:tr("Minimum added date"), default_selector: "string", disable_int:true, value_string:"*", variants: ["*", '15 days', '30 days', '60 days'], help: 
		{
			description: tr("Select only fingerprints, which added date matches certain condition"), examples:
			[
				{
					code:"*",description:tr("Doesn't filter on added date")
				},
				{
					code:"15 days",description:tr("Select only fingerprints, which was added to database no more than 15 days ago")
				}
			]
		} }) %>
		<%= _.template($('#input_constructor').html())({id:"BrowserVersion", description:tr("Minimum browser version"), default_selector: "string", value_string:"*", variants: 
		[
			"*", 
			"111<br/><span style='color:gray;font-size:small'>" + tr("Specific browser version or higher") + "</span>",
			"current<br/><span style='color:gray;font-size:small'>" + tr("Browser version equals to BAS browser version or higher") + "</span>"
		], help: 
		{
			description: tr("Select only fingerprints, which has certain browser version. It is recommended to use this option together with explicitly specifying browser name. For example, you can select fingerprints for Chrome browser with version greater than 75. By setting \"Minimum browser version\" and \"Maximum browser version\" to same value, you can select exact version. If you use \"current\" as input parameter for both minimum and maximum browser version, it will help to avoid discrepancy between BAS browser version and browser version from fingerprint. Therefore browser authenticity will be significantly increased."), examples:
			[
				{
					code:"*",description:tr("Doesn't filter on browser version")
				},
				{
					code:"111",description:tr("Browser version must be equal or greater than 111")
				},
				{
					code:"current",description:tr("Browser version must be equal or greater than BAS browser version")
				}
			]
		} }) %>
		<%= _.template($('#input_constructor').html())({id:"MaxBrowserVersion", description:tr("Maximum browser version"), default_selector: "string", value_string:"*", variants: 
		[
			"*", 
			"111<br/><span style='color:gray;font-size:small'>" + tr("Specific browser version or lower") + "</span>",
			"current<br/><span style='color:gray;font-size:small'>" + tr("Browser version equals to BAS browser version or lower") + "</span>"
		], help: 
		{
			description: tr("Select only fingerprints, which has certain browser version. It is recommended to use this option together with explicitly specifying browser name. For example, you can select fingerprints for Chrome browser with version less than 75. By setting \"Minimum browser version\" and \"Maximum browser version\" to same value, you can select exact version. If you use \"current\" as input parameter for both minimum and maximum browser version, it will help to avoid discrepancy between BAS browser version and browser version from fingerprint. Therefore browser authenticity will be significantly increased."), examples:
			[
				{
					code:"*",description:tr("Doesn't filter on browser version")
				},
				{
					code:"111",description:tr("Browser version must be equal or less than 111")
				},
				{
					code:"current",description:tr("Browser version must be equal or less than BAS browser version")
				}
				
			]
		} }) %>
		<%= _.template($('#input_constructor').html())({id:"MinimumWidth", description:tr("Minimum browser width"), default_selector: "string", value_string:"*", variants: ["*", "1600"], help: 
		{
			description: tr("Select only fingerprints, which has minimum browser width."), examples:
			[
				{
					code:"*",description:tr("Doesn't filter on browser width")
				},
				{
					code:"1600",description:tr("Browser width must be equal or greater than 1600")
				}
			]
		} }) %>
		<%= _.template($('#input_constructor').html())({id:"MinimumHeight", description:tr("Minimum browser height"), default_selector: "string", value_string:"*", variants: ["*", "900"], help: 
		{
			description: tr("Select only fingerprints, which has minimum browser height."), examples:
			[
				{
					code:"*",description:tr("Doesn't filter on browser height")
				},
				{
					code:"900",description:tr("Browser height must be equal or greater than 900")
				}
			]
		} }) %>

		<%= _.template($('#input_constructor').html())({id:"MaximumWidth", description:tr("Maximum browser width"), default_selector: "string", value_string:"*", variants: ["*", "2000"], help: 
		{
			description: tr("Select only fingerprints, which has maximum browser width."), examples:
			[
				{
					code:"*",description:tr("Doesn't filter on browser width")
				},
				{
					code:"2000",description:tr("Browser width must be equal or less than 2000")
				}
			]
		} }) %>
		<%= _.template($('#input_constructor').html())({id:"MaximumHeight", description:tr("Maximum browser height"), default_selector: "string", value_string:"*", variants: ["*", "1200"], help: 
		{
			description: tr("Select only fingerprints, which has maximum browser height."), examples:
			[
				{
					code:"*",description:tr("Doesn't filter on browser height")
				},
				{
					code:"1200",description:tr("Browser height must be equal or less than 1200")
				}
			]
		} }) %>
		
	<%= _.template($('#block_end').html())() %>

	<%= _.template($('#block_start').html())({id: "CustomServersBlock", name: tr("Custom servers"), description: tr(`
	<span>${tr('If you are the site owner, you can collect fingerprints from users of your site.')}</span> 
	<a href="#" class="tr-en"  onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/doku.php?id=customservers'); return false;">
		<span>${tr('Learn more.')}</span>
	</a>
	<a href="#" class="tr-ru"  onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/doku.php?id=ru:customservers'); return false;">
		<span>${tr('Learn more.')}</span>
	</a>
	
	`)}) %>

	<%= _.template($('#input_constructor').html())({id:"EnableCustomServers", description:tr("Use custom server"), default_selector: "string", disable_int:true, value_string: "false", variants: ["true", "false"],
		help: 
		{
			description: tr("This option is useful only if you have custom server functionality enabled in your account. Otherwise, it will always give error. If you have this setting enabled, fingerprint will be obtained only from custom server. It is compatible with PerfectCanvas. Check wiki for more info."), examples:
			[
				{
				code:"true",description:tr("Use custom server to obtain fingerprint.")
				},
				{
				code:"false",description:tr("Obtain fingerprint from common server.")
				}
			]
		} }) %>

	<%= _.template($('#input_constructor').html())({id:"DynamicPerfectCanvas", description:tr("PerfetCanvas rendering in real time"), default_selector: "string", disable_int:true, value_string: "true", variants: ["true", "false"],
		help: 
		{
			description: tr("In case if fingerprint with specified PerfectCanvas request is not present in static database, canvas data will be rendered in real time from machines which are currently connected. This is default behavior, but sometimes you may avoid dynamic rendering to save time or for some other reason. In order to do that, set this value to false. This settings has no effect if PerfectCanvas request is not set."), examples:
			[
				{
				code:"true",description:tr("Fingerprints with specified PerfectCanvas request will be searched inside static database, if not found, canvas data will be rendered in real time from machines which are currently connected.")
				},
				{
				code:"false",description:tr("Fingerprints with specified PerfectCanvas request will be searched inside static database, if not found, error will occur.")
				}
			]
		} }) %>

	<%= _.template($('#block_end').html())() %>

	<%= _.template($('#block_start').html())({id: "Support", name: tr("Researching sites protection"), description: `
		<span>
			<span>${tr('Use following page to report about anti-bot detection on particular site, ')}</span>
			<a href="#" onclick="BrowserAutomationStudio_OpenUrl('https://community.bablosoft.com/topic/12199/'); return false;" class="tr-en">link</a>
			<a href="#" onclick="BrowserAutomationStudio_OpenUrl('https://community.bablosoft.com/topic/12198/'); return false;" class="tr-ru">ссылка</a>
		
		</span>
	`}) %>
		
	<%= _.template($('#block_end').html())() %>


  <%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable To Save"), default_variable: "FINGERPRINT", help: 
	{
		description: tr("This variable will contain fingerprint string. Fingerprint string contains all data necessary to change fingerprint. It can be used immediately with 'Apply fingerprint' action. It also can be saved to a text file or database and reused later.")
	}}) %>

</div>


<div class="tooltipinternal">
      <div class="tr tooltip-paragraph-first-fold">Get browser fingerprint, which contains: user agent, screen size, navigator, fonts and many more.</div>
	<div class="tr tooltip-paragraph-fold">After execution of this action will complete, [[FINGERPRINT]] variable will contain fingerprint string. Fingerprint string contains all data necessary to change fingerprint. It can be used immediately with 'Apply fingerprint' action. It also can be saved to a text file or database and reused later.</div>
      <div class="tr tooltip-paragraph-fold">Using this action is like user agent switcher on steroids. It changes not only user agent, but a lot of browser internals to make BAS actually look like firefox, chrome, safari, on desktop or on mobile.</div>
      <div class="tr tooltip-paragraph-fold">Several additional features may be used to make your browser less detectable. Apply proxy through 'Proxy' action. Change timezone with 'Set timezone and geolocation by ip' action. Use, store and reuse profiles, because starting browser on newly created profiles may be suspicious.</div>
      <div class="tooltip-paragraph-fold"><span class="tr">Fingerprints are fetched from</span> <a href="#"  onclick="BrowserAutomationStudio_OpenUrl('https://fingerprints.bablosoft.com'); return false;">
  		FingerprintSwitcher</a> <span class="tr">service, in order to use that service, you need to </span>
		<a href="#"  onclick="BrowserAutomationStudio_OpenUrl('https://bablosoft.com/shop/FingerprintSwitcher'); return false;" class="tr">buy a key</a>.
      </div>
      <div class="tr tooltip-paragraph-fold">However, you may get fingerprints for free with Microsoft Windows,Chrome tags and with requests limits. In order to run free version, leave 'Service key' field blank.</div>
      <div class="tr tooltip-paragraph-fold">Fingerprint service fetches data from a real browsers on real devices, so you may be sure, that fingerprints are always fresh - contain actual user agent, plugin versions, etc.</div>
      <div class="tr tooltip-paragraph-fold">Applying fingerprint doesn't require browser restart, so it won't reset your settings.</div>

	  <div class="tooltip-paragraph-fold"><span class="tr">By default FingerprintSwitcher adds noise to canvas data. If you want to have more accurate results, use PerfectCanvas technology. It renders canvas data on real device and then replaces canvas fingerprinting results inside your browser. In order to use it, some preparation must be made. You need to fill "PerfectCanvas request" field. Data for this field can be obtained by using CanvasInspector application. This application is free and easy to install, it allows to get PerfectCanvas request for site that you working with. More info and download link can be found</span> 
	  <a href="#" class="tr-en tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/doku.php?id=perfectcanvas#how_to_obtain_perfectcanvas_request'); return false;">here</a>
	  <a href="#" class="tr-ru tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/doku.php?id=ru:perfectcanvas#%D0%BA%D0%B0%D0%BA_%D0%BF%D0%BE%D0%BB%D1%83%D1%87%D0%B8%D1%82%D1%8C_%D0%B7%D0%B0%D0%BF%D1%80%D0%BE%D1%81_perfectcanvas'); return false;">here</a>.</div>

	  <div class="tr tooltip-paragraph-fold">In order to replace fingerprint with PerfectCanvas, it is sufficient to call this action with "PerfectCanvas request" field filled out, and then use fingerprint by calling "Apply fingerprint" action.</div>

	  <div class="tr tooltip-paragraph-fold">"PerfectCanvas request" data is obtained one time for site, not for each fingerprint request.</div>

	  <div class="tr tooltip-paragraph-fold">If you leave "PerfectCanvas request" field empty, this action will return standart fingerprint with noise canvas replacement.</div>
	  <div class="tr tooltip-paragraph-fold">Getting fingerprint with PerfectCanvas may require some time depending on request queue length and tag combination that you are using. If you are using combination of tags which are rare, this can be slow. Use * as input for "Tags" field to speed up process. Obtaining fingerprint for popular service may also be slow.</div>
	  <div class="tr tooltip-paragraph-fold">Getting fingerprint with PerfectCanvas can only performed in single thread, you can still use several threads, but rest of the threads will be waiting, while only one will work.</div>

	  <div class="tr tooltip-paragraph-fold">If you have "PerfectCanvas request" field prepared for "site A", and you visit "site B" then canvas data for "site B" will be replaced by adding noise. If you want canvas data to be replaced for both sites, you have to prepare request for both "site A" and "site B".</div>

	  <div class="tr tooltip-paragraph-fold">In order to check if data for PerfectCanvas was actually replaced check log at BrowserAutomationStudio/apps/CURRENT_BAS_VERSION/s/PROCESS_ID.txt file.</div>


	  <div class="tooltip-paragraph-fold"><span class="tr">If you want to select fingerprint with specific browser version, set "Minimum browser version" and "Maximum browser version" parameter to same value. Instead of selecting specific version, you can use special word "current". By using "current" in both minium and maximum version parameters you can unsure that BAS browser version will match browser version from fingerprint. Therefore browser authenticity will be significantly increased.</span>
		<span class="tr-en">
			<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=browserautomationstudio_2023-04-30_07-34-05.png'); return false;">screen</a>.
		</span>
		<span class="tr-ru">
			<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=browserautomationstudio_2023-04-30_08-24-55.png'); return false;">скрин</a>.
		</span>
	  </div>

	  <div class="tr tooltip-paragraph-fold">Each BAS version may be shipped with one or several browser versions. You can change browser version for current thread, in order to do that use "Browser Settings" action with "Browser Version" parameter. After browser version for current thread will be changed, using special word "current" will select fingerprints of changed version.</div>


      <div class="tooltip-paragraph-last-fold"><span class="tr">You can find more information and get test fingerprints on following page</span> <a href="#"  onclick="BrowserAutomationStudio_OpenUrl('https://fingerprints.bablosoft.com'); return false;">
  		FingerprintSwitcher</a>.</div>
</div>


<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
