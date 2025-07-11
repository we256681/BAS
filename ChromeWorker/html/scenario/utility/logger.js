window.log = (() => {
  let enabled = false;

  if (enabled) {
    console.log('Please execute the "log.disable()" function in order to disable logs');
  } else {
    console.log('Please execute the "log.enable()" function in order to enable logs');
  }

  ['log', 'warn', 'info', 'time', 'debug', 'error', 'trace', 'timeEnd', 'timeLog'].forEach(method => {
    const original = console[method];
    console[method] = (...args) => {
      if (enabled) {
        original.call(console, ...args);
      }
    };
  });

  return {
    disable() {
      enabled = false;
    },

    enable() {
      enabled = true;
    },

    ...Object.fromEntries(
      ['debug', 'error', 'warn', 'info'].map(level => {
        return [level, (message, ...args) => console[level](`[${level}] ${message}`, ...args)];
      })
    ),
  };
})();
