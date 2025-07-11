(function (tasks, _) {
  const AntiCaptchaTask = _.inherit(tasks.BaseTask, function (type, options) {
    tasks.BaseTask.call(this, type, options);
    this.data['type'] = options.name;
  });

  AntiCaptchaTask.prototype.applyProxy = function (proxy, _, userAgent) {
    if (proxy != null && proxy.server && proxy.Port) {
      if (proxy.password && proxy.name) {
        this.data['proxyPassword'] = proxy.password;
        this.data['proxyLogin'] = proxy.name;
      }
      this.data['proxyType'] = proxy['IsHttp'] ? 'http' : 'socks5';
      this.data['proxyAddress'] = proxy.server;
      this.data['proxyPort'] = proxy.Port;
      this.data['userAgent'] = userAgent;
    }
    return this.data;
  };

  AntiCaptchaTask.prototype.getSolution = function (response) {
    return response;
  };

  tasks.AntiCaptchaTask = AntiCaptchaTask;
})(BASCaptchaSolver.tasks, BASCaptchaSolver.utils);