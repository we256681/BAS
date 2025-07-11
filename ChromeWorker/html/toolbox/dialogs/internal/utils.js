BasDialogsLib.utils = {
  /**
   * Get a collection of default items for the selected type.
   * @param {JQuery} $element - selected element.
   * @param {String} type - selected type.
   */
  getDefaultCollection(type, $element) {
    if (type === 'resource') return _ResourceCollection.toJSON();
    if (type === 'function') return _FunctionCollection.toJSON();

    return [
      ...($element.attr('disable_globals') !== 'true'
        ? _GlobalVariableCollection.toJSON()
        : []),
      ...($element.attr('disable_locals') !== 'true'
        ? _VariableCollection.toJSON()
        : []),
    ];
  },

  /**
   * Get a collection of recent items for the selected type.
   * @param {JQuery} $element - selected element.
   * @param {String} type - selected type.
   */
  getRecentCollection(type, $element) {
    const store = BasDialogsLib.store, base = this.getDefaultCollection(type, $element);

    if (type === 'resource')
      return store.resources.filter((r) =>
        base.some(store.predicates.resources(r))
      );

    if (type === 'function')
      return store.functions.filter((f) =>
        base.some(store.predicates.functions(f))
      );

    return store.variables.filter((v) =>
      base.some(store.predicates.variables(v))
    );
  },

  /**
   * Get the display name for the selected object using its type.
   * @param {Object} target - selected object.
   * @param {String} type - selected type.
   */
  getDisplayName(target, type) {
    if (type === 'resource') return target.name;
    if (type === 'function') return target.name;

    return target.global ? `GLOBAL:${target.name}` : target.name;
  },

  /**
   * Restore cursor position for input field using its selector.
   * @param {String} selector - input field selector.
   */
  restoreCursor(selector) {
    const $input = $(selector).trigger('focus');

    if ($input.is('textarea') || $input.is('input')) {
      const position = $input.data('position'), value = $input.val();

      $input[0].setSelectionRange(
        value.length - position,
        value.length - position
      );
    }
  },

  /**
   * Save cursor position for input field using its selector.
   * @param {String} selector - input field selector.
   */
  saveCursor(selector) {
    const $input = $(selector); let position = 0;

    if ($input.is('textarea')) {
      position = cursorPosition($input[0]).start;
    }

    if ($input.is('input')) {
      position = cursorPosition($input[0]).start;
    }

    $input.data('position', _.size($input.val()) - position);
  },

  /**
   * Check that selected html element is clickable.
   * @param {HTMLElement} element - html element.
   */
  isClickable(element) {
    return _.any([
      element.parentNode.dataset.clickable === 'true',
      element.dataset.clickable === 'true',
    ]);
  },

  /**
   * Show the list item header by toggling CSS classes.
   * @param {JQuery} $header - selected header element.
   */
  showHeader($header) {
    $header.addClass('modal-header-visible')
      .removeClass('modal-header-hidden');
  },

  /**
   * Hide the list item header by toggling CSS classes.
   * @param {JQuery} $header - selected header element.
   */
  hideHeader($header) {
    $header.addClass('modal-header-hidden')
      .removeClass('modal-header-visible');
  },

  /** 
   * Trim the selected string and convert to lowercase.
   */
  format(string) {
    return string.toLowerCase().trim();
  },

  /**
   * Get an array of all possible action objects.
   */
  getActions() {
    return Object.entries(_A).map(([name, action]) => {
      const popup = (!action.group && action.class && action.class === 'browser');

      try {
        const html = $(`#${name}`).html(), list = _([...html.split(/<%=(.*?)%>/gs)
          .filter((src) => !src.includes('#path'))
          .filter((src) => !src.includes('#back'))
          .map((src) => {
            return [...src.matchAll(/html\(\)\)\((.*)\)/gs)]
              .filter((match) => match[1].length)
              .map((match) => {
                const function_params = {}, model = {}, t = {}, c = {}, data = eval(`(${match[1]})`);

                if (!data.default_selector) {
                  if (data.default_variable && data.default_variable.length) {
                    return data.default_variable;
                  }

                  return '';
                }

                return '';
              });
          })])
          .flatten()
          .compact()
          .uniq()
          .run();

        return { description: tr(action.name), ref: name, popup, variables: list };
      } catch (e) {
        return { description: tr(action.name), ref: name, popup, variables: [] };
      }
    });
  },
};
