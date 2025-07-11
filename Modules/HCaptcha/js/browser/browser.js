(function () {
  Object.defineProperty(window, 'hcaptcha', {
    set: function (value) {
      _BAS_HIDE(BrowserAutomationStudio_HCaptcha) = value;

      const originalRenderFn = value.render;
      _BAS_HIDE(BrowserAutomationStudio_HCaptcha).render = function (container, params) {
        if (params) {
          if (params.callback) {
            _BAS_HIDE(BrowserAutomationStudio_HCaptchaCallback) = params.callback;
          }

          if (params.sitekey) {
            _BAS_HIDE(BrowserAutomationStudio_HCaptchaSitekey) = params.sitekey;
          }

          if (params.size) {
            _BAS_HIDE(BrowserAutomationStudio_HCaptchaSize) = params.size;
          }
        }

        return originalRenderFn(container, params);
      };

      const originalGetResponseFn = value.getResponse;
      _BAS_HIDE(BrowserAutomationStudio_HCaptcha).getResponse = function () {
        if (typeof (_BAS_HIDE(BrowserAutomationStudio_HCaptchaResult)) === 'string') {
          return _BAS_HIDE(BrowserAutomationStudio_HCaptchaResult);
        }
        return originalGetResponseFn.apply(this, arguments);
      };

      const originalExecuteFn = value.execute;
      _BAS_HIDE(BrowserAutomationStudio_HCaptcha).execute = function () {
        return originalExecuteFn.apply(this, arguments);
      };

      const originalResetFn = value.reset;
      _BAS_HIDE(BrowserAutomationStudio_HCaptcha).reset = function () {
        return originalResetFn.apply(this, arguments);
      };
    },
    get: function () {
      return _BAS_HIDE(BrowserAutomationStudio_HCaptcha);
    }
  });

  _BAS_HIDE(BrowserAutomationStudio_HCaptchaSolved) = function (token) {
    _BAS_HIDE(BrowserAutomationStudio_HCaptchaResult) = token;
    let callback = _BAS_HIDE(BrowserAutomationStudio_HCaptchaCallback);
    if (typeof (callback) !== 'undefined') {
      if (typeof (callback) === 'string') callback = eval(callback);
      try { callback(token); } catch (e) { }
    }
  };
})();