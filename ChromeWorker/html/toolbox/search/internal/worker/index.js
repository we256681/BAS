SearchLib.Worker = {
  create: function ({ onmessage, onerror }) {
    const worker = new Worker(URL.createObjectURL(new Blob(
      [`(${SearchLib.Worker.source.toString()})()`],
      { type: 'text/javascript' }
    )));
    worker.onmessage = onmessage;
    worker.onerror = onerror;
    return worker;
  },

  source: function () {
    const libUrl = 'file:///html/toolbox/search/internal';
    importScripts('file:///html/toolbox/lodash.js');
    importScripts(`${libUrl}/library.min.js`);
    importScripts(`${libUrl}/documents.js`);
    importScripts(`${libUrl}/engine.js`);
    importScripts(`${libUrl}/helper.js`);
    const map = BasSearchEngine.weights;

    self.onmessage = function (e) {
      if (e.data.type === 'initialize') {
        self.engine = new BasSearchEngine({
          documents: e.data.documents,
          fields: [
            { name: 'descriptions', weight: ({ type }) => map.descriptions[type] },
            { name: 'suggestions', weight: ({ type }) => map.suggestions[type] },
            { name: 'timestamps', weight: ({ type }) => map.timestamps[type] },
            { name: 'variables', weight: ({ type }) => map.variables[type] },
            { name: 'module', weight: ({ type }) => map.module[type] },
            { name: 'name', weight: ({ type }) => map.name[type] }
          ],
          distance: 0.3,
          limit: 500,
          ref: 'key'
        });

        postMessage({ type: e.data.type });
      }

      if (e.data.type === 'search') {
        const results = self.engine.search(e.data.query);
        postMessage({ type: e.data.type, results });
      }

      if (e.data.type === 'recent') {
        const results = self.engine.recent(e.data.query);
        postMessage({ type: e.data.type, results });
      }

      if (e.data.type === 'update') {
        if (self.engine) self.engine.history = e.data.history;
        postMessage({ type: e.data.type });
      }
    };
  }
}