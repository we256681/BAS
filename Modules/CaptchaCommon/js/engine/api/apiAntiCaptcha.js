(function (solver, _) {
  const AntiCaptchaApi = _.inherit(solver.CaptchaApi, function (method, options) {
    solver.CaptchaApi.call(this, method, options);
    this.options.isJsonApi = '1';
  });
  solver.AntiCaptchaApi = AntiCaptchaApi;
})(BASCaptchaSolver, BASCaptchaSolver.utils);