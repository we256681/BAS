(function (solver, _) {
  function CaptchaApi(method, options) {
    this.solve = _.bind(solve, this);
    this.options = options;
    this.method = method;
  };

  CaptchaApi.prototype.setApiUrl = function (url) {
    if (typeof (url) === 'string') {
      this.options.apiUrl = url.trim();
    }
    return this;
  };

  CaptchaApi.prototype.setApiKey = function (key) {
    if (typeof (key) === 'string') {
      this.options.apiKey = key.trim();
    }
    return this;
  };

  function solve() {
    const self = this, task = _function_argument('task').validate(self);
    const waitTimeout = _function_argument('waitTimeout') || 5000;
    const waitDelay = _function_argument('waitDelay') || 5000;
    const options = self.options, params = task.serialize();

    params['is_json_interface'] = options.isJsonApi;
    params['service_url'] = options.apiUrl;
    params['service_key'] = options.apiKey;
    params['timeout'] = waitTimeout;
    params['delay'] = waitDelay;

    _solve_captcha(self.method, task.image, params, false, function () {
      if (_result().indexOf('CAPTCHA_FAIL') >= 0) fail(_result());
      _function_return(_function_argument('task').getSolution(_result()));
    });
  };

  solver.CaptchaApi = CaptchaApi;
})(BASCaptchaSolver, BASCaptchaSolver.utils);