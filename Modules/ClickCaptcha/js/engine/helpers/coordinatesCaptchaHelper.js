(function (solver, _) {
  const CoordinatesCaptchaHelper = _.inherit(solver.helpers.BaseHelper, function (options) {
    solver.helpers.BaseHelper.call(this, options, {});
    this.submitCaptcha = _.bind(submitCaptcha, this);
    this.extractData = _.bind(extractData, this);
    this.initialize = _.bind(initialize, this);
    this.emulateMouse = options.emulateMouse;
  });

  function submitCaptcha() {
    const points = _function_argument('points'), self = this;

    _call_function(_.script, {
      element: self.$element(),
      script: '(self.getBoundingClientRect().left + scrollx + positionx)'
    })!
    const offsetX = parseInt(_result_function());

    _call_function(_.script, {
      element: self.$element(),
      script: '(self.getBoundingClientRect().top + scrolly + positiony)'
    })!
    const offsetY = parseInt(_result_function());

    _do(function () {
      if (points.length === 0) _break();
      const point = points.shift();
      const x = offsetX + point[0];
      const y = offsetY + point[1];

      _if(self.emulateMouse, function () {
        move(x, y)!
      })!
      mouse(x, y)!
    })!
  }

  function extractData() {
    const self = this;

    self.$element().render_base64(function () {
      _function_return({ imageData: _result() });
    });
  };

  function initialize() {
    const self = this; _call_function(self.ensureSelector, {})!

    _if_else(_result_function(), function () {
      _call_function(self.extractData, {})!
    }, function () {
      _function_return(null);
    })!
  };

  solver.helpers.CoordinatesCaptchaHelper = CoordinatesCaptchaHelper;
})(BASCaptchaSolver, BASCaptchaSolver.utils);