class DocumentsStore {
  /**
   * Create an instance of `DocumentsStore` class.
   * @constructor
   */
  constructor ({ actions, schema, video, tasks, wiki, dict, lang }) {
    this.actions = actions;
    this.schema = schema;
    this.tasks = tasks;
    this.video = video;
    this.wiki = wiki;
    this.dict = dict;
    this.lang = lang;
  }

  /**
   * Get an array of all action items.
   */
  getActionItems() {
    return this.actions
      .filter(([name]) => {
        if (this.schema && this.schema.length === 0) {
          return !name.includes('Database');
        }
        return true;
      })
      .map(([name, action]) => {
        const descriptions = this.getActionDescriptions(action.srcText);
        const variables = this.getActionVariables(action.srcHtml);

        const item = {
          popup: (!action.group && action.class && action.class === 'browser'),
          suggestions: this.getActionSuggestion(action),
          descriptions: descriptions.array,
          description: descriptions.short,
          name: tr(action.name),
          variables: variables,
          type: 'action',
          timestamps: [],
          timecodes: {},
          key: name,
        };

        if (item.popup) {
          item.description += tr(' This action works only with element inside browser.');
          item.module = tr('Browser > Element');
          item.icon = '../icons/element.png';
        } else {
          const group = this.getActionGroup(name);
          item.module = group.description;
          item.group = group.name;
          item.icon = group.icon;
        }

        return item;
      });
  }

  /**
   * Get action descriptions object using the selected action source.
   * @param {String} source - selected action source.
   */
  getActionDescriptions(source) {
    const getTextContent = (el) => {
      const data = $(el).html();
      const node = $('<div />');
      node.append(data);

      if (this.lang === 'en') node.find('.tr-ru').remove();
      if (this.lang === 'ru') node.find('.tr-en').remove();

      node.find('.tr').each(function () {
        $(this).html((_, html) => tr(html));
      });

      return $('<div />').append(tr(node.html())).text();
    };

    const array = _.map(
      $(source).find('[class*="tooltip-paragraph"]'),
      getTextContent
    );

    const short = _.map(
      $(source).find('[class*="short-description"]'),
      getTextContent
    );

    if (!array.length && !short.length) {
      return { short: [], array: [] };
    } else {
      return {
        short: short.length ? short[0] : array[0],
        array: array.length ? array : short
      };
    }
  }

  /**
   * Get action variables array using the selected action source.
   * @param {String} source - selected action source.
   */
  getActionVariables(source) {
    try {
      const results = _.uniq([...source.split(/<%=(.*?)%>/gs)
        .filter((src) => !src.includes('#path'))
        .filter((src) => !src.includes('#back'))
        .map((src) => {
          const variables = [...src.matchAll(/html\(\)\)\((.*)\)/gs)]
            .filter((match) => match[1].length)
            .map((match) => {
              const function_params = {}, model = {}, t = {}, c = {};
              const obj = eval('(' + match[1] + ')');

              if (!obj.default_selector) {
                if (obj.default_variable && obj.default_variable.length) {
                  return obj.default_variable;
                } else {
                  return '';
                }
              }

              return obj.description;
            });

          if (src.includes('#targetfields')) {
            variables.push(tr('Target tab number'));
            variables.push(tr('Target url'));
          }

          if (src.includes('#movefields')) {
            variables.push(tr('Deviation'));
            variables.push(tr('Gravity'));
            variables.push(tr('Speed'));
          }

          return variables;
        })].flat());

      return _.compact(results);
    } catch (e) {
      return [];
    }
  }

  /**
   * Get action suggestion array using the selected action object.
   * @param {Object} action - selected action object.
   */
  getActionSuggestion(action) {
    const source = action.suggestion;
    return source ? this.getSuggestion(source[this.lang]) : [];
  }

  /**
   * Get link suggestion array using the selected link object.
   * @param {Object} link - selected link object.
   */
  getLinkSuggestion(link) {
    const source = link.suggestion;
    return source ? this.getSuggestion(source) : [];
  }

  /**
   * Get suggestion array from the selected suggestion string.
   * @param {String} suggestion - selected suggestion string.
   */
  getSuggestion(suggestion) {
    return suggestion.split(',')
      .map((str) => str.trim())
      .filter((s) => s !== ' ')
      .filter((s) => s !== '');
  }

  /**
   * Get an array of all video items.
   */
  getVideoItems() { return this.getLinkItems(this.video, 'youtube'); }

  /**
   * Get an array of all wiki items.
   */
  getWikiItems() { return this.getLinkItems(this.wiki, 'wiki'); }

  /**
   * Get an array of all link items for the selected type.
   * @param {Object[]} items - selected items array.
   * @param {String} type - selected items type.
   * @private
   */
  getLinkItems(items, type) {
    return items
      .filter((item) => this.lang === item.lang)
      .map((item) => {
        return {
          timestamps: _.values(item.timestamps || {}),
          timecodes: _.invert(item.timestamps || {}),
          suggestions: this.getLinkSuggestion(item),
          descriptions: [item.description],
          description: item.description,
          icon: `../icons/${type}.png`,
          name: item.name,
          key: item.url,
          type: type
        };
      });
  }

  /**
   * Get action group object using the selected action name.
   * @param {String} action - selected action name.
   */
  getActionGroup(action) {
    const name = _.get(this.dict, action, 'browser');

    return _.find(this.tasks, {
      type: 'group',
      name: name
    });
  }
}
