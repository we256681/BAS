'use strict';

window.CallstackPanel = {
  name: 'CallstackPanel',

  components: {
    CallstackList,
  },

  props: {
    sortings: {
      type: Array,
      default: () => [],
    },

    filters: {
      type: Array,
      default: () => [],
    },

    options: {
      type: Array,
      default: () => [],
    },

    reverse: {
      type: Boolean,
      default: false,
    },

    query: {
      type: String,
      default: '',
    },

    title: {
      type: String,
      required: true,
    },

    name: {
      type: String,
      required: true,
    },

    data: {
      type: Array,
      required: true,
    },

    diff: {
      type: Array,
      required: true,
    },
  },

  computed: {
    activeFilters() {
      return this.filters.filter(f => f.active).map(f => f.name);
    },

    isEmpty() {
      return !Object.keys(this.data).length;
    },
  },

  template: /*html*/ `
    <div class="app-panel">
      <div v-show="!isEmpty" class="app-panel-content">
        <callstack-list :data="data" :filters="activeFilters" />
      </div>
      <div v-show="isEmpty" class="app-panel-title" v-t="title"></div>
    </div>
  `,
};
