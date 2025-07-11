(function (solver, _) {
  const HCaptchaHelper = _.inherit(solver.helpers.BaseHelper, function (options) {
    solver.helpers.BaseHelper.call(this, options, {});
    this.submitCaptcha = _.bind(submitCaptcha, this);
    this.extractData = _.bind(extractData, this);
    this.initialize = _.bind(initialize, this);

    this.gRecaptchaResponse = _.bind(function () {
      return this.$element().xpath('//textarea[@name="g-recaptcha-response"]');
    }, this);

    this.hCaptchaResponse = _.bind(function () {
      return this.$element().xpath('//textarea[@name="h-captcha-response"]');
    }, this);
  });

  function submitCaptcha() {
    const self = this;

    _call_function(_.script, {
      script: 'self.value = ' + JSON.stringify(_function_argument('token')),
      element: self.gRecaptchaResponse()
    })!

    _call_function(_.script, {
      script: 'self.value = ' + JSON.stringify(_function_argument('token')),
      element: self.hCaptchaResponse()
    })!

    self.$element().script('_BAS_HIDE(BrowserAutomationStudio_HCaptchaSolved)(' + JSON.stringify(_function_argument('token')) + ')')!
  };

  function extractData() {
    const self = this;

    _call_function(_.script, { element: self.$element(), script: '_BAS_HIDE(BrowserAutomationStudio_HCaptchaCallback)' })!
    var callBack = _result_function();
    _if(!callBack.length, function () {
      _call_function(_.script, {
        element: self.$element(), script: "_BAS_HIDE(BrowserAutomationStudio_HCaptchaCallback) = self.parentNode.dataset.callback"
      })!
      callBack = _result_function();
    })!

    _call_function(_.script, { element: self.$element(), script: '_BAS_HIDE(BrowserAutomationStudio_HCaptchaSitekey)' })!
    var siteKey = _result_function();
    _if(!siteKey.length, function () {
      _call_function(_.script, {
        element: self.$element(), script: "_BAS_HIDE(BrowserAutomationStudio_HCaptchaSitekey) = self.parentNode.dataset.sitekey"
      })!
      siteKey = _result_function();
    })!

    _call_function(_.script, { element: self.$element(), script: 'location.href' }, function () {
      _function_return({ callBack: callBack, siteKey: siteKey, pageUrl: _result_function() });
    });
  };

  function initialize() {
    const self = this; _call_function(self.ensureSelector, {})!

    _if_else(_result_function(), function () {
      _do_with_params({ self: self }, function () {
        self.$element().script('((self.children.length && self.children[0].src.includes("assets.hcaptcha.com")) || self.src.includes("assets.hcaptcha.com")) ? 1 : 0')!
        const self = _cycle_param('self');
        if (_iterator() == 3) _break();
        if (_result() == 1) _break();
        self.removeFrameFromRight();
      })!

      _call_function(_.exist, { element: self.gRecaptchaResponse() })!
      if (_result_function() !== 1 && false) { // not required
        fail(tr("Can't find HCaptcha 'g-recaptcha-response' textarea element"));
      }

      _call_function(_.exist, { element: self.hCaptchaResponse() })!
      if (_result_function() !== 1 && true) { // is required
        fail(tr("Can't find HCaptcha 'h-captcha-response' textarea element"));
      }

      _call_function(self.extractData, {})!
    }, function () {
      _function_return(null);
    })!
  };

  solver.helpers.HCaptchaHelper = HCaptchaHelper;
})(BASCaptchaSolver, BASCaptchaSolver.utils);