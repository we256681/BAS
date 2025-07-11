/**
 * This file contains code that creates safe copies of the original methods and properties for the classes and the global object.
 * Adding or modifying such copies is simplified as much as possible, including using the special `$bind` function declared below.
 * In order to add new data we need to decide what exactly will be copied - a static method, a prototype method, or an entire class:
 *
 * - If a replacement for static class methods is needed, they can be added as is, while retaining the full name, such as `JSON.parse`, `Math.abs`, and so on.
 * - The same applies to copies of the classes themselves - they can be specified with the original name. For the most part, this may be needed for instantiation or type checking.
 * - Separately, copies of methods from prototypes or a global object are added - in this case, you don't need to specify a name, it's generated automatically inside the `$bind` function.
 *
 * The `$bind` method creates a wrapper over the original functions from the specified object or prototype and stores it in a separate object.
 *
 * For regular objects we need to wrap the original method with a function that takes a context as the first argument.
 * For such copies, the name is automatically generated using the `$Prototype.method` pattern.
 * In case of a type mismatch, an exception will be thrown when calling such methods.
 *
 * Binding for properties of a `global` object is a special case, and doesn't require the context to be
 * passed - instead, it's bound ahead of time and stored. The name is generated in the same way as for
 * other methods, only without the dollar sign at the beginning.
 *
 * If several prototypes have the same function, it's necessary to create a wrapper with a
 * common name, inside which the required method will be selected depending on the context.
 *
 * For example, for `document.querySelector` and for `element.querySelector` we will add a
 * separate function called `$Node.querySelector`. Although there is no such method in
 * the `Node` prototype, both types inherit from it, so this name would make sense.
 *
 * Separately, it's worth adding methods that are always executed in a specific context.
 * For example, `document.evaluate` always requires a document context and there is no
 * point in adding a copy via `$bind` because it will require `this` as the first
 * argument - in such case, we will add a copy of `$Document.evaluate` manually.
 *
 * The `BAS_SAFE` macro is a shorthand for getting a safe copy from a special object:
 *
 * _BAS_HIDE(BrowserAutomationStudio_SafeData)["Date.now"] === _BAS_SAFE(Date.now);
 *
 * So, in the example above, the two values are identical. This approach works for
 * any types, including classes - the differences can only be in naming. As a
 * result, in order to replace the existing code with the use of safe copies,
 * we will need to take just a few simple steps:
 *
 * 1. If the replacement target is a global object or a static method, we can
 * simply add it to the right place and enclose it in the code with a macro:
 *
 *    // Before:
 *    let time = Date.now();
 *    let url = new URL('https://example.com');
 *
 *    // After:
 *    let time = _BAS_SAFE(Date.now)();
 *    let url = new _BAS_SAFE(URL)('https://example.com');
 *
 * 2. If the target is a global method or window property, we can simply add
 * it to the bindings in the right place and wrap it with a macro:
 *
 *    // Before:
 *    let int = parseInt('1');
 *    let width = window.innerWidth;
 *
 *    // After:
 *    let int = _BAS_SAFE(Window.parseInt)('1');
 *    let width = _BAS_SAFE(Window.innerWidth);
 *
 * 3. After all, if the target is a prototype method, then the approach is
 * almost the same as for a global object, only the context for the call
 * must always be passed as the first argument:
 *
 *    // Before:
 *    let str = [1, 2, 3].join('-');
 *    let body = document.body;
 *
 *    // After:
 *    let str = _BAS_SAFE($Array.join)([1, 2, 3], '-');
 *    let body = _BAS_SAFE($Document.body)(document);
 *
 * It is important to note that for prototypes, even copies of getters must
 * be called with a context, while for window properties, calling the
 * getter as a function is not necessary.
 */
