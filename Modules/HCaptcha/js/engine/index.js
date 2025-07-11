(function (global) {
  global.BASCaptchaSolver.solveHCaptcha = function () {
    const args = _function_arguments();
    BASCaptchaSolver.api = BASCaptchaSolver.getService(args);

    BASCaptchaSolver.setHelper(new BASCaptchaSolver.helpers.HCaptchaHelper(args));
    _call_function(BASCaptchaSolver.helper.initialize, {})!
    const data = _result_function();
    if (data === null) _break();

    _call_function(BASCaptchaSolver.api.solve, {
      task: new BASCaptchaSolver.api.HCaptchaTask({
        userAgent: args.userAgent,
        pageUrl: data.pageUrl,
        siteKey: data.siteKey,
        proxy: args.proxy
      }),
      waitTimeout: args.taskWaitTimeout,
      waitDelay: args.taskWaitDelay
    })!

    _call_function(BASCaptchaSolver.helper.submitCaptcha, { token: _result_function() })!
  };
})(this);