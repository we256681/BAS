'use strict';

const post = (type, payload, callback) => {
  if (callback) {
    window.addEventListener('message', ({ data }) => {
      if (data.type === type) {
        callback(data.payload);
      }
    }, { once: true });
  }

  window.parent.postMessage({ type, payload: payload || {} }, '*');
};

const parseJSON = (text, reviver) => {
  try {
    return JSON.parse(text, reviver);
  } catch {
    return {};
  }
};

const debounce = (fn, delay) => {
  let timeout = 0;
  return function (...args) {
    if (!timeout) {
      fn.apply(this, args);
    }
    clearTimeout(timeout);
    timeout = setTimeout(() => (timeout = 0), delay);
  };
};

const setClipboard = data => {
  const text = window.btoa(unescape(encodeURIComponent(data)));
  BrowserAutomationStudio_SetClipboard(text, false, true);
};

const typeOf = (() => {
  const { toString } = Object.prototype;

  return value => {
    if (value === null) return 'null';
    if (value !== Object(value)) return typeof value;
    return toString.call(value).slice(8, -1).toLowerCase();
  };
})();

const hasOwn = (() => {
  const has = Object.prototype.hasOwnProperty;
  return (object, key) => object != null && has.call(object, key);
})();