(function (proxy = {}) {
  function $bind(target, keys = []) {
    const name = target === window ? 'Window' : target.name;

    for (const key of keys) {
      const property = Object.getOwnPropertyDescriptor(target === window ? target : target.prototype, key);

      for (const field of ['get', 'value']) {
        if (typeof property[field] === 'function') {
          if (target === window) {
            Object.defineProperty(proxy, `${name}.${key}`, { [field]: property[field].bind(target) });
          } else {
            const get = Function.prototype.call.bind(property[field]);
            const set = Function.prototype.call.bind(property['set']);
            proxy[`$${name}.${key}`] = function (context, ...args) {
              if (context == null) return context[key];
              if (property['set'] && args.length) {
                return set(context, ...args);
              }
              return get(context, ...args);
            };
          }
          break;
        }
      }
    }
  }

  const { $is } = (function () {
    const $call = Function.prototype.call;
    const $getPrototypeOf = Object.getPrototypeOf;
    const $toString = $call.bind(Object.prototype.toString);

    return {
      $is(value, proto) {
        if (value instanceof proxy[proto]) return true;

        for (let it = value; it != null; it = $getPrototypeOf(it)) {
          if ($toString(it) === `[object ${proto}]`) return true;
        }

        return false;
      },
    };
  })();

  // Add custom bindings:
  Object.assign(proxy, {
    ...$bind(URL, ['href']),
    ...$bind(Response, ['blob']),
    ...$bind(Number, ['toString']),
    ...$bind(Object, ['toString']),
    ...$bind(Symbol, ['toString']),
    ...$bind(Selection, ['toString']),
    ...$bind(RegExp, ['exec', 'test']),
    ...$bind(String, ['trim', 'split', 'match', 'substr', 'charAt', 'search', 'replace', 'indexOf', 'includes', 'substring', 'charCodeAt', 'codePointAt', 'lastIndexOf', 'toLowerCase', 'toUpperCase']),
    ...$bind(Array, ['pop', 'map', 'sort', 'push', 'join', 'shift', 'slice', 'splice', 'concat', 'unshift', 'indexOf', 'includes']),
    ...$bind(XPathResult, ['snapshotItem', 'snapshotLength']),
    ...$bind(NodeList, ['length']),
    ...$bind(HTMLElement, ['click']),
    ...$bind(DOMRectList, ['length']),
    ...$bind(NamedNodeMap, ['length']),
    ...$bind(DOMRectReadOnly, ['toJSON']),
    ...$bind(HTMLInputElement, ['value']),
    ...$bind(HTMLOptionElement, ['value']),
    ...$bind(HTMLSelectElement, ['value']),
    ...$bind(HTMLTextAreaElement, ['value']),
    ...$bind(HTMLAnchorElement, ['href', 'download']),
    ...$bind(DocumentFragment, ['children', 'childElementCount', 'querySelector', 'querySelectorAll']),
    ...$bind(ShadowRoot, ['innerHTML', 'elementFromPoint', 'elementsFromPoint']),
    ...$bind(Document, ['body', 'children', 'readyState', 'scrollingElement', 'documentElement', 'childElementCount', 'querySelector', 'querySelectorAll', 'elementFromPoint', 'elementsFromPoint']),
    ...$bind(Element, ['id', 'tagName', 'matches', 'children', 'attributes', 'outerHTML', 'innerHTML', 'className', 'localName', 'scrollTop', 'scrollLeft', 'shadowRoot', 'scrollHeight', 'clientHeight', 'childElementCount', 'removeAttribute', 'setAttribute', 'hasAttribute', 'getAttribute', 'querySelector', 'querySelectorAll', 'getClientRects', 'getBoundingClientRect', 'scrollIntoView', 'scrollIntoViewIfNeeded']),
    ...$bind(Node, ['baseURI', 'nodeType', 'nodeName', 'nodeValue', 'parentNode', 'childNodes', 'textContent', 'getRootNode', 'appendChild', 'removeChild', 'previousSibling']),
    ...$bind(window, ['getComputedStyle', 'getSelection', 'innerHeight', 'innerWidth', 'parseInt', 'escape', 'fetch', 'find']),
  });

  // Add static methods:
  Object.assign(proxy, {
    'String.fromCharCode': String.fromCharCode,
    'URL.createObjectURL': URL.createObjectURL,
    'URL.revokeObjectURL': URL.revokeObjectURL,
    'JSON.stringify': JSON.stringify,
    'Object.hasOwn': Object.hasOwn,
    'Math.random': Math.random,
    'JSON.parse': JSON.parse,
    'Math.round': Math.round,
    'Math.floor': Math.floor,
    'Math.ceil': Math.ceil,
    'Math.min': Math.min,
    'Math.max': Math.max,
    'Math.abs': Math.abs,
    'Date.now': Date.now,
  });

  // Add global objects:
  Object.assign(proxy, {
    HTMLTextAreaElement,
    HTMLSelectElement,
    HTMLOptionElement,
    HTMLInputElement,
    ShadowRoot,
    Document,
    Element,
    RegExp,
    Node,
    URL,
  });

  // Add extra wrappers:
  _BAS_HIDE(BrowserAutomationStudio_SafeData) = Object.assign(proxy, {
    ['$Node.querySelectorAll'](element, selector) {
      if ($is(element, 'Document')) {
        return proxy['$Document.querySelectorAll'](element, selector);
      } else if ($is(element, 'ShadowRoot')) {
        return proxy['$DocumentFragment.querySelectorAll'](element, selector);
      }
      return proxy['$Element.querySelectorAll'](element, selector);
    },

    ['$Node.querySelector'](element, selector) {
      if ($is(element, 'Document')) {
        return proxy['$Document.querySelector'](element, selector);
      } else if ($is(element, 'ShadowRoot')) {
        return proxy['$DocumentFragment.querySelector'](element, selector);
      }
      return proxy['$Element.querySelector'](element, selector);
    },

    ['$Node.elementsFromPoint'](target, x, y) {
      if ($is(target, 'ShadowRoot')) {
        return proxy['$ShadowRoot.elementsFromPoint'](target, x, y);
      }
      return proxy['$Document.elementsFromPoint'](target, x, y);
    },

    ['$Node.elementFromPoint'](target, x, y) {
      if ($is(target, 'ShadowRoot')) {
        return proxy['$ShadowRoot.elementFromPoint'](target, x, y);
      }
      return proxy['$Document.elementFromPoint'](target, x, y);
    },

    ['$Node.childElementCount'](target) {
      if ($is(target, 'Document')) {
        return proxy['$Document.childElementCount'](target);
      } else if ($is(target, 'ShadowRoot')) {
        return proxy['$DocumentFragment.childElementCount'](target);
      }
      return proxy['$Element.childElementCount'](target);
    },

    ['$Node.children'](target) {
      if ($is(target, 'Document')) {
        return proxy['$Document.children'](target);
      } else if ($is(target, 'ShadowRoot')) {
        return proxy['$DocumentFragment.children'](target);
      }
      return proxy['$Element.children'](target);
    },

    ['$Node.value'](target) {
      if ($is(target, 'HTMLTextAreaElement')) {
        return proxy['$HTMLTextAreaElement.value'](target);
      } else if ($is(target, 'HTMLSelectElement')) {
        return proxy['$HTMLSelectElement.value'](target);
      } else if ($is(target, 'HTMLOptionElement')) {
        return proxy['$HTMLOptionElement.value'](target);
      }
      return proxy['$HTMLInputElement.value'](target);
    },

    ['$Node.tagName'](target) {
      if (target == null) return target['tagName'];
      if ($is(target, 'Element')) {
        return proxy['$Element.tagName'](target);
      }
    },

    ['$Node.localName'](target) {
      if (target == null) return target['localName'];
      if ($is(target, 'Element')) {
        return proxy['$Element.localName'](target);
      }
    },

    ['$Node.shadowRoot'](target) {
      if (target == null) return target['shadowRoot'];
      if ($is(target, 'Element')) {
        return proxy['$Element.shadowRoot'](target);
      }
    },

    ['$Node.innerHTML'](target, ...args) {
      if (target == null) return target['innerHTML'];
      if ($is(target, 'Element')) {
        return proxy['$Element.innerHTML'](target, ...args);
      } else if ($is(target, 'ShadowRoot')) {
        return proxy['$ShadowRoot.innerHTML'](target, ...args);
      }
    },

    ['$Node.outerHTML'](target, ...args) {
      if (target == null) return target['outerHTML'];
      if ($is(target, 'Element')) {
        return proxy['$Element.outerHTML'](target, ...args);
      }
    },

    ['$Node.setAttribute'](target, ...args) {
      if (target == null) return target['setAttribute'];
      if ($is(target, 'Element')) {
        return proxy['$Element.setAttribute'](target, ...args);
      }
    },

    ['$Node.hasAttribute'](target, ...args) {
      if (target == null) return target['hasAttribute'];
      if ($is(target, 'Element')) {
        return proxy['$Element.hasAttribute'](target, ...args);
      }
    },

    ['$Node.getAttribute'](target, ...args) {
      if (target == null) return target['getAttribute'];
      if ($is(target, 'Element')) {
        return proxy['$Element.getAttribute'](target, ...args);
      }
    },

    ['$Node.removeAttribute'](target, ...args) {
      if (target == null) return target['removeAttribute'];
      if ($is(target, 'Element')) {
        return proxy['$Element.removeAttribute'](target, ...args);
      }
    },

    ['$Node.nodeName']: (function ($rawFunction) {
      return function (target) {
        if (target == null) return target['nodeName'];
        if ($is(target, 'Node')) {
          return $rawFunction(target);
        }
      };
    })(proxy['$Node.nodeName']),

    ['$Node.nodeType']: (function ($rawFunction) {
      return function (target) {
        if (target == null) return target['nodeType'];
        if ($is(target, 'Node')) {
          return $rawFunction(target);
        }
      };
    })(proxy['$Node.nodeType']),

    ['$Node.getRootNode']: (function ($rawFunction) {
      return function (target) {
        if (target == null) return target['getRootNode'];
        if ($is(target, 'Node')) {
          return $rawFunction(target);
        }
      };
    })(proxy['$Node.getRootNode']),

    ['$Document.evaluate']: Document.prototype.evaluate.bind(document),

    ['$Document.createElement']: Document.prototype.createElement.bind(document),
  });
})();
