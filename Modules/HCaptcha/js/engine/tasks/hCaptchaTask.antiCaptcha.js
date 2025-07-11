(function (tasks, api, _) {
  const HCaptchaTask = _.inherit(tasks.AntiCaptchaTask, function (params) {
    tasks.AntiCaptchaTask.call(this, 'HCaptcha', {
      name: 'HCaptchaTask' + ((params.proxy && params.proxy.server && params.proxy.Port) ? '' : 'Proxyless'),
      rules: {
        siteKey: { name: 'websiteKey' },
        pageUrl: { name: 'websiteURL' },
      },
      params: params
    });
  });
  HCaptchaTask.prototype.getSolution = function (response) {
    return response;
  };
  api.prototype.HCaptchaTask = HCaptchaTask;
})(BASCaptchaSolver.tasks, BASCaptchaSolver.AntiCaptchaApi, BASCaptchaSolver.utils);