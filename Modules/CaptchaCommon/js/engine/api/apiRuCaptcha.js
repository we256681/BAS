(function (solver, _) {
  const RuCaptchaApi = _.inherit(solver.CaptchaApi, function (method, options) {
    solver.CaptchaApi.call(this, method, options);
    this.options.isJsonApi = '0';
  });
  solver.RuCaptchaApi = RuCaptchaApi;
})(BASCaptchaSolver, BASCaptchaSolver.utils);