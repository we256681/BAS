(function (tasks, api, _) {
  const HCaptchaTask = _.inherit(tasks.RuCaptchaTask, function (params) {
    tasks.RuCaptchaTask.call(this, 'HCaptcha', {
      name: 'hcaptcha',
      rules: {
        invisible: { optional: true },
        siteKey: { name: 'sitekey' },
        pageUrl: { name: 'pageurl' },
        data: { optional: true },
      },
      params: params
    });
  });
  api.prototype.HCaptchaTask = HCaptchaTask;
})(BASCaptchaSolver.tasks, BASCaptchaSolver.RuCaptchaApi, BASCaptchaSolver.utils);