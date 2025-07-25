_SMS.SmsRegApi.prototype.getRawSite = function(site){
	var sites = {
		"Other": "other",
		"VK": "vk",
		"Telegram": "telegram",
		"Microsoft": "microsoft",
		"WhatsApp": "whatsapp",
		"Facebook": "facebook",
		"Instagram": "instagram",
		"Viber": "viber",
		"Yahoo": "yahoo",
		"Aol": "aol",
		"Uber": "uber",
		"Twitter": "twitter",
		"WeChat": "wechat",
		"Google": "gmail",
		"KakaoTalk": "kakaotalk",
		"Imo": "imo",
		"TikTok": "tiktok",
		"Tinder": "tinder",
		"Naver": "naver",
		"Mamba": "mamba",
		"Ok.ru": "classmates",
		"Discord": "discord",
		"Apple": "apple",
		"Mail.ru": "mailru",
		"Yandex": "yandex",
		"Amazon": "amazon",
		"Steam": "steam",
		"Avito": "avito",
		"Netflix": "netflix",
		"Magnit": "magnit",
		"ProtonMail": "protonmail",
		"Alibaba": "alipay",
		"DiDi": "didi",
		"X5ID": "p5ka",
		"Aliexpress": "aliexpress",
		"Bolt": "bolt",
		"LINE Messenger": "lineme",
		"Gett": "gett",
		"Blizzard": "blizzard",
		"OZON": "ozonru",
		"Yalla": "yalla",
		"Craigslist": "craigslist",
		"LinkedIN": "linkedin",
		"OLX": "olx",
		"Airbnb": "airbnb",
		"Drom.ru": "dromru",
		"DrugVokrug": "drugvokrug",
		"PayPal": "paypal",
		"Snapchat": "snapchat",
		"Samokat": "samokat",
		"CoinBase": "coinbase",
		"Weibo": "weibo",
		"Glovo": "glovo",
		"eBay": "ebay",
		"Delivery Club": "deliveryclub",
		"Wildberries": "wildberries",
		"Twitch": "twitch",
		"MVideo": "mvideo",
		"Lenta": "lenta",
		"SberMarket": "sbermarket",
		"Taobao": "taobao",
		"KFC": "kfc",
		"Burger King": "burgerking",
		"AliPay": "alipay",
		"ICQ": "icq",
		"Wolt": "wolt",
		"SEOsprint": "seosprint",
		"Youla": "youla",
		"Faberlic": "faberlic",
		"ZdravCity": "zdravcity",
		"Whoosh": "whoosh",
		"Ukrnet": "ukrnet",
		"AVON": "avon",
		"IVI": "ivi",
		"Crypto.com": "cryptocom",
		"TradingView": "tradingview",
		"FACEIT": "faceit",
		"Signal": "signal",
		"PaySend": "paysend",
		"Oriflame": "oriflame",
		"OpenAI": "openai",
		"Uklon": "uklon",
		"McDonalds": "mcdonalds",
		"PaxFuL": "paxful",
		"Okko": "okko",
		"Revolut": "revolut",
		"Apteka.ru": "aptekaru",
		"Twilio": "twilio",
		"Kolesa.kz": "kolesakz",
		"Letual": "letoile",
		"Stormgain": "stormgain",
		"Lamoda": "lamoda",
		"Wise": "wise",
		"MeetMe": "meetme",
		"Lidl": "lidl",
		"YouDo": "youdo",
		"WebMoney": "webmoney",
		"YooMoney": "yoomoney",
		"Fotostrana": "fotostrana",
		"Badoo": "badoo",
		"1Cupis": "cupis",
		"Parimatch": "parimatch",
		"LocalBitcoins": "localbitcoins",
		"KoronaPay": "koronapay",
		"POCHTABANK": "pochtabank",
		"RESO": "resoru",
		"Binance": "binance",
		"Zadarma": "zadarma",
		"Tilda": "tilda",
		"profi.ru": "profi",
		"reg.ru": "regru",
		"Sravni.ru": "sravni",
		"iHerb": "iherb",
		"Ekapusta": "ekapusta",
		"Salampay": "salampay",
		"Zaymer": "zaymer",
		"Unistream": "unistream",
		"Delimobil": "delimobil",
		"NovaPoshta": "novaposhta",
		"Payoneer": "payoneer",
		"EAPTEKA": "eapteka",
		"NEXMO": "nexmo",
		"PREMIER": "premierone",
		"Raiffeisen": "raiffeisen",
		"AlfaStrah": "alfastrah",
		"MyACUVUE": "myacuvue",
		"RoboForex": "roboforex",
		"Neteller": "neteller",
		"Spotify": "spotify",
		"gusli.net": "gusli",
		"Matbea.com": "matbea",
		"Rostelecom": "rostelecom",
		"Selfcontrol": "selfcontrol",
		"BLS-SPAIN": "blsspain"
	};
	return sites.hasOwnProperty(site) ? sites[site] : sites["Other"];
};