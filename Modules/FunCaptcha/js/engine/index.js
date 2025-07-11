(function (global) {
  global.BASCaptchaSolver.solveFunCaptcha = function () {
    const args = _function_arguments();
    BASCaptchaSolver.api = BASCaptchaSolver.getService(args);

    BASCaptchaSolver.setHelper(new BASCaptchaSolver.helpers.FunCaptchaHelper(args));
    _call_function(BASCaptchaSolver.helper.initialize, {})!
    const data = _result_function();
    if (data === null) _break();

    _call_function(BASCaptchaSolver.api.solve, {
      task: new BASCaptchaSolver.api.FunCaptchaTask({
        subdomainUrl: data.subdomainUrl,
        publicKey: data.publicKey,
        userAgent: args.userAgent,
        pageUrl: data.pageUrl,
        proxy: args.proxy
      }),
      waitTimeout: args.taskWaitTimeout,
      waitDelay: args.taskWaitDelay
    })!

    _call_function(BASCaptchaSolver.helper.submitCaptcha, { token: _result_function() })!
  };
})(this);