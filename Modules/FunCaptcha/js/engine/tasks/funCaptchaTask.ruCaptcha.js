(function (tasks, api, _) {
  const FunCaptchaTask = _.inherit(tasks.RuCaptchaTask, function (params) {
    tasks.RuCaptchaTask.call(this, 'FunCaptcha', {
      name: 'funcaptcha',
      rules: {
        subdomainUrl: { optional: true, name: 'surl' },
        publicKey: { name: 'publickey' },
        pageUrl: { name: 'pageurl' },
        data: { optional: true },
      },
      params: params
    });
  });
  api.prototype.FunCaptchaTask = FunCaptchaTask;
})(BASCaptchaSolver.tasks, BASCaptchaSolver.RuCaptchaApi, BASCaptchaSolver.utils);