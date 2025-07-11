class BasVariablesDialog extends BasDialogsLib.BasModalDialog {
  /**
   * Create an instance of `BasVariablesDialog` class.
   * @param {JQuery} element - target element object.
   * @constructor
   */
  constructor (element) {
    const actions = BasDialogsLib.utils.getActions();

    super({
      items: BasDialogsLib.utils.getDefaultCollection('variable', element).map((item) => {
        const action = actions.find(({ variables }) => variables.includes(item.name));

        if (!action) {
          let description;

          if (item.name === 'FOREACH_DATA') {
            description = tr('Current list element in foreach loop');
          }

          if (item.name === 'CYCLE_INDEX') {
            description = tr('Current loop repetition');
          }

          return { description, ...item };
        }

        return {
          description: action.description,
          popup: action.popup,
          name: action.name,
          ref: action.ref,
          ...item
        };
      }),
      options: BasDialogsLib.options.variablesOptions,
      recent: BasDialogsLib.store.variables,
      metadata: {
        findPredicate: BasDialogsLib.store.predicates.variables,
        template: BasDialogsLib.templates.variablesContent,
        pluralName: 'variables',
        singleName: 'variable',
        color: 'green'
      }
    });

    this.useGlobals = element.attr('disable_globals') !== 'true';
    this.useLocals = element.attr('disable_locals') !== 'true';
    this.selector = element.attr('data-result-target');
    BasDialogsLib.utils.saveCursor(this.selector);
  }

  /**
   * Handler function for the `close` event.
   * @param {String} name - selected item name.
   */
  onClose(name, { global }) {
    if (name.length) {
      BasDialogsLib.store.addVariable({ name, global }, global);
      BasDialogsLib.insertHelper.insertVariable(this.selector, name, `[[${global ? 'GLOBAL:' : ''}${name}]]`);
    }
  }

  /**
   * Handler function for the `add` event.
   */
  onAdd() {
    if (this.useLocals) {
      BrowserAutomationStudio_OpenAction('setvariable');
    } else {
      BrowserAutomationStudio_OpenAction('globalset');
    }

    this.closeDialog();
  }
}