_SMS.VakSmsApi.prototype.getRawCountry = function(country){
	var countries = {
		"Any": "",
		"RU": "ru",
		"KZ": "kz",
		"UA": "ua",
		"PL": "pl",
		"GB": "gb",
		"DE": "de",
		"MD": "md",
		"PH": "ph",
		"ID": "id",
		"LA": "la",
		"RO": "ro",
		"EE": "ee",
		"LT": "lt",
		"SE": "se",
		"NL": "nl",
		"LV": "lv",
		"ES": "es",
		"FR": "fr",
		"PT": "pt",
		"VN": "vn",
		"HK": "hk",
		"MX": "mx",
		"GE": "ge",
		"FI": "fi",
		"DK": "dk"
	};
	return countries.hasOwnProperty(country) ? countries[country] : countries["Any"];
};