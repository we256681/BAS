_SMS.SmsRegApi.prototype.getRawCountry = function(country){
	var countries = {
		"Any": "all",
		"RU": "ru",
		"KZ": "kz",
		"UA": "ua",
		"PL": "pl",
		"GB": "gb",
		"DE": "de",
		"MD": "md",
		"FR": "fr",
		"CN": "cn"
	};
	return countries.hasOwnProperty(country) ? countries[country] : countries["Any"];
};