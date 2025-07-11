(function (solver, _) {
  function BaseHelper(options) {
    this.waiter = options.waiter;
    this.query = options.query;
    this.path = options.path;
  };

  BaseHelper.prototype.removeFrameFromRight = function () {
    this.query = removePart(this.query, '>FRAME>', false);
  };

  BaseHelper.prototype.removeFrameFromLeft = function () {
    this.query = removePart(this.query, '>FRAME>', true);
  };

  BaseHelper.prototype.ensureSelector = function () {
    _call_function(BASCaptchaSolver.helper.waiter, {})!
    BASCaptchaSolver.helper.path().exist()!
    _function_return(_result() === 1);
  };

  BaseHelper.prototype.submitCaptcha = function () {
    fail(tr('Method "BaseHelper#submitCaptcha" is not implemented'));
  };

  BaseHelper.prototype.extractData = function () {
    fail(tr('Method "BaseHelper#extractData" is not implemented'));
  };

  BaseHelper.prototype.$element = function () {
    return get_element_selector(BASCaptchaSolver.helper.query, false);
  };

  function removePart(target, match, left) {
    const index = target[left ? 'indexOf' : 'lastIndexOf'](match);
    return target.slice(0, index >= 0 ? index : target.length);
  };

  solver.helpers.BaseHelper = BaseHelper;
})(BASCaptchaSolver, BASCaptchaSolver.utils);