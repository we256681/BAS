'use strict';

(({ App, Backbone, $, _ }) => {
  const { Inspector } = App;

  const Model = Backbone.Model.extend({
    defaults: () => ({
      value: '',
      type: '',
      name: '',
      mode: '',
      path: [],
    }),

    initialize({ value, type, path }) {
      this.isChanged = () => _.any([
        value !== this.get('value'),
        type !== this.get('type'),
      ]);
      this.set('name', path[0]);
    },
  });

  Inspector.Modal = Backbone.View.extend({
    className: 'modal modal-centered',

    attributes: { tabIndex: -1 },

    events: {
      'click #inspectorModalSearchVariable'() {
        const name = this.model.get('name');
        const mode = this.model.get('mode');

        $('#findinput').val(mode === 'resource' ? `{{${name}}}` : name);
        _ActionFinder.FindNext(true);
        _ActionFinder.Show();
        this.cancel();
      },

      'click #inspectorModalClearData'() {
        const type = this.model.get('type');

        for (const el of this.$('form')[0].elements) {
          if (el.type !== 'radio' && el.type !== 'hidden' && el.closest('[data-type]').dataset.type === type) {
            $(el).val(el.type === 'number' ? 0 : '').trigger('input');
          }
        }
      },

      'click [data-copy-target]'(e) {
        let val = this.model.get(e.currentTarget.dataset.copyTarget);

        if (Array.isArray(val)) {
          val = val.reduce((v, k) => v + `[${JSON.stringify(k)}]`);
        }

        BrowserAutomationStudio_SetClipboard(utf8_to_b64(val), false, true);
      },

      'click .btn-cancel': 'cancel',

      'click .btn-accept': 'accept',

      'click .btn-close': 'close',

      'input :input[required]'(e) {
        if (e.target.type === 'radio' && !e.target.checked) return;
        const el = e.target, model = this.model.set('value', el.value);

        if (model.get('type') === 'string' || el.checkValidity()) {
          this.$('.btn-accept').prop('disabled', false);
          this.$('form').removeClass('invalid');
        } else {
          this.$('.btn-accept').prop('disabled', true);
          this.$('form').addClass('invalid');
        }

        this.$('.error').html($t(el.validationMessage));
      },

      'changed.bs.select'(e) {
        this.model.set('type', e.target.value);
      },

      'hidden.bs.select'(e) {
        e.target.nextElementSibling.blur();
      },

      'loaded.bs.select'(e) {
        e.target.nextElementSibling.blur();
      },

      'keydown'(e) {
        if (e.target.tagName !== 'TEXTAREA') {
          if (e.key === 'Escape') {
            this.$('.btn-cancel').click();
          }
          if (e.key === 'Enter') {
            this.$('.btn-accept').click();
          }
        }
        e.stopPropagation();
      },
    },

    initialize({ value, type, path, mode }) {
      if (type === 'date') value = isNaN(value) ? '' : value.toISOString().slice(0, 19);
      [type, value] = ['object', 'array'].includes(type) ? ['script', JSON.stringify(value, null, 4)] : [type, `${value}`];

      this.model = new Model({ value, type, path, mode: mode.slice(0, -1) }).on('change:type', (_, type) => {
        for (const el of this.$('form').trigger('reset')[0].elements) {
          const field = el.closest('[data-type]');
          el.required = field.dataset.type === type;
          if (el.required) $(el).trigger('input');
          $(field).toggle(el.required);
        }

        const description = $t(`inspector.descriptions.${type}`);
        this.$('#inspectorModalDescription').html(description);
      });
    },

    render() {
      if (this.$el.is(':empty')) {
        const html = this.template({
          types: ['script', 'undefined', 'boolean', 'string', 'number', 'date', 'null'],
          ...this.model.toJSON(),
        });
        this.$el.html(html).modal({ backdrop: 'static' }).find('select').selectpicker();
      }
      return this;
    },

    cancel() {
      return this.close(false);
    },

    accept() {
      return this.close(true);
    },

    close() {
      this.$('select').selectpicker('destroy');
      this.options.callback(arguments[0], {
        changed: this.model.isChanged(),
        ...this.model.toJSON(),
      });
      this.$el.modal('hide');
      return this.remove();
    },

    template: _.template(/*html*/ `
      <div class="modal-dialog" role="document" style="flex: 1;">
        <div class="inspector-modal-content">
          <div class="inspector-modal-header">
            <h4><%= $t('inspector.header.' + mode, { name }) %></h4>
            <h6><%= $t('inspector.subheader.' + mode) %></h6>
            <button type="button" class="btn-close" data-dismiss="modal" aria-label="Close">
              <svg width="12" height="12" viewBox="0 0 12 12" xmlns="http://www.w3.org/2000/svg">
                <path d="M12 1.05L10.95 0L6 4.95L1.05 0L0 1.05L4.95 6L0 10.95L1.05 12L6 7.05L10.95 12L12 10.95L7.05 6L12 1.05Z" fill="#606060" />
              </svg>
            </button>
          </div>
          <div class="inspector-modal-body">
            <select data-style="inspector-modal-select" <%= mode === 'resource' ? 'disabled' : '' %>>
              <% types.forEach(item => { %>
                <option value="<%= item %>" <%= item === type ? 'selected' : '' %>><%= $t('inspector.types.' + item) %></option>
              <% }) %>
            </select>
            <form class="inspector-modal-form" spellcheck="false" autocomplete="off" autocapitalize="off" novalidate>
              <% types.forEach(item => { %>
                <% const match = item === type, modifier = mode === 'resource' ? 'readonly' : match ? 'required' : '' %>
                <div data-type="<%= item %>" style="display: <%= match ? 'flex' : 'none' %>;">
                  <% if (item === 'boolean') { %>
                    <% ['true', 'false'].forEach((val, idx) => { %>
                      <div class="pretty p-default p-round" style="margin: 9px 12px;">
                        <input type="radio" name="boolean" value="<%- val %>" <%= (match ? value === val : idx === 0) ? 'checked' : '' %> <%= modifier %>>
                        <div class="state">
                          <label><%= $t('inspector.' + val) %></label>
                        </div>
                      </div>
                    <% }) %>
                  <% } else if (item === 'script') { %>
                    <textarea style="height: 40vh;" <%= modifier %>><%- match ? value : '' %></textarea>
                  <% } else if (item === 'string') { %>
                    <textarea style="height: 40vh;" <%= modifier %>><%- match ? value : '' %></textarea>
                  <% } else if (item === 'number') { %>
                    <input type="number" value="<%- match ? value : 0 %>" step="any" <%= modifier %>>
                  <% } else if (item === 'date' ) { %>
                    <input type="datetime-local" value="<%- match ? value : '' %>" step="1" <%= modifier %>>
                  <% } else { %>
                    <input type="hidden" value="<%- item %>" <%= modifier %>>
                  <% } %>
                </div>
              <% }) %>
              <span class="error"></span>
            </form>
            <div class="inspector-modal-tools dropdown" style="display: flex;">
              <button type="button" data-toggle="dropdown">
                <svg width="16" height="16" viewBox="0 0 16 16" xmlns="http://www.w3.org/2000/svg">
                  <path d="M0 3h16M0 8h16M0 13h16" stroke="#606060" stroke-linecap="square" />
                </svg>
                <svg width="16" height="16" viewBox="0 0 16 16" xmlns="http://www.w3.org/2000/svg">
                  <path d="M12.6065 2.70703 2.70703 12.6065m0-9.89947 9.89947 9.89947" stroke="#fff" stroke-linecap="square" />
                </svg>
              </button>
              <button type="button" data-copy-target="value" style="flex: 1; border-left: 0;">
                <svg width="16" height="16" viewBox="0 0 16 16" xmlns="http://www.w3.org/2000/svg">
                  <path d="M12 3V0H2v12h4v3h9V3h-3Zm-6 8H3V1h8v2H6v8Zm8 3H7V4h7v10Z" fill="#606060" />
                </svg>
                <span style="margin-left: 12px;"><%= $t('inspector.copyData') %></span>
              </button>
              <ul class="dropdown-menu">
                <li>
                  <a href="#/" id="inspectorModalSearchVariable">
                    <svg width="24" height="16" viewBox="0 0 16 16" xmlns="http://www.w3.org/2000/svg">
                      <path d="m15.7164 15.111-4.2359-3.9328c2.1796-2.63853 1.8355-6.65365-.8031-8.83329C8.03894.165276 4.02382.509429 1.84418 3.14794-.335456 5.78644.00869703 9.80156 2.6472 11.9812c2.29436 1.9502 5.73589 1.9502 8.0302 0l4.2359 3.9329.8031-.8031ZM1.50003 7.16306c0-2.86795 2.29435-5.1623 5.16229-5.1623 2.86795 0 5.16228 2.29435 5.16228 5.1623 0 2.86794-2.29433 5.16234-5.16228 5.16234-2.86794 0-5.16229-2.2944-5.16229-5.16234Z" fill="#fff" />
                    </svg>
                    <%= $t('inspector.search.' + mode) %>
                  </a>
                </li>
                <% if (path.length > 1) { %>
                  <li>
                    <a href="#/" data-copy-target="path">
                      <svg width="24" height="16" viewBox="0 0 16 16" xmlns="http://www.w3.org/2000/svg">
                        <path d="M12 3V0H2v12h4v3h9V3h-3Zm-6 8H3V1h8v2H6v8Zm8 3H7V4h7v10Z" fill="#fff" />
                      </svg>
                      <%= $t('inspector.copyPath.' + mode) %>
                    </a>
                  </li>
                <% } %>
                <% if (path.length > 0) { %>
                  <li>
                    <a href="#/" data-copy-target="name">
                      <svg width="24" height="16" viewBox="0 0 24 16" xmlns="http://www.w3.org/2000/svg">
                        <path d="M3 1v1h3v6h1V2h3V1H3ZM12 6v1h4v9h1V7h4V6h-9Z" fill="#fff" />
                      </svg>
                      <%= $t('inspector.copyName.' + mode) %>
                    </a>
                  </li>
                <% } %>
                <% if (mode === 'variable') { %>
                  <li>
                    <a href="#/" id="inspectorModalClearData">
                      <svg width="24" height="16" viewBox="0 0 16 16" xmlns="http://www.w3.org/2000/svg">
                        <path d="M3.4834 14.498h11.5v1h-11.5v-1ZM13.6733 5.25329l-3.96501-3.96c-.09288-.09298-.20316-.16674-.32456-.21706C9.26233 1.0259 9.1322 1 9.00079 1c-.13142 0-.26155.0259-.38294.07623-.1214.05032-.23169.12408-.32456.21706l-7 7c-.09298.09287-.16674.20316-.21706.32456C1.0259 8.73924 1 8.86937 1 9.00079c0 .13141.0259.26154.07623.38294.05032.1214.12408.23168.21706.32456l2.255 2.29001h4.795l5.33001-5.33001c.093-.09288.1667-.20316.217-.32456.0504-.1214.0763-.25153.0763-.38294 0-.13142-.0259-.26155-.0763-.38294-.0503-.1214-.124-.23169-.217-.32456ZM7.92829 10.9983h-3.945l-2-2.00001 3.155-3.155 3.965 3.96-1.175 1.19501Zm1.88-1.88001-3.96-3.965 3.135-3.155 4.00001 3.965-3.17501 3.155Z" fill="#fff" />
                      </svg>
                      <%= $t('inspector.clearData') %>
                    </a>
                  </li>
                <% } %>
              </ul>
            </div>
            <div class="inspector-modal-description">
              <svg width="16" height="16" viewBox="0 0 16 16" xmlns="http://www.w3.org/2000/svg">
                <path d="M8 0C3.58172 0 0 3.58172 0 8c0 4.4183 3.58172 8 8 8 4.4183 0 8-3.5817 8-8 0-4.41828-3.5817-8-8-8Z" fill="#606060" />
                <path d="m8.26 10.168.336-4.508V3H7.112v2.66l.294 4.508h.854Zm.434 2.8v-1.666H7v1.666h1.694Z" fill="#fff" />
              </svg>
              <div id="inspectorModalDescription" style="margin-left: 14px; flex: 1;"><%= $t('inspector.descriptions.' + type) %></div>
            </div>
          </div>
          <div class="inspector-modal-footer">
            <% if (mode === 'variable') { %>
              <button type="button" class="btn-base btn-accept"><%= $t('inspector.save') %></button>
            <% } %>
            <button type="button" class="btn-base btn-cancel"><%= $t('Cancel') %></button>
          </div>
        </div>
      </div>
    `),
  });

  _.extend(_L, {
    'inspector.subheader.variable': { en: 'Edit variable type and value', ru: 'Изменить тип и значение переменной' },
    'inspector.subheader.resource': { en: 'View resource type and value', ru: 'Просмотр типа и значения ресурса' },
    'inspector.header.variable': { en: 'Edit the "{name}" variable', ru: 'Изменить переменную "{name}"' },
    'inspector.header.resource': { en: 'View the "{name}" resource', ru: 'Просмотр ресурса "{name}"' },
    'inspector.search.variable': { en: 'Search for variable in project', ru: 'Поиск переменной в проекте' },
    'inspector.search.resource': { en: 'Search for resource in project', ru: 'Поиск ресурса в проекте' },
    'inspector.copyPath.variable': { en: 'Copy the variable path', ru: 'Копировать путь переменной' },
    'inspector.copyPath.resource': { en: 'Copy the resource path', ru: 'Копировать путь ресурса' },
    'inspector.copyName.variable': { en: 'Copy the variable name', ru: 'Копировать имя переменной' },
    'inspector.copyName.resource': { en: 'Copy the resource name', ru: 'Копировать имя ресурса' },
    'inspector.copyData': { en: 'Copy to clipboard', ru: 'Копировать в буфер обмена' },
    'inspector.clearData': { en: 'Clear data', ru: 'Очистить данные' },
    'inspector.save': { en: 'Save changes', ru: 'Сохранить изменения' },
    'inspector.descriptions.undefined': {
      en: '<b>Undefined</b> is a primitive type that is a special value. Variables that have no value assigned are of type <b>undefined</b>.',
      ru: '<b>Undefined</b> - примитивный тип, являющийся специальным значением. Переменные, не имеющие присвоенного значения, обладают типом <b>undefined</b>.',
    },
    'inspector.descriptions.boolean': {
      en: 'Logical type accepting only one of two values - <b>true</b> or <b>false</b>.',
      ru: 'Логический тип, принимающий только одно из двух значений - <b>true</b> (истина) или <b>false</b> (ложь).',
    },
    'inspector.descriptions.script': {
      en: 'The value of this type is treated as arbitrary <b>JavaScript</b> code. You can use it to specify data in <b>raw</b> form. Arrays and objects are set and displayed in the same way as in the code.',
      ru: 'Значение данного типа рассматривается как произвольный <b>JavaScript</b> код. С помощью него можно задать данные в <b>сыром</b> виде. Массивы и объекты задаются и отображаются так же, как и в коде.',
    },
    'inspector.descriptions.string': {
      en: '<b>String</b> is one of the primitive types that represent any text data.',
      ru: '<b>Строка</b> - один из примитивных типов, представляющий собой любые текстовые данные.',
    },
    'inspector.descriptions.number': {
      en: '<b>Number</b> is one of the primitive types. It can represent both integer values and values with a fractional part.',
      ru: '<b>Число</b> - один из примитивных типов. Может представлять собой как целые значения, так и значения с дробной частью.',
    },
    'inspector.descriptions.date': {
      en: '<b>Date</b> is a data type that represents a specific moment in time in a platform-independent format.',
      ru: '<b>Date</b> - тип данных, который представляет определенный момент времени в независимом от платформы формате.',
    },
    'inspector.descriptions.null': {
      en: '<b>Null</b> is a primitive type. Unlike <b>undefined</b>, which denotes uncertainty, it is the definite meaning of the absence of an object.',
      ru: '<b>Null</b> - примитивный тип. В отличие от <b>undefined</b>, обозначающего неопределенность, является определённым значением отсутствия объекта.',
    },
    'inspector.types.undefined': { en: 'Undefined', ru: 'Undefined' },
    'inspector.types.boolean': { en: 'Boolean', ru: 'Булево' },
    'inspector.types.script': { en: 'Expression', ru: 'Выражение' },
    'inspector.types.string': { en: 'String', ru: 'Строка' },
    'inspector.types.number': { en: 'Number', ru: 'Число' },
    'inspector.types.date': { en: 'Date', ru: 'Дата' },
    'inspector.types.null': { en: 'Null', ru: 'Null' },
    'inspector.false': { en: 'False', ru: 'False' },
    'inspector.true': { en: 'True', ru: 'True' },
    'Please enter a valid value. The field is incomplete or has an invalid date.': { ru: 'Введите верное значение. Поле не заполнено или введена недействительная дата.' },
    'Please fill out this field.': { ru: 'Заполните это поле.' },
    'Please enter a number.': { ru: 'Введите число.' },
  });
})(window);
