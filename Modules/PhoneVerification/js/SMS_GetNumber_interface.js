<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({
		id: "service",
		description: tr("Service"),
		default_selector: "string",
		variants: [
			"sms-activate.org<br/><span style='color:gray'>" + tr("Ex sms-activate.ru") + "</span>",
			"smshub.org",
			"5sim.net",
			"onlinesim.ru",
			"sms-acktiwator.ru",
			"sms-reg.com",
			"vak-sms.com",
			"give-sms.com",
			"getsms.online",
			"sms-man.ru",
			"activation.pw",
			"cheapsms.ru",
			"smsvk.net",
			"smscode.me<br/><span style='color:gray'>" + tr("Ex sms.kopeechka.store") + "</span>",
			"365sms.org<br/><span style='color:gray'>" + tr("Ex 365sms.ru") + "</span>",
			"smspva.com",
			"simsms.org"
		],
		disable_int: true,
		value_string: "sms-activate.org",
		help: {
			description: tr("Service for receiving SMS, from which need to get a phone number."),
			examples: [
				{code: "sms-activate.org", description: "https://sms-activate.org"},
				{code: "smshub.org", description: "https://smshub.org"},
				{code: "sms-reg.com", description: "https://sms-reg.com"}
			]
		}
	}) %>
	<%= _.template($('#input_constructor').html())({
		id: "apiKey",
		description: tr("API key"),
		default_selector: "string",
		disable_int: true,
		value_string: "",
		help: {
			description: tr("API key of the SMS receiving service. The key for the service selected in the \"Service\" parameter. Depending on the service, you can get it in your personal account or in the service settings."),
			examples: [
				{code: "8b1a9953c4611296a827abf8c47804d7"},
				{code: "79916U5718g2266a7bff7fad356c6cb280b3ea"},
				{code: "f4d559ba78aa6c4701c1995ae9977c03"}
			]
		}
	}) %>
	<div class="col-xs-12">
		<form class="form-horizontal">
		  <div class="form-group">
			<div class="col-xs-2" style="width: auto;">
			  <div class="input-group">
				<span data-preserve="true" data-preserve-type="select" data-preserve-id="site">
				  <select class="form-control input-sm" id="site" placeholder="Site">
					<option class="tr" title="sms-activate.org, smshub.org, 5sim.net, getsms.online, smsvk.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, smscode.me, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Other" selected="selected">Other</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, getsms.online, smsvk.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, smscode.me, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="VK">VK</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, getsms.online, smsvk.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, smscode.me, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Telegram">Telegram</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, getsms.online, smsvk.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, smscode.me, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Microsoft">Microsoft</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, getsms.online, smsvk.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, smscode.me, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="WhatsApp">WhatsApp</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, getsms.online, smsvk.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, smscode.me, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Facebook">Facebook</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, getsms.online, smsvk.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, smscode.me, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Instagram">Instagram</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, getsms.online, smsvk.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, smscode.me, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Viber">Viber</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, getsms.online, smsvk.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Yahoo">Yahoo</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, getsms.online, smsvk.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Aol">Aol</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, getsms.online, smsvk.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Uber">Uber</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, getsms.online, smsvk.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Twitter">Twitter</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, getsms.online, smsvk.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="WeChat">WeChat</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, getsms.online, smsvk.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Google">Google</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, getsms.online, smsvk.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="KakaoTalk">KakaoTalk</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, smsvk.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, smscode.me, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="Imo">Imo</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, smsvk.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="TikTok">TikTok</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, getsms.online, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Tinder">Tinder</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, getsms.online, smsvk.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Naver">Naver</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, smsvk.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Mamba">Mamba</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, getsms.online, smsvk.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, give-sms.com" value="Ok.ru">Ok.ru</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, getsms.online, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Discord">Discord</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, smscode.me, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Apple">Apple</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, getsms.online, smsvk.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, give-sms.com" value="Mail.ru">Mail.ru</option>
					<option title="sms-activate.org, smshub.org, activation.pw, smspva.com, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Mail.ru Group">Mail.ru Group</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, getsms.online, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Yandex">Yandex</option>
					<option title="sms-activate.org, smshub.org, smsvk.net, cheapsms.ru, 365sms.org, activation.pw, sms-acktiwator.ru" value="YandexGo">YandexGo</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="Yandex.Eda">Yandex.Eda</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, smscode.me, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Amazon">Amazon</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, getsms.online, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Steam">Steam</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, getsms.online, smsvk.net, 365sms.org, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Avito">Avito</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, smsvk.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Netflix">Netflix</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Magnit">Magnit</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, smsvk.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="ProtonMail">ProtonMail</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, smsvk.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, give-sms.com" value="Alibaba">Alibaba</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="DiDi">DiDi</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, smsvk.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="X5ID">X5ID</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, getsms.online, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="BlaBlaCar">BlaBlaCar</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Aliexpress">Aliexpress</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Bolt">Bolt</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="LINE Messenger">LINE Messenger</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, smsvk.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru" value="Gett">Gett</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Blizzard">Blizzard</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="OZON">OZON</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Yalla">Yalla</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Craigslist">Craigslist</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="LinkedIN">LinkedIN</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Grindr">Grindr</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, getsms.online, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="OLX">OLX</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Airbnb">Airbnb</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, smsvk.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru, give-sms.com" value="CityMobil">CityMobil</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, smsvk.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="Drom.ru">Drom.ru</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="DrugVokrug">DrugVokrug</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="PayPal">PayPal</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Snapchat">Snapchat</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, onlinesim.ru, sms-acktiwator.ru, give-sms.com" value="Samokat">Samokat</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, smsvk.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="Skout">Skout</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="CoinBase">CoinBase</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Nike">Nike</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Tencent QQ">Tencent QQ</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Weibo">Weibo</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="Glovo">Glovo</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="eBay">eBay</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, onlinesim.ru, sms-acktiwator.ru, give-sms.com" value="Delivery Club">Delivery Club</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, onlinesim.ru, vak-sms.com, give-sms.com" value="Wildberries">Wildberries</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="SportMaster">SportMaster</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, onlinesim.ru, vak-sms.com, give-sms.com" value="Twitch">Twitch</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Careem">Careem</option>
					<option title="sms-activate.org, smshub.org, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="MVideo">MVideo</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, vak-sms.com, give-sms.com" value="DodoPizza">DodoPizza</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Adidas">Adidas</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, onlinesim.ru, vak-sms.com, give-sms.com" value="Lenta">Lenta</option>
					<option title="sms-activate.org, smshub.org, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, vak-sms.com, give-sms.com" value="SberMarket">SberMarket</option>
					<option title="sms-activate.org, smshub.org, cheapsms.ru, 365sms.org, sms-man.ru, onlinesim.ru, give-sms.com" value="SberMegaMarket">SberMegaMarket</option>
					<option title="sms-activate.org, smshub.org, cheapsms.ru, 365sms.org, onlinesim.ru, give-sms.com" value="SberApteka">SberApteka</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Zoho">Zoho</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="Taobao">Taobao</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="inDriver">inDriver</option>
					<option title="sms-activate.org, smshub.org, cheapsms.ru, 365sms.org, sms-man.ru, sms-reg.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="KFC">KFC</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="Deliveroo">Deliveroo</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="FoodPanda">FoodPanda</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, sms-reg.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Burger King">Burger King</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, smsvk.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="Kwai">Kwai</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="JD.com">JD.com</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="AliPay">AliPay</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, onlinesim.ru, sms-acktiwator.ru" value="ICQ">ICQ</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="MiChat">MiChat</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="Wolt">Wolt</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="Shopee">Shopee</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="myGLO">myGLO</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="IQOS">IQOS</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Amasia">Amasia</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, cheapsms.ru, 365sms.org, activation.pw, sms-reg.com, onlinesim.ru, sms-acktiwator.ru" value="SEOsprint">SEOsprint</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, vak-sms.com, give-sms.com" value="BIGO LIVE">BIGO LIVE</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru, give-sms.com" value="Zalo">Zalo</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, getsms.online, 365sms.org, activation.pw, sms-reg.com, sms-acktiwator.ru, give-sms.com" value="Youla">Youla</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="Rambler">Rambler</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, onlinesim.ru, sms-acktiwator.ru" value="Faberlic">Faberlic</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="NTT Game">NTT Game</option>
					<option title="sms-activate.org, smshub.org, cheapsms.ru, 365sms.org, sms-man.ru, sms-reg.com, onlinesim.ru, sms-acktiwator.ru, give-sms.com" value="ZdravCity">ZdravCity</option>
					<option title="sms-activate.org, smshub.org, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, sms-acktiwator.ru" value="Whoosh">Whoosh</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, give-sms.com" value="Ticketmaster">Ticketmaster</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, onlinesim.ru, sms-acktiwator.ru" value="Ukrnet">Ukrnet</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, vak-sms.com" value="LiveScore">LiveScore</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, onlinesim.ru, sms-acktiwator.ru" value="AVON">AVON</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, onlinesim.ru, give-sms.com" value="IVI">IVI</option>
					<option title="sms-activate.org, smshub.org, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, vak-sms.com, give-sms.com" value="Vkusvill">Vkusvill</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, cheapsms.ru, 365sms.org, activation.pw, onlinesim.ru, vak-sms.com, give-sms.com" value="PGbonus">PGbonus</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-acktiwator.ru, vak-sms.com" value="Alfa">Alfa</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, vak-sms.com" value="1688">1688</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Kaggle">Kaggle</option>
					<option title="sms-activate.org, smshub.org, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, vak-sms.com, give-sms.com" value="KazanExpress">KazanExpress</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, onlinesim.ru, vak-sms.com" value="Crypto.com">Crypto.com</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, sms-acktiwator.ru, vak-sms.com" value="Lazada">Lazada</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru" value="Clubhouse">Clubhouse</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru" value="Prom.ua">Prom.ua</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="Getir">Getir</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="Hily">Hily</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, sms-acktiwator.ru, vak-sms.com" value="Monese">Monese</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, vak-sms.com" value="Globus">Globus</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="Keybase">Keybase</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru, give-sms.com" value="Truecaller">Truecaller</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, sms-acktiwator.ru, vak-sms.com" value="Grab">Grab</option>
					<option title="sms-activate.org, smshub.org, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, onlinesim.ru, vak-sms.com" value="TradingView">TradingView</option>
					<option title="sms-activate.org, smshub.org, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, onlinesim.ru, give-sms.com" value="FACEIT">FACEIT</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru, give-sms.com" value="BitClout">BitClout</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, give-sms.com" value="Tango">Tango</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="OffGamers">OffGamers</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="METRO">METRO</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="Wish">Wish</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru, give-sms.com" value="Baidu">Baidu</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="Foody">Foody</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, cheapsms.ru, 365sms.org, activation.pw, onlinesim.ru, sms-acktiwator.ru, give-sms.com" value="РСА">РСА</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, vak-sms.com" value="Signal">Signal</option>
					<option title="smshub.org, 5sim.net, smsvk.net, cheapsms.ru, activation.pw, smspva.com, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Qiwi">Qiwi</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="OkCupid">OkCupid</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, vak-sms.com" value="Dostavista">Dostavista</option>
					<option title="sms-activate.org, smshub.org, cheapsms.ru, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, give-sms.com" value="DetskiyMir">DetskiyMir</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru, give-sms.com" value="ELDORADO">ELDORADO</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-acktiwator.ru, vak-sms.com" value="Papara">Papara</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru" value="OfferUp">OfferUp</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, vak-sms.com" value="PaySend">PaySend</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, smspva.com, sms-acktiwator.ru, vak-sms.com" value="Paysafecard">Paysafecard</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="HQ Trivia">HQ Trivia</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, onlinesim.ru" value="Oriflame">Oriflame</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, sms-reg.com, smspva.com, onlinesim.ru, vak-sms.com" value="OpenAI">OpenAI</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, onlinesim.ru, sms-acktiwator.ru" value="Uklon">Uklon</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, vak-sms.com" value="Zomato">Zomato</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru" value="Rediffmail">Rediffmail</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, vak-sms.com" value="HumbleBundle">HumbleBundle</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="Vernyi">Vernyi</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, sms-acktiwator.ru" value="McDonalds">McDonalds</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, sms-acktiwator.ru" value="WestStein">WestStein</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, onlinesim.ru" value="PaxFuL">PaxFuL</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru" value="Fiverr">Fiverr</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-acktiwator.ru, give-sms.com" value="Huya">Huya</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru" value="Douyu">Douyu</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru" value="Global24">Global24</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, give-sms.com" value="Dominos Pizza">Dominos Pizza</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru" value="888casino">888casino</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, give-sms.com" value="MEGA">MEGA</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, sms-acktiwator.ru" value="EasyPay">EasyPay</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru" value="TanTan">TanTan</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, sms-acktiwator.ru" value="POF.com">POF.com</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru" value="PaddyPower">PaddyPower</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru" value="CliQQ">CliQQ</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, give-sms.com" value="Happn">Happn</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru, vak-sms.com, give-sms.com" value="Auchan">Auchan</option>
					<option title="sms-activate.org, smshub.org, cheapsms.ru, 365sms.org, sms-man.ru, sms-reg.com, onlinesim.ru, give-sms.com" value="Okko">Okko</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="Vinted">Vinted</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-acktiwator.ru, vak-sms.com" value="Paycell">Paycell</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, vak-sms.com" value="Mercado">Mercado</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, sms-reg.com, smspva.com, sms-acktiwator.ru, vak-sms.com" value="Revolut">Revolut</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru" value="MyFishka">MyFishka</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="GroupMe">GroupMe</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, activation.pw, sms-reg.com, onlinesim.ru, give-sms.com" value="Apteka.ru">Apteka.ru</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="Socios">Socios</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="Wmaraci">Wmaraci</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="Gittigidiyor">Gittigidiyor</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="MOMO">MOMO</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="Ininal">Ininal</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="Quipp">Quipp</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, sms-reg.com, smspva.com, give-sms.com" value="Twilio">Twilio</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, sms-acktiwator.ru, vak-sms.com" value="SneakersnStuff">SneakersnStuff</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="Digikala">Digikala</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru" value="MapleSEA">MapleSEA</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="99app">99app</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru, vak-sms.com" value="CDKeys">CDKeys</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="Surveytime">Surveytime</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="Hopi">Hopi</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-acktiwator.ru" value="Potato Chat">Potato Chat</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="Yemeksepeti">Yemeksepeti</option>
					<option title="sms-activate.org, smshub.org, smsvk.net, 365sms.org, sms-man.ru, activation.pw, smscode.me" value="PingPong">PingPong</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru, vak-sms.com" value="Magnolia">Magnolia</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="Gamekit">Gamekit</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="mzadqatar">mzadqatar</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-acktiwator.ru" value="1xBet">1xBet</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-acktiwator.ru" value="Perekrestok">Perekrestok</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="Airtel">Airtel</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Douyin">Douyin</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, vak-sms.com" value="Kotak811">Kotak811</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="Icrypex">Icrypex</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="Olacabs">Olacabs</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru, vak-sms.com" value="Miratorg">Miratorg</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="GameArena">GameArena</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, give-sms.com" value="MTS CashBack">MTS CashBack</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru, give-sms.com" value="YouStar">YouStar</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com" value="IFood">IFood</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, sms-acktiwator.ru, vak-sms.com" value="Gojek">Gojek</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru, sms-acktiwator.ru" value="hezzl">hezzl</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, onlinesim.ru" value="Kolesa.kz">Kolesa.kz</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-acktiwator.ru" value="Aitu">Aitu</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="SnappFood">SnappFood</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-reg.com, onlinesim.ru, vak-sms.com, give-sms.com" value="Letual">Letual</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru, sms-acktiwator.ru" value="DENT">DENT</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="Onliner.by">Onliner.by</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="Immowelt">Immowelt</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, vak-sms.com" value="Yubo">Yubo</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, sms-acktiwator.ru, give-sms.com" value="RedBook">RedBook</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="CAIXA">CAIXA</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="Swvl">Swvl</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-reg.com, onlinesim.ru, vak-sms.com, give-sms.com" value="Stormgain">Stormgain</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, vak-sms.com, give-sms.com" value="Premium.one">Premium.one</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru, vak-sms.com" value="InboxLv">InboxLv</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-acktiwator.ru" value="MrGreen">MrGreen</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, sms-reg.com, onlinesim.ru, give-sms.com" value="Lamoda">Lamoda</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="Bitaqaty">Bitaqaty</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="Algida">Algida</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru" value="Lyft">Lyft</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="Haraj">Haraj</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="Eneba">Eneba</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="Tosla">Tosla</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, vak-sms.com" value="Stoloto">Stoloto</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-acktiwator.ru" value="Miloan">Miloan</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru, vak-sms.com" value="Kvartplata+">Kvartplata+</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-acktiwator.ru" value="ezBuy">ezBuy</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="CafeBazaar">CafeBazaar</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, smspva.com, sms-acktiwator.ru" value="MoneyLion">MoneyLion</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-reg.com, smspva.com, vak-sms.com, give-sms.com" value="Wise">Wise</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, onlinesim.ru, vak-sms.com, give-sms.com" value="Xiaomi">Xiaomi</option>
					<option title="sms-activate.org, smshub.org, getsms.online, 365sms.org, onlinesim.ru, vak-sms.com, give-sms.com" value="YAPPY">YAPPY</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, smspva.com, sms-acktiwator.ru" value="BetFair">BetFair</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="Dream11">Dream11</option>
					<option title="sms-activate.org, smshub.org, cheapsms.ru, 365sms.org, onlinesim.ru, vak-sms.com, give-sms.com" value="Вкусно и Точка">Вкусно и Точка</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="Bykea">Bykea</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="KuCoinPlay">KuCoinPlay</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="Yaay">Yaay</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, sms-acktiwator.ru, vak-sms.com" value="BIP Messenger">BIP Messenger</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="Touchance">Touchance</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, give-sms.com" value="Likee">Likee</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="JustDating">JustDating</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="NCsoft">NCsoft</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru" value="Pairs">Pairs</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw" value="NRJ Music Awards">NRJ Music Awards</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, onlinesim.ru, vak-sms.com, give-sms.com" value="Okolo">Okolo</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw" value="Brand20.ua">Brand20.ua</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru" value="OnTaxi">OnTaxi</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru" value="MyMusicTaste">MyMusicTaste</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="Bumble">Bumble</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw" value="PayTM">PayTM</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, activation.pw, onlinesim.ru, give-sms.com" value="LYKA">LYKA</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, onlinesim.ru, sms-acktiwator.ru, give-sms.com" value="CELEBe">CELEBe</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, sms-acktiwator.ru" value="LightChat">LightChat</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, sms-acktiwator.ru" value="HandyPick">HandyPick</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru" value="Fiqsy">Fiqsy</option>
					<option title="sms-activate.org, smshub.org, cheapsms.ru, 365sms.org, sms-acktiwator.ru, give-sms.com" value="Urent">Urent</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw" value="Disney Hotstar">Disney Hotstar</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, give-sms.com" value="Mocospace">Mocospace</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, give-sms.com" value="iQIYI">iQIYI</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru" value="hamrahaval">hamrahaval</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru" value="Taksheel">Taksheel</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru" value="MyLove">MyLove</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw" value="Trendyol">Trendyol</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, vak-sms.com" value="Kufar.by">Kufar.by</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, vak-sms.com" value="Ace2Three">Ace2Three</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, sms-acktiwator.ru, vak-sms.com" value="Indomaret">Indomaret</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru" value="Pyro Music">Pyro Music</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw" value="GG">GG</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru" value="CoinField">CoinField</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, onlinesim.ru, give-sms.com" value="Megogo">Megogo</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, smspva.com" value="Dundle">Dundle</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru" value="Agroinform">Agroinform</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw" value="Divar">Divar</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, activation.pw, onlinesim.ru, give-sms.com" value="Switips">Switips</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, give-sms.com" value="Mos.ru">Mos.ru</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru" value="JKF">JKF</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-acktiwator.ru, vak-sms.com, give-sms.com" value="Lukoil">Lukoil</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw" value="CMTcuzdan">CMTcuzdan</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, sms-acktiwator.ru" value="Rozetka">Rozetka</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, give-sms.com" value="Trip">Trip</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw" value="SpotHit">SpotHit</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw" value="Humta">Humta</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw" value="Nifty">Nifty</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, onlinesim.ru, sms-acktiwator.ru, give-sms.com" value="Skype">Skype</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw" value="Blued">Blued</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru" value="RuTube">RuTube</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, smspva.com" value="Verse">Verse</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, smspva.com" value="Wing Money">Wing Money</option>
					<option title="sms-activate.org, smshub.org, getsms.online, 365sms.org, sms-acktiwator.ru, give-sms.com" value="Betting">Betting</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, sms-acktiwator.ru" value="CityBase">CityBase</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw" value="Hepsiburada.com">Hepsiburada.com</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw" value="Bisu">Bisu</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, give-sms.com" value="163.com">163.com</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw" value="Banqi">Banqi</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw" value="Dhani">Dhani</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw" value="SheerID">SheerID</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw" value="Carousell">Carousell</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru" value="Gamer">Gamer</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw" value="Şikayet var">Şikayet var</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru" value="BillMill">BillMill</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw" value="Payberry">Payberry</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru" value="SSOid.net">SSOid.net</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, activation.pw, onlinesim.ru, vak-sms.com" value="Tatneft">Tatneft</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, give-sms.com" value="Karusel">Karusel</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, sms-acktiwator.ru" value="YouGotaGift">YouGotaGift</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw" value="Picpay">Picpay</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw" value="Monobank">Monobank</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw" value="Cita Previa">Cita Previa</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru" value="Q12 Trivia">Q12 Trivia</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw, onlinesim.ru" value="Okta">Okta</option>
					<option title="5sim.net, getsms.online, sms-reg.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="MeetMe">MeetMe</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-acktiwator.ru, vak-sms.com" value="Alfagift">Alfagift</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, vak-sms.com, give-sms.com" value="CommunityGaming">CommunityGaming</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, onlinesim.ru, vak-sms.com" value="Perfluence">Perfluence</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-acktiwator.ru, vak-sms.com" value="4Fun">4Fun</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, smspva.com, sms-acktiwator.ru" value="DoorDash">DoorDash</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw" value="Coindcx">Coindcx</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw" value="NoBroker">NoBroker</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw" value="Quack">Quack</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw" value="Skroutz">Skroutz</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw" value="Powerkredite">Powerkredite</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, vak-sms.com" value="GCash">GCash</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw" value="Onet">Onet</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw" value="MPL">MPL</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw" value="Noon">Noon</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw" value="KeyPay">KeyPay</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, onlinesim.ru, give-sms.com" value="Ximalaya">Ximalaya</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw" value="GalaxyWin">GalaxyWin</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw" value="IndiaGold">IndiaGold</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-reg.com, sms-acktiwator.ru" value="Lidl">Lidl</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw" value="Mobile01">Mobile01</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw" value="Allegro">Allegro</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, onlinesim.ru, give-sms.com" value="TenChat">TenChat</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, give-sms.com" value="Hermes">Hermes</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw" value="RosaKhutor">RosaKhutor</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw" value="JamesDelivery">JamesDelivery</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw" value="ChaingeFinance">ChaingeFinance</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw" value="Myntra">Myntra</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, vak-sms.com, give-sms.com" value="Pivko24">Pivko24</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw" value="E bike Gewinnspiel">E bike Gewinnspiel</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw" value="Payzapp">Payzapp</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw" value="Iwplay">Iwplay</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw" value="Gabi">Gabi</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, onlinesim.ru, give-sms.com" value="КухняНаРайоне">КухняНаРайоне</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw" value="irancell">irancell</option>
					<option title="sms-activate.org, smshub.org, getsms.online, 365sms.org, sms-acktiwator.ru" value="IZI">IZI</option>
					<option title="sms-activate.org, smshub.org, getsms.online, 365sms.org, give-sms.com" value="Joyride">Joyride</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, sms-acktiwator.ru" value="Poshmark">Poshmark</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, sms-acktiwator.ru" value="ZCity">ZCity</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw" value="GlobalTel">GlobalTel</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw" value="GoFundMe">GoFundMe</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw" value="Meta">Meta</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw" value="SportGully">SportGully</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, activation.pw, vak-sms.com" value="Tokopedia">Tokopedia</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, activation.pw, give-sms.com" value="Zupee">Zupee</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, smspva.com, sms-acktiwator.ru" value="GoogleVoice">GoogleVoice</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw" value="Ziglu">Ziglu</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, smspva.com, sms-acktiwator.ru" value="Hinge">Hinge</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, activation.pw, sms-acktiwator.ru" value="Hotline">Hotline</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, activation.pw, give-sms.com" value="CallApp">CallApp</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, activation.pw, smspva.com" value="FastMail">FastMail</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw" value="PurePlatfrom">PurePlatfrom</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw" value="dbrUA">dbrUA</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw" value="ShellBox">ShellBox</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-acktiwator.ru, vak-sms.com" value="OVO">OVO</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, vak-sms.com, give-sms.com" value="Expressmoney">Expressmoney</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, onlinesim.ru, give-sms.com" value="SellMonitor">SellMonitor</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw" value="RoyalWin">RoyalWin</option>
					<option title="smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw" value="23red">23red</option>
					<option title="5sim.net, cheapsms.ru, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="Blockchain">Blockchain</option>
					<option title="5sim.net, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="GameFlip">GameFlip</option>
					<option title="5sim.net, sms-reg.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="YouDo">YouDo</option>
					<option title="smsvk.net, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="TaxiMaxim">TaxiMaxim</option>
					<option title="cheapsms.ru, sms-reg.com, smspva.com, sms-acktiwator.ru, vak-sms.com" value="WebMoney">WebMoney</option>
					<option title="cheapsms.ru, sms-reg.com, smspva.com, sms-acktiwator.ru, vak-sms.com" value="YooMoney">YooMoney</option>
					<option title="cheapsms.ru, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru" value="Fotostrana">Fotostrana</option>
					<option title="sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="Badoo">Badoo</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, vak-sms.com" value="Dana">Dana</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, give-sms.com" value="Ys4fun">Ys4fun</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, activation.pw" value="Mewt">Mewt</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, activation.pw" value="Pocket52">Pocket52</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, activation.pw" value="Primaries 2020">Primaries 2020</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-acktiwator.ru" value="Giftcloud">Giftcloud</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, vak-sms.com" value="Virgo">Virgo</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, vak-sms.com" value="BLIBLI">BLIBLI</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, onlinesim.ru" value="MarketGuru">MarketGuru</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, onlinesim.ru" value="Starbucks">Starbucks</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, activation.pw" value="VitaExpress">VitaExpress</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, give-sms.com" value="Taikang">Taikang</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-acktiwator.ru" value="Nextdoor">Nextdoor</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, activation.pw" value="Siply">Siply</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, vak-sms.com" value="LOCO">LOCO</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, give-sms.com" value="DewuPoison">DewuPoison</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, vak-sms.com" value="Vidio">Vidio</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, give-sms.com" value="UU163">UU163</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, onlinesim.ru" value="HeyBox">HeyBox</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-man.ru" value="Stripe">Stripe</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, activation.pw" value="LazyPay">LazyPay</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, give-sms.com" value="Weverse">Weverse</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-acktiwator.ru" value="Venmo">Venmo</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, give-sms.com" value="Wink">Wink</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, vak-sms.com" value="CourseHero">CourseHero</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, activation.pw" value="Magicbricks">Magicbricks</option>
					<option title="sms-activate.org, smshub.org, cheapsms.ru, onlinesim.ru" value="Uteka">Uteka</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, give-sms.com" value="NimoTV">NimoTV</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, give-sms.com" value="GalaxyChat">GalaxyChat</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, activation.pw" value="Akulaku">Akulaku</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org" value="FreeChargeApp">FreeChargeApp</option>
					<option title="sms-activate.org, smshub.org, 5sim.net, 365sms.org" value="Astropay">Astropay</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-acktiwator.ru" value="Mercari">Mercari</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, vak-sms.com" value="Zhihu">Zhihu</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, activation.pw" value="InFund">InFund</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, give-sms.com" value="Mihuashi">Mihuashi</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, give-sms.com" value="Bilibili">Bilibili</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, activation.pw" value="IPLwin">IPLwin</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, activation.pw" value="RRSA">RRSA</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, activation.pw" value="Roposo">Roposo</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-acktiwator.ru" value="FoodHub">FoodHub</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-acktiwator.ru" value="SoulApp">SoulApp</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, vak-sms.com" value="OneAset">OneAset</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, give-sms.com" value="Familia">Familia</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-acktiwator.ru" value="MrQ">MrQ</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, sms-acktiwator.ru" value="Zilch">Zilch</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, activation.pw" value="Flipkart">Flipkart</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, onlinesim.ru" value="Weidian">Weidian</option>
					<option title="sms-activate.org, smshub.org, 365sms.org, vak-sms.com" value="Bukalapak">Bukalapak</option>
					<option title="5sim.net, smspva.com, sms-acktiwator.ru, vak-sms.com" value="iCard">iCard</option>
					<option title="5sim.net, cheapsms.ru, onlinesim.ru, sms-acktiwator.ru" value="Okey">Okey</option>
					<option title="smsvk.net, activation.pw, smspva.com, sms-acktiwator.ru" value="Goods">Goods</option>
					<option title="cheapsms.ru, sms-reg.com, onlinesim.ru, vak-sms.com" value="1Cupis">1Cupis</option>
					<option title="cheapsms.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru" value="Steemit">Steemit</option>
					<option title="activation.pw, sms-reg.com, smspva.com, sms-acktiwator.ru" value="Parimatch">Parimatch</option>
					<option title="sms-reg.com, smspva.com, onlinesim.ru, vak-sms.com" value="LocalBitcoins">LocalBitcoins</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="AIS">AIS</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="MoneyPay">MoneyPay</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="TurkiyePetrolleri">TurkiyePetrolleri</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Brahma">Brahma</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Dosi">Dosi</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="TradeUP">TradeUP</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Santander">Santander</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="GuruBets">GuruBets</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="CloudChat">CloudChat</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="ROBINHOOD">ROBINHOOD</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Iti">Iti</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="RummyLoot">RummyLoot</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Porbet">Porbet</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="TeenPattiStarpro">TeenPattiStarpro</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="SamsungShop">SamsungShop</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="redBus">redBus</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="YoWin">YoWin</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="GyFTR">GyFTR</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="HOP">HOP</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="PharmEasy">PharmEasy</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Taki">Taki</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Voltz">Voltz</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="MyDailyCash">MyDailyCash</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Leboncoin1">Leboncoin1</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Parkplus">Parkplus</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Bazos">Bazos</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Tick">Tick</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Fora">Fora</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="IceCasino">IceCasino</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="My11Circle">My11Circle</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Sizeer">Sizeer</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="MonobankIndia">MonobankIndia</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Bit">Bit</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="HappyFresh">HappyFresh</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="YikYak">YikYak</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Loanflix">Loanflix</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="MobiKwik">MobiKwik</option>
					<option title="sms-activate.org, 5sim.net, vak-sms.com" value="32red">32red</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="WinzoGame">WinzoGame</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Ruten">Ruten</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Lalamove">Lalamove</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Vlife">Vlife</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="JungleeRummy">JungleeRummy</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="MoneyСontrol">MoneyСontrol</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Lotus">Lotus</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Swiggy">Swiggy</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Getmega">Getmega</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Uplay">Uplay</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Meliuz">Meliuz</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="ContactSys">ContactSys</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="UWIN">UWIN</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="AGIBANK">AGIBANK</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="AdaKami">AdaKami</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="eWallet">eWallet</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="RummyWealth">RummyWealth</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Adani">Adani</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="GMNG">GMNG</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="FWDMAX">FWDMAX</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Vivo">Vivo</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="CrefisaMais">CrefisaMais</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="CashFly">CashFly</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Akudo">Akudo</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="СhampionСasino">СhampionСasino</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="JTExpress">JTExpress</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Telegram 2.0">Telegram 2.0</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Budweiser">Budweiser</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="SuperS">SuperS</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="BigC">BigC</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="BeReal">BeReal</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="AgriDevelop">AgriDevelop</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Depop">Depop</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="ApostaGanha">ApostaGanha</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Fotka">Fotka</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="RummyOla">RummyOla</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Bitso">Bitso</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Leboncoin">Leboncoin</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="IndiaPlays">IndiaPlays</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="AUBANK">AUBANK</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Thisshop">Thisshop</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="IRCTC">IRCTC</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Wondermart">Wondermart</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="BinBin">BinBin</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Kirana">Kirana</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="G2G">G2G</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="RecargaPay">RecargaPay</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="PagSmile">PagSmile</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="RummyCulture">RummyCulture</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Foxtrot">Foxtrot</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="炙热星河">炙热星河</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Setel">Setel</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="MIXMART">MIXMART</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Corona">Corona</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="DealShare">DealShare</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Codashop">Codashop</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Nanovest">Nanovest</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="EscapeFromTarkov">EscapeFromTarkov</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="99acres">99acres</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Gemgala">Gemgala</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Kaya">Kaya</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="WashXpress">WashXpress</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Probo">Probo</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="24betting">24betting</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Subito">Subito</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Dotz">Dotz</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="LoveLocal">LoveLocal</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="LongHu">LongHu</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Cathay">Cathay</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Band">Band</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Biedronka">Biedronka</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Eyecon">Eyecon</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="Grofers">Grofers</option>
					<option title="sms-activate.org, smshub.org, 365sms.org" value="SpatenOktoberfest">SpatenOktoberfest</option>
					<option title="smshub.org, activation.pw, give-sms.com" value="Banks">Banks</option>
					<option title="5sim.net, onlinesim.ru, vak-sms.com" value="Akelni">Akelni</option>
					<option title="5sim.net, onlinesim.ru, sms-acktiwator.ru" value="Azino777">Azino777</option>
					<option title="5sim.net, onlinesim.ru, sms-acktiwator.ru" value="Azino888">Azino888</option>
					<option title="5sim.net, sms-acktiwator.ru, vak-sms.com" value="BitTube">BitTube</option>
					<option title="5sim.net, onlinesim.ru, vak-sms.com" value="Dixy">Dixy</option>
					<option title="5sim.net, sms-acktiwator.ru, vak-sms.com" value="Dukascopy">Dukascopy</option>
					<option title="5sim.net, onlinesim.ru, vak-sms.com" value="Electroneum">Electroneum</option>
					<option title="5sim.net, onlinesim.ru, sms-acktiwator.ru" value="IOST">IOST</option>
					<option title="5sim.net, onlinesim.ru, vak-sms.com" value="Lianxin">Lianxin</option>
					<option title="5sim.net, onlinesim.ru, sms-acktiwator.ru" value="Nimses">Nimses</option>
					<option title="5sim.net, sms-man.ru, vak-sms.com" value="B4U Cabs">B4U Cabs</option>
					<option title="5sim.net, sms-man.ru, vak-sms.com" value="ClickEntregas">ClickEntregas</option>
					<option title="getsms.online, cheapsms.ru, onlinesim.ru" value="Pandao">Pandao</option>
					<option title="cheapsms.ru, onlinesim.ru, sms-acktiwator.ru" value="Beget">Beget</option>
					<option title="cheapsms.ru, onlinesim.ru, vak-sms.com" value="Book24">Book24</option>
					<option title="cheapsms.ru, sms-reg.com, smspva.com" value="KoronaPay">KoronaPay</option>
					<option title="cheapsms.ru, onlinesim.ru, vak-sms.com" value="Marlboro">Marlboro</option>
					<option title="cheapsms.ru, sms-reg.com, vak-sms.com" value="POCHTABANK">POCHTABANK</option>
					<option title="cheapsms.ru, sms-reg.com, onlinesim.ru" value="RESO">RESO</option>
					<option title="cheapsms.ru, onlinesim.ru, sms-acktiwator.ru" value="pochta.ru">pochta.ru</option>
					<option title="sms-reg.com, onlinesim.ru, vak-sms.com" value="Binance">Binance</option>
					<option title="sms-reg.com, onlinesim.ru, sms-acktiwator.ru" value="Zadarma">Zadarma</option>
					<option title="sms-reg.com, onlinesim.ru, vak-sms.com" value="Tilda">Tilda</option>
					<option title="sms-reg.com, onlinesim.ru, vak-sms.com" value="profi.ru">profi.ru</option>
					<option title="sms-reg.com, onlinesim.ru, vak-sms.com" value="reg.ru">reg.ru</option>
					<option title="sms-reg.com, onlinesim.ru, vak-sms.com" value="Sravni.ru">Sravni.ru</option>
					<option title="sms-reg.com, onlinesim.ru, vak-sms.com" value="iHerb">iHerb</option>
					<option title="smspva.com, onlinesim.ru, sms-acktiwator.ru" value="G2A">G2A</option>
					<option title="onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="zenly">zenly</option>
					<option title="onlinesim.ru, sms-acktiwator.ru, vak-sms.com" value="FarPost">FarPost</option>
					<option title="5sim.net, vak-sms.com" value="Cekkazan">Cekkazan</option>
					<option title="5sim.net, vak-sms.com" value="Domdara">Domdara</option>
					<option title="5sim.net, vak-sms.com" value="Edgeless">Edgeless</option>
					<option title="5sim.net, vak-sms.com" value="Forwarding">Forwarding</option>
					<option title="5sim.net, sms-acktiwator.ru" value="KomandaCard">KomandaCard</option>
					<option title="5sim.net, vak-sms.com" value="LBRY">LBRY</option>
					<option title="5sim.net, vak-sms.com" value="OpenPoint">OpenPoint</option>
					<option title="5sim.net, onlinesim.ru" value="Oracle">Oracle</option>
					<option title="5sim.net, onlinesim.ru" value="PokerMaster">PokerMaster</option>
					<option title="5sim.net, onlinesim.ru" value="PUBG">PUBG</option>
					<option title="5sim.net, onlinesim.ru" value="Winston">Winston</option>
					<option title="5sim.net, sms-man.ru" value="Nana">Nana</option>
					<option title="5sim.net, vak-sms.com" value="NHSEVEN">NHSEVEN</option>
					<option title="cheapsms.ru, vak-sms.com" value="Tinkoff">Tinkoff</option>
					<option title="cheapsms.ru, onlinesim.ru" value="Benzuber">Benzuber</option>
					<option title="cheapsms.ru, smspva.com" value="CONTACT">CONTACT</option>
					<option title="cheapsms.ru, sms-reg.com" value="Ekapusta">Ekapusta</option>
					<option title="cheapsms.ru, onlinesim.ru" value="GPNbonus">GPNbonus</option>
					<option title="cheapsms.ru, onlinesim.ru" value="Hohoro">Hohoro</option>
					<option title="cheapsms.ru, sms-acktiwator.ru" value="MicroKlad">MicroKlad</option>
					<option title="cheapsms.ru, sms-reg.com" value="Salampay">Salampay</option>
					<option title="cheapsms.ru, sms-reg.com" value="Zaymer">Zaymer</option>
					<option title="cheapsms.ru, vak-sms.com" value="Rosneft">Rosneft</option>
					<option title="cheapsms.ru, vak-sms.com" value="Совкомбанк">Совкомбанк</option>
					<option title="sms-man.ru, onlinesim.ru" value="Hotmail">Hotmail</option>
					<option title="sms-man.ru, activation.pw" value="Unacademy">Unacademy</option>
					<option title="activation.pw, onlinesim.ru" value="TamTam">TamTam</option>
					<option title="activation.pw, onlinesim.ru" value="RealResearch">RealResearch</option>
					<option title="activation.pw, onlinesim.ru" value="My.Games">My.Games</option>
					<option title="activation.pw, sms-acktiwator.ru" value="sweet.tv">sweet.tv</option>
					<option title="activation.pw, onlinesim.ru" value="uphold">uphold</option>
					<option title="activation.pw, smspva.com" value="Swagbucks">Swagbucks</option>
					<option title="sms-reg.com, vak-sms.com" value="Unistream">Unistream</option>
					<option title="sms-reg.com, onlinesim.ru" value="Delimobil">Delimobil</option>
					<option title="sms-reg.com, sms-acktiwator.ru" value="NovaPoshta">NovaPoshta</option>
					<option title="sms-reg.com, vak-sms.com" value="Payoneer">Payoneer</option>
					<option title="sms-reg.com, onlinesim.ru" value="EAPTEKA">EAPTEKA</option>
					<option title="sms-reg.com, onlinesim.ru" value="NEXMO">NEXMO</option>
					<option title="sms-reg.com, onlinesim.ru" value="PREMIER">PREMIER</option>
					<option title="sms-reg.com, vak-sms.com" value="Raiffeisen">Raiffeisen</option>
					<option title="sms-reg.com, onlinesim.ru" value="AlfaStrah">AlfaStrah</option>
					<option title="sms-reg.com, onlinesim.ru" value="MyACUVUE">MyACUVUE</option>
					<option title="sms-reg.com, onlinesim.ru" value="RoboForex">RoboForex</option>
					<option title="sms-reg.com, smspva.com" value="Neteller">Neteller</option>
					<option title="sms-reg.com, vak-sms.com" value="Spotify">Spotify</option>
					<option title="sms-reg.com, onlinesim.ru" value="gusli.net">gusli.net</option>
					<option title="sms-reg.com, onlinesim.ru" value="Matbea.com">Matbea.com</option>
					<option title="sms-reg.com, vak-sms.com" value="Rostelecom">Rostelecom</option>
					<option title="sms-reg.com, onlinesim.ru" value="Selfcontrol">Selfcontrol</option>
					<option title="sms-reg.com, smspva.com" value="BLS-SPAIN">BLS-SPAIN</option>
					<option title="smspva.com, onlinesim.ru" value="PapaJohns">PapaJohns</option>
					<option title="smspva.com, sms-acktiwator.ru" value="Raketa">Raketa</option>
					<option title="smspva.com, vak-sms.com" value="Grailed">Grailed</option>
					<option title="smspva.com, onlinesim.ru" value="Sisal">Sisal</option>
					<option title="smspva.com, onlinesim.ru" value="E-NUM">E-NUM</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="Lino.network">Lino.network</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="Appbonus">Appbonus</option>
					<option title="onlinesim.ru, vak-sms.com" value="Hey_plus">Hey_plus</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="WhatsAround">WhatsAround</option>
					<option title="onlinesim.ru, vak-sms.com" value="hh.ru">hh.ru</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="WOG.ua">WOG.ua</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="Jollychic">Jollychic</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="jiayuan">jiayuan</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="Plenty of Fish">Plenty of Fish</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="Seagm">Seagm</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="Auto.ru">Auto.ru</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="GetResponse">GetResponse</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="Spaces">Spaces</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="Qrooto">Qrooto</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="Gorzdrav">Gorzdrav</option>
					<option title="onlinesim.ru, vak-sms.com" value="Tanuki">Tanuki</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="Sunlight">Sunlight</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="LDinfo">LDinfo</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="4game">4game</option>
					<option title="onlinesim.ru, vak-sms.com" value="Kontur">Kontur</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="QIP">QIP</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="Loveplanet">Loveplanet</option>
					<option title="onlinesim.ru, vak-sms.com" value="BIGLION">BIGLION</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="PetrI">PetrI</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="Gem4me">Gem4me</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="ProDoctorov">ProDoctorov</option>
					<option title="onlinesim.ru, vak-sms.com" value="2domains">2domains</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="Taxify">Taxify</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="ihc.ru">ihc.ru</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="Sprinthost">Sprinthost</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="Vscale">Vscale</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="Sony">Sony</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="PMSM">PMSM</option>
					<option title="onlinesim.ru, vak-sms.com" value="Selectel">Selectel</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="Studwork">Studwork</option>
					<option title="onlinesim.ru, vak-sms.com" value="openbroker">openbroker</option>
					<option title="onlinesim.ru, vak-sms.com" value="Getcontact">Getcontact</option>
					<option title="onlinesim.ru, vak-sms.com" value="hunter">hunter</option>
					<option title="onlinesim.ru, vak-sms.com" value="coolclever">coolclever</option>
					<option title="onlinesim.ru, vak-sms.com" value="OSTIN">OSTIN</option>
					<option title="onlinesim.ru, vak-sms.com" value="Yamaguchi">Yamaguchi</option>
					<option title="onlinesim.ru, vak-sms.com" value="SOKOLOV">SOKOLOV</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="GoGo">GoGo</option>
					<option title="onlinesim.ru, vak-sms.com" value="Шоколадница">Шоколадница</option>
					<option title="onlinesim.ru, vak-sms.com" value="Coinut">Coinut</option>
					<option title="onlinesim.ru, vak-sms.com" value="rocketreach">rocketreach</option>
					<option title="onlinesim.ru, vak-sms.com" value="CAOMoscow">CAOMoscow</option>
					<option title="onlinesim.ru, vak-sms.com" value="Hoff">Hoff</option>
					<option title="onlinesim.ru, vak-sms.com" value="VODOROBOT">VODOROBOT</option>
					<option title="onlinesim.ru, vak-sms.com" value="Winelab">Winelab</option>
					<option title="onlinesim.ru, vak-sms.com" value="Trovo">Trovo</option>
					<option title="onlinesim.ru, vak-sms.com" value="ultra.io">ultra.io</option>
					<option title="onlinesim.ru, vak-sms.com" value="Foodora">Foodora</option>
					<option title="onlinesim.ru, vak-sms.com" value="ogon.ru">ogon.ru</option>
					<option title="onlinesim.ru, vak-sms.com" value="Золотое яблоко">Золотое яблоко</option>
					<option title="onlinesim.ru, vak-sms.com" value="Gogym">Gogym</option>
					<option title="onlinesim.ru, vak-sms.com" value="litnet">litnet</option>
					<option title="onlinesim.ru, vak-sms.com" value="Buff.163">Buff.163</option>
					<option title="onlinesim.ru, vak-sms.com" value="Петрович">Петрович</option>
					<option title="onlinesim.ru, vak-sms.com" value="Кошелек">Кошелек</option>
					<option title="onlinesim.ru, vak-sms.com" value="Едем.рф">Едем.рф</option>
					<option title="onlinesim.ru, vak-sms.com" value="Tier">Tier</option>
					<option title="onlinesim.ru, vak-sms.com" value="microworkers">microworkers</option>
					<option title="onlinesim.ru, vak-sms.com" value="mybeautybonus">mybeautybonus</option>
					<option title="onlinesim.ru, vak-sms.com" value="ftx.com">ftx.com</option>
					<option title="onlinesim.ru, vak-sms.com" value="Dosmart">Dosmart</option>
					<option title="onlinesim.ru, sms-acktiwator.ru" value="Smart">Smart</option>
					<option title="onlinesim.ru, vak-sms.com" value="Slivki.by">Slivki.by</option>
					<option title="onlinesim.ru, vak-sms.com" value="Aurora+">Aurora+</option>
					<option title="onlinesim.ru, vak-sms.com" value="150bar">150bar</option>
					<option title="onlinesim.ru, vak-sms.com" value="Cofix">Cofix</option>
					<option title="onlinesim.ru, vak-sms.com" value="InternetOpros">InternetOpros</option>
					<option title="onlinesim.ru, vak-sms.com" value="Stars coffee">Stars coffee</option>
					<option title="onlinesim.ru, vak-sms.com" value="checkin">checkin</option>
					<option title="sms-acktiwator.ru, vak-sms.com" value="Wmmail">Wmmail</option>
				  </select>
				</span>
			  </div>
			</div>
			<label class="control-label text-right tr" style="padding-top:5px !important;">Site</label>
		  </div>
		</form>
	</div>
	<%= _.template($('#input_constructor').html())({
		id: "country",
		description: tr("Country"),
		default_selector: "string",
		disable_int: true,
		value_string: "RU",
		variants: [
			"RU<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, getsms.online, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com'>" + tr("Russian Federation") + "</span>",
			"KZ<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, getsms.online, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com'>" + tr("Kazakhstan") + "</span>",
			"UA<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com'>" + tr("Ukraine") + "</span>",
			"PL<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com'>" + tr("Poland") + "</span>",
			"GB<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com'>" + tr("United Kingdom") + "</span>",
			"DE<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com'>" + tr("Germany") + "</span>",
			"MD<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com'>" + tr("Moldova") + "</span>",
			"PH<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com'>" + tr("Philippines") + "</span>",
			"ID<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com'>" + tr("Indonesia") + "</span>",
			"LA<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com'>" + tr("Lao People's Democratic Republic") + "</span>",
			"RO<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com'>" + tr("Romania") + "</span>",
			"EE<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com'>" + tr("Estonia") + "</span>",
			"LT<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com'>" + tr("Lithuania") + "</span>",
			"SE<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com'>" + tr("Sweden") + "</span>",
			"NL<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com'>" + tr("Netherlands") + "</span>",
			"LV<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com'>" + tr("Latvia") + "</span>",
			"ES<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com'>" + tr("Spain") + "</span>",
			"FR<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, smspva.com, onlinesim.ru, vak-sms.com'>" + tr("France") + "</span>",
			"PT<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru, vak-sms.com'>" + tr("Portugal") + "</span>",
			"MY<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru'>" + tr("Malaysia") + "</span>",
			"KE<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru'>" + tr("Kenya") + "</span>",
			"VN<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, vak-sms.com'>" + tr("Vietnam") + "</span>",
			"KG<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru'>" + tr("Kyrgyzstan") + "</span>",
			"HK<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, vak-sms.com'>" + tr("Hong Kong") + "</span>",
			"CO<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru'>" + tr("Colombia") + "</span>",
			"TH<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru'>" + tr("Thailand") + "</span>",
			"MX<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, vak-sms.com'>" + tr("Mexico") + "</span>",
			"CZ<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru'>" + tr("Czech Republic") + "</span>",
			"BR<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru'>" + tr("Brazil") + "</span>",
			"GE<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru, vak-sms.com'>" + tr("Georgia") + "</span>",
			"FI<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, vak-sms.com'>" + tr("Finland") + "</span>",
			"US<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru, sms-acktiwator.ru'>" + tr("United States") + "</span>",
			"US (Virtual)<br/><span style='color:gray' title='sms-activate.org, smshub.org, 365sms.org, activation.pw'>" + tr("United States") + "</span>",
			"CN<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, sms-reg.com, onlinesim.ru'>" + tr("China") + "</span>",
			"IL<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru'>" + tr("Israel") + "</span>",
			"NG<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru'>" + tr("Nigeria") + "</span>",
			"IN<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru'>" + tr("India") + "</span>",
			"IE<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru'>" + tr("Ireland") + "</span>",
			"HT<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru'>" + tr("Haiti") + "</span>",
			"RS<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru'>" + tr("Serbia") + "</span>",
			"MA<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru'>" + tr("Morocco") + "</span>",
			"AR<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru'>" + tr("Argentina") + "</span>",
			"UZ<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru'>" + tr("Uzbekistan") + "</span>",
			"HR<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru'>" + tr("Croatia") + "</span>",
			"AT<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru'>" + tr("Austria") + "</span>",
			"BY<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru, sms-acktiwator.ru'>" + tr("Belarus") + "</span>",
			"TR<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru'>" + tr("Turkey") + "</span>",
			"CY<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru'>" + tr("Cyprus") + "</span>",
			"EG<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Egypt") + "</span>",
			"EG (Virtual)<br/><span style='color:gray' title='smspva.com'>" + tr("Egypt") + "</span>",
			"KH<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com'>" + tr("Cambodia") + "</span>",
			"CI<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Cote d'Ivoire") + "</span>",
			"GM<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Gambia") + "</span>",
			"ZA<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com'>" + tr("South Africa") + "</span>",
			"CA<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com'>" + tr("Canada") + "</span>",
			"CA (Virtual)<br/><span style='color:gray' title='onlinesim.ru'>" + tr("Canada") + "</span>",
			"GH<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Ghana") + "</span>",
			"GH (Virtual)<br/><span style='color:gray' title='smspva.com'>" + tr("Ghana") + "</span>",
			"CM<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Cameroon") + "</span>",
			"TD<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Chad") + "</span>",
			"IQ<br/><span style='color:gray' title='sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, smspva.com, onlinesim.ru'>" + tr("Iraq") + "</span>",
			"SA<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Saudi Arabia") + "</span>",
			"TW<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Taiwan, Province of China") + "</span>",
			"BD<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com'>" + tr("Bangladesh") + "</span>",
			"SN<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Senegal") + "</span>",
			"LK<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Sri Lanka") + "</span>",
			"PE<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Peru") + "</span>",
			"PK<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Pakistan") + "</span>",
			"PK (Virtual)<br/><span style='color:gray' title='smspva.com'>" + tr("Pakistan") + "</span>",
			"NZ<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com'>" + tr("New Zealand") + "</span>",
			"GN<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Guinea") + "</span>",
			"VE<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Venezuela") + "</span>",
			"MN<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Mongolia") + "</span>",
			"AF<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Afghanistan") + "</span>",
			"UG<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Uganda") + "</span>",
			"NP<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Nepal") + "</span>",
			"BE<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw, smspva.com, onlinesim.ru'>" + tr("Belgium") + "</span>",
			"IT<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw, smspva.com, onlinesim.ru'>" + tr("Italy") + "</span>",
			"PY<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com'>" + tr("Paraguay") + "</span>",
			"HN<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Honduras") + "</span>",
			"TN<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Tunisia") + "</span>",
			"NI<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Nicaragua") + "</span>",
			"BO<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Bolivia, plurinational state of") + "</span>",
			"SV<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("El Salvador") + "</span>",
			"EC<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Ecuador") + "</span>",
			"BA<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw, smspva.com, sms-acktiwator.ru'>" + tr("Bosnia and Herzegovina") + "</span>",
			"DO<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com'>" + tr("Dominican Republic") + "</span>",
			"MR<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Mauritania") + "</span>",
			"SL<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Sierra Leone") + "</span>",
			"JO<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Jordan") + "</span>",
			"GR<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Greece") + "</span>",
			"LR<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Liberia") + "</span>",
			"TJ<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Tajikistan") + "</span>",
			"CL<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, smspva.com'>" + tr("Chile") + "</span>",
			"BF<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Burkina Faso") + "</span>",
			"DK<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, smspva.com, onlinesim.ru, vak-sms.com'>" + tr("Denmark") + "</span>",
			"MM<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw'>" + tr("Myanmar") + "</span>",
			"TZ<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw'>" + tr("Tanzania, United Republic Of") + "</span>",
			"YE<br/><span style='color:gray' title='sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Yemen") + "</span>",
			"DZ<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw'>" + tr("Algeria") + "</span>",
			"SI<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw, smspva.com'>" + tr("Slovenia") + "</span>",
			"ML<br/><span style='color:gray' title='sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Mali") + "</span>",
			"AO<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw'>" + tr("Angola") + "</span>",
			"PG<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw'>" + tr("Papua New Guinea") + "</span>",
			"MZ<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw'>" + tr("Mozambique") + "</span>",
			"BG<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Bulgaria") + "</span>",
			"GT<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw'>" + tr("Guatemala") + "</span>",
			"ZW<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw'>" + tr("Zimbabwe") + "</span>",
			"TG<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Togo") + "</span>",
			"JM<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw'>" + tr("Jamaica") + "</span>",
			"TT<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw'>" + tr("Trinidad and Tobago") + "</span>",
			"BJ<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw'>" + tr("Benin") + "</span>",
			"BW<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw'>" + tr("Botswana") + "</span>",
			"GW<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Guinea-Bissau") + "</span>",
			"GY<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw'>" + tr("Guyana") + "</span>",
			"MW<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Malawi") + "</span>",
			"SR<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw'>" + tr("Suriname") + "</span>",
			"RE<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw'>" + tr("Reunion") + "</span>",
			"ZM<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Zambia") + "</span>",
			"AM<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw'>" + tr("Armenia") + "</span>",
			"CG<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw'>" + tr("Congo") + "</span>",
			"GA<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw'>" + tr("Gabon") + "</span>",
			"MU<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw'>" + tr("Mauritius") + "</span>",
			"BT<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw'>" + tr("Bhutan") + "</span>",
			"MV<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw'>" + tr("Maldives") + "</span>",
			"TM<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw'>" + tr("Turkmenistan") + "</span>",
			"CH<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Switzerland") + "</span>",
			"AU<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, sms-man.ru, activation.pw'>" + tr("Australia") + "</span>",
			"MK<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Macedonia, The Former Yugoslav Republic Of") + "</span>",
			"MG<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Madagascar") + "</span>",
			"CD<br/><span style='color:gray' title='sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw'>" + tr("Congo, Democratic Republic of the") + "</span>",
			"MO<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Macao") + "</span>",
			"ET<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Ethiopia") + "</span>",
			"HU<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Hungary") + "</span>",
			"TL<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Timor-Leste") + "</span>",
			"CR<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Costa Rica") + "</span>",
			"AE<br/><span style='color:gray' title='sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw'>" + tr("United Arab Emirates") + "</span>",
			"PR<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Puerto Rico") + "</span>",
			"KW<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Kuwait") + "</span>",
			"LY<br/><span style='color:gray' title='sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw'>" + tr("Libyan Arab Jamahiriya") + "</span>",
			"SZ<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Eswatini") + "</span>",
			"OM<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Oman") + "</span>",
			"PA<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Panama") + "</span>",
			"BB<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Barbados") + "</span>",
			"BI<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Burundi") + "</span>",
			"BS<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Bahamas") + "</span>",
			"BZ<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Belize") + "</span>",
			"DM<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw'>" + tr("Dominica") + "</span>",
			"GD<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Grenada") + "</span>",
			"KM<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Comoros") + "</span>",
			"KN<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Saint Kitts and Nevis") + "</span>",
			"LS<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Lesotho") + "</span>",
			"NA<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Namibia") + "</span>",
			"NE<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Niger") + "</span>",
			"RW<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Rwanda") + "</span>",
			"SK<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Slovakia") + "</span>",
			"BH<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Bahrain") + "</span>",
			"LB<br/><span style='color:gray' title='sms-activate.org, smshub.org, 365sms.org, sms-man.ru, activation.pw'>" + tr("Lebanon") + "</span>",
			"AL<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Albania") + "</span>",
			"UY<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Uruguay") + "</span>",
			"GP<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Guadeloupe") + "</span>",
			"GF<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("French Guiana") + "</span>",
			"LC<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Saint Lucia") + "</span>",
			"LU<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Luxembourg") + "</span>",
			"VC<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Saint Vincent and the Grenadines") + "</span>",
			"GQ<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Equatorial Guinea") + "</span>",
			"AG<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Antigua and Barbuda") + "</span>",
			"NO<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Norway") + "</span>",
			"AW<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, 365sms.org, activation.pw'>" + tr("Aruba") + "</span>",
			"NC<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("New Caledonia") + "</span>",
			"CV<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, sms-man.ru, activation.pw'>" + tr("Cape Verde") + "</span>",
			"QA<br/><span style='color:gray' title='sms-activate.org, smshub.org, sms-man.ru, activation.pw'>" + tr("Qatar") + "</span>",
			"BN<br/><span style='color:gray' title='sms-activate.org, smshub.org, 365sms.org, activation.pw'>" + tr("Brunei Darussalam") + "</span>",
			"CF<br/><span style='color:gray' title='sms-activate.org, smshub.org, sms-man.ru, activation.pw'>" + tr("Central African Republic") + "</span>",
			"IS<br/><span style='color:gray' title='sms-activate.org, smshub.org, sms-man.ru, activation.pw'>" + tr("Iceland") + "</span>",
			"SO<br/><span style='color:gray' title='sms-activate.org, smshub.org, sms-man.ru, activation.pw'>" + tr("Somalia") + "</span>",
			"DJ<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, activation.pw'>" + tr("Djibouti") + "</span>",
			"KY<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, activation.pw'>" + tr("Cayman Islands") + "</span>",
			"ME<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, activation.pw'>" + tr("Montenegro") + "</span>",
			"ER<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, activation.pw'>" + tr("Eritrea") + "</span>",
			"SS<br/><span style='color:gray' title='sms-activate.org, smshub.org, sms-man.ru, activation.pw'>" + tr("South Sudan") + "</span>",
			"ST<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, activation.pw'>" + tr("Sao Tome and Principe") + "</span>",
			"MS<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, activation.pw'>" + tr("Montserrat") + "</span>",
			"AI<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, activation.pw'>" + tr("Anguilla") + "</span>",
			"SC<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, activation.pw'>" + tr("Seychelles") + "</span>",
			"SG<br/><span style='color:gray' title='sms-activate.org, smshub.org, 5sim.net, onlinesim.ru'>" + tr("Singapore") + "</span>",
			"AZ<br/><span style='color:gray' title='5sim.net, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Azerbaijan") + "</span>",
			"IR<br/><span style='color:gray' title='365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Iran, Islamic Republic of") + "</span>",
			"SD<br/><span style='color:gray' title='365sms.org, sms-man.ru, activation.pw, onlinesim.ru'>" + tr("Sudan") + "</span>",
			"MC<br/><span style='color:gray' title='sms-activate.org, smshub.org, activation.pw'>" + tr("Monaco") + "</span>",
			"JP<br/><span style='color:gray' title='5sim.net, sms-man.ru, activation.pw'>" + tr("Japan") + "</span>",
			"SY<br/><span style='color:gray' title='365sms.org, sms-man.ru, activation.pw'>" + tr("Syrian Arab Republic") + "</span>",
			"FJ<br/><span style='color:gray' title='sms-activate.org, smshub.org'>" + tr("Fiji") + "</span>",
			"CU<br/><span style='color:gray' title='sms-man.ru, activation.pw'>" + tr("Cuba") + "</span>",
			"WS<br/><span style='color:gray' title='5sim.net'>" + tr("Samoa") + "</span>",
			"SB<br/><span style='color:gray' title='5sim.net'>" + tr("Solomon Islands") + "</span>",
			"TO<br/><span style='color:gray' title='5sim.net'>" + tr("Tonga") + "</span>",
			"TC<br/><span style='color:gray' title='5sim.net'>" + tr("Turks and Caicos Islands") + "</span>",
			"VG<br/><span style='color:gray' title='5sim.net'>" + tr("Virgin Islands, British") + "</span>",
			"BM<br/><span style='color:gray' title='activation.pw'>" + tr("Bermuda") + "</span>",
			"EH<br/><span style='color:gray' title='activation.pw'>" + tr("Western Sahara") + "</span>"
		],
		help: {
			description: tr("Optional parameter.") + " " + tr("Country of the number."),
			examples: [
				{code: "RU", description: tr("Russian Federation")},
				{code: "UA", description: tr("Ukraine")},
				{code: "US", description: tr("United States")},
				{code: tr("Empty string"), description: tr("Depends on the service used, maybe the last used country or some specific country, you can find out more in the description of the API of the service.")}
			]
		}
	}) %>
	<%= _.template($('#block_start').html())({id:"Additional", name: tr("Additional settings"), description: ""}) %>
		<%= _.template($('#input_constructor').html())({
			id: "operator",
			description: tr("Operator"),
			default_selector: "string",
			disable_int: true,
			value_string: "",
			help: {
				description: tr("Optional parameter.") + " " + tr("Cellular operator of the number, in the form in which it is perceived by the SMS receiving service."),
				examples: [
					{code: "megafon", description: tr("MegaFon")},
					{code: "kyivstar", description: tr("Kyivstar")},
					{code: "tele2", description: "Tele2"},
					{code: tr("Empty string"), description: tr("Use any operator.")}
				]
			}
		}) %>
		<%= _.template($('#input_constructor').html())({
			id: "customSite",
			description: tr("Custom site"),
			default_selector: "string",
			disable_int: true,
			value_string: "",
			help: {
				description: tr("Optional parameter.") + " " + tr("Custom value of the site, in the form in which it is perceived by the SMS receiving service. If this parameter is specified, then it will be used instead of the \"Site\" parameter and sent to the service without preliminary processing."),
				examples: [
					{code: "wa", description: "WhatsApp"},
					{code: "tg", description: "Telegram"},
					{code: "go", description: "Google"},
					{code: tr("Empty string"), description: tr("Use value from \"") + tr("Site") + tr("\" parameter.")}
				]
			}
		}) %>
		<%= _.template($('#input_constructor').html())({
			id: "customCountry",
			description: tr("Custom country"),
			default_selector: "string",
			disable_int: true,
			value_string: "",
			help: {
				description: tr("Optional parameter.") + " " + tr("Custom value of the country, in the form in which it is perceived by the SMS receiving service. If this parameter is specified, then it will be used instead of the \"Country\" parameter and sent to the service without preliminary processing."),
				examples: [
					{code: "0", description: tr("Russian Federation")},
					{code: "1", description: tr("Ukraine")},
					{code: "187", description: tr("United States")},
					{code: tr("Empty string"), description: tr("Use value from \"") + tr("Country") + tr("\" parameter.")}
				]
			}
		}) %>
		<%= _.template($('#input_constructor').html())({
			id: "serverUrl",
			description: tr("Server url"),
			default_selector: "string",
			disable_int: true,
			value_string: "",
			help: {
				description: tr("Optional parameter.") + " " + tr("Url of the SMS receiving service server. Use this parameter to specify the url of the server, if the required service is not in the list of available ones, but it works through an API similar to the selected service."),
				examples: [
					{code: "https://sms.org"},
					{code: "http://receive-sms.com"},
					{code: "http://127.0.0.1:8888"},
					{code: tr("Empty string"), description: tr("Use default server url, https://api.sms-activate.org for sms-activate.org, etc")}
				]
			}
		}) %>
	<%= _.template($('#block_end').html())() %>
	<%= _.template($('#variable_constructor').html())({
		id: "Save",
		description: tr("Variable to save the result"),
		default_variable: "PHONE_NUMBER",
		help: {
			description:tr("Variable in which, after successful execution of the action, the phone number will be written."),
			examples: [
				{code: "79001112323"},
				{code: "17039688838"},
				{code: "380048698566"}
			]
		}
	}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get phone number from the SMS receiving service.</div>
	<div class="tr tooltip-paragraph-fold">This action will return a string containing the received phone number.</div>
	<div class="tooltip-paragraph-fold"><span class="tr">Example</span>: <code>"79001112323"</code></div>
	<div class="tr tooltip-paragraph-fold">The received number must be used in the "Get activation code" action to get the code from SMS, or in the "Change activation status" action to change the number status.</div>
	<div class="tr tooltip-paragraph-fold">You can specify your value for the operator, site and country in the corresponding parameters located in the additional settings. Please note that these values must be specified in the form in which the service perceives them, they will be sent as you specified them.</div>
	<div class="tr tooltip-paragraph-fold">If the required service is not in the list of available ones, but it works through an API similar to the selected service, then you can specify its server url in the corresponding parameter located in the additional settings.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", use_timeout:true, visible:true}) %>