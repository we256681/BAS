(({ App, $, _ }) => {
  const toString = Object.prototype.toString;

  _.extend($.fn.selectpicker.Constructor.DEFAULTS, {
    template: { caret: '' },
  });

  App.utils.parseJSON = function (text, reviver) {
    return mutate(JSON.parse(text, reviver), value => {
      if (typeof value === 'string') {
        if (value.startsWith('__undefined__')) {
          return undefined;
        } else if (value.startsWith('__date__')) {
          return new Date(value.slice(8));
        }
      }
      return value;
    });
  };

  $.fn.slideDownEx = function (...args) {
    return this.each(function () {
      const $el = $(this);
      if (!$el.is(':visible')) $.fn.slideDown.apply($el, args);
    });
  };

  $.fn.slideUpEx = function (...args) {
    return this.each(function () {
      const $el = $(this);
      if (!$el.is(':hidden')) $.fn.slideUp.apply($el, args);
    });
  };

  _.mixin({
    isSymbol: val => toString.call(val) === '[object Symbol]',

    isError: val => toString.call(val) === '[object Error]',

    sleep: time => new Promise(fn => setTimeout(fn, time)),

    attempt: (func, ...args) => {
      try {
        return func.apply(null, args);
      } catch (e) {
        return _.isError(e) ? e : new Error(e);
      }
    },
  });

  function mutate(obj, mutator) {
    if (obj && typeof obj === 'object') {
      Object.keys(obj).forEach(key => {
        obj[key] = mutate(obj[key], mutator);
      });
    }
    return mutator(obj);
  }
})(window);
