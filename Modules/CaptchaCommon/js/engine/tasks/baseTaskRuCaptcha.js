(function (tasks, _) {
  const RuCaptchaTask = _.inherit(tasks.BaseTask, function (type, options) {
    tasks.BaseTask.call(this, type, options);
    this.data['method'] = options.name;
  });

  RuCaptchaTask.prototype.applyProxy = function (proxy, _, userAgent) {
    if (proxy != null && proxy.server && proxy.Port) {
      this.data['proxy'] = proxy.server + ':' + proxy.Port;
      if (proxy.password && proxy.name) {
        this.data['proxy'] = proxy.name + ':' + proxy.password + '@' + this.data['proxy'];
      }
      this.data['proxytype'] = proxy['IsHttp'] ? 'HTTP' : 'SOCKS5';
      this.data['userAgent'] = userAgent;
    }
    return this.data;
  };

  RuCaptchaTask.prototype.getSolution = function (response) {
    return response;
  };

  tasks.RuCaptchaTask = RuCaptchaTask;
})(BASCaptchaSolver.tasks, BASCaptchaSolver.utils);