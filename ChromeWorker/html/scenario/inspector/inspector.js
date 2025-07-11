'use strict';

(({ App, Backbone, _ }) => {
  const { Inspector } = App;

  Inspector.View = Backbone.View.extend({
    initialize() {
      let attached = false;

      window.addEventListener('message', ({ data: { type, payload } }) => {
        // prettier-ignore
        switch (type) {
          case 'focusAction': return BrowserAutomationStudio_FocusAction(payload.id);
          case 'confirm': return confirm(payload);
          case 'prompt': return prompt(payload);
          case 'edit': return edit(payload);
          case 'hide': return this.hide();
          case 'show': return this.show();
          case 'get': return get(payload);
          default: {
            if (type === 'destroyed') {
              attached = false;
            }
            if (type === 'mounted') {
              attached = true;
            }
            this.trigger(type, payload);
          }
        }
      });

      const confirm = ({ message }) => {
        bootbox.confirm(message, result => {
          this.send({ type: 'confirm', payload: { result } });
        });
      };

      const prompt = ({ message }) => {
        bootbox.prompt(message, result => {
          this.send({ type: 'prompt', payload: { result } });
        });
      };

      const get = ({ path }) => {
        request({ path }).then(value => {
          this.send({ type: 'get', payload: { value } });
        });
      };

      this.send = async message => {
        if (!attached) {
          await new Promise(resolve => this.once('mounted', resolve));
        }
        this.$('iframe')[0].contentWindow.postMessage(message, '*');
      };
    },

    render() {
      if (!this.el.isConnected) {
        const { body } = document;

        this.setElement('#inspector').$el.resizable({
          onDragStart(e, ...args) {
            body.style.cursor = e.type === 'mouseup' ? '' : 'ns-resize';
            BrowserAutomationStudio_PreserveInterfaceState();
          },
          onDragEnd(e, ...args) {
            body.style.cursor = e.type === 'mouseup' ? '' : 'ns-resize';
            BrowserAutomationStudio_PreserveInterfaceState();
          },
          onDrag(e, ...args) {
            const height = Math.min(args[2], window.innerHeight - 300);
            return args[0].css('height', Math.max(110, height)), false;
          },
          handleSelector: '.handle',
        });

        new ResizeObserver(() => this.calc()).observe(this.el);
        BrowserAutomationStudio_AskForVariablesUpdate();
      }
      return this;
    },

    wait() {
      clearTimeout(this.timeout);

      this.timeout = setTimeout(() => {
        if (arguments[0]) {
          this.$('.loader').show();
        } else {
          this.$('.loader').hide();
        }
      }, 150);
    },

    show() {
      this.$el.show();
      BrowserAutomationStudio_PreserveInterfaceState();
      return this;
    },

    hide() {
      this.$el.hide();
      BrowserAutomationStudio_PreserveInterfaceState();
      return this;
    },

    calc() {
      const height = this.$el.is(':visible') && this.$el.height();
      $('.main').css('padding-bottom', `${height + 50 + 0}px`);
    },
  });

  function edit(options) {
    if (document.querySelector('.modal.in')) return;

    Promise.resolve().then(() => options.mode === 'variables' ? request(options) : options.value).then(value => {
      const callback = (accept, { changed, value, type }) => {
        if (accept && changed) {
          let [root, ...path] = options.path;
          path = path.map(k => `[${JSON.stringify(k)}]`).join('');

          _.attempt(() => {
            if (type === 'date') {
              value = `_parse_date("${value}", "auto")`;
            } else if (type === 'script') {
              value = JSON.stringify(eval(`(${value})`));
            } else if (type === 'string') {
              value = JSON.stringify(value);
            }

            VariablesUpdateNeeded = true;
            BrowserAutomationStudio_Execute(`
              (function (root, value) {
                try {
                  if (root.indexOf("GLOBAL:") === 0) {
                    root = root.slice(7);
                    var obj = JSON.parse(P("basglobal", root) || "{}");
                    obj${path} = value;
                    PSet("basglobal", root, JSON.stringify(obj));
                  } else {
                    GLOBAL["VAR_" + root]${path} = value;
                  }
                } catch (e) {}
              })(${JSON.stringify(root)}, ${value});
              section_start("test", -3)!
            `);
          });
        }
      };

      return new Inspector.Modal({ ...options, callback, value }).render();
    });
  }

  function request({ path }) {
    return new Promise(resolve => {
      const pointer = JSON.stringify(path);
      App.once('variablesRequest', value => resolve(value[pointer]));
      BrowserAutomationStudio_Execute(`request_variables(${JSON.stringify(pointer)})!\nsection_start("test", -3)!`);
    });
  }
})(window);
