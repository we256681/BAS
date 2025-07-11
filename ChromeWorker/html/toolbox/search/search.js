class SearchManager {
  /**
   * Create an instance of `SearchManager` class.
   * @constructor
   */
  constructor () {
    const store = new DocumentsStore({
      actions: Object.entries(_A).map(([name, action]) => {
        const source = $(`#${name}`);
        return [name, {
          srcText: source.text(),
          srcHtml: source.html(),
          ...action
        }];
      }),
      tasks: _TaskCollection.toJSON(),
      schema: _Schema,
      video: _VIDEO,
      wiki: _WIKI,
      dict: _A2G,
      lang: _K
    });

    this.worker = SearchLib.Worker.create({
      onmessage: ({ data }) => {
        if (data.type === 'search') {
          this.$recentHeader.hide();
          this.$emptyHeader.hide();
          this.render(data.results);
        }

        if (data.type === 'recent') {
          this.$recentHeader.show();
          this.$emptyHeader.hide();
          this.render(data.results);
        }
      }
    });

    $(window).load(() => _.defer(() => {
      this.worker.postMessage({
        type: 'initialize',
        documents: [
          ...store.getActionItems(),
          ...store.getVideoItems(),
          ...store.getWikiItems()
        ]
      });
    }));

    this.registerHandlers();
    this.pagesCount = 1;
    this.pageIndex = 0;
    this.query = null;
  }

  /**
   * Register all event handlers associated with the search page.
   */
  registerHandlers() {
    const self = this;

    $(document).on({
      mouseover(e) {
        if (!self.isModuleTarget(e)) $(this).css('border-color', '#7699af');
      },
      mouseout(e) {
        if (!self.isModuleTarget(e)) $(this).css('border-color', '#f2f5f7');
      },
      click(e) {
        const data = $(this).data();
        const group = data.group;
        const popup = data.popup;
        const key = data.key;
        e.stopPropagation();

        if (self.isModuleTarget(e) && group) {
          BrowserAutomationStudio_GotoGroup(group);
          return self.hide();
        }

        if (data.type === 'youtube' || data.type === 'wiki') {
          if (data.timecode) {
            BrowserAutomationStudio_OpenUrl(key + data.timecode);
          } else {
            BrowserAutomationStudio_OpenUrl(key);
          }
        } else if (!popup) {
          BrowserAutomationStudio_OpenAction(key);
        } else if (popup) {
          BrowserAutomationStudio_Notify('search', data.name);
        }
      }
    }, '.result-item');

    $(document).on('click', '.results-recent', (e) => {
      e.stopPropagation();
    });

    $(document).on('click', '.results-empty', (e) => {
      e.stopPropagation();
    });

    $(document).on('click', '#nextpage', (e) => {
      e.preventDefault();
      this.showPage(++this.pageIndex);
    });

    $(document).on('click', '#prevpage', (e) => {
      e.preventDefault();
      this.showPage(--this.pageIndex);
    });

    $(document).on('click', '#results', (e) => {
      e.preventDefault();
      this.hide();
    });

    $(document).on('click', '.search', (e) => {
      e.preventDefault();
      this.hide();
    });

    $(document).keydown((e) => {
      if (this.$actions.is(':visible') && e.key.length === 1) {
        this.show();
      }
    });

    $(window).resize(() => {
      if (!this.$search.length || this.$search.is(':hidden')) return;

      if (this.query) {
        this.search(this.query);
      } else {
        this.recent();
      }
    });
  }

  /**
   * Check that the event target contains a class that belongs to the action module.
   * @param {{target: HTMLInputElement}} target - selected event target.
   */
  isModuleTarget({ target }) {
    return [
      target.parentNode.className,
      target.className
    ].includes('item-module');
  }

  /**
   * Check if all results are visible on the search page.
   * @readonly
   */
  get resultsVisible() {
    const bounding = this.$results.get(0).getBoundingClientRect();
    const docElement = document.documentElement;
    const docHeight = docElement.clientHeight;
    const docWidth = docElement.clientWidth;

    return _.every([
      bounding.bottom <= (window.innerHeight || docHeight) * (100 / _Z),
      bounding.right <= (window.innerWidth || docWidth) * (100 / _Z),
      bounding.left >= 0,
      bounding.top >= 0
    ]);
  }

  /**
   * Perform an action search using the selected query.
   * @param {String} query - selected query string.
   */
  search(query) {
    this.$search.addClass('disabled').css('filter', 'grayscale(100%)');
    this.query = query;
    this.worker.postMessage({ type: 'search', query: query });
  }

  /**
   * Perform an action search using the action history.
   */
  recent() {
    this.$search.addClass('disabled').css('filter', 'grayscale(100%)');
    this.query = null;
    this.worker.postMessage({ type: 'recent', query: null });
  }

  /**
   * Render the search results using selected items array.
   * @param {Object[]} items - selected items array.
   */
  render(items) {
    this.$search.removeClass('disabled').css('filter', 'grayscale(0%)');
    this.$results.empty();
    this.pagesCount = 1;
    this.pageIndex = 0;
    const results = [];

    items.slice(0, 100).forEach((item, index) => {
      const prev = this.pagesCount - 1;
      const next = this.pagesCount - 0;
      results.push(this.renderItem({
        page: prev,
        index,
        item
      }));

      if (!this.resultsVisible) {
        _.initial(results).forEach(result => result.hide());

        if (index === 0) {
          _.last(results).data('page', prev);
        } else {
          _.last(results).data('page', next);
        }

        this.pagesCount += (index === 0) ? 0 : 1;
      }
    });

    this.showPage(0);
  }

  /**
   * Update the results page using the total results count.
   * @param {Number} count - total results count.
   */
  updateHtml(count) {
    this.$nextPage.prop('disabled', this.pageIndex === this.pagesCount - 1);
    this.$prevPage.prop('disabled', this.pageIndex === 0);
    this.$pagination.toggle(this.pagesCount > 1);
    this.$currPage.html(this.pageIndex + 1);
    this.$lastPage.html(this.pagesCount);

    if (this.query === null) {
      this.$emptyHeader.html(tr('No recent actions found'));
    } else {
      this.$emptyHeader.html(tr('Nothing found'));
    }

    this.$recentHeader.toggle(count !== 0 && this.query === null);
    this.$emptyHeader.toggle(count === 0);
  }

  /**
   * Show the results page with the selected page index.
   * @param {Number} index - selected page index.
   */
  showPage(index) {
    const results = $('.result-item').each(function () {
      const { keywords, page } = $(this).data();

      if (page === index) {
        const opts = { separateWordSearch: false, diacritics: false };
        const description = $(this).find('.item-description');
        const additional = $(this).find('.item-additional');
        const module = $(this).find('.item-module');
        const name = $(this).find('.item-name');
        const data = additional.data();
        $(this).unmark();

        keywords.forEach(({ field, matches }) => {
          if (!matches.length) return;

          if (field === 'descriptions' && (!additional.length)) {
            return description.mark(matches, opts);
          }

          if (field === 'module' && (!module.is(':empty'))) {
            return module.mark(matches, opts);
          }

          if (field === 'name' && (!name.is(':empty'))) {
            return name.mark(matches, opts);
          }

          if (data && field === data.type) {
            additional.mark(matches, opts);
          }
        });
      }

      $(this).toggle(page === index);
    });

    this.updateHtml(results.length);
  }

  /**
   * Initialize search page.
   */
  initialize() {
    this.$recentHeader = $('.results-recent').hide();
    this.$emptyHeader = $('.results-empty').hide();
    this.$recentHeader.text(tr('Recent actions'));

    this.$searchClear = $('#searchclear');
    this.$searchInput = $('#searchinput');

    this.$pagination = $('#pagination');
    this.$prevPage = $('#prevpage');
    this.$nextPage = $('#nextpage');
    this.$currPage = $('#currpage');
    this.$lastPage = $('#lastpage');

    this.$results = $('#results');
    this.$actions = $('.actions');
    this.$search = $('.search');

    this.toggle(true);
  }

  /**
   * Show or hide search page content depending on the condition.
   * @param {Boolean} hide - toggle condition.
   */
  toggle(hide) {
    this.worker.postMessage({ history: ActionHistory, type: 'update' });
    $(document.body).css('overflow', hide ? 'visible' : 'hidden');
    this.$actions.toggle(hide);
    this.$search.toggle(!hide);
    this.$pagination.hide();

    if (!hide) {
      this.$searchInput.focus().val('');
      this.recent();
    } else {
      this.$searchInput.blur().val('');
    }

    this.$searchClear.toggle(!hide);
  }

  /**
   * Show search page content if it's not already visible.
   */
  show() {
    if (!this.$search.is(':visible')) this.toggle(false);
  }

  /**
   * Hide search page content if it's not already hidden.
   */
  hide() {
    if (!this.$search.is(':hidden')) this.toggle(true);
  }

  /**
   * Render the search result using selected item properties.
   * @param {Object} properties - selected item properties.
   * @param {Number} properties.index - result index.
   * @param {Object} properties.item - result item.
   * @param {Number} properties.page - result page.
   * @returns {Object} rendered result object.
   */
  renderItem({ item, page, index }) {
    item.truncate = SearchLib.TextProcessor.truncate.bind(SearchLib.TextProcessor);
    const temp = { ...item, index };
    const data = { ...item, page };
    const template = _.template(`
      <li class="result-item bg-<%= type === 'action' ? 'action' : 'link' %>">
        <div class="result-item-left">
          <img draggable="false" class="item-icon" src="<%= icon %>">
          <span class="item-index"><%= _.padLeft(index + 1, 2, '0') %></span>
        </div>
        <div class="result-item-right">
          <div>
            <div class="item-name"><%= name %></div>
            <div class="item-description"><%= description %></div>
            <% if (descInfo.best) { %>
              <div class="item-additional text-<%= descInfo.color %>" data-type="descriptions">
                <%= truncate(descriptions[descInfo.index], keywords.find((k) => k.field === 'descriptions').matches) %>
              </div>
            <% } %>
            <% if (suggInfo.best) { %>
              <div class="item-additional text-<%= suggInfo.color %>" data-type="suggestions">
                <%= truncate(suggestions[suggInfo.index], keywords.find((k) => k.field === 'suggestions').matches) %>
              </div>
            <% } %>
            <% if (timeInfo.best) { %>
              <div class="item-additional text-<%= timeInfo.color %>" data-type="timestamps">
                <%= truncate(timestamps[timeInfo.index], keywords.find((k) => k.field === 'timestamps').matches) %>
              </div>
            <% } %>
            <% if (varsInfo.best) { %>
              <div class="item-additional text-<%= varsInfo.color %>" data-type="variables">
                <%= truncate(variables[varsInfo.index], keywords.find((k) => k.field === 'variables').matches) %>
              </div>
            <% } %>
          </div>
          <% if (type === 'action') { %>
            <div class="item-module"><%= module %></div>
          <% } %>
        </div>
      </li>
    `);

    return $(template(temp)).data(data).appendTo(this.$results);
  }
}