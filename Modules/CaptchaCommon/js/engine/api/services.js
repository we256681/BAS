(function (solver) {
  const services = {
    'AntiCaptcha': {
      api: new solver.AntiCaptchaApi('antigate', {
        supportedTasks: ['FunCaptcha', 'HCaptcha'],
        name: 'AntiCaptcha'
      }),
      url: 'https://api.anti-captcha.com/'
    },
    'RuCaptcha': {
      api: new solver.RuCaptchaApi('rucaptcha', {
        supportedTasks: ['CoordinatesCaptcha', 'FunCaptcha', 'HCaptcha'],
        name: 'RuCaptcha'
      }),
      url: 'https://rucaptcha.com/'
    },
    '2Captcha': {
      api: new solver.RuCaptchaApi('2captcha', {
        supportedTasks: ['CoordinatesCaptcha', 'FunCaptcha', 'HCaptcha'],
        name: '2Captcha'
      }),
      url: 'https://2captcha.com/'
    },
  };

  function findService(serviceName) {
    var name = serviceName.toLowerCase().replace('-newapi', '');

    for (var key in services) {
      if (key.toLowerCase() === name || services[key].api.method === name) {
        return services[key];
      }
    }
  };

  solver.getService = function (options) {
    const service = findService(options.serviceName);

    if (!service) {
      if (_K === 'en') {
        fail('Service "' + options.serviceName + '" is unavailable');
      } else {
        fail('Сервис "' + options.serviceName + '" недоступен');
      }
    }

    return service.api.setApiKey(options.serviceKey).setApiUrl(options.serviceUrl || service.url);
  };
})(BASCaptchaSolver);