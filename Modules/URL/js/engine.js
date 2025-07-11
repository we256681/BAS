_L["URL object"] = {"ru":"Объект URL"};
_L["New URL items"] = {"ru":"Новые элементы URL"};

function _encode_url_component(str){
	_validate_argument_type(str, 'string', 'String', '_encode_url_component');
	return encodeURIComponent(str);
};
function _decode_url_component(str){
	_validate_argument_type(str, 'string', 'String', '_decode_url_component');
	if(str.indexOf('+') > -1){
		str = str.replace(/\+/g, " ");
	};
	return decodeURIComponent(str);
};
function _normalize_url(url_string, user_options){
	_validate_argument_type(url_string, 'string', 'URL', '_normalize_url');
	var options = {
		base_url: {},
		default_protocol: 'http',
		normalize_protocol: true,
		force_http: false,
		force_https: false,
		strip_authentication: true,
		strip_query: false,
		strip_hash: false,
		strip_protocol: false,
		strip_www: true,
		remove_query_parameters: [/^utm_\w+/i],
		remove_trailing_slash: true,
		remove_directory_index: [/^index\.[a-z]+$/],
		sort_query_parameters: true
	};
	
	if(!_is_nilb(user_options) && typeof user_options==='object'){
		for(key in user_options){
			options[key] = (key=="remove_query_parameters" || key=="remove_directory_index") ? _to_arr(user_options[key]) : user_options[key];
		};
		if(_ends_with(options.default_protocol, ':')){
			options.default_protocol = options.default_protocol.slice(0, -1);
		};
	};
	
	var DATA_URL_DEFAULT_MIME_TYPE = 'text/plain';
	var DATA_URL_DEFAULT_CHARSET = 'us-ascii';

	var test_parameter = function(name, filters){
		return filters.some(function(filter){return filter instanceof RegExp ? filter.test(name) : filter === name});
	};

	var normalize_data_url = function(url_string){
		var strip_hash = options.strip_hash;
		var match = /^data:(.*?),(.*?)(?:#(.*))?$/.exec(url_string);

		if(!match){
			return url_string;
		};

		var type = match[1];
		var data = match[2];
		var hash = match[3];
		var media_type = type.split(';');
		hash = strip_hash ? '' : hash;

		var isBase64 = false;
		if(media_type[media_type.length - 1] === 'base64'){
			media_type.pop();
			isBase64 = true;
		};

		var mime_type = (media_type.shift() || '').toLowerCase();
		var attributes = media_type.map(function(attribute){
			var split = attribute.split('=').map(function(string){return string.trim()});
			var key = split[0];
			var value = _avoid_nilb(split[1], '');

			if(key === 'charset'){
				value = value.toLowerCase();

				if(value === DATA_URL_DEFAULT_CHARSET){
					return '';
				};
			};

			return key + _is_nilb(value) ? '' : '=' + value;
		}).filter(Boolean);

		var normalized_media_type = attributes.slice();

		if(isBase64){
			normalized_media_type.push('base64');
		};

		if(normalized_media_type.length !== 0 || (mime_type && mime_type !== DATA_URL_DEFAULT_MIME_TYPE)){
			normalized_media_type.unshift(mime_type);
		};

		return 'data:' + normalized_media_type.join(';') + ',' + (isBase64 ? data.trim() : data) + (hash ? ('#' + hash) : '')
	};

	url_string = url_string.trim();

	if(/^data:/i.test(url_string)){
		return normalize_data_url(url_string);
	};
	
	var has_relative_protocol = _starts_with(url_string, '//');
	
	if(!new RegExp("^(?:ftp|http|ws)s?://").test(url_string) && !new RegExp("^/[^/]").test(url_string)){
		var tlds = "AAA|AARP|ABARTH|ABB|ABBOTT|ABBVIE|ABC|ABLE|ABOGADO|ABUDHABI|AC|ACADEMY|ACCENTURE|ACCOUNTANT|ACCOUNTANTS|ACO|ACTOR|AD|ADAC|ADS|ADULT|AE|AEG|AERO|AETNA|AF|AFAMILYCOMPANY|AFL|AFRICA|AG|AGAKHAN|AGENCY|AI|AIG|AIGO|AIRBUS|AIRFORCE|AIRTEL|AKDN|AL|ALFAROMEO|ALIBABA|ALIPAY|ALLFINANZ|ALLSTATE|ALLY|ALSACE|ALSTOM|AM|AMERICANEXPRESS|AMERICANFAMILY|AMEX|AMFAM|AMICA|AMSTERDAM|ANALYTICS|ANDROID|ANQUAN|ANZ|AO|AOL|APARTMENTS|APP|APPLE|AQ|AQUARELLE|AR|ARAB|ARAMCO|ARCHI|ARMY|ARPA|ART|ARTE|AS|ASDA|ASIA|ASSOCIATES|AT|ATHLETA|ATTORNEY|AU|AUCTION|AUDI|AUDIBLE|AUDIO|AUSPOST|AUTHOR|AUTO|AUTOS|AVIANCA|AW|AWS|AX|AXA|AZ|AZURE|BA|BABY|BAIDU|BANAMEX|BANANAREPUBLIC|BAND|BANK|BAR|BARCELONA|BARCLAYCARD|BARCLAYS|BAREFOOT|BARGAINS|BASEBALL|BASKETBALL|BAUHAUS|BAYERN|BB|BBC|BBT|BBVA|BCG|BCN|BD|BE|BEATS|BEAUTY|BEER|BENTLEY|BERLIN|BEST|BESTBUY|BET|BF|BG|BH|BHARTI|BI|BIBLE|BID|BIKE|BING|BINGO|BIO|BIZ|BJ|BLACK|BLACKFRIDAY|BLOCKBUSTER|BLOG|BLOOMBERG|BLUE|BM|BMS|BMW|BN|BNPPARIBAS|BO|BOATS|BOEHRINGER|BOFA|BOM|BOND|BOO|BOOK|BOOKING|BOSCH|BOSTIK|BOSTON|BOT|BOUTIQUE|BOX|BR|BRADESCO|BRIDGESTONE|BROADWAY|BROKER|BROTHER|BRUSSELS|BS|BT|BUDAPEST|BUGATTI|BUILD|BUILDERS|BUSINESS|BUY|BUZZ|BV|BW|BY|BZ|BZH|CA|CAB|CAFE|CAL|CALL|CALVINKLEIN|CAM|CAMERA|CAMP|CANCERRESEARCH|CANON|CAPETOWN|CAPITAL|CAPITALONE|CAR|CARAVAN|CARDS|CARE|CAREER|CAREERS|CARS|CASA|CASE|CASEIH|CASH|CASINO|CAT|CATERING|CATHOLIC|CBA|CBN|CBRE|CBS|CC|CD|CEB|CENTER|CEO|CERN|CF|CFA|CFD|CG|CH|CHANEL|CHANNEL|CHARITY|CHASE|CHAT|CHEAP|CHINTAI|CHRISTMAS|CHROME|CHURCH|CI|CIPRIANI|CIRCLE|CISCO|CITADEL|CITI|CITIC|CITY|CITYEATS|CK|CL|CLAIMS|CLEANING|CLICK|CLINIC|CLINIQUE|CLOTHING|CLOUD|CLUB|CLUBMED|CM|CN|CO|COACH|CODES|COFFEE|COLLEGE|COLOGNE|COM|COMCAST|COMMBANK|COMMUNITY|COMPANY|COMPARE|COMPUTER|COMSEC|CONDOS|CONSTRUCTION|CONSULTING|CONTACT|CONTRACTORS|COOKING|COOKINGCHANNEL|COOL|COOP|CORSICA|COUNTRY|COUPON|COUPONS|COURSES|CPA|CR|CREDIT|CREDITCARD|CREDITUNION|CRICKET|CROWN|CRS|CRUISE|CRUISES|CSC|CU|CUISINELLA|CV|CW|CX|CY|CYMRU|CYOU|CZ|DABUR|DAD|DANCE|DATA|DATE|DATING|DATSUN|DAY|DCLK|DDS|DE|DEAL|DEALER|DEALS|DEGREE|DELIVERY|DELL|DELOITTE|DELTA|DEMOCRAT|DENTAL|DENTIST|DESI|DESIGN|DEV|DHL|DIAMONDS|DIET|DIGITAL|DIRECT|DIRECTORY|DISCOUNT|DISCOVER|DISH|DIY|DJ|DK|DM|DNP|DO|DOCS|DOCTOR|DOG|DOMAINS|DOT|DOWNLOAD|DRIVE|DTV|DUBAI|DUCK|DUNLOP|DUPONT|DURBAN|DVAG|DVR|DZ|EARTH|EAT|EC|ECO|EDEKA|EDU|EDUCATION|EE|EG|EMAIL|EMERCK|ENERGY|ENGINEER|ENGINEERING|ENTERPRISES|EPSON|EQUIPMENT|ER|ERICSSON|ERNI|ES|ESQ|ESTATE|ESURANCE|ET|ETISALAT|EU|EUROVISION|EUS|EVENTS|EXCHANGE|EXPERT|EXPOSED|EXPRESS|EXTRASPACE|FAGE|FAIL|FAIRWINDS|FAITH|FAMILY|FAN|FANS|FARM|FARMERS|FASHION|FAST|FEDEX|FEEDBACK|FERRARI|FERRERO|FI|FIAT|FIDELITY|FIDO|FILM|FINAL|FINANCE|FINANCIAL|FIRE|FIRESTONE|FIRMDALE|FISH|FISHING|FIT|FITNESS|FJ|FK|FLICKR|FLIGHTS|FLIR|FLORIST|FLOWERS|FLY|FM|FO|FOO|FOOD|FOODNETWORK|FOOTBALL|FORD|FOREX|FORSALE|FORUM|FOUNDATION|FOX|FR|FREE|FRESENIUS|FRL|FROGANS|FRONTDOOR|FRONTIER|FTR|FUJITSU|FUJIXEROX|FUN|FUND|FURNITURE|FUTBOL|FYI|GA|GAL|GALLERY|GALLO|GALLUP|GAME|GAMES|GAP|GARDEN|GAY|GB|GBIZ|GD|GDN|GE|GEA|GENT|GENTING|GEORGE|GF|GG|GGEE|GH|GI|GIFT|GIFTS|GIVES|GIVING|GL|GLADE|GLASS|GLE|GLOBAL|GLOBO|GM|GMAIL|GMBH|GMO|GMX|GN|GODADDY|GOLD|GOLDPOINT|GOLF|GOO|GOODYEAR|GOOG|GOOGLE|GOP|GOT|GOV|GP|GQ|GR|GRAINGER|GRAPHICS|GRATIS|GREEN|GRIPE|GROCERY|GROUP|GS|GT|GU|GUARDIAN|GUCCI|GUGE|GUIDE|GUITARS|GURU|GW|GY|HAIR|HAMBURG|HANGOUT|HAUS|HBO|HDFC|HDFCBANK|HEALTH|HEALTHCARE|HELP|HELSINKI|HERE|HERMES|HGTV|HIPHOP|HISAMITSU|HITACHI|HIV|HK|HKT|HM|HN|HOCKEY|HOLDINGS|HOLIDAY|HOMEDEPOT|HOMEGOODS|HOMES|HOMESENSE|HONDA|HORSE|HOSPITAL|HOST|HOSTING|HOT|HOTELES|HOTELS|HOTMAIL|HOUSE|HOW|HR|HSBC|HT|HU|HUGHES|HYATT|HYUNDAI|IBM|ICBC|ICE|ICU|ID|IE|IEEE|IFM|IKANO|IL|IM|IMAMAT|IMDB|IMMO|IMMOBILIEN|IN|INC|INDUSTRIES|INFINITI|INFO|ING|INK|INSTITUTE|INSURANCE|INSURE|INT|INTEL|INTERNATIONAL|INTUIT|INVESTMENTS|IO|IPIRANGA|IQ|IR|IRISH|IS|ISMAILI|IST|ISTANBUL|IT|ITAU|ITV|IVECO|JAGUAR|JAVA|JCB|JCP|JE|JEEP|JETZT|JEWELRY|JIO|JLL|JM|JMP|JNJ|JO|JOBS|JOBURG|JOT|JOY|JP|JPMORGAN|JPRS|JUEGOS|JUNIPER|KAUFEN|KDDI|KE|KERRYHOTELS|KERRYLOGISTICS|KERRYPROPERTIES|KFH|KG|KH|KI|KIA|KIM|KINDER|KINDLE|KITCHEN|KIWI|KM|KN|KOELN|KOMATSU|KOSHER|KP|KPMG|KPN|KR|KRD|KRED|KUOKGROUP|KW|KY|KYOTO|KZ|LA|LACAIXA|LAMBORGHINI|LAMER|LANCASTER|LANCIA|LAND|LANDROVER|LANXESS|LASALLE|LAT|LATINO|LATROBE|LAW|LAWYER|LB|LC|LDS|LEASE|LECLERC|LEFRAK|LEGAL|LEGO|LEXUS|LGBT|LI|LIDL|LIFE|LIFEINSURANCE|LIFESTYLE|LIGHTING|LIKE|LILLY|LIMITED|LIMO|LINCOLN|LINDE|LINK|LIPSY|LIVE|LIVING|LIXIL|LK|LLC|LLP|LOAN|LOANS|LOCKER|LOCUS|LOFT|LOL|LONDON|LOTTE|LOTTO|LOVE|LPL|LPLFINANCIAL|LR|LS|LT|LTD|LTDA|LU|LUNDBECK|LUPIN|LUXE|LUXURY|LV|LY|MA|MACYS|MADRID|MAIF|MAISON|MAKEUP|MAN|MANAGEMENT|MANGO|MAP|MARKET|MARKETING|MARKETS|MARRIOTT|MARSHALLS|MASERATI|MATTEL|MBA|MC|MCKINSEY|MD|ME|MED|MEDIA|MEET|MELBOURNE|MEME|MEMORIAL|MEN|MENU|MERCKMSD|METLIFE|MG|MH|MIAMI|MICROSOFT|MIL|MINI|MINT|MIT|MITSUBISHI|MK|ML|MLB|MLS|MM|MMA|MN|MO|MOBI|MOBILE|MODA|MOE|MOI|MOM|MONASH|MONEY|MONSTER|MORMON|MORTGAGE|MOSCOW|MOTO|MOTORCYCLES|MOV|MOVIE|MP|MQ|MR|MS|MSD|MT|MTN|MTR|MU|MUSEUM|MUTUAL|MV|MW|MX|MY|MZ|NA|NAB|NAGOYA|NAME|NATIONWIDE|NATURA|NAVY|NBA|NC|NE|NEC|NET|NETBANK|NETFLIX|NETWORK|NEUSTAR|NEW|NEWHOLLAND|NEWS|NEXT|NEXTDIRECT|NEXUS|NF|NFL|NG|NGO|NHK|NI|NICO|NIKE|NIKON|NINJA|NISSAN|NISSAY|NL|NO|NOKIA|NORTHWESTERNMUTUAL|NORTON|NOW|NOWRUZ|NOWTV|NP|NR|NRA|NRW|NTT|NU|NYC|NZ|OBI|OBSERVER|OFF|OFFICE|OKINAWA|OLAYAN|OLAYANGROUP|OLDNAVY|OLLO|OM|OMEGA|ONE|ONG|ONL|ONLINE|ONYOURSIDE|OOO|OPEN|ORACLE|ORANGE|ORG|ORGANIC|ORIGINS|OSAKA|OTSUKA|OTT|OVH|PA|PAGE|PANASONIC|PARIS|PARS|PARTNERS|PARTS|PARTY|PASSAGENS|PAY|PCCW|PE|PET|PF|PFIZER|PG|PH|PHARMACY|PHD|PHILIPS|PHONE|PHOTO|PHOTOGRAPHY|PHOTOS|PHYSIO|PICS|PICTET|PICTURES|PID|PIN|PING|PINK|PIONEER|PIZZA|PK|PL|PLACE|PLAY|PLAYSTATION|PLUMBING|PLUS|PM|PN|PNC|POHL|POKER|POLITIE|PORN|POST|PR|PRAMERICA|PRAXI|PRESS|PRIME|PRO|PROD|PRODUCTIONS|PROF|PROGRESSIVE|PROMO|PROPERTIES|PROPERTY|PROTECTION|PRU|PRUDENTIAL|PS|PT|PUB|PW|PWC|PY|QA|QPON|QUEBEC|QUEST|QVC|RACING|RADIO|RAID|RE|READ|REALESTATE|REALTOR|REALTY|RECIPES|RED|REDSTONE|REDUMBRELLA|REHAB|REISE|REISEN|REIT|RELIANCE|REN|RENT|RENTALS|REPAIR|REPORT|REPUBLICAN|REST|RESTAURANT|REVIEW|REVIEWS|REXROTH|RICH|RICHARDLI|RICOH|RIGHTATHOME|RIL|RIO|RIP|RMIT|RO|ROCHER|ROCKS|RODEO|ROGERS|ROOM|RS|RSVP|RU|RUGBY|RUHR|RUN|RW|RWE|RYUKYU|SA|SAARLAND|SAFE|SAFETY|SAKURA|SALE|SALON|SAMSCLUB|SAMSUNG|SANDVIK|SANDVIKCOROMANT|SANOFI|SAP|SARL|SAS|SAVE|SAXO|SB|SBI|SBS|SC|SCA|SCB|SCHAEFFLER|SCHMIDT|SCHOLARSHIPS|SCHOOL|SCHULE|SCHWARZ|SCIENCE|SCJOHNSON|SCOR|SCOT|SD|SE|SEARCH|SEAT|SECURE|SECURITY|SEEK|SELECT|SENER|SERVICES|SES|SEVEN|SEW|SEX|SEXY|SFR|SG|SH|SHANGRILA|SHARP|SHAW|SHELL|SHIA|SHIKSHA|SHOES|SHOP|SHOPPING|SHOUJI|SHOW|SHOWTIME|SHRIRAM|SI|SILK|SINA|SINGLES|SITE|SJ|SK|SKI|SKIN|SKY|SKYPE|SL|SLING|SM|SMART|SMILE|SN|SNCF|SO|SOCCER|SOCIAL|SOFTBANK|SOFTWARE|SOHU|SOLAR|SOLUTIONS|SONG|SONY|SOY|SPACE|SPORT|SPOT|SPREADBETTING|SR|SRL|SS|ST|STADA|STAPLES|STAR|STATEBANK|STATEFARM|STC|STCGROUP|STOCKHOLM|STORAGE|STORE|STREAM|STUDIO|STUDY|STYLE|SU|SUCKS|SUPPLIES|SUPPLY|SUPPORT|SURF|SURGERY|SUZUKI|SV|SWATCH|SWIFTCOVER|SWISS|SX|SY|SYDNEY|SYMANTEC|SYSTEMS|SZ|TAB|TAIPEI|TALK|TAOBAO|TARGET|TATAMOTORS|TATAR|TATTOO|TAX|TAXI|TC|TCI|TD|TDK|TEAM|TECH|TECHNOLOGY|TEL|TEMASEK|TENNIS|TEVA|TF|TG|TH|THD|THEATER|THEATRE|TIAA|TICKETS|TIENDA|TIFFANY|TIPS|TIRES|TIROL|TJ|TJMAXX|TJX|TK|TKMAXX|TL|TM|TMALL|TN|TO|TODAY|TOKYO|TOOLS|TOP|TORAY|TOSHIBA|TOTAL|TOURS|TOWN|TOYOTA|TOYS|TR|TRADE|TRADING|TRAINING|TRAVEL|TRAVELCHANNEL|TRAVELERS|TRAVELERSINSURANCE|TRUST|TRV|TT|TUBE|TUI|TUNES|TUSHU|TV|TVS|TW|TZ|UA|UBANK|UBS|UG|UK|UNICOM|UNIVERSITY|UNO|UOL|UPS|US|UY|UZ|VA|VACATIONS|VANA|VANGUARD|VC|VE|VEGAS|VENTURES|VERISIGN|VERSICHERUNG|VET|VG|VI|VIAJES|VIDEO|VIG|VIKING|VILLAS|VIN|VIP|VIRGIN|VISA|VISION|VIVA|VIVO|VLAANDEREN|VN|VODKA|VOLKSWAGEN|VOLVO|VOTE|VOTING|VOTO|VOYAGE|VU|VUELOS|WALES|WALMART|WALTER|WANG|WANGGOU|WATCH|WATCHES|WEATHER|WEATHERCHANNEL|WEBCAM|WEBER|WEBSITE|WED|WEDDING|WEIBO|WEIR|WF|WHOSWHO|WIEN|WIKI|WILLIAMHILL|WIN|WINDOWS|WINE|WINNERS|WME|WOLTERSKLUWER|WOODSIDE|WORK|WORKS|WORLD|WOW|WS|WTC|WTF|XBOX|XEROX|XFINITY|XIHUAN|XIN|XN--11B4C3D|XN--1CK2E1B|XN--1QQW23A|XN--2SCRJ9C|XN--30RR7Y|XN--3BST00M|XN--3DS443G|XN--3E0B707E|XN--3HCRJ9C|XN--3OQ18VL8PN36A|XN--3PXU8K|XN--42C2D9A|XN--45BR5CYL|XN--45BRJ9C|XN--45Q11C|XN--4GBRIM|XN--54B7FTA0CC|XN--55QW42G|XN--55QX5D|XN--5SU34J936BGSG|XN--5TZM5G|XN--6FRZ82G|XN--6QQ986B3XL|XN--80ADXHKS|XN--80AO21A|XN--80AQECDR1A|XN--80ASEHDB|XN--80ASWG|XN--8Y0A063A|XN--90A3AC|XN--90AE|XN--90AIS|XN--9DBQ2A|XN--9ET52U|XN--9KRT00A|XN--B4W605FERD|XN--BCK1B9A5DRE4C|XN--C1AVG|XN--C2BR7G|XN--CCK2B3B|XN--CG4BKI|XN--CLCHC0EA0B2G2A9GCD|XN--CZR694B|XN--CZRS0T|XN--CZRU2D|XN--D1ACJ3B|XN--D1ALF|XN--E1A4C|XN--ECKVDTC9D|XN--EFVY88H|XN--FCT429K|XN--FHBEI|XN--FIQ228C5HS|XN--FIQ64B|XN--FIQS8S|XN--FIQZ9S|XN--FJQ720A|XN--FLW351E|XN--FPCRJ9C3D|XN--FZC2C9E2C|XN--FZYS8D69UVGM|XN--G2XX48C|XN--GCKR3F0F|XN--GECRJ9C|XN--GK3AT1E|XN--H2BREG3EVE|XN--H2BRJ9C|XN--H2BRJ9C8C|XN--HXT814E|XN--I1B6B1A6A2E|XN--IMR513N|XN--IO0A7I|XN--J1AEF|XN--J1AMH|XN--J6W193G|XN--JLQ61U9W7B|XN--JVR189M|XN--KCRX77D1X4A|XN--KPRW13D|XN--KPRY57D|XN--KPU716F|XN--KPUT3I|XN--L1ACC|XN--LGBBAT1AD8J|XN--MGB9AWBF|XN--MGBA3A3EJT|XN--MGBA3A4F16A|XN--MGBA7C0BBN0A|XN--MGBAAKC7DVF|XN--MGBAAM7A8H|XN--MGBAB2BD|XN--MGBAH1A3HJKRD|XN--MGBAI9AZGQP6J|XN--MGBAYH7GPA|XN--MGBBH1A|XN--MGBBH1A71E|XN--MGBC0A9AZCG|XN--MGBCA7DZDO|XN--MGBCPQ6GPA1A|XN--MGBERP4A5D4AR|XN--MGBGU82A|XN--MGBI4ECEXP|XN--MGBPL2FH|XN--MGBT3DHD|XN--MGBTX2B|XN--MGBX4CD0AB|XN--MIX891F|XN--MK1BU44C|XN--MXTQ1M|XN--NGBC5AZD|XN--NGBE9E0A|XN--NGBRX|XN--NODE|XN--NQV7F|XN--NQV7FS00EMA|XN--NYQY26A|XN--O3CW4H|XN--OGBPF8FL|XN--OTU796D|XN--P1ACF|XN--P1AI|XN--PBT977C|XN--PGBS0DH|XN--PSSY2U|XN--Q7CE6A|XN--Q9JYB4C|XN--QCKA1PMC|XN--QXA6A|XN--QXAM|XN--RHQV96G|XN--ROVU88B|XN--RVC1E0AM3E|XN--S9BRJ9C|XN--SES554G|XN--T60B56A|XN--TCKWE|XN--TIQ49XQYJ|XN--UNUP4Y|XN--VERMGENSBERATER-CTB|XN--VERMGENSBERATUNG-PWB|XN--VHQUV|XN--VUQ861B|XN--W4R85EL8FHU5DNRA|XN--W4RS40L|XN--WGBH1C|XN--WGBL6A|XN--XHQ521B|XN--XKC2AL3HYE2A|XN--XKC2DL3A5EE0H|XN--Y9A3AQ|XN--YFRO4I67O|XN--YGBI2AMMX|XN--ZFR164B|XXX|XYZ|YACHTS|YAHOO|YAMAXUN|YANDEX|YE|YODOBASHI|YOGA|YOKOHAMA|YOU|YOUTUBE|YT|YUN|ZA|ZAPPOS|ZARA|ZERO|ZIP|ZM|ZONE|ZUERICH|ZW|TEST|AMAZON|SPA|कॉम|セール|佛山|ಭಾರತ|慈善|集团|在线|한국|ଭାରତ|大众汽车|点看|คอม|ভাৰত|ভারত|八卦|موقع|বাংলা|公益|公司|香格里拉|网站|移动|我爱你|МОСКВА|ҚАЗ|КАТОЛИК|ОНЛАЙН|САЙТ|联通|СРБ|БГ|БЕЛ|קום|时尚|微博|淡马锡|ファッション|ОРГ|नेट|ストア|アマゾン|삼성|சிங்கப்பூர்|商标|商店|商城|ДЕТИ|МКД|ЕЮ|ポイント|新闻|家電|كوم|中文网|中信|中国|中國|娱乐|谷歌|భారత్|ලංකා|電訊盈科|购物|クラウド|ભારત|通販|भारतम्|भारत|भारोत|网店|संगठन|餐厅|网络|КОМ|УКР|香港|亚马逊|诺基亚|食品|飞利浦|台湾|台灣|手机|МОН|الجزائر|عمان|ارامكو|ایران|العليان|اتصالات|امارات|بازار|موريتانيا|پاکستان|الاردن|بارت|بھارت|المغرب|ابوظبي|البحرين|السعودية|ڀارت|كاثوليك|سودان|همراه|عراق|مليسيا|澳門|닷컴|政府|شبكة|بيتك|عرب|ᲒᲔ|机构|组织机构|健康|ไทย|سورية|招聘|РУС|РФ|تونس|大拿|ລາວ|みんな|グーグル|ΕΥ|ΕΛ|世界|書籍|ഭാരതം|ਭਾਰਤ|网址|닷넷|コム|天主教|游戏|VERMÖGENSBERATER|VERMÖGENSBERATUNG|企业|信息|嘉里大酒店|嘉里|مصر|قطر|广东|இலங்கை|இந்தியா|ՀԱՅ|新加坡|فلسطين|政务";
		url_string = url_string.replace(new RegExp("^((?:ftp|http|ws)s?)?[:/]{1,3}"), function(a, p){
			var protocol = p || options.default_protocol;
			if(_ends_with(protocol, ':')){
				protocol = protocol.slice(0, -1);
			};
			return protocol + "://"
		});
		url_string = new RegExp("^[a-z][a-z0-9.+-]*://", "i").test(url_string) ? url_string : (new RegExp("^([a-zа-яё0-9]+(-+[a-zа-яё0-9]+)*\\.)+(" + tlds + ")", "i").test(url_string) ? (options.default_protocol + "://" + url_string) : url_string);
	};

	var url_obj = new _url(url_string, {normalize: false, base_url: options.base_url, rfail: false});

	if(options.force_http && url_obj.protocol === 'https'){
		url_obj.set('protocol', 'http');
	};

	if(options.force_https && url_obj.protocol === 'http'){
		url_obj.set('protocol', 'https');
	};

	if(options.strip_authentication){
		url_obj.set('username', '');
		url_obj.set('password', '');
	};

	if(options.strip_hash){
		url_obj.set('hash', '');
	};

	if(url_obj.pathname){
		url_obj.set('pathname', url_obj.pathname.replace(/(\b(?:[a-z][a-z\d+\-.]{1,50}:))?\/{2,}/g, function($0,$1){return $1?$0:'/';}));
	};

	if(url_obj.pathname){
		try {
			url_obj.set('pathname', decodeURI(url_obj.pathname));
		} catch (_) {}
	};

	if(Array.isArray(options.remove_directory_index) && options.remove_directory_index.length > 0){
		var path_components = url_obj.pathname.split('/');
		var last_component = path_components[path_components.length - 1];

		if(test_parameter(last_component, options.remove_directory_index)){
			path_components = path_components.slice(0, path_components.length - 1);
			url_obj.set('pathname', path_components.slice(1).join('/') + '/');
		};
	};

	if(url_obj.hostname){
		url_obj.set('hostname', url_obj.hostname.replace(/\.$/g, ''));

		if(options.strip_www && /^www\.(?!www\.)(?:[a-z\-\d]{1,63})\.(?:[a-z.\-\d]{2,63})$/.test(url_obj.hostname)){
			url_obj.set('hostname', url_obj.hostname.replace(/^www\./, ''));
		};
	};
	
	if(Array.isArray(options.remove_query_parameters) && options.remove_query_parameters.length > 0){
		for(key in url_obj.query){
			if(test_parameter(key, options.remove_query_parameters)){
				delete url_obj.query[key];
			};
		};
	};

	if(options.sort_query_parameters){
		var obj_sorted = {};
		_sort_arr(Object.keys(url_obj.query), true).forEach(function(key){
			obj_sorted[key] = url_obj.query[key];
		});
		url_obj.set('query', obj_sorted);
	};

	if(options.remove_trailing_slash){
		url_obj.set('pathname', url_obj.pathname.replace(/[\/]+$/, ''));
	};
	
	if(has_relative_protocol && !options.normalize_protocol){
		url_obj.set('protocol', '');
		url_obj.set('slashes', true);
	};
	
	if(options.strip_protocol){
		url_obj.set('protocol', '');
		url_obj.set('slashes', false);
	};
	
	if(options.strip_query){
		url_obj.set('query', {});
	};
	
	var old_url_string = url_string;

	url_string = url_obj.toString();

	return url_string;
};
function _url(address, user_options){
	_validate_argument_type(address, 'string', 'URL', '_url');
	
	if(!(this instanceof _url)){
		return new _url(address, user_options);
	};
	
	var options = {
		normalize: true,
		base_url: {},
		query_parser: true,
		rfail: false
	};
	
	if(!_is_nilb(user_options) && typeof user_options==='object'){
		for(key in user_options){
			options[key] = user_options[key];
		};
	};
	
	var parser = (options.query_parser && typeof options.query_parser !== 'function') ? _query_string.decode : options.query_parser;
	address = _trim_left(address);
	if(options.normalize){
		address = _normalize_url(address, {
			base_url: options.base_url,
			strip_authentication: false,
			strip_hash: false,
			strip_www: false,
			strip_protocol: false,
			remove_query_parameters: [],
			remove_directory_index: []
		});
	};
	
	var original_url = address;
	
	var slashes = new RegExp("^[A-Za-z][A-Za-z0-9+-.]*:\\/\\/");
	var protocolre = /^([a-z][a-z0-9.+-]*:)?(\/\/)?([\S\s]*)/i;
	var rules = [
		['#', 'hash'],
		['?', 'query'],
		function sanitize(address){
			return address.replace('\\', '/');
		},
		['/', 'pathname'],
		['@', 'auth', 1],
		[NaN, 'host', undefined, 1, 1],
		[/:(\d+)$/, 'port', undefined, 1],
		[NaN, 'hostname', undefined, 1, 1]
	];
	var ignore = {hash:1,query:1};
	this.required = function(port, protocol){
		protocol = _ends_with(protocol, ':') ? protocol.slice(0, -1) : protocol;
		port = +port;

		if(!port){return false};

		switch(protocol){
			case 'http':
			case 'ws':
				return port !== 80;

			case 'https':
			case 'wss':
				return port !== 443;

			case 'ftp':
				return port !== 21;

			case 'gopher':
				return port !== 70;

			case 'file':
				return false;
		}

		return port !== 0;
	};
	var lolcation = function(loc){
		var globalVar = '';

		if(typeof window !== 'undefined'){
			globalVar = window;
		}else if(typeof global !== 'undefined'){
			globalVar = global;
		}else if(typeof self !== 'undefined'){
			globalVar = self;
		}else{
			globalVar = {}
		};

		var location = globalVar.location || {};
		loc = loc || location;

		var finaldestination = {};
		var type = typeof loc;
		var key = '';

		if('blob:' === loc.protocol){
			finaldestination = new _url(unescape(loc.pathname), {normalize: false, rfail: false});
		}else if('string' === type){
			finaldestination = new _url(loc, {normalize: false, rfail: false});
			for (key in ignore) delete finaldestination[key];
		}else if('object' === type){
			for(key in loc){
				if(key in ignore){continue};
				finaldestination[key] = loc[key];
			};

			if(finaldestination.slashes === undefined){
				finaldestination.slashes = slashes.test(loc.href);
			};
		};

		return finaldestination;
	};
	var extract_protocol = function(address){
		address = _trim_left(address);
		var match = protocolre.exec(address);

		return {
			protocol: match[1] ? match[1].toLowerCase() : '',
			slashes: !!match[2],
			rest: match[3]
		};
	};
	var resolve = function(relative, base){
		if(relative === ''){return base};

		var path = (base || '/').split('/').slice(0, -1).concat(relative.split('/'));
		var i = path.length;
		var last = path[i - 1];
		var unshift = false;
		var up = 0;

		while (i--) {
			if(path[i] === '.' || (path[i] === '' && i !== 0)){
				path.splice(i, 1);
			}else if(path[i] === '..'){
				path.splice(i, 1);
				up++;
			}else if(up){
				if(i === 0){unshift = true};
				path.splice(i, 1);
				up--;
			};
		};

		if(unshift){path.unshift('')};
		if(last === '.' || last === '..'){path.push('')};

		return path.join('/');
	};

	var index = '';
	var url = this;
	var instructions = rules.slice();
	
	base_url = lolcation(options.base_url);

	var extracted = extract_protocol(address || '');
	var relative = !extracted.protocol && !extracted.slashes;
	url.slashes = extracted.slashes || relative && base_url.slashes;
	url.protocol = extracted.protocol || base_url.protocol || '';
	address = extracted.rest;

	if(!extracted.slashes){instructions[3] = [/(.*)/, 'pathname']};

	for(var i = 0; i < instructions.length; i++){
		var instruction = instructions[i];

		if(typeof instruction === 'function'){
			address = instruction(address);
			continue;
		};

		var parse = instruction[0];
		var key = instruction[1];

		if(parse !== parse){
			url[key] = address;
		}else if('string' === typeof parse){
			if(~(index = address.indexOf(parse))){
				if('number' === typeof instruction[2]){
					url[key] = address.slice(0, index);
					address = address.slice(index + instruction[2]);
				}else{
					url[key] = address.slice(index);
					address = address.slice(0, index);
				};
			};
		}else if((index = parse.exec(address))){
			url[key] = index[1];
			address = address.slice(0, index.index);
		};
	
		url[key] = url[key] || (relative && instruction[3] ? base_url[key] || '' : '');

		if(instruction[4]){url[key] = url[key].toLowerCase()};
	};
	
	if(parser){url.query = parser(url.query)};
	
	if((relative && base_url.slashes && !_starts_with(url.pathname, '/') && (url.pathname !== '' || base_url.pathname !== '')) || new RegExp("\\/(\\.{1,2}|)\\/").test(url.pathname)){
		url.pathname = resolve(url.pathname, base_url.pathname);
	};
	
	if(!url.required(url.port, url.protocol)){
		url.host = url.hostname;
		url.port = '';
	};
	
	url.username = '';
	url.password = '';
	if(url.auth){
		var instruction = url.auth.split(':');
		url.username = instruction[0] || '';
		url.password = instruction[1] || '';
	};
	
	url.origin = url.protocol && url.host && url.protocol !== 'file' ? url.protocol + '://' + url.host : 'null';
	
	if(options.rfail && url.protocol==="" && url.host==="" && url.origin==="null"){
		fail(_K=="ru" ? ("Указан недействительный URL | " + original_url) : ("Invalid URL specified | " + original_url));
	};
	
	if(url.protocol && _ends_with(url.protocol, ':')){
		url.protocol = url.protocol.slice(0, -1);
	};
	
	if(url.hash && _starts_with(url.hash, '#')){
		url.hash = url.hash.slice(1);
	};
	
	url.href = url.toString();
};
_url.prototype.set = function(part, value, fn){
	var url = this;
	var rules = [
		['#', 'hash'],
		['?', 'query'],
		function sanitize(address){
			return address.replace('\\', '/');
		},
		['/', 'pathname'],
		['@', 'auth', 1],
		[NaN, 'host', undefined, 1, 1],
		[/:(\d+)$/, 'port', undefined, 1],
		[NaN, 'hostname', undefined, 1, 1]
	];
	if(!url.slashes){rules[3] = [/(.*)/, 'pathname']};
	
	switch(part){
		case 'query':
			if('string' === typeof value && value.length){
				value = (fn || _query_string.decode)(value);
			};
			url[part] = value;
			break;
		case 'port':
			url[part] = value;
			if(!url.required(value, url.protocol)){
				url.host = url.hostname;
				url[part] = '';
			}else if(value){
				url.host = url.hostname +':'+ value;
			};
			break;
		case 'hostname':
			url[part] = value;
			if(url.port){
				value += ':'+ url.port
			};
			url.host = value;
			break;
		case 'host':
			url[part] = value;
			if(/:\d+$/.test(value)){
				value = value.split(':');
				url.port = value.pop();
				url.hostname = value.join(':');
			}else{
				url.hostname = value;
				url.port = '';
			};
			break;
		case 'protocol':
			url.protocol = value.toLowerCase();
			if(url.protocol && _ends_with(url.protocol, ':')){
				url.protocol = url.protocol.slice(0, -1);
			};
			url.slashes = (value !== "");
			break;
		case 'pathname':
		case 'hash':
			if(value){
				if(part === 'pathname'){
					url[part] = _starts_with(value, '/') ? value : '/' + value;
				}else{
					url[part] = _starts_with(value, '#') ? value.slice(1) : value;
				};
			}else{
				url[part] = value;
			};
			break;
		default:
			url[part] = value;
	};
	
	for(var i = 0; i < rules.length; i++){
		var ins = rules[i];
		if(ins[4]){url[ins[1]] = url[ins[1]].toLowerCase()};
	};
	
	url.origin = url.protocol && url.host && url.protocol !== 'file' ? url.protocol + '://' + url.host : 'null';
	
	url.href = url.toString();
	
	return url;
};
_url.prototype.toString = function(stringify){
	if(_is_nilb(stringify) || 'function' !== typeof stringify){stringify = _query_string.encode};
	
	var url = this;
	var query = '';
	var protocol = url.protocol;
	if(protocol && !_ends_with(protocol, ':')){protocol += ':'};
	
	var result = protocol + (url.slashes ? '//' : '');
	
	if(url.username){
		result += url.username;
		if(url.password){result += ':' + url.password};
		result += '@';
	};
	
	result += url.host + url.pathname;
	
	if(url.pathname==='' && ((url.query && !(JSON.stringify(url.query)==="{\"\":\"\"}")) || url.hash)){
		result += '/';
	};
	
	if(url.query && !(JSON.stringify(url.query)==="{\"\":\"\"}")){
		query = ('object' === typeof url.query) ? stringify(url.query) : url.query;
		if(query){result += _starts_with(query, '?') ? query : ('?' + query)};
	};
	
	if(url.hash){result += (_starts_with(url.hash, '#') ? '' : '#') + url.hash};
	
	return result;
};
function _generate_url(url_obj){
	_validate_argument_type(url_obj, 'object', 'URL object', '_generate_url');
	
	if(url_obj instanceof _url){
		return url_obj.toString();
	};
	
    var new_url = new _url("", {normalize: false, rfail: false});
    for(key in url_obj){
        new_url.set(key, url_obj[key]);
    };
	
    return new_url.toString();
};
function _change_url(url, params, options){
	_validate_argument_type(url, 'string', 'URL', '_change_url');
	_validate_argument_type(params, 'object', 'New URL items', '_change_url');
	options = _avoid_nilb(options, {});
	
    var url_obj = new _url(url, options);
    for(key in params){
		var value = params[key]==='-BAS-CLEAR-' ? '' : params[key];
		if(key==='query' && value !== ''){
			var query = 'string'===typeof value ? _query_string.decode(value) : value;
			value = options.clear_query ? {} : url_obj.query;
			for(query_key in query){
				var query_value = query[query_key];
				if(query_value==='-BAS-CLEAR-'){
					if(value[query_key]){
						delete value[query_key];
					};
				}else{
					value[query_key] = query_value;
				};
			};
		};
		url_obj.set(key, value);
    };
	
    return url_obj.toString();
};
function _ua(uastring, extensions){
	_validate_argument_type(uastring, 'string', 'User-Agent', '_ua');
	
	if(!(this instanceof _ua)){
        return new _ua(uastring, extensions);
    };

    if(typeof uastring === 'object'){
        extensions = uastring;
        uastring = undefined;
    };
	
	var EMPTY = '';
	var UNKNOWN = '?';
	var FUNC_TYPE = 'function';
	var UNDEF_TYPE = 'undefined';
	var OBJ_TYPE = 'object';
	var STR_TYPE = 'string';
	var MAJOR = 'major';
	var MODEL = 'model';
	var NAME = 'name';
	var TYPE = 'type';
	var VENDOR = 'vendor';
	var VERSION = 'version';
	var ARCHITECTURE = 'architecture';
	var CONSOLE = 'console';
	var MOBILE = 'mobile';
	var TABLET = 'tablet';
	var SMARTTV = 'smarttv';
	var WEARABLE = 'wearable';
	var EMBEDDED = 'embedded';

	var util = {
		extend: function (regexes, extensions) {
			var mergedRegexes = {};
			for (var i in regexes) {
				if (extensions[i] && extensions[i].length % 2 === 0) {
					mergedRegexes[i] = extensions[i].concat(regexes[i]);
				} else {
					mergedRegexes[i] = regexes[i];
				}
			}
			return mergedRegexes;
		},
		has: function (str1, str2) {
			if (typeof str1 === "string") {
				return str2.toLowerCase().indexOf(str1.toLowerCase()) !== -1;
			} else {
				return false;
			}
		},
		lowerize: function (str) {
			return str.toLowerCase();
		},
		major: function (version) {
			return typeof(version) === STR_TYPE ? parseFloat(version.replace(/[^\d\.]/g, '').split(".")[0]) : undefined;
		},
		trim: function (str) {
			return str.replace(/^[\s\uFEFF\xA0]+|[\s\uFEFF\xA0]+$/g, '');
		}
	};
	
	var mapper = {
		rgx: function(ua, arrays){

			var i = 0;
			var j = undefined;
			var k = undefined;
			var p = undefined;
			var q = undefined;
			var j = undefined;
			var matches = undefined;
			var match = undefined;
			
			while (i < arrays.length && !matches) {

				var regex = arrays[i];
				var props = arrays[i + 1];
				j = 0;
				k = 0;
				
				while (j < regex.length && !matches) {

					matches = regex[j++].exec(ua);

					if (!!matches) {
						for (p = 0; p < props.length; p++) {
							match = matches[++k];
							q = props[p];
							if (typeof q === OBJ_TYPE && q.length > 0) {
								if (q.length == 2) {
									if (typeof q[1] == FUNC_TYPE) {
										this[q[0]] = q[1].call(this, match);
									} else {
										this[q[0]] = q[1];
									}
								} else if (q.length == 3) {
									if (typeof q[1] === FUNC_TYPE && !(q[1].exec && q[1].test)) {
										this[q[0]] = match ? q[1].call(this, match, q[2]) : undefined;
									} else {
										this[q[0]] = match ? match.replace(q[1], q[2]) : undefined;
									}
								} else if (q.length == 4) {
									this[q[0]] = match ? q[3].call(this, match.replace(q[1], q[2])) : undefined;
								}
							} else {
								this[q] = match ? match : undefined;
							}
						}
					}
				}
				i += 2;
			}
		},
		str: function (str, map) {

			for (var i in map) {
				if (typeof map[i] === OBJ_TYPE && map[i].length > 0) {
					for (var j = 0; j < map[i].length; j++) {
						if (util.has(map[i][j], str)) {
							return (i === UNKNOWN) ? undefined : i;
						}
					}
				} else if (util.has(map[i], str)) {
					return (i === UNKNOWN) ? undefined : i;
				}
			}
			return str;
		}
	};
	
	var maps = {

        browser : {
            oldsafari : {
                version : {
                    '1.0'   : '/8',
                    '1.2'   : '/1',
                    '1.3'   : '/3',
                    '2.0'   : '/412',
                    '2.0.2' : '/416',
                    '2.0.3' : '/417',
                    '2.0.4' : '/419',
                    '?'     : '/'
                }
            }
        },

        device : {
            amazon : {
                model : {
                    'Fire Phone' : ['SD', 'KF']
                }
            },
            sprint : {
                model : {
                    'Evo Shift 4G' : '7373KT'
                },
                vendor : {
                    'HTC'       : 'APA',
                    'Sprint'    : 'Sprint'
                }
            }
        },

        os : {
            windows : {
                version : {
                    'ME'        : '4.90',
                    'NT 3.11'   : 'NT3.51',
                    'NT 4.0'    : 'NT4.0',
                    '2000'      : 'NT 5.0',
                    'XP'        : ['NT 5.1', 'NT 5.2'],
                    'Vista'     : 'NT 6.0',
                    '7'         : 'NT 6.1',
                    '8'         : 'NT 6.2',
                    '8.1'       : 'NT 6.3',
                    '10'        : ['NT 6.4', 'NT 10.0'],
                    'RT'        : 'ARM'
                }
            }
        }
    };
	
	var regexes = {

        browser : [[

            // Presto based
            /(opera\smini)\/([\w\.-]+)/i,                                       // Opera Mini
            /(opera\s[mobiletab]{3,6}).+version\/([\w\.-]+)/i,                  // Opera Mobi/Tablet
            /(opera).+version\/([\w\.]+)/i,                                     // Opera > 9.80
            /(opera)[\/\s]+([\w\.]+)/i                                          // Opera < 9.80
            ], [NAME, VERSION], [

            /(opios)[\/\s]+([\w\.]+)/i                                          // Opera mini on iphone >= 8.0
            ], [[NAME, 'Opera Mini'], VERSION], [

            /\s(opr)\/([\w\.]+)/i                                               // Opera Webkit
            ], [[NAME, 'Opera'], VERSION], [

            // Mixed
            /(kindle)\/([\w\.]+)/i,                                             // Kindle
            /(lunascape|maxthon|netfront|jasmine|blazer|instagram)[\/\s]?([\w\.]*)/i,
                                                                                // Lunascape/Maxthon/Netfront/Jasmine/Blazer/Instagram
            // Trident based
            /(avant\s|iemobile|slim)(?:browser)?[\/\s]?([\w\.]*)/i,
                                                                                // Avant/IEMobile/SlimBrowser
            /(bidubrowser|baidubrowser)[\/\s]?([\w\.]+)/i,                      // Baidu Browser
            /(?:ms|\()(ie)\s([\w\.]+)/i,                                        // Internet Explorer

            // Webkit/KHTML based
            /(rekonq)\/([\w\.]*)/i,                                             // Rekonq
            /(chromium|flock|rockmelt|midori|epiphany|silk|skyfire|ovibrowser|bolt|iron|vivaldi|iridium|phantomjs|bowser|quark|qupzilla|falkon)\/([\w\.-]+)/i,
                                                                                // Chromium/Flock/RockMelt/Midori/Epiphany/Silk/Skyfire/Bolt/Iron/Iridium/PhantomJS/Bowser/QupZilla/Falkon
            /(puffin|brave|whale|qqbrowserlite|qq)\/([\w\.]+)/i                 // Puffin/Brave/Whale/QQBrowserLite/QQ, aka ShouQ
            ], [NAME, VERSION], [

            /(konqueror)\/([\w\.]+)/i                                           // Konqueror
            ], [[NAME, 'Konqueror'], VERSION], [

            /(trident).+rv[:\s]([\w\.]{1,9}).+like\sgecko/i                     // IE11
            ], [[NAME, 'IE'], VERSION], [

            /(edge|edgios|edga|edg)\/((\d+)?[\w\.]+)/i                          // Microsoft Edge
            ], [[NAME, 'Edge'], VERSION], [

            /(yabrowser)\/([\w\.]+)/i                                           // Yandex
            ], [[NAME, 'Yandex'], VERSION], [

            /(Avast)\/([\w\.]+)/i                                               // Avast Secure Browser
            ], [[NAME, 'Avast Secure Browser'], VERSION], [

            /(AVG)\/([\w\.]+)/i                                                 // AVG Secure Browser
            ], [[NAME, 'AVG Secure Browser'], VERSION], [

            /(focus)\/([\w\.]+)/i                                               // Firefox Focus
            ], [[NAME, 'Firefox Focus'], VERSION], [

            /(opt)\/([\w\.]+)/i                                                 // Opera Touch
            ], [[NAME, 'Opera Touch'], VERSION], [

            /((?:[\s\/])uc?\s?browser|(?:juc.+)ucweb)[\/\s]?([\w\.]+)/i         // UCBrowser
            ], [[NAME, 'UCBrowser'], VERSION], [

            /(comodo_dragon)\/([\w\.]+)/i                                       // Comodo Dragon
            ], [[NAME, /_/g, ' '], VERSION], [

            /((?:windowswechat)? qbcore)\/([\w\.]+).*(?:windowswechat)?/i       // WeChat Desktop for Windows Built-in Browser
            ], [[NAME, 'WeChat(Win) Desktop'], VERSION], [

            /(micromessenger)\/([\w\.]+)/i                                      // WeChat
            ], [[NAME, 'WeChat'], VERSION], [

            /m?(qqbrowser|baiduboxapp|2345Explorer)[\/\s]?([\w\.]+)/i           // QQBrowser/Baidu App/2345 Browser
            ], [NAME, VERSION], [

            /(MetaSr)[\/\s]?([\w\.]+)/i                                         // SouGouBrowser
            ], [NAME], [

            /(LBBROWSER)/i                                                      // LieBao Browser
            ], [NAME], [

            /(weibo)__([\d\.]+)/i                                               // Weibo
            ], [NAME, VERSION], [

            /xiaomi\/miuibrowser\/([\w\.]+)/i                                   // MIUI Browser
            ], [VERSION, [NAME, 'MIUI Browser']], [

            /;fbav\/([\w\.]+);/i                                                // Facebook App for iOS & Android with version
            ], [VERSION, [NAME, 'Facebook']], [
            
            /FBAN\/FBIOS|FB_IAB\/FB4A/i                                         // Facebook App for iOS & Android without version
            ], [[NAME, 'Facebook']], [

            /safari\s(line)\/([\w\.]+)/i,                                       // Line App for iOS
            /droid.+(line)\/([\w\.]+)\/iab/i                                    // Line App for Android
            ], [NAME, VERSION], [

            /headlesschrome(?:\/([\w\.]+)|\s)/i                                 // Chrome Headless
            ], [VERSION, [NAME, 'Chrome Headless']], [

            /\swv\).+(chrome)\/([\w\.]+)/i                                      // Chrome WebView
            ], [[NAME, /(.+)/, '$1 WebView'], VERSION], [

            /((?:oculus|samsung)browser)\/([\w\.]+)/i
            ], [[NAME, /(.+(?:g|us))(.+)/, '$1 $2'], VERSION], [                // Oculus / Samsung Browser

            /droid.+version\/([\w\.]+)\s+(?:mobile\s?safari|safari)*/i          // Android Browser
            ], [VERSION, [NAME, 'Android Browser']], [

            /(coc_coc_browser)\/([\w\.]+)/i                                     // Coc Coc Browser
            ], [[NAME, 'Coc Coc'], VERSION], [

            /(sailfishbrowser)\/([\w\.]+)/i                                     // Sailfish Browser
            ], [[NAME, 'Sailfish Browser'], VERSION], [

            /(chrome|omniweb|arora|[tizenoka]{5}\s?browser)\/v?([\w\.]+)/i
                                                                                // Chrome/OmniWeb/Arora/Tizen/Nokia
            ], [NAME, VERSION], [

            /(dolfin)\/([\w\.]+)/i                                              // Dolphin
            ], [[NAME, 'Dolphin'], VERSION], [

            /(qihu|qhbrowser|qihoobrowser|360browser)/i                         // 360
            ], [[NAME, '360 Browser']], [

            /((?:android.+)crmo|crios)\/([\w\.]+)/i                             // Chrome for Android/iOS
            ], [[NAME, 'Chrome'], VERSION], [

            /(coast)\/([\w\.]+)/i                                               // Opera Coast
            ], [[NAME, 'Opera Coast'], VERSION], [

            /fxios\/([\w\.-]+)/i                                                // Firefox for iOS
            ], [VERSION, [NAME, 'Firefox']], [

            /version\/([\w\.]+)\s.*mobile\/\w+\s(safari)/i                      // Mobile Safari
            ], [VERSION, [NAME, 'Mobile Safari']], [

            /version\/([\w\.]+)\s.*(mobile\s?safari|safari)/i                   // Safari & Safari Mobile
            ], [VERSION, NAME], [

            /webkit.+?(gsa)\/([\w\.]+)\s.*(mobile\s?safari|safari)(\/[\w\.]+)/i // Google Search Appliance on iOS
            ], [[NAME, 'GSA'], VERSION], [

            /webkit.+?(mobile\s?safari|safari)(\/[\w\.]+)/i                     // Safari < 3.0
            ], [NAME, [VERSION, mapper.str, maps.browser.oldsafari.version]], [

            /(webkit|khtml)\/([\w\.]+)/i
            ], [NAME, VERSION], [

            // Gecko based
            /(navigator|netscape)\/([\w\.-]+)/i                                 // Netscape
            ], [[NAME, 'Netscape'], VERSION], [
            /(swiftfox)/i,                                                      // Swiftfox
            /(icedragon|iceweasel|camino|chimera|fennec|maemo\sbrowser|minimo|conkeror)[\/\s]?([\w\.\+]+)/i,
                                                                                // IceDragon/Iceweasel/Camino/Chimera/Fennec/Maemo/Minimo/Conkeror
            /(firefox|seamonkey|k-meleon|icecat|iceape|firebird|phoenix|palemoon|basilisk|waterfox)\/([\w\.-]+)$/i,

                                                                                // Firefox/SeaMonkey/K-Meleon/IceCat/IceApe/Firebird/Phoenix
            /(firefox)\/([\w\.]+)\s[\w\s\-]+\/[\w\.]+$/i,                       // Other Firefox-based
            /(mozilla)\/([\w\.]+)\s.+rv\:.+gecko\/\d+/i,                        // Mozilla

            // Other
            /(polaris|lynx|dillo|icab|doris|amaya|w3m|netsurf|sleipnir)[\/\s]?([\w\.]+)/i,
                                                                                // Polaris/Lynx/Dillo/iCab/Doris/Amaya/w3m/NetSurf/Sleipnir
            /(links)\s\(([\w\.]+)/i,                                            // Links
            /(gobrowser)\/?([\w\.]*)/i,                                         // GoBrowser
            /(ice\s?browser)\/v?([\w\._]+)/i,                                   // ICE Browser
            /(mosaic)[\/\s]([\w\.]+)/i                                          // Mosaic
            ], [NAME, VERSION]
        ],

        cpu : [[

            /(?:(amd|x(?:(?:86|64)[_-])?|wow|win)64)[;\)]/i                     // AMD64
            ], [[ARCHITECTURE, 'amd64']], [

            /(ia32(?=;))/i                                                      // IA32 (quicktime)
            ], [[ARCHITECTURE, util.lowerize]], [

            /((?:i[346]|x)86)[;\)]/i                                            // IA32
            ], [[ARCHITECTURE, 'ia32']], [

            // PocketPC mistakenly identified as PowerPC
            /windows\s(ce|mobile);\sppc;/i
            ], [[ARCHITECTURE, 'arm']], [

            /((?:ppc|powerpc)(?:64)?)(?:\smac|;|\))/i                           // PowerPC
            ], [[ARCHITECTURE, /ower/, '', util.lowerize]], [

            /(sun4\w)[;\)]/i                                                    // SPARC
            ], [[ARCHITECTURE, 'sparc']], [

            /((?:avr32|ia64(?=;))|68k(?=\))|arm(?:64|(?=v\d+[;l]))|(?=atmel\s)avr|(?:irix|mips|sparc)(?:64)?(?=;)|pa-risc)/i
                                                                                // IA64, 68K, ARM/64, AVR/32, IRIX/64, MIPS/64, SPARC/64, PA-RISC
            ], [[ARCHITECTURE, util.lowerize]]
        ],

        device : [[

            /\((ipad|playbook);[\w\s\),;-]+(rim|apple)/i                        // iPad/PlayBook
            ], [MODEL, VENDOR, [TYPE, TABLET]], [

            /applecoremedia\/[\w\.]+ \((ipad)/                                  // iPad
            ], [MODEL, [VENDOR, 'Apple'], [TYPE, TABLET]], [

            /(apple\s{0,1}tv)/i                                                 // Apple TV
            ], [[MODEL, 'Apple TV'], [VENDOR, 'Apple'], [TYPE, SMARTTV]], [

            /(archos)\s(gamepad2?)/i,                                           // Archos
            /(hp).+(touchpad)/i,                                                // HP TouchPad
            /(hp).+(tablet)/i,                                                  // HP Tablet
            /(kindle)\/([\w\.]+)/i,                                             // Kindle
            /\s(nook)[\w\s]+build\/(\w+)/i,                                     // Nook
            /(dell)\s(strea[kpr\s\d]*[\dko])/i                                  // Dell Streak
            ], [VENDOR, MODEL, [TYPE, TABLET]], [

            /(alexa)webm/i,
            new RegExp("(kf[A-z]+)(\\sbuild\\/|\\)).+silk\\/", "i")             // Kindle Fire HD
            ], [MODEL, [VENDOR, 'Amazon'], [TYPE, TABLET]], [
			                                                                    // Fire Phone
            new RegExp("(sd|kf)[0349hijorstuw]+(\\sbuild\\/|\\)).+silk\\/", "i")
			], [[MODEL, mapper.str, maps.device.amazon.model], [VENDOR, 'Amazon'], [TYPE, MOBILE]], [
            /droid.+aft([\w])(\sbuild\/|\))/i                                   // Fire TV
            ], [MODEL, [VENDOR, 'Amazon'], [TYPE, SMARTTV]], [

            /\((ip(?:hone|od)[\s\w]*);/i                                        // iPod/iPhone
            ], [MODEL, [VENDOR, 'Apple'], [TYPE, MOBILE]], [

            /(blackberry)[\s-]?(\w+)/i,                                         // BlackBerry
            /(blackberry|benq|palm(?=\-)|sonyericsson|acer|asus|dell|meizu|motorola|polytron)[\s_-]?([\w-]*)/i,
                                                                                // BenQ/Palm/Sony-Ericsson/Acer/Asus/Dell/Meizu/Motorola/Polytron
            /(hp)\s([\w\s]+\w)/i,                                               // HP iPAQ
            /(asus)-?(\w+)/i                                                    // Asus
            ], [VENDOR, MODEL, [TYPE, MOBILE]], [
            /\(bb10;\s(\w+)/i                                                   // BlackBerry 10
            ], [MODEL, [VENDOR, 'BlackBerry'], [TYPE, MOBILE]], [
                                                                                // Asus Tablets
            /droid.+(transfo[prime\s]{4,10}\s\w+|eeepc|slider\s\w+|nexus 7|padfone|p00c)/i
            ], [MODEL, [VENDOR, 'Asus'], [TYPE, TABLET]], [

            new RegExp("sony\\stablet\\s[ps]\\sbuild\\/", "i"),                 // Sony
            /(?:sony)?sgp\w+(?:\sbuild\/|\))/i
            ], [[MODEL, 'Xperia Tablet'], [VENDOR, 'Sony'], [TYPE, TABLET]], [
            /droid.+\s([c-g]\d{4}|so[-l]\w+|xq-a\w[4-7][12])(?=\sbuild\/|\).+chrome\/(?![1-6]{0,1}\d\.))/i
            ], [MODEL, [VENDOR, 'Sony'], [TYPE, MOBILE]], [

            /\s(ouya)\s/i,                                                      // Ouya
            /(nintendo)\s([wids3utch]+)/i                                       // Nintendo
            ], [VENDOR, MODEL, [TYPE, CONSOLE]], [

            /droid.+;\s(shield)\sbuild/i                                        // Nvidia
            ], [MODEL, [VENDOR, 'Nvidia'], [TYPE, CONSOLE]], [

            /(playstation\s[345portablevi]+)/i                                  // Playstation
            ], [MODEL, [VENDOR, 'Sony'], [TYPE, CONSOLE]], [

            /(sprint\s(\w+))/i                                                  // Sprint Phones
            ], [[VENDOR, mapper.str, maps.device.sprint.vendor], [MODEL, mapper.str, maps.device.sprint.model], [TYPE, MOBILE]], [

            /(htc)[;_\s-]{1,2}([\w\s]+(?=\)|\sbuild)|\w+)/i,                    // HTC
            /(zte)-(\w*)/i,                                                     // ZTE
            /(alcatel|geeksphone|nexian|panasonic|(?=;\s)sony)[_\s-]?([\w-]*)/i
                                                                                // Alcatel/GeeksPhone/Nexian/Panasonic/Sony
            ], [VENDOR, [MODEL, /_/g, ' '], [TYPE, MOBILE]], [

            /(nexus\s9)/i                                                       // HTC Nexus 9
            ], [MODEL, [VENDOR, 'HTC'], [TYPE, TABLET]], [

            /d\/huawei([\w\s-]+)[;\)]/i,                                        // Huawei
            /droid.+\s(nexus\s6p|vog-[at]?l\d\d|ane-[at]?l[x\d]\d|eml-a?l\d\da?|lya-[at]?l\d[\dc]|clt-a?l\d\di?)/i,
            /droid.+\s((?:A(?:GS2?|KA|LP|N[AE]|QM|RE|SK|TH)|B(?:A(?:C|H2)|G2|KL|LA|MH|Z[AKT])|C(?:AZ|DY|LT|OL|[MOR]R)|DUK|E(?:BG|DI|L[ES]|ML|V[AR])|FRD|G(?:LK|RA)|H(?:D[LN]|MA|LK|RY|WI)|INE|J(?:DN2|MM|NY|SN)|K(?:NT|OB|SA)|L(?:IO|LD|ON|[RY]A)|M(?:AR|ED|[HL]A|ON|RX|T7)|N(?:EO|TS|XT)|OXF|P(?:AR|CT|IC|LK|RA)|R(?:IO|VL)|S(?:C[ML]|EA|HT|PN|TF)|T(?:A[HS]|NY)|V(?:[CI]E|KY|OG|RD)|W(?:AS|LZ)|Y(?:635|AL))-[ATU][LN][01259][019])[;\)\s]/i

            ], [MODEL, [VENDOR, 'Huawei'], [TYPE, MOBILE]], [

            /droid.+(bah2?-a?[lw]\d{2})/i                                       // Huawei MediaPad
            ], [MODEL, [VENDOR, 'Huawei'], [TYPE, TABLET]], [

            /(microsoft);\s(lumia[\s\w]+)/i                                     // Microsoft Lumia
            ], [VENDOR, MODEL, [TYPE, MOBILE]], [

            /[\s\(;](xbox(?:\sone)?)[\s\);]/i                                   // Microsoft Xbox
            ], [MODEL, [VENDOR, 'Microsoft'], [TYPE, CONSOLE]], [
            /(kin\.[onetw]{3})/i                                                // Microsoft Kin
            ], [[MODEL, /\./g, ' '], [VENDOR, 'Microsoft'], [TYPE, MOBILE]], [

                                                                                // Motorola
            new RegExp("\\s(milestone|droid(?:[2-4x]|\\s(?:bionic|x2|pro|razr))?:?(\\s4g)?)[\\w\\s]+build\\/", "i"),
            /\smot[\s-](\w*)/i,
            /(moto[\s\w\(\)]+(?=\sbuild|\)))/i,
            new RegExp("(XT\\d{3,4}) build\\/", "i"),
            /(nexus\s6)/i
            ], [MODEL, [VENDOR, 'Motorola'], [TYPE, MOBILE]], [
            new RegExp("droid.+\\s(mz60\\d|xoom[\\s2]{0,2})\\sbuild\\/", "i")
            ], [MODEL, [VENDOR, 'Motorola'], [TYPE, TABLET]], [

            /hbbtv\/\d+\.\d+\.\d+\s+\([\w\s]*;\s*(\w[^;]*);([^;]*)/i            // HbbTV devices
            ], [[VENDOR, util.trim], [MODEL, util.trim], [TYPE, SMARTTV]], [

            /hbbtv.+maple;(\d+)/i
            ], [[MODEL, /^/, 'SmartTV'], [VENDOR, 'Samsung'], [TYPE, SMARTTV]], [

            /\(dtv[\);].+(aquos)/i                                              // Sharp
            ], [MODEL, [VENDOR, 'Sharp'], [TYPE, SMARTTV]], [

            /droid.+((sch-i[89]0\d|shw-m380s|SM-P605|SM-P610|SM-P587|gt-p\d{4}|gt-n\d+|sgh-t8[56]9|nexus 10))/i,
            /((SM-T\w+))/i
            ], [[VENDOR, 'Samsung'], MODEL, [TYPE, TABLET]], [                  // Samsung
            /smart-tv.+(samsung)/i
            ], [VENDOR, [TYPE, SMARTTV], MODEL], [
            /((s[cgp]h-\w+|gt-\w+|galaxy\snexus|sm-\w[\w\d]+))/i,
            /\s(sam)(?:sung)[\s-]([\w-]+)/i,
            /sec-((sgh\w+))/i
            ], [[VENDOR, 'Samsung'], MODEL, [TYPE, MOBILE]], [

            /sie-(\w*)/i                                                        // Siemens
            ], [MODEL, [VENDOR, 'Siemens'], [TYPE, MOBILE]], [

            /(maemo|nokia).*(n900|lumia\s\d+)/i,                                // Nokia
            /(nokia)[\s_-]?([\w\.-]*)/i
            ], [[VENDOR, 'Nokia'], MODEL, [TYPE, MOBILE]], [

            /droid[x\d\.\s;]+\s([ab][1-7]\-?[0178a]\d\d?)/i                     // Acer
            ], [MODEL, [VENDOR, 'Acer'], [TYPE, TABLET]], [

            /droid.+([vl]k\-?\d{3})\s+build/i                                   // LG Tablet
            ], [MODEL, [VENDOR, 'LG'], [TYPE, TABLET]], [
            /droid\s3\.[\s\w;-]{10}(lg?)-([06cv9]{3,4})/i                       // LG Tablet
            ], [[VENDOR, 'LG'], MODEL, [TYPE, TABLET]], [
            /linux;\snetcast.+smarttv/i,                                        // LG SmartTV
            /lg\snetcast\.tv-201\d/i
            ], [[VENDOR, 'LG'], MODEL, [TYPE, SMARTTV]], [
            /(nexus\s[45])/i,                                                   // LG
            /lg[e;\s\/-]+(\w*)/i,
            /droid.+lg(\-?[\d\w]+)\s+build/i
            ], [MODEL, [VENDOR, 'LG'], [TYPE, MOBILE]], [

            /(lenovo)\s?(s(?:5000|6000)(?:[\w-]+)|tab(?:[\s\w]+)|[\w-]+)/i      // Lenovo tablets
            ], [VENDOR, MODEL, [TYPE, TABLET]], [
            /droid.+(ideatab[a-z0-9\-\s]+)/i                                    // Lenovo
            ], [MODEL, [VENDOR, 'Lenovo'], [TYPE, TABLET]], [
            /(lenovo)[_\s-]?([\w-]+)/i
            ], [VENDOR, MODEL, [TYPE, MOBILE]], [

            /linux;.+((jolla));/i                                               // Jolla
            ], [VENDOR, MODEL, [TYPE, MOBILE]], [

            /((pebble))app\/[\d\.]+\s/i                                         // Pebble
            ], [VENDOR, MODEL, [TYPE, WEARABLE]], [

            /droid.+;\s(oppo)\s?([\w\s]+)\sbuild/i                              // OPPO
            ], [VENDOR, MODEL, [TYPE, MOBILE]], [

            /crkey/i                                                            // Google Chromecast
            ], [[MODEL, 'Chromecast'], [VENDOR, 'Google'], [TYPE, SMARTTV]], [

            /droid.+;\s(glass)\s\d/i                                            // Google Glass
            ], [MODEL, [VENDOR, 'Google'], [TYPE, WEARABLE]], [

            /droid.+;\s(pixel c)[\s)]/i                                         // Google Pixel C
            ], [MODEL, [VENDOR, 'Google'], [TYPE, TABLET]], [

            /droid.+;\s(pixel( [2-9]a?)?( xl)?)[\s)]/i                          // Google Pixel
            ], [MODEL, [VENDOR, 'Google'], [TYPE, MOBILE]], [

            /droid.+;\s(\w+)\s+build\/hm\1/i,                                   // Xiaomi Hongmi 'numeric' models
            /droid.+(hm[\s\-_]?note?[\s_]?(?:\d\w)?)\sbuild/i,                  // Xiaomi Hongmi
            /droid.+(redmi[\s\-_]?(?:note|k)?(?:[\s_]?[\w\s]+))(?:\sbuild|\))/i,      
                                                                                // Xiaomi Redmi
            /droid.+(mi[\s\-_]?(?:a\d|one|one[\s_]plus|note lte)?[\s_]?(?:\d?\w?)[\s_]?(?:plus)?)\sbuild/i
                                                                                // Xiaomi Mi
            ], [[MODEL, /_/g, ' '], [VENDOR, 'Xiaomi'], [TYPE, MOBILE]], [
            /droid.+(mi[\s\-_]?(?:pad)(?:[\s_]?[\w\s]+))(?:\sbuild|\))/i        // Mi Pad tablets
            ],[[MODEL, /_/g, ' '], [VENDOR, 'Xiaomi'], [TYPE, TABLET]], [
            /droid.+;\s(m[1-5]\snote)\sbuild/i                                  // Meizu
            ], [MODEL, [VENDOR, 'Meizu'], [TYPE, MOBILE]], [
            /(mz)-([\w-]{2,})/i
            ], [[VENDOR, 'Meizu'], MODEL, [TYPE, MOBILE]], [

            /droid.+a000(1)\s+build/i,                                          // OnePlus
            /droid.+oneplus\s(a\d{4})[\s)]/i
            ], [MODEL, [VENDOR, 'OnePlus'], [TYPE, MOBILE]], [

            /droid.+[;\/]\s*(RCT[\d\w]+)\s+build/i                              // RCA Tablets
            ], [MODEL, [VENDOR, 'RCA'], [TYPE, TABLET]], [

            /droid.+[;\/\s](Venue[\d\s]{2,7})\s+build/i                         // Dell Venue Tablets
            ], [MODEL, [VENDOR, 'Dell'], [TYPE, TABLET]], [

            /droid.+[;\/]\s*(Q[T|M][\d\w]+)\s+build/i                           // Verizon Tablet
            ], [MODEL, [VENDOR, 'Verizon'], [TYPE, TABLET]], [

            /droid.+[;\/]\s+(Barnes[&\s]+Noble\s+|BN[RT])(\S(?:.*\S)?)\s+build/i
            ], [[VENDOR, 'Barnes & Noble'], MODEL, [TYPE, TABLET]], [

            /droid.+[;\/]\s+(TM\d{3}.*\b)\s+build/i                             // Barnes & Noble Tablet
            ], [MODEL, [VENDOR, 'NuVision'], [TYPE, TABLET]], [

            /droid.+;\s(k88)\sbuild/i                                           // ZTE K Series Tablet
            ], [MODEL, [VENDOR, 'ZTE'], [TYPE, TABLET]], [

            /droid.+;\s(nx\d{3}j)\sbuild/i                                      // ZTE Nubia
            ], [MODEL, [VENDOR, 'ZTE'], [TYPE, MOBILE]], [

            /droid.+[;\/]\s*(gen\d{3})\s+build.*49h/i                           // Swiss GEN Mobile
            ], [MODEL, [VENDOR, 'Swiss'], [TYPE, MOBILE]], [

            /droid.+[;\/]\s*(zur\d{3})\s+build/i                                // Swiss ZUR Tablet
            ], [MODEL, [VENDOR, 'Swiss'], [TYPE, TABLET]], [

            /droid.+[;\/]\s*((Zeki)?TB.*\b)\s+build/i                           // Zeki Tablets
            ], [MODEL, [VENDOR, 'Zeki'], [TYPE, TABLET]], [

            /(android).+[;\/]\s+([YR]\d{2})\s+build/i,
            /droid.+[;\/]\s+(Dragon[\-\s]+Touch\s+|DT)(\w{5})\sbuild/i          // Dragon Touch Tablet
            ], [[VENDOR, 'Dragon Touch'], MODEL, [TYPE, TABLET]], [

            /droid.+[;\/]\s*(NS-?\w{0,9})\sbuild/i                              // Insignia Tablets
            ], [MODEL, [VENDOR, 'Insignia'], [TYPE, TABLET]], [

            /droid.+[;\/]\s*((NXA|Next)-?\w{0,9})\s+build/i                     // NextBook Tablets
            ], [MODEL, [VENDOR, 'NextBook'], [TYPE, TABLET]], [

            /droid.+[;\/]\s*(Xtreme\_)?(V(1[045]|2[015]|30|40|60|7[05]|90))\s+build/i
            ], [[VENDOR, 'Voice'], MODEL, [TYPE, MOBILE]], [                    // Voice Xtreme Phones

            /droid.+[;\/]\s*(LVTEL\-)?(V1[12])\s+build/i                        // LvTel Phones
            ], [[VENDOR, 'LvTel'], MODEL, [TYPE, MOBILE]], [

            /droid.+;\s(PH-1)\s/i
            ], [MODEL, [VENDOR, 'Essential'], [TYPE, MOBILE]], [                // Essential PH-1

            /droid.+[;\/]\s*(V(100MD|700NA|7011|917G).*\b)\s+build/i            // Envizen Tablets
            ], [MODEL, [VENDOR, 'Envizen'], [TYPE, TABLET]], [

            /droid.+[;\/]\s*(Le[\s\-]+Pan)[\s\-]+(\w{1,9})\s+build/i            // Le Pan Tablets
            ], [VENDOR, MODEL, [TYPE, TABLET]], [

            /droid.+[;\/]\s*(Trio[\s\w\-\.]+)\s+build/i                         // MachSpeed Tablets
            ], [MODEL, [VENDOR, 'MachSpeed'], [TYPE, TABLET]], [

            /droid.+[;\/]\s*(Trinity)[\-\s]*(T\d{3})\s+build/i                  // Trinity Tablets
            ], [VENDOR, MODEL, [TYPE, TABLET]], [

            /droid.+[;\/]\s*TU_(1491)\s+build/i                                 // Rotor Tablets
            ], [MODEL, [VENDOR, 'Rotor'], [TYPE, TABLET]], [

            //android.+(KS(.+))\s+build/i                                        // Amazon Kindle Tablets
            //], [MODEL, [VENDOR, 'Amazon'], [TYPE, TABLET]], [

            /droid.+(Gigaset)[\s\-]+(Q\w{1,9})\s+build/i                        // Gigaset Tablets
            ], [VENDOR, MODEL, [TYPE, TABLET]], [

            /[\s\/\(](android\stv|smart-?tv)[;\)\s]/i                           // SmartTV from Unidentified Vendors
            ], [[TYPE, SMARTTV]], [
                                                                                // Android Phones from Unidentified Vendors
            /droid .+?; ([^;]+?)(?: build|\) applewebkit).+? mobile safari/i
            ], [MODEL, [TYPE, MOBILE]], [
                                                                                // Android Tablets from Unidentified Vendors
            /droid .+?;\s([^;]+?)(?: build|\) applewebkit).+?(?! mobile) safari/i
            ], [MODEL, [TYPE, TABLET]], [

            /\s(tablet|tab)[;\/]/i,                                             // Unidentifiable Tablet
            /\s(mobile)(?:[;\/]|\ssafari)/i                                     // Unidentifiable Mobile
            ], [[TYPE, util.lowerize], VENDOR, MODEL], [

            /(android[\w\.\s\-]{0,9});.+build/i                                 // Generic Android Device
            ], [MODEL, [VENDOR, 'Generic']], [

            /(phone)/i
            ], [[TYPE, MOBILE]]
        ],

        engine : [[

            /windows.+\sedge\/([\w\.]+)/i                                       // EdgeHTML
            ], [VERSION, [NAME, 'EdgeHTML']], [

            /webkit\/537\.36.+chrome\/(?!27)([\w\.]+)/i                         // Blink
            ], [VERSION, [NAME, 'Blink']], [

            /(presto)\/([\w\.]+)/i,                                             // Presto
            /(webkit|trident|netfront|netsurf|amaya|lynx|w3m|goanna)\/([\w\.]+)/i,
                                                                                // WebKit/Trident/NetFront/NetSurf/Amaya/Lynx/w3m/Goanna
            /(khtml|tasman|links)[\/\s]\(?([\w\.]+)/i,                          // KHTML/Tasman/Links
            /(icab)[\/\s]([23]\.[\d\.]+)/i                                      // iCab
            ], [NAME, VERSION], [

            /rv\:([\w\.]{1,9}).+(gecko)/i                                       // Gecko
            ], [VERSION, NAME]
        ],

        os : [[

            // Xbox, consider this before other Windows-based devices
            /(xbox);\s+xbox\s([^\);]+)/i,                                       // Microsoft Xbox (360, One, X, S, Series X, Series S)

            // Windows based
            /microsoft\s(windows)\s(vista|xp)/i                                 // Windows (iTunes)
            ], [NAME, VERSION], [
            /(windows)\snt\s6\.2;\s(arm)/i,                                     // Windows RT
            /(windows\sphone(?:\sos)*)[\s\/]?([\d\.\s\w]*)/i,                   // Windows Phone
            /(windows\smobile|windows)[\s\/]?([ntce\d\.\s]+\w)/i
            ], [NAME, [VERSION, mapper.str, maps.os.windows.version]], [
            /(win(?=3|9|n)|win\s9x\s)([nt\d\.]+)/i
            ], [[NAME, 'Windows'], [VERSION, mapper.str, maps.os.windows.version]], [

            // Mobile/Embedded OS
            /\((bb)(10);/i                                                      // BlackBerry 10
            ], [[NAME, 'BlackBerry'], VERSION], [
            /(blackberry)\w*\/?([\w\.]*)/i,                                     // Blackberry
            /(tizen|kaios)[\/\s]([\w\.]+)/i,                                    // Tizen/KaiOS
            /(android|webos|palm\sos|qnx|bada|rim\stablet\sos|meego|sailfish|contiki)[\/\s-]?([\w\.]*)/i
                                                                                // Android/WebOS/Palm/QNX/Bada/RIM/MeeGo/Contiki/Sailfish OS
            ], [NAME, VERSION], [
            /(symbian\s?os|symbos|s60(?=;))[\/\s-]?([\w\.]*)/i                  // Symbian
            ], [[NAME, 'Symbian'], VERSION], [
            /\((series40);/i                                                    // Series 40
            ], [NAME], [
            /mozilla.+\(mobile;.+gecko.+firefox/i                               // Firefox OS
            ], [[NAME, 'Firefox OS'], VERSION], [

            // Google Chromecast
            /crkey\/([\d\.]+)/i                                                 // Google Chromecast
            ], [VERSION, [NAME, 'Chromecast']], [

            // Console
            /(nintendo|playstation)\s([wids345portablevuch]+)/i,                // Nintendo/Playstation

            // GNU/Linux based
            /(mint)[\/\s\(]?(\w*)/i,                                            // Mint
            /(mageia|vectorlinux)[;\s]/i,                                       // Mageia/VectorLinux
            /(joli|[kxln]?ubuntu|debian|suse|opensuse|gentoo|(?=\s)arch|slackware|fedora|mandriva|centos|pclinuxos|redhat|zenwalk|linpus)[\/\s-]?(?!chrom)([\w\.-]*)/i,
                                                                                // Joli/Ubuntu/Debian/SUSE/Gentoo/Arch/Slackware
                                                                                // Fedora/Mandriva/CentOS/PCLinuxOS/RedHat/Zenwalk/Linpus
            /(hurd|linux)\s?([\w\.]*)/i,                                        // Hurd/Linux
            /(gnu)\s?([\w\.]*)/i                                                // GNU
            ], [NAME, VERSION], [

            /(cros)\s[\w]+\s([\w\.]+\w)/i                                       // Chromium OS
            ], [[NAME, 'Chromium OS'], VERSION],[

            // Solaris
            /(sunos)\s?([\w\.\d]*)/i                                            // Solaris
            ], [[NAME, 'Solaris'], VERSION], [

            // BSD based
            /\s([frentopc-]{0,4}bsd|dragonfly)\s?([\w\.]*)/i                    // FreeBSD/NetBSD/OpenBSD/PC-BSD/DragonFly
            ], [NAME, VERSION],[

            /(haiku)\s(\w+)/i                                                   // Haiku
            ], [NAME, VERSION],[

            /cfnetwork\/.+darwin/i,
            /ip[honead]{2,4}(?:.*os\s([\w]+)\slike\smac|;\sopera)/i             // iOS
            ], [[VERSION, /_/g, '.'], [NAME, 'iOS']], [

            /(mac\sos\sx)\s?([\w\s\.]*)/i,
            /(macintosh|mac(?=_powerpc)\s)/i                                    // Mac OS
            ], [[NAME, 'Mac OS'], [VERSION, /_/g, '.']], [

            // Other
            /((?:open)?solaris)[\/\s-]?([\w\.]*)/i,                             // Solaris
            /(aix)\s((\d)(?=\.|\)|\s)[\w\.])*/i,                                // AIX
            /(plan\s9|minix|beos|os\/2|amigaos|morphos|risc\sos|openvms|fuchsia)/i,
                                                                                // Plan9/Minix/BeOS/OS2/AmigaOS/MorphOS/RISCOS/OpenVMS/Fuchsia
            /(unix)\s?([\w\.]*)/i                                               // UNIX
            ], [NAME, VERSION]
        ]
    };

    var ua = uastring || EMPTY;
	ua = _trim(ua);
    var rgxmap = extensions ? util.extend(regexes, extensions) : regexes;
	
	var get_type = function(){
        if(!ua || ua === ''){
            return 'desktop';
        } else if (ua.match(/FlipboardProxy/i)) {
            return 'bot';
        } else if (ua.match(/Applebot/i)) {
            return 'bot';
        } else if (ua.match(/GoogleTV|SmartTV|SMART-TV|Internet TV|NetCast|NETTV|AppleTV|boxee|Kylo|Roku|DLNADOC|hbbtv|CrKey|CE\-HTML/i)) {
            return 'tv';
        } else if (ua.match(/Xbox|PLAYSTATION (3|4)|Wii/i)) {
            return 'tv';
        } else if (ua.match(/QtCarBrowser/i)) {
            return 'car';
        } else if (ua.match(/Googlebot|bingbot/i)) {
            return 'bot';
        } else if (ua.match(/WhatsApp/i)) {
            return 'bot';
        } else if (ua.match(/iP(a|ro)d/i) || (ua.match(/tablet/i) && !ua.match(/RX-34/i)) || ua.match(/FOLIO/i)) {
            return 'tablet';
        } else if (ua.match(/Linux/i) && ua.match(/Android/i) && !ua.match(/Fennec|mobi|HTC Magic|HTCX06HT|Nexus One|SC-02B|fone 945/i)) {
            return 'tablet';
        } else if (ua.match(/Kindle/i) || (ua.match(/Mac OS/i) && ua.match(/Silk/i)) || (ua.match(/AppleWebKit/i) && ua.match(/Silk/i) && !ua.match(/Playstation Vita/i))) {
            return 'tablet';
        } else if (ua.match(/GT-P10|SC-01C|SHW-M180S|SGH-T849|SCH-I800|SHW-M180L|SPH-P100|SGH-I987|zt180|HTC( Flyer|_Flyer)|Sprint ATP51|ViewPad7|pandigital(sprnova|nova)|Ideos S7|Dell Streak 7|Advent Vega|A101IT|A70BHT|MID7015|Next2|nook/i) || (ua.match(/MB511/i) && ua.match(/RUTEM/i))) {
            return 'tablet';
        } else if (ua.match(/BOLT|Fennec|Iris|Maemo|Minimo|Mobi|mowser|NetFront|Novarra|Prism|RX-34|Skyfire|Tear|XV6875|XV6975|Google Wireless Transcoder/i) && !ua.match(/AdsBot-Google-Mobile/i)) {
            return 'mobile';
        } else if (ua.match(/Opera/i) && ua.match(/Windows NT 5/i) && ua.match(/HTC|Xda|Mini|Vario|SAMSUNG\-GT\-i8000|SAMSUNG\-SGH\-i9/i)) {
            return 'mobile';
        } else if ((ua.match(/Windows( )?(NT|XP|ME|9)/) && !ua.match(/Phone/i)) && !ua.match(/Bot|Spider|ia_archiver|NewsGator/i) || ua.match(/Win( ?9|NT)/i) || ua.match(/Go-http-client/i)) {
            return 'desktop';
        } else if (ua.match(/Macintosh|PowerPC/i) && !ua.match(/Silk|moatbot/i)) {
            return 'desktop';
        } else if (ua.match(/Linux/i) && ua.match(/X11/i) && !ua.match(/Charlotte|JobBot/i)) {
            return 'desktop';
        } else if (ua.match(/CrOS/)) {
            return 'desktop';
        } else if (ua.match(/Solaris|SunOS|BSD/i)) {
            return 'desktop';
        } else if (ua.match(/Mozilla\/5\.0 \(\)|AdsBot-Google-Mobile/i) && ua.match(/Phone/i)) {
            return 'bot';
        } else if (ua.match(/Mozilla\/5\.0 \(\)|jack|Applebot|FlipboardProxy|Go 1.1 package|HTMLParser|simplereach|python-requests|ShowyouBot|MetaURI|nineconnections|(^Java\/[0-9._]*)|Commons-HttpClient|InAGist|HTTP-Java-Client|curl|Wget|Bot|B-O-T|Crawler|Spider|Spyder|Yahoo|ia_archiver|Covario-IDS|findlinks|DataparkSearch|larbin|Mediapartners-Google|NG-Search|Snappy|Teoma|Jeeves|Charlotte|NewsGator|TinEye|Cerberian|SearchSight|Zao|Scrubby|Qseero|PycURL|Pompos|oegp|SBIder|yoogliFetchAgent|yacy|webcollage|VYU2|voyager|updated|truwoGPS|StackRambler|Sqworm|silk|semanticdiscovery|ScoutJet|Nymesis|NetResearchServer|MVAClient|mogimogi|Mnogosearch|Arachmo|Accoona|holmes|htdig|ichiro|webis|LinkWalker|lwp-trivial|facebookexternalhit|monit\/|ELB-HealthChecker\/|JobBot|GoogleCloudMonitoring|GoogleStackdriverMonitoring|gomezagent|apm synthetic agent|ruxitsynthetic|ktxn|khte|ktht|dynatrace|TrendsmapResolver|WinHTTP/i) && !ua.match(/phone|Playstation/i)) {
            return 'bot';
        } else {
            return 'mobile';
        }
    };
	
	var ua_obj = this;
	ua_obj.ua = ua;
	ua_obj.platform = {
		type: get_type()
	};
	ua_obj.browser = {
		name: undefined,
		version: undefined
	};
	mapper.rgx.call(ua_obj.browser, ua, rgxmap.browser);
	ua_obj.browser.major = util.major(ua_obj.browser.version);
	ua_obj.engine = {
		name: undefined,
		version: undefined
	};
	mapper.rgx.call(ua_obj.engine, ua, rgxmap.engine);
	ua_obj.os = {
		name: undefined,
		version: undefined
	};
	mapper.rgx.call(ua_obj.os, ua, rgxmap.os);
	ua_obj.device = {
		vendor: undefined,
		model: undefined,
		type: undefined
	};
	mapper.rgx.call(ua_obj.device, ua, rgxmap.device);
	ua_obj.cpu = {
		architecture: undefined
	};
	mapper.rgx.call(ua_obj.cpu, ua, rgxmap.cpu);
};
_ua.prototype.set = function(uastring){
	var ua_obj = this;
	var new_ua_obj = new _ua(uastring);
	for(key in new_ua_obj){
		ua_obj[key] = new_ua_obj[key];
	};
	return ua_obj;
};
_ua.prototype.changeBrowserVersion = function(version){
	var ua_obj = this;
	ua_obj.set(ua_obj.ua.split(ua_obj.browser.version).join(version.toString()));
	return ua_obj;
};
_ua.prototype.toString = function(){
	return this.ua;
};
_query_string = (function(){
	var hexTable = new Array(256);
	for(var i = 0; i < 256; ++i){
		hexTable[i] = '%' + ((i < 16 ? '0' : '') + i.toString(16)).toUpperCase();
	};
	
	var noEscape = [
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0
	];

	var isHexTable = [
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	];
	
	function stringifyPrimitive(v){
		if(typeof v === 'string'){
			return v;
		};
		if(typeof v ==='number' && isFinite(v)){
			return '' + v;
		};
		if(typeof v === 'boolean'){
			return v ? 'true' : 'false';
		};
		return '';
	};


	function encodeStringified(v, encode){
		if(typeof v === 'string'){
			return (v.length ? encode(v) : '');
		};
		if(typeof v === 'number' && isFinite(v)){
			return (Math.abs(v) < 1e21 ? '' + v : encode('' + v));
		};
		if(typeof v === 'boolean'){
			return v ? 'true' : 'false';
		};
		return '';
	};


	function encodeStringifiedCustom(v, encode){
		return encode(stringifyPrimitive(v));
	};

	function charCodes(str){
		if(str.length === 0){
			return [];
		};
		if(str.length === 1){
			return [str.charCodeAt(0)];
		};
		var ret = new Array(str.length);
		for(var i = 0; i < str.length; ++i){
			ret[i] = str.charCodeAt(i);
		};
		return ret;
	};
	
	var defSepCodes = [38];
	var defEqCodes = [61];

	function addKeyVal(obj, key, value, keyEncoded, valEncoded, decode){
		if(key.length > 0 && keyEncoded){
			key = decodeStr(key, decode);
		};
		if(value.length > 0 && valEncoded){
			value = decodeStr(value, decode);
		};
		
		if(obj[key] === undefined){
			obj[key] = value;
		}else{
			var curValue = obj[key];
			if(curValue.pop){
				curValue[curValue.length] = value;
			}else{
				obj[key] = [curValue, value];
			};
		};
	};
	
	function encodeStr(str, noEscapeTable, hexTable){
		var len = str.length;
		if(len === 0){
			return '';
		};

		var out = '';
		var lastPos = 0;
		var i = 0;

		outer:
		for(; i < len; i++){
			var c = str.charCodeAt(i);
			
			while(c < 0x80){
				if(noEscapeTable[c] !== 1){
					if(lastPos < i){
						out += str.slice(lastPos, i);
					};
					lastPos = i + 1;
					out += hexTable[c];
				};

				if(++i === len){
					break outer;
				};

				c = str.charCodeAt(i);
			};

			if(lastPos < i){
				out += str.slice(lastPos, i);
			};
			
			if(c < 0x800){
				lastPos = i + 1;
				out += hexTable[0xC0 | (c >> 6)] + hexTable[0x80 | (c & 0x3F)];
				continue;
			};
			if (c < 0xD800 || c >= 0xE000) {
				lastPos = i + 1;
				out += hexTable[0xE0 | (c >> 12)] + hexTable[0x80 | ((c >> 6) & 0x3F)] + hexTable[0x80 | (c & 0x3F)];
				continue;
			};
			
			++i;
			
			if(i >= len){
				fail('_query_string.escape: ' + (_K==="ru" ? ('Неверный формат URI.') : ('URI malformed.')))
			};

			var c2 = str.charCodeAt(i) & 0x3FF;
			
			lastPos = i + 1;
			c = 0x10000 + (((c & 0x3FF) << 10) | c2);
			out += hexTable[0xF0 | (c >> 18)] + hexTable[0x80 | ((c >> 12) & 0x3F)] + hexTable[0x80 | ((c >> 6) & 0x3F)] + hexTable[0x80 | (c & 0x3F)];
		};
		if(lastPos === 0){
			return str;
		};
		if(lastPos < len){
			return out + str.slice(lastPos);
		};
		return out;
	};
	
	function decodeStr(s, decoder){
		try{
			return decoder(s);
		}catch(e){
			return QueryString.unescape(s);
		};
	};
	
	function qsEscape(str) {
		if(typeof str !== 'string'){
			if(typeof str === 'object'){
				str = String(str);
			}else{
				str += '';
			};
		};
		return encodeStr(str, noEscape, hexTable);
	};

	function qsUnescape(s){
		try{
			return decodeURIComponent(s);
		}catch(e){
			return unescape(s);
		};
	};
	
	function stringify(obj, sep, eq, options){
		sep = _avoid_nil(sep, '&');
		eq = _avoid_nil(eq, '=');
		
		var encode = QueryString.escape;
		if(options && typeof options.encodeURIComponent === 'function'){
			encode = options.encodeURIComponent;
		};
		var convert = (encode === qsEscape ? encodeStringified : encodeStringifiedCustom);
		
		if(obj !== null && typeof obj === 'object'){
			var keys = Object.keys(obj);
			var len = keys.length;
			var fields = '';
			for(var i = 0; i < len; ++i){
				var k = keys[i];
				var v = obj[k];
				var ks = convert(k, encode);
				ks += eq;

				if(Array.isArray(v)){
					var vlen = v.length;
					if(vlen === 0){
						continue;
					};
					if(fields){
						fields += sep;
					};
					for(var j = 0; j < vlen; ++j){
						if(j){
							fields += sep;
						};
						fields += ks;
						fields += convert(v[j], encode);
					};
				}else{
					if(fields){
					  fields += sep;
					};
					fields += ks;
					fields += convert(v, encode);
				};
			};
			return fields;
		};
		return '';
	};
	
	function parse(qs, sep, eq, options){
		var obj = {};
		
		if(typeof qs !== 'string' || qs.length === 0){
			return obj;
		};
		
		var sepCodes = (!sep ? defSepCodes : charCodes(sep + ''));
		var eqCodes = (!eq ? defEqCodes : charCodes(eq + ''));
		var sepLen = sepCodes.length;
		var eqLen = eqCodes.length;
		
		var pairs = 1000;
		if(options && typeof options.maxKeys === 'number'){
			pairs = (options.maxKeys > 0 ? options.maxKeys : -1);
		};
		
		var decode = QueryString.unescape;
		if(options && typeof options.decodeURIComponent === 'function'){
			decode = options.decodeURIComponent;
		};
		var customDecode = (decode !== qsUnescape);
		
		var firstChar = qs.charCodeAt(0);
		if(firstChar === 63 || firstChar === 35){
			qs = qs.slice(1);
		};
		
		var lastPos = 0;
		var sepIdx = 0;
		var eqIdx = 0;
		var key = '';
		var value = '';
		var keyEncoded = customDecode;
		var valEncoded = customDecode;
		var plusChar = (customDecode ? '%20' : ' ');
		var encodeCheck = 0;
		for(var i = 0; i < qs.length; ++i){
			var code = qs.charCodeAt(i);
			
			if(code === sepCodes[sepIdx]){
				if(++sepIdx === sepLen){
					var end = i - sepIdx + 1;
					if(eqIdx < eqLen){
						if(lastPos < end){
							key += qs.slice(lastPos, end);
						}else if(key.length === 0){
							if(--pairs === 0){
								return obj;
							};
							lastPos = i + 1;
							sepIdx = eqIdx = 0;
							continue;
						};
					}else if(lastPos < end){
						value += qs.slice(lastPos, end);
					};

					addKeyVal(obj, key, value, keyEncoded, valEncoded, decode);

					if(--pairs === 0){
						return obj;
					};
					keyEncoded = valEncoded = customDecode;
					key = value = '';
					encodeCheck = 0;
					lastPos = i + 1;
					sepIdx = eqIdx = 0;
				};
			}else{
				sepIdx = 0;
				if(eqIdx < eqLen){
					if(code === eqCodes[eqIdx]){
						if(++eqIdx === eqLen){
							var end = i - eqIdx + 1;
							if(lastPos < end){
								key += qs.slice(lastPos, end);
							};
							encodeCheck = 0;
							lastPos = i + 1;
						};
						continue;
					}else{
						eqIdx = 0;
						if(!keyEncoded){
							if(code === 37){
								encodeCheck = 1;
								continue;
							}else if(encodeCheck > 0){
								if(isHexTable[code] === 1){
									if(++encodeCheck === 3){
										keyEncoded = true;
									};
									continue;
								}else{
									encodeCheck = 0;
								};
							};
						};
					};
					if(code === 43){
						if (lastPos < i){
							key += qs.slice(lastPos, i);
						};
						key += plusChar;
						lastPos = i + 1;
						continue;
					};
				};
				if(code === 43){
					if(lastPos < i){
						value += qs.slice(lastPos, i);
					};
					value += plusChar;
					lastPos = i + 1;
				}else if(!valEncoded){
					if(code === 37){
						encodeCheck = 1;
					}else if(encodeCheck > 0){
						if(isHexTable[code] === 1){
							if(++encodeCheck === 3){
								valEncoded = true;
							};
						}else{
							encodeCheck = 0;
						};
					};
				};
			};
		};
		
		if(lastPos < qs.length){
			if(eqIdx < eqLen){
				key += qs.slice(lastPos);
			}else if(sepIdx < sepLen){
				value += qs.slice(lastPos);
			};
		}else if(eqIdx === 0 && key.length === 0){
			return obj;
		};
		
		addKeyVal(obj, key, value, keyEncoded, valEncoded, decode);
		
		return obj;
	};
	
	var QueryString = {
		unescape: qsUnescape,
		escape: qsEscape,

		stringify: stringify,
		encode: stringify,

		parse: parse,
		decode: parse
	};

	return QueryString;
}());
_punycode = (function(){
	var maxInt = 2147483647;
	var base = 36;
	var tMin = 1;
	var tMax = 26;
	var skew = 38;
	var damp = 700;
	var initialBias = 72;
	var initialN = 128;
	var delimiter = '-';
	var regexPunycode = /^xn--/i;
	var regexNonASCII = /[^\x00-\x7E]/;
	var regexSeparators = /[\x2E\u3002\uFF0E\uFF61]/g;
	var errors = {
		'overflow': {'en':'Overflow: input needs wider integers to process','ru':'Переполнение: входные данные требуют более широких целых чисел для обработки'},
		'not-basic': {'en':'Illegal input >= 0x80 (not a basic code point)','ru':'Незаконно, входные данные >= 0x80 (не базовый код)'},
		'invalid-input': {'en':'Invalid input','ru':'Неверные входные данные'}
	};
	var baseMinusTMin = base - tMin;
	var error = function(type){
		fail('_punycode: ' + errors[type][_K]);
	};
	var map = function(array, fn){
		var result = [];
		var length = array.length;
		while(length--){
			result[length] = fn(array[length]);
		};
		return result;
	};
	var mapDomain = function(string, fn){
		var atPos = string.lastIndexOf('@');
		var result = '';
		if(atPos > -1){
			result = string.substring(0, atPos + 1);
			string = string.substring(atPos + 1);
		}
		string = string.replace(regexSeparators, '\x2E');
		var labels = string.split('.');
		var encoded = map(labels, fn).join('.');
		return result + encoded;
	}
	var ucs2decode = function(string){
		var output = [];
		var counter = 0;
		var length = string.length;
		while(counter < length){
			var value = string.charCodeAt(counter++);
			if(value >= 0xD800 && value <= 0xDBFF && counter < length){
				var extra = string.charCodeAt(counter++);
				if((extra & 0xFC00) == 0xDC00){
					output.push(((value & 0x3FF) << 10) + (extra & 0x3FF) + 0x10000);
				}else{
					output.push(value);
					counter--;
				};
			}else{
				output.push(value);
			};
		};
		return output;
	};
	var ucs2encode = function(args){
		if(!Array.isArray(args)){
			args = Array.prototype.slice.call(arguments);
		};
		var MAX_SIZE = 0x4000;
		var codeUnits = [];
		var highSurrogate;
		var lowSurrogate;
		var index = -1;
		var length = args.length;
		if(!length){
			return '';
		}
		var output = '';
		while(++index < length){
			var codePoint = Number(args[index]);
			if(!isFinite(codePoint) || codePoint < 0 || codePoint > 0x10FFFF || Math.floor(codePoint) != codePoint){
				error('invalid-input');
			};
			if (codePoint <= 0xFFFF) {
				codeUnits.push(codePoint);
			}else{
				codePoint -= 0x10000;
				highSurrogate = (codePoint >> 10) + 0xD800;
				lowSurrogate = (codePoint % 0x400) + 0xDC00;
				codeUnits.push(highSurrogate, lowSurrogate);
			};
			if(index + 1 == length || codeUnits.length > MAX_SIZE){
				output += String.fromCharCode.apply(null, codeUnits);
				codeUnits.length = 0;
			};
		};
		return output;
	};
	var basicToDigit = function(codePoint){
		if(codePoint - 0x30 < 0x0A){
			return codePoint - 0x16;
		};
		if(codePoint - 0x41 < 0x1A){
			return codePoint - 0x41;
		};
		if(codePoint - 0x61 < 0x1A){
			return codePoint - 0x61;
		};
		return base;
	};
	var digitToBasic = function(digit, flag){
		return digit + 22 + 75 * (digit < 26) - ((flag != 0) << 5);
	};
	var adapt = function(delta, numPoints, firstTime){
		var k = 0;
		delta = firstTime ? Math.floor(delta / damp) : delta >> 1;
		delta += Math.floor(delta / numPoints);
		for (; delta > baseMinusTMin * tMax >> 1; k += base) {
			delta = Math.floor(delta / baseMinusTMin);
		};
		return Math.floor(k + (baseMinusTMin + 1) * delta / (delta + skew));
	};
	var decode = function(input){
		_validate_argument_type(input, 'string', 'String', '_punycode.decode');
		var output = [];
		var inputLength = input.length;
		var i = 0;
		var n = initialN;
		var bias = initialBias;
		
		var basic = input.lastIndexOf(delimiter);
		if(basic < 0){
			basic = 0;
		};

		for(var j = 0; j < basic; ++j){
			if (input.charCodeAt(j) >= 0x80){
				error('not-basic');
			};
			output.push(input.charCodeAt(j));
		};

		for(var index = basic > 0 ? basic + 1 : 0; index < inputLength;){
			var oldi = i;
			for (var w = 1, k = base;; k += base) {

				if(index >= inputLength){
					error('invalid-input');
				};

				var digit = basicToDigit(input.charCodeAt(index++));

				if(digit >= base || digit > Math.floor((maxInt - i) / w)){
					error('overflow');
				};

				i += digit * w;
				var t = k <= bias ? tMin : (k >= bias + tMax ? tMax : k - bias);

				if(digit < t){
					break;
				};

				var baseMinusT = base - t;
				if(w > Math.floor(maxInt / baseMinusT)){
					error('overflow');
				};

				w *= baseMinusT;
			};

			var out = output.length + 1;
			bias = adapt(i - oldi, out, oldi == 0);
			
			if(Math.floor(i / out) > maxInt - n){
				error('overflow');
			};

			n += Math.floor(i / out);
			i %= out;
			
			output.splice(i++, 0, n);
		};

		return ucs2encode(output);
	};
	var encode = function(input){
		_validate_argument_type(input, 'string', 'String', '_punycode.encode');
		var output = [];
		
		input = ucs2decode(input);
		
		var inputLength = input.length;
		
		var n = initialN;
		var delta = 0;
		var bias = initialBias;
		
		for(var i = 0; i < input.length; i++){
			var currentValue = input[i];
			if(currentValue < 0x80){
				output.push(String.fromCharCode(currentValue));
			};
		};

		var basicLength = output.length;
		var handledCPCount = basicLength;
		
		if(basicLength){
			output.push(delimiter);
		};
		
		while(handledCPCount < inputLength){
			var m = maxInt;
			for(var i = 0; i < input.length; i++){
				var currentValue = input[i];
				if(currentValue >= n && currentValue < m){
					m = currentValue;
				};
			};
			
			var handledCPCountPlusOne = handledCPCount + 1;
			if(m - n > Math.floor((maxInt - delta) / handledCPCountPlusOne)){
				error('overflow');
			};

			delta += (m - n) * handledCPCountPlusOne;
			n = m;
			
			for(var i = 0; i < input.length; i++){
				var currentValue = input[i];
				if(currentValue < n && ++delta > maxInt){
					error('overflow');
				};
				if(currentValue == n){
					var q = delta;
					for(var k = base;; k += base){
						var t = k <= bias ? tMin : (k >= bias + tMax ? tMax : k - bias);
						if (q < t) {
							break;
						};
						var qMinusT = q - t;
						var baseMinusT = base - t;
						output.push(
							String.fromCharCode(digitToBasic(t + qMinusT % baseMinusT, 0))
						);
						q = Math.floor(qMinusT / baseMinusT);
					};

					output.push(String.fromCharCode(digitToBasic(q, 0)));
					bias = adapt(delta, handledCPCountPlusOne, handledCPCount == basicLength);
					delta = 0;
					++handledCPCount;
				};
			};

			++delta;
			++n;
		}
		return output.join('');
	};
	var toUnicode = function(input){
		_validate_argument_type(input, 'string', 'String', '_punycode.toUnicode');
		return mapDomain(input, function(string){
			return regexPunycode.test(string) ? decode(string.slice(4)) : string;
		});
	};
	var toASCII = function(input){
		_validate_argument_type(input, 'string', 'String', '_punycode.toASCII');
		return mapDomain(input, function(string){
			return regexNonASCII.test(string) ? 'xn--' + encode(string) : string;
		});
	};
	var urlToUnicode = function(input){
		_validate_argument_type(input, 'string', 'String', '_punycode.urlToUnicode');
		var url_obj = new _url(input, {rfail: false});
		if(url_obj.host){
			var new_url = input.replace(url_obj.host, toUnicode(url_obj.host));
			return new_url;
		};
		return toUnicode(input);
	};
	var urlToASCII = function(input){
		_validate_argument_type(input, 'string', 'String', '_punycode.urlToASCII');
		var url_obj = new _url(input, {rfail: false});
		if(url_obj.host){
			var new_url = input.replace(url_obj.host, toASCII(url_obj.host));
			return new_url;
		};
		return toASCII(input);
	};
	return {
		'ucs2': {
			'decode': ucs2decode,
			'encode': ucs2encode
		},
		'decode': decode,
		'encode': encode,
		'toASCII': toASCII,
		'toUnicode': toUnicode,
		'urlToASCII': urlToASCII,
		'urlToUnicode': urlToUnicode
	};
}());