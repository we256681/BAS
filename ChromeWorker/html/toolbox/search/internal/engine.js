class BasSearchEngine extends SearchLib.SearchEngine {
  static weights = {
    descriptions: { youtube: 0.25, action: 0.10, wiki: 0.25 },
    suggestions: { youtube: 0.00, action: 0.10, wiki: 0.25 },
    timestamps: { youtube: 0.25, action: 0.00, wiki: 0.00 },
    variables: { youtube: 0.00, action: 0.10, wiki: 0.00 },
    module: { youtube: 0.00, action: 0.50, wiki: 0.00 },
    name: { youtube: 0.50, action: 0.20, wiki: 0.50 }
  };

  /**
   * Create an instance of `BasSearchEngine` class.
   * @param {Object} config - search engine configuration object.
   * @param {Object[]} config.documents - index documents array.
   * @param {Object[]} config.fields - index fields array.
   * @param {Number} config.distance - distance number.
   * @param {Number} config.limit - limit number.
   * @param {String} config.ref - ref string.
   * @constructor
   */
  constructor ({ documents, distance, fields, limit, ref }) {
    super({
      scoring(score, { document }) {
        const { type } = document;
        if (type === 'youtube') return score * 0.7;
        if (type === 'wiki') return score * 0.7;
        return score;
      },
      useEnglishStemmer: false,
      useRussianStemmer: false,
      tokenizerOptions: {
        trimRightRegex: SearchLib.TextProcessor.trimRightRegex,
        trimLeftRegex: SearchLib.TextProcessor.trimLeftRegex,
        tokenizeRegex: SearchLib.TextProcessor.tokenizeRegex
      },
      distance,
      limit
    });

    this.createIndex({ documents, fields, ref }); 
    this.history = [];
    this.cache = {};
  }

  /**
   * Check that the selected query string exists in the search cache.
   * @param {String} query - selected query string.
   */
  inCache(query) {
    return Object.prototype.hasOwnProperty.call(this.cache, query) === true;
  }

  /**
   * Perform an action search using the selected query.
   * @param {String} query - selected query string.
   * @returns {Object[]} search results array.
   */
  search(query) {
    const queryStr = SearchLib.TextProcessor.trim(query, true); let results = [];

    if (this.inCache(queryStr)) {
      return this.cache[queryStr];
    }

    if (!this.canceled) {
      results = super.search(queryStr, []);
    }

    if (this.canceled) {
      results = super.search(queryStr, ['descriptions']);
    }

    this.cache[queryStr] = _.uniq(results, ({ document }) => document.key)
      .filter(({ document }) => {
        const ignored = [
          'httpclientgetcookiesforurl',
          'getcookiesforurl',
          'javascript',
          'script',
          'check',
        ];
        return !ignored.includes(document.key);
      })
      .map((match) => {
        const descInfo = this.getDescriptionInfo(match);
        const suggInfo = this.getSuggestionInfo(match);
        const timeInfo = this.getTimestampInfo(match);
        const varsInfo = this.getVariableInfo(match);
        const keywords = this.getKeywords(match);
        _.max([descInfo, suggInfo, timeInfo, varsInfo], 'score').max = true;
        const document = match.document;

        if (document.type === 'action') {
          const variable = document.variables[varsInfo.index];

          if (variable && variable === variable.toUpperCase()) {
            varsInfo.color = 'green';
          } else {
            varsInfo.color = 'dark';
          }
        }

        const array = document.descriptions;
        const short = document.description;
        const item = array[descInfo.index];
        descInfo.skip = short.includes(item);

        return { descInfo, suggInfo, timeInfo, varsInfo, keywords, timecode: this.getTimecode(document, timeInfo), ...document };
      });

    return this.cache[queryStr];
  }

  /**
   * Perform an action search using the action history.
   * @returns {Object[]} search results array.
   */
  recent() {
    return this.history.filter((key) => this.store.findByRef(key)).map((key) => ({
      ...this.store.findByRef(key),
      descInfo: {},
      suggInfo: {},
      timeInfo: {},
      varsInfo: {},
      keywords: []
    }));
  }

  getKeywords({ document, metadata, tokens, query }) {
    const trim = (str) => SearchLib.TextProcessor.trim(str);
    const keywords = {};
    const ignored = {};

    Object.values(metadata).forEach((value) => {
      Object.entries(value).forEach(([fieldName, fieldData]) => {
        const [field, index] = fieldName.split('_');
        if (!keywords[field]) keywords[field] = [];
        if (!ignored[field]) ignored[field] = [];
        const keywordsList = keywords[field];
        const ignoredList = ignored[field];

        const property = index
          ? document[field][index].toLowerCase()
          : document[field].toLowerCase();

        if (!property.includes(query.toLowerCase())) {
          fieldData.tokenOriginal.forEach((source) => {
            const token = tokens.find((string) => {
              if (ignoredList.includes(string)) return false;
              const includes = source.includes(string);
              ignoredList.push(string);
              return includes;
            });

            if (!token) {
              keywordsList.push({ comparator: source + field, match: source });
            } else {
              keywordsList.push({ comparator: token + field, match: token });
            }
          });
        } else {
          const token = trim(query);
          keywordsList.push({ comparator: token + field, match: token });
          keywordsList.push({ comparator: query + field, match: query });
        }
      });
    });

    return Object.entries(keywords).map(([field, array]) => ({
      field, matches: _(array)
        .uniq('comparator')
        .map(v => v.match)
        .value()
    }));
  }

  getTimecode({ timestamps, timecodes }, { index }) {
    if (timestamps.length && index !== null) {
      const time = timecodes[timestamps[index]].split(':');

      if (time.length === 3) {
        return `?t=${time[0]}h${time[1]}m${time[2]}s`;
      }

      if (time.length === 2) {
        return `?t=${time[0]}m${time[1]}s`;
      }

      return null;
    }

    return null;
  }

  /**
   * Get the description info object using the selected scores array.
   * @param {{scores: Object[]}} source - selected source object with scores array.
   */
  getDescriptionInfo({ scores }) { return this.getFieldInfo('descriptions', scores); }

  /**
   * Get the suggestion info object using the selected scores array.
   * @param {{scores: Object[]}} source - selected source object with scores array.
   */
  getSuggestionInfo({ scores }) { return this.getFieldInfo('suggestions', scores); }

  /**
   * Get the timestamp info object using the selected scores array.
   * @param {{scores: Object[]}} source - selected source object with scores array.
   */
  getTimestampInfo({ scores }) { return this.getFieldInfo('timestamps', scores); }

  /**
   * Get the variable info object using the selected scores array.
   * @param {{scores: Object[]}} source - selected source object with scores array.
   */
  getVariableInfo({ scores }) { return this.getFieldInfo('variables', scores); }

  /**
   * Get the field info object using the selected scores array.
   * @param {Object[]} scores - selected scores array.
   * @param {String} source - selected source string.
   */
  getFieldInfo(source, scores) {
    const additional = scores.find((v) => v.field === source);
    const module = scores.find((v) => v.field === 'module');
    const name = scores.find((v) => v.field === 'name');

    const score = additional.score * additional.weight;
    const gtModule = score > (module.score * module.weight);
    const gtName = score > (name.score * name.weight);
    const found = gtModule && gtName;

    const info = {
      index: found ? additional.index : null,
      color: 'dark',
      found: found,
      score: score,
      skip: false,
      max: false,
      get best() {
        return info.found && info.max && !info.skip;
      },
    };

    return info;
  }
}
