(function (solver) {
  function BaseTask(type, options) {
    this.params = options.params;
    this.rules = options.rules;
    this.image = options.image;
    this.type = type;
    this.data = {};
  };

  BaseTask.prototype.validate = function (api) {
    const options = api.options;

    if (options.supportedTasks.indexOf(this.type) < 0) {
      if (_K === 'en') {
        fail('Service `' + options.name + '` does not support `' + this.type + '`');
      } else {
        fail('Сервис `' + options.name + '` не поддерживает `' + this.type + '`');
      }
    }

    return this;
  };

  BaseTask.prototype.serialize = function () {
    const self = this;

    Object.keys(self.rules).forEach(function (key) {
      const param = self.params[key];
      const rule = self.rules[key];

      if (typeof (param) === 'undefined' && !rule.optional) {
        if (_K === 'en') {
          fail('Parameter "' + key + '" is not specified');
        } else {
          fail('Параметр "' + key + '" не указан');
        }
      } else if (typeof (param) !== 'undefined') {
        self.data[rule.name || key] = param;
      }
    });

    return self.applyProxy(self.params.proxy, self.params.cookies, self.params.userAgent);
  };

  solver.tasks.BaseTask = BaseTask;
})(BASCaptchaSolver);