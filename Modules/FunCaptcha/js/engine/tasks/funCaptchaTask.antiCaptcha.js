(function (tasks, api, _) {
  const FunCaptchaTask = _.inherit(tasks.AntiCaptchaTask, function (params) {
    tasks.AntiCaptchaTask.call(this, 'FunCaptcha', {
      name: 'FunCaptchaTask' + ((params.proxy && params.proxy.server && params.proxy.Port) ? '' : 'Proxyless'),
      rules: {
        subdomainUrl: { optional: true, name: 'funcaptchaApiJSSubdomain' },
        publicKey: { name: 'websitePublicKey' },
        pageUrl: { name: 'websiteURL' },
        data: { optional: true },
      },
      params: params
    });
  });
  FunCaptchaTask.prototype.getSolution = function (response) {
    const solution = JSON.parse(response);
    return solution['token'];
  };
  api.prototype.FunCaptchaTask = FunCaptchaTask;
})(BASCaptchaSolver.tasks, BASCaptchaSolver.AntiCaptchaApi, BASCaptchaSolver.utils);