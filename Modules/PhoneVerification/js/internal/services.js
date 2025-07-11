_SMS.getServices = function(){
	return {
		"sms-activate.org": {
			api: this.SmsActivateApi,
			config: {
				name: 'Sms-activate',
				url: 'https://api.sms-activate.org',
				supportedMethods: [
					'getNumbersCount',
					'getCountries'
				],
				ref: 'browserAutomationStudio',
				limits: [
					{
						requestsPerInterval: 200,
						interval: 'minute',
						type: 'service'
					},
					{
						requestsPerInterval: 10,
						interval: 'second',
						type: 'service'
					},
					{
						requestsPerInterval: 2,
						interval: 'second',
						type: 'thread'
					}
				]
			},
			aliases: ["sms-activate.ru", "sms-activate", "smsactivate"]
		},
		"smshub.org": {
			api: this.SmsActivateApi,
			config: {
				name: 'SMSHUB',
				url: 'http://smshub.org',
				supportedMethods: [
					'getNumbersCount'
				],
				limits: [
					{
						requestsPerInterval: 200,
						interval: 'minute',
						type: 'service'
					},
					{
						requestsPerInterval: 10,
						interval: 'second',
						type: 'service'
					},
					{
						requestsPerInterval: 2,
						interval: 'second',
						type: 'thread'
					}
				]
			},
			aliases: ["smshub"]
		},
		"5sim.net": {
			api: this.SmsActivateApi,
			config: {
				name: '5SIM',
				url: 'http://api1.5sim.net',
				supportedMethods: [
					'getNumbersCount'
				],
				ref: '598fdb60',
				limits: [
					{
						requestsPerInterval: 200,
						interval: 'minute',
						type: 'service'
					},
					{
						requestsPerInterval: 10,
						interval: 'second',
						type: 'service'
					},
					{
						requestsPerInterval: 2,
						interval: 'second',
						type: 'thread'
					}
				]
			},
			aliases: ["5sim"]
		},
		"365sms.org": {
			api: this.SmsActivateApi,
			config: {
				name: '365SMS',
				url: 'https://365sms.org',
				supportedMethods: [
					'getNumbersCount'
				],
				ref: '2glCMQmT',
				limits: [
					{
						requestsPerInterval: 100,
						interval: 'minute',
						type: 'service'
					},
					{
						requestsPerInterval: 3,
						interval: 'second',
						type: 'service'
					},
					{
						requestsPerInterval: 1,
						interval: 'second',
						type: 'thread'
					}
				]
			},
			aliases: ["365sms.ru", "365sms"]
		},
		"sms-man.ru": {
			api: this.SmsActivateApi,
			config: {
				name: 'SMS@MAN',
				url: 'https://api.sms-man.ru',
				supportedMethods: [
					'getNumbersCount',
					'getSites',
					'getCountries'
				],
				ref: 'lyevZ418dni4',
				limits: [
					{
						requestsPerInterval: 100,
						interval: 'minute',
						type: 'service'
					},
					{
						requestsPerInterval: 3,
						interval: 'second',
						type: 'service'
					},
					{
						requestsPerInterval: 1,
						interval: 'second',
						type: 'thread'
					}
				]
			},
			aliases: ["sms-man", "smsman"]
		},
		"activation.pw": {
			api: this.SmsActivateApi,
			config: {
				name: 'ActivationPw',
				url: 'https://activation.pw',
				supportedMethods: [
					'getNumbersCount'
				],
				ref: '4dcbfedf7b81f8d067a78a6d825e36de',
				limits: [
					{
						requestsPerInterval: 100,
						interval: 'minute',
						type: 'service'
					},
					{
						requestsPerInterval: 10,
						interval: 'second',
						type: 'service'
					},
					{
						requestsPerInterval: 2,
						interval: 'second',
						type: 'thread'
					}
				]
			},
			aliases: ["activationpw", "activation"]
		},
		"getsms.online": {
			api: this.SmsActivateApi,
			config: {
				name: 'GetSMS',
				url: 'https://smsactivateapi.getsms.online',
				supportedMethods: [
					'getNumbersCount'
				],
				ref: '20111',
				limits: [
					{
						requestsPerInterval: 100,
						interval: 'minute',
						type: 'service'
					},
					{
						requestsPerInterval: 10,
						interval: 'second',
						type: 'service'
					},
					{
						requestsPerInterval: 2,
						interval: 'second',
						type: 'thread'
					}
				]
			},
			aliases: ["getsms"]
		},
		"cheapsms.ru": {
			api: this.SmsActivateApi,
			config: {
				name: 'CheapSMS',
				url: 'https://cheapsms.pro',
				path: '/handler/index',
				supportedMethods: [
					'getNumbersCount'
				],
				limits: [
					{
						requestsPerInterval: 100,
						interval: 'minute',
						type: 'service'
					},
					{
						requestsPerInterval: 10,
						interval: 'second',
						type: 'service'
					},
					{
						requestsPerInterval: 2,
						interval: 'second',
						type: 'thread'
					}
				]
			},
			aliases: ["cheapsms"]
		},
		"smsvk.net": {
			api: this.SmsActivateApi,
			config: {
				name: 'SmsVK',
				url: 'http://smsvk.net',
				supportedMethods: [
					'getNumbersCount'
				],
				ref: 'bablosoft',
				limits: [
					{
						requestsPerInterval: 100,
						interval: 'minute',
						type: 'service'
					},
					{
						requestsPerInterval: 10,
						interval: 'second',
						type: 'service'
					},
					{
						requestsPerInterval: 2,
						interval: 'second',
						type: 'thread'
					}
				]
			},
			aliases: ["smsvk"]
		},
		"smscode.me": {
			api: this.SmsActivateApi,
			config: {
				name: 'SMScode.me',
				url: 'https://smscode.me',
				supportedMethods: [
					'getNumbersCount'
				],
				ref: 'bablosoft',
				limits: [
					{
						requestsPerInterval: 100,
						interval: 'minute',
						type: 'service'
					},
					{
						requestsPerInterval: 6,
						interval: 'second',
						type: 'service'
					},
					{
						requestsPerInterval: 1,
						interval: 'second',
						type: 'thread'
					}
				]
			},
			aliases: ["sms.kopeechka.store", "kopeechka", "smscodeme", "smscode"]
		},
		"sms-reg.com": {
			api: this.SmsRegApi,
			config: {
				name: 'SMS-REG',
				url: 'https://api.sms-reg.com',
				supportedMethods: [
					'getSites'
				],
				ref: 'RUBMC9BX6OIRJG3S',
				refTitle: 'appid',
				limits: [
					{
						requestsPerInterval: 100,
						interval: 'minute',
						type: 'service'
					},
					{
						requestsPerInterval: 3,
						interval: 'second',
						type: 'service'
					},
					{
						requestsPerInterval: 1,
						interval: 'second',
						type: 'thread'
					}
				]
			},
			aliases: ["sms-reg", "smsreg"]
		},
		"smspva.com": {
			api: this.SmsPvaApi,
			config: {
				name: 'SMSpva',
				url: 'http://smspva.com',
				supportedMethods: [
					'getNumbersCount'
				],
				limits: [
					{
						requestsPerInterval: 100,
						interval: 'minute',
						type: 'service'
					},
					{
						requestsPerInterval: 5,
						interval: 'second',
						type: 'service'
					},
					{
						requestsPerInterval: 1,
						interval: 'second',
						type: 'thread'
					}
				]
			},
			aliases: ["smspva"]
		},
		"simsms.org": {
			api: this.SmsPvaApi,
			config: {
				name: 'SIMsms',
				url: 'http://simsms.org',
				supportedMethods: [
					'getNumbersCount'
				],
				limits: [
					{
						requestsPerInterval: 100,
						interval: 'minute',
						type: 'service'
					},
					{
						requestsPerInterval: 5,
						interval: 'second',
						type: 'service'
					},
					{
						requestsPerInterval: 1,
						interval: 'second',
						type: 'thread'
					}
				]
			},
			aliases: ["simsms"]
		},
		"onlinesim.ru": {
			api: this.OnlineSimApi,
			config: {
				name: 'OnlineSIM',
				url: 'https://onlinesim.ru',
				supportedMethods: [
					'getNumbersCount'
				],
				ref: '2451761',
				refTitle: 'dev_id',
				limits: [
					{
						requestsPerInterval: 30,
						interval: 30000,
						type: 'service'
					},
					{
						requestsPerInterval: 2,
						interval: 'second',
						type: 'service'
					},
					{
						requestsPerInterval: 1,
						interval: 'second',
						type: 'thread'
					}
				]
			},
			aliases: ["onlinesim"]
		},
		"sms-acktiwator.ru": {
			api: this.SmsAcktiwatorApi,
			config: {
				name: 'SmsAcktiwator',
				url: 'https://sms-acktiwator.ru',
				supportedMethods: [
					'getNumbersCount',
					'getSites',
					'getCountries'
				],
				limits: [
					{
						requestsPerInterval: 100,
						interval: 'minute',
						type: 'service'
					},
					{
						requestsPerInterval: 6,
						interval: 'second',
						type: 'service'
					},
					{
						requestsPerInterval: 1,
						interval: 'second',
						type: 'thread'
					}
				]
			},
			aliases: ["sms-acktiwator", "smsacktiwator"]
		},
		"vak-sms.com": {
			api: this.VakSmsApi,
			config: {
				name: 'VAK-SMS',
				url: 'https://vak-sms.com',
				supportedMethods: [
					'getNumbersCount'
				],
				ref: '1007',
				refTitle: 'softId',
				limits: [
					{
						requestsPerInterval: 100,
						interval: 'minute',
						type: 'service'
					},
					{
						requestsPerInterval: 10,
						interval: 'second',
						type: 'service'
					},
					{
						requestsPerInterval: 2,
						interval: 'second',
						type: 'thread'
					}
				]
			},
			aliases: ["vak-sms", "vaksms"]
		},
		"give-sms.com": {
			api: this.GiveSmsApi,
			config: {
				name: 'Give-SMS',
				url: 'https://give-sms.com',
				supportedMethods: [
					'getNumbersCount'
				],
				limits: [
					{
						requestsPerInterval: 100,
						interval: 'minute',
						type: 'service'
					},
					{
						requestsPerInterval: 5,
						interval: 'second',
						type: 'service'
					},
					{
						requestsPerInterval: 1,
						interval: 'second',
						type: 'thread'
					}
				]
			},
			aliases: ["give-sms", "givesms"]
		}
	};
};
_SMS.getServiceApi = function(data){
	var services = this.getServices();
	var service = data.service;
	if(!services.hasOwnProperty(service)){
		die(_K=="ru" ? ('Сервиса ' + service + ' нет в списке доступных') : (service + ' service is not in the list of available'), true);
	};
	var obj = services[service];
	try{
		return (new obj.api(obj.config, data));
	}catch(e){
		die(_K=="ru" ? ('Класс сервиса ' + service + ' поврежден или отсутствует') : ('Class of service ' + service + ' is corrupted or missing'), true);
	};
};
_SMS.getBasicName = function(service){
	service = service.toLowerCase();
	var services = this.getServices();
	if(services.hasOwnProperty(service)){
		return service;
	};
	for(var name in services){
		if(services[name].aliases && services[name].aliases.indexOf(service) > -1){
			return name;
		};
	};
	return service;
};