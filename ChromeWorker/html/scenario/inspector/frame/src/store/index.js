'use strict';

const store = new Vuex.Store({
  state() {
    const state = Object.assign({}, parseJSON(bas.state));

    ['variables', 'resources', 'callstack'].forEach(id => {
      state[id] = Object.assign({}, state[id]);

      (id === 'callstack' ? ['filters'] : ['sortings', 'filters', 'options', 'groups', 'nodes']).forEach(key => {
        if (typeOf(state[id][key]) !== (key === 'nodes' ? 'object' : 'array')) {
          state[id][key] = key === 'nodes' ? {} : [];
        }
      });
    });

    return { ...state, counters: { variables: {}, resources: {} } };
  },
  mutations: {
    setNodeCollapsed(state, { id, path }) {
      const pointer = JSON.stringify(path);
      state[id].nodes[pointer] = false;
    },
    setNodeExpanded(state, { id, path }) {
      const pointer = JSON.stringify(path);
      state[id].nodes[pointer] = true;
    },
    removeNode(state, { id, path }) {
      const pointer = JSON.stringify(path);
      delete state[id].nodes[pointer];
    },
    setCounters(state, { id, counters }) {
      state.counters[id] = counters;
    },
    setSortings(state, { id, sortings }) {
      state[id].sortings = sortings;
    },
    setFilters(state, { id, filters }) {
      state[id].filters = filters;
    },
    setOptions(state, { id, options }) {
      state[id].options = options;
    },
    setGroups(state, { id, groups }) {
      state[id].groups = groups;
    },
  },
});

store.subscribe(({ type }, { variables, resources, callstack }) => {
  if (type !== 'setCounters') {
    BrowserAutomationStudio_SaveInterfaceJson(JSON.stringify({ variables, resources, callstack }));
  }
});
