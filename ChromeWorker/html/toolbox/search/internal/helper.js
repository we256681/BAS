SearchLib.TextProcessor = {
  /**
   * Tokenizer for splitting a string into sentences.
   */
  sentenceTokenizer: new SearchLib.SentenceTokenizer(),

  /**
   * Regex pattern for trimming sentences (right).
   */
  trimRightSentenceRegex: /[^\wА-Яа-яЁё)"'#+]+$/,

  /**
   * Regex pattern for trimming sentences (left).
   */
  trimLeftSentenceRegex: /^[^\wА-Яа-яЁё("'#+]+/,

  /**
   * Regex pattern for default tokenization.
   */
  tokenizeRegex: /[^\wА-Яа-яЁё0-9_.#+]+/,

  /**
   * Regex pattern for trimming (right).
   */
  trimRightRegex: /[^\wА-Яа-яЁё"#+]+$/,

  /**
   * Regex pattern for trimming (left).
   */
  trimLeftRegex: /^[^\wА-Яа-яЁё"#+]+/,

  /**
   * Max chars limit for truncating.
   */
  truncateLimit: 200,

  /**
   * Truncate the source text using an array of highlighted keywords and the most relevant sentence.
   * @param {String[]} keywords - selected highlighted keywords array.
   * @param {String} string - selected source text string.
   * @returns {String} truncated text string.
   */
  truncate(string, keywords) {
    let { sentence } = this.getSentence(string, keywords.flatMap((k) => this.tokenize(k.toLowerCase())));
    let truncStart = false;
    let truncEnd = false;
    let next = 'start';

    while ((sentence = this.trim(sentence, true)).length >= this.truncateLimit) {
      const tokens = this.tokenize(sentence);
      const head = tokens.shift();
      const last = tokens.pop();

      const truncate = (defaultNext) => {
        next = defaultNext || next;

        if (next === 'start') {
          sentence = sentence.slice(head.length, sentence.length);
          if (!truncStart) truncStart = true;
          return (next = 'end');
        }

        if (next === 'end') {
          sentence = sentence.slice(0, sentence.length - last.length - 1);
          if (!truncEnd) truncEnd = true;
          return (next = 'start');
        }
      };

      const skipHead = keywords.includes(head);
      const skipLast = keywords.includes(last);
      if (!skipHead && !skipLast) truncate();
      else if (skipLast) truncate('start');
      else if (skipHead) truncate('end');
      else truncate();
    }

    if (truncStart) sentence = `... ${sentence}`;
    if (truncEnd) sentence = `${sentence} ...`;
    return _.escape(sentence);
  },

  /**
   * Get the sentence string from the source text that has the maximum number of highlighted keywords.
   * @param {String[]} keywords - selected highlighted keywords array.
   * @param {String} string - selected source text string.
   * @returns {{sentence: String}} object with sentence.
   */
  getSentence(string, keywords) {
    const sentences = this.sentenceTokenizer.tokenize(string).map((sentence) => {
      const filtered = keywords.filter((k) => {
        const lower = sentence.toLowerCase();
        return lower.includes(k);
      });

      return {
        filtered: filtered,
        sentence: sentence
      }
    });

    return _.max(sentences, (v) => v.filtered.length);
  },

  /**
   * Trim the selected text by removing non-word characters from the right end of the string.
   * @param {Boolean} asSentence - trim selected text as a sentence.
   * @param {String} string - selected text string.
   * @returns {String} processed text string.
   */
  trimRight(string, asSentence = false) {
    return string.replace(asSentence ? this.trimRightSentenceRegex : this.trimRightRegex, '');
  },

  /**
   * Trim the selected text by removing non-word characters from the left end of the string.
   * @param {Boolean} asSentence - trim selected text as a sentence.
   * @param {String} string - selected text string.
   * @returns {String} processed text string.
   */
  trimLeft(string, asSentence = false) {
    return string.replace(asSentence ? this.trimLeftSentenceRegex : this.trimLeftRegex, '');
  },

  /**
   * Trim the selected text by removing non-word characters using the processor patterns.
   * @param {Boolean} asSentence - trim selected text as a sentence.
   * @param {String} string - selected text string.
   * @returns {String} processed text string.
   */
  trim(string, asSentence = false) {
    let result = string.slice();
    result = this.trimRight(result, asSentence);
    result = this.trimLeft(result, asSentence);
    return result;
  },

  /**
   * Tokenize the selected text by splitting string with pattern.
   * @param {String} string - selected text string.
   * @returns {String[]} array of tokens.
   */
  tokenize(string) {
    return string.split(this.tokenizeRegex)
      .map((token) => this.trim(token))
      .filter((token) => token !== ' ')
      .filter((token) => token !== '');
  }
}