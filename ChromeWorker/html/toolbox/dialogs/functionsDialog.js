class BasFunctionsDialog extends BasDialogsLib.BasModalDialog {
  /**
   * Create an instance of `BasFunctionsDialog` class.
   * @param {JQuery} element - target element object.
   * @constructor
   */
  constructor (element) {
    super({
      items: BasDialogsLib.utils.getDefaultCollection('function', element),
      options: BasDialogsLib.options.functionsOptions,
      recent: BasDialogsLib.store.functions,
      metadata: {
        findPredicate: BasDialogsLib.store.predicates.functions,
        template: BasDialogsLib.templates.functionsContent,
        pluralName: 'functions',
        singleName: 'function',
        color: 'dark'
      }
    });

    this.selector = element.attr('data-result-target');
    BasDialogsLib.utils.saveCursor(this.selector);
  }

  /**
   * Handler function for the `close` event.
   * @param {String} name - selected item name.
   */
  onClose(name) {
    if (name.length) {
      BasDialogsLib.store.addFunction({ name });
      $(this.selector).val(name);
    }
    _MainView.funcchange(name);
  }

  /**
   * Handler function for the `add` event.
   */
  onAdd() {
    $(BasDialogsLib.templates.createFunction())
      .on('hidden.bs.modal', function () {
        $(this).remove();
      })
      .css('z-index', '9999999')
      .modal('show');
  }
}