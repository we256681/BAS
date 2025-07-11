(function () {
  function extendFunCaptcha(fn) {
    const extended = function () {
      if (arguments[0] && arguments[0].callback) {
        _BAS_HIDE(BrowserAutomationStudio_FunCaptchaCallback) = arguments[0].callback;
      }
      fn.apply(this, arguments);
    };

    extended.prototype = Object.create(fn.prototype);
    return extended;
  };

  Object.defineProperty(window, 'FunCaptcha', {
    set: function () {
      _BAS_HIDE(BrowserAutomationStudio_FunCaptcha) = extendFunCaptcha(
        ArkoseEnforcement = extendFunCaptcha(ArkoseEnforcement)
      );
    },
    get: function () {
      return _BAS_HIDE(BrowserAutomationStudio_FunCaptcha);
    }
  });

  _BAS_HIDE(BrowserAutomationStudio_FunCaptchaSolved) = function (token) {
    let callback = _BAS_HIDE(BrowserAutomationStudio_FunCaptchaCallback);
    if (typeof (callback) !== 'undefined') {
      if (typeof (callback) === 'string') callback = eval(callback);
      try { callback(token); } catch (e) { }
    }
  };
})();