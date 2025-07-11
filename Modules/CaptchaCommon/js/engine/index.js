(function (global) {
  global.BASCaptchaSolver = {
    helpers: {},

    tasks: {},

    utils: {},

    setHelper: function (value) {
      BASCaptchaSolver.helper = value;
    },

    setDebug: function (value) {
      BASCaptchaSolver.debug = value;
    },
  };
})(this);