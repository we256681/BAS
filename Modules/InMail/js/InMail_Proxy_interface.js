<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({
		id: "proxyString",
		description: tr("Proxy String"),
		default_selector: "string",
		disable_int: true,
		help: {
			description: tr("String with information about proxy. It may contain ip, port, proxy type in different formats. This string may also contain login and password, if it doesn't, auth can be set with \"Proxy Login\" and \"Proxy Password\" parameters."),
			examples: [
				{code: "210.10.10.10:1085"},
				{code: "username:password@210.10.10.10:1085"},
				{code: "socks5://210.10.10.10:1085"},
				{code: "socks:210.10.10.10:1085:username:password"},
				{code: "http:username:password:210.10.10.10:1085"},
				{code: "{{proxy}}", description: tr("Get from resource")},
				{code: tr("Empty string"), description: tr("Without proxy")}
			]
		}
	}) %>
	<%= _.template($('#input_constructor').html())({
		id: "proxyType",
		description: tr("Proxy Type"),
		default_selector: "string",
		disable_int: true,
		value_string: "http",
		variants: [
			"http",
			"socks5",
			{value: "auto", description: tr("Proxy type will be extracted from proxy line")}
		],
		help: {
			description: tr("socks5 and http proxy types are supported."),
			examples: [
				{code: "socks"},
				{code: "socks5", description: tr("Same as socks")},
				{code: "http"},
				{code: "https", description: tr("Same as http")}
			]
		}
	}) %>
	<%= _.template($('#input_constructor').html())({
		id: "proxyUsername",
		description: tr("Proxy Login. Can be blank."),
		default_selector: "string",
		disable_int: true,
		help: {
			description: tr("Proxy login, overrides login set in proxy string. Useful if you have many proxy with same login and password.")
		}
	}) %>
	<%= _.template($('#input_constructor').html())({
		id: "proxyPassword",
		description: tr("Proxy password. Can be blank."),
		default_selector: "string",
		disable_int: true,
		help: {
			description: tr("Proxy password, overrides password set in proxy string. Useful if you have many proxy with same login and password.")
		}
	}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">By default mail client works without proxy, this action sets proxy for mail client.</div>
	<div class="tr tooltip-paragraph-fold">Use resources if you want to take them from file, url or database.</div>
	<div class="tr tooltip-paragraph-fold">You can change proxy once and it will be applied to every action which works with email.</div>
	<div class="tr tooltip-paragraph-last-fold">Proxy with empty parameter will reset it to default value(direct connection).</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
