class BasResourcesDialog extends BasDialogsLib.BasModalDialog {
  /**
   * Create an instance of `BasResourcesDialog` class.
   * @param {JQuery} element - target element object.
   * @constructor
   */
  constructor (element) {
    super({
      items: BasDialogsLib.utils.getDefaultCollection('resource', element),
      options: BasDialogsLib.options.resourcesOptions,
      recent: BasDialogsLib.store.resources,
      metadata: {
        findPredicate: BasDialogsLib.store.predicates.resources,
        template: BasDialogsLib.templates.resourcesContent,
        pluralName: 'resources',
        singleName: 'resource',
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
  onClose(name, { options }) {
    let resource = name;

    if (options.resourceDontDie) {
      resource += '|onlyfail';
    }

    if (!options.resourceReuse) {
      resource += '|notreuse';
    }

    if (name.length) {
      BasDialogsLib.store.addResource({ name });
      BasDialogsLib.insertHelper.insertResource(this.selector, resource, `{{${resource}}}`);
    }
  }

  /**
   * Handler function for the `add` event.
   */
  onAdd() {
    $(BasDialogsLib.templates.createResource())
      .on('hidden.bs.modal', function () {
        $(this).remove();
      })
      .css('z-index', '9999999')
      .modal('show');
  }
}