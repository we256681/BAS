'use strict';

window.App = {
  name: 'App',

  components: {
    SearchFilter,
  },

  data() {
    const tabs = ['variables', 'resources', 'callstack'].map(name => ({
      component: name === 'callstack' ? CallstackPanel : GroupsPanel,
      props: {
        data: name === 'callstack' ? [] : {},
        title: `tabs.${name}Empty`,
        reverse: false,
        sortings: [],
        filters: [],
        options: [],
        diff: null,
        query: '',
      },
      name,
    }));

    return {
      tabs,
      tab: tabs[0],
      search: false,
      groupsMenu: false,
      filtersMenu: false,
    };
  },

  computed: {
    sortings() {
      return this.tab.props.sortings;
    },

    filters() {
      return this.tab.props.filters;
    },

    options() {
      return this.tab.props.options;
    },

    reverse() {
      return this.tab.props.reverse;
    },

    data() {
      return this.tab.props.data;
    },

    flat() {
      const { options } = this.tab.props;
      return options.length && options[0].active;
    },

    ref() {
      return this.$refs.tab[this.tabs.indexOf(this.tab)];
    },
  },

  watch: {
    sortings(sortings) {
      this.$store.commit('setSortings', { id: this.tab.name, sortings });
    },

    filters(filters) {
      this.$store.commit('setFilters', { id: this.tab.name, filters });
    },

    options(options) {
      this.$store.commit('setOptions', { id: this.tab.name, options });
    },

    search(search) {
      if (!search) this.tab.props.query = '';
    },

    tab({ name }) {
      if (name === 'callstack') {
        this.$nextTick(() => {
          window.dispatchEvent(new Event('resize'));
        });
        this.search = false;
      }
    },
  },

  created() {
    this.tabs.forEach(({ name, props }) => {
      const { sortings, options, filters } = this.$store.state[name];

      if (name !== 'callstack') {
        props.sortings = ['frequency', 'dateModified', 'dateCreated', 'alphabetically'].map(name => {
          const sorting = sortings.find(item => item.name === name);
          return { name, active: sorting ? sorting.active : name === 'alphabetically' };
        });

        props.filters = ['undefined', 'boolean', 'object', 'string', 'number', 'array', 'date', 'null'].map(name => {
          const filter = filters.find(item => item.name === name);
          return { name, active: filter ? filter.active : true };
        });

        props.options = ['groups'].map(name => {
          const option = options.find(item => item.name === name);
          return { name, active: option ? option.active : false };
        });
      } else {
        props.filters = ['functions', 'actions'].map(name => {
          const filter = filters.find(item => item.name === name);
          return { name, active: filter ? filter.active : true };
        });
      }
    });
  },

  mounted() {
    window.addEventListener('message', this.handleMessage);
    post('mounted');
  },

  destroyed() {
    window.removeEventListener('message', this.handleMessage);
    post('destroyed');
  },

  methods: {
    getClickOutsideConfig(type) {
      return {
        middleware: e => !e.target.closest('.app-tool > .active'),
        handler: () => (this[`${type}Menu`] = false),
        capture: true,
      };
    },

    handleMessage({ data }) {
      const { type, payload } = data;

      if (payload && type === 'update') {
        this.tabs.forEach(({ name, props }) => {
          if (hasOwn(payload, name)) {
            if (hasOwn(payload.diff, name)) {
              props.diff = payload.diff[name];
            }
            props.data = payload[name];
          }
        });
      }
    },

    toggleSorting(item) {
      this.tab.props.sortings = this.sortings.map(({ name, active }) => ({
        active: name === item.name,
        name,
      }));

      if (item.active) this.tab.props.reverse = !this.reverse;
    },

    toggleFilter(item) {
      this.tab.props.filters = this.filters.map(({ name, active }) => ({
        active: name === item.name ? !active : active,
        name,
      }));
    },

    toggleOption(item) {
      this.tab.props.options = this.options.map(({ name, active }) => ({
        active: name === item.name ? !active : active,
        name,
      }));
    },

    toggleFlat(item) {
      if (item.name === 'groups' && item.active) {
        post('confirm', { message: this.$t('groups.toggle') }, ({ result }) => {
          if (result) this.toggleOption(item);
        });
      } else {
        this.toggleOption(item);
      }
      this.groupsMenu = false;
    },

    addGroup() {
      this.groupsMenu = false;
      this.ref.addGroup();
    },

    hide() {
      post('hide');
    },

    show() {
      post('show');
    },
  },

  template: /*html*/ `
    <div id="app">
      <div class="app-header">
        <div class="app-toolbar">
          <ul class="app-tabs">
            <li v-for="item in tabs" :key="item.name" :class="{ active: item === tab }" class="app-tab">
              <a href="#" @click.prevent="tab = item">
                <img :src="'src/assets/icons/' + item.name + '.svg'" alt>
                {{ $t('tabs.' + item.name) }}
              </a>
            </li>
          </ul>
          <div class="app-tool">
            <button type="button" :title="$t('buttons.search')" :disabled="tab.name === 'callstack'" @click="search = !search">
              <svg width="14" height="14" viewBox="0 0 16 16" xmlns="http://www.w3.org/2000/svg">
                <path d="m15.7164 15.111-4.2359-3.9328c2.1796-2.63853 1.8355-6.65365-.8031-8.83329C8.03894.165276 4.02382.509429 1.84418 3.14794-.335456 5.78644.00869703 9.80156 2.6472 11.9812c2.29436 1.9502 5.73589 1.9502 8.0302 0l4.2359 3.9329.8031-.8031ZM1.50003 7.16306c0-2.86795 2.29435-5.1623 5.16229-5.1623 2.86795 0 5.16228 2.29435 5.16228 5.1623 0 2.86794-2.29433 5.16234-5.16228 5.16234-2.86794 0-5.16229-2.2944-5.16229-5.16234Z" />
              </svg>
            </button>
            <button type="button" :class="{ active: filtersMenu }" :title="$t('buttons.filters')" @click="filtersMenu = !filtersMenu">
              <svg width="14" height="14" viewBox="0 0 16 16" xmlns="http://www.w3.org/2000/svg">
                <path d="M15.0001 2L1 2V4L5.91452 10.5V15H9.91452V10.5L15.0001 4V2ZM8.91452 10.0855V14H6.91452V10.0855L2.4145 4H13.5861L8.91452 10.0855Z" />
              </svg>
            </button>
            <button v-if="tab.name !== 'callstack'" type="button" :class="{ active: groupsMenu }" :title="$t('buttons.groups')" @click="groupsMenu = !groupsMenu">
              <svg width="16" height="16" viewBox="0 0 16 16" xmlns="http://www.w3.org/2000/svg">
                <path d="M6.5 3.5V2h-6v12h15V3.5h-9Zm8 9.5h-13V5h13v8Z" />
              </svg>
            </button>
            <button type="button" style="margin-left: 16px;" @click="hide">
              <svg width="16" height="16" viewBox="0 0 16 16" xmlns="http://www.w3.org/2000/svg">
                <path d="M2.87348 12.2583L3.93414 13.3189L8 9.25305L12.0659 13.3189L13.1265 12.2583L9.06066 8.19239L13.1265 4.12652L12.0659 3.06586L8 7.13173L3.93414 3.06586L2.87348 4.12652L6.93934 8.19239L2.87348 12.2583Z" />
              </svg>
            </button>
          </div>
          <ul v-show="filtersMenu" class="app-menu" v-click-outside="getClickOutsideConfig('filters')">
            <li v-for="item in sortings" :key="item.name" :class="{ active: item.active }">
              <a href="#" @click.prevent="toggleSorting(item)">
                <span v-t="'filtersMenu.sortings.' + item.name"></span>
                <img :style="{ transform: reverse ? 'rotate(180deg)' : '' }" src="src/assets/icons/arrows.svg" alt>
              </a>
            </li>
            <li v-if="sortings.length">
              <hr class="divider">
            </li>
            <li v-for="item in filters" :key="item.name" :class="{ active: item.active }">
              <a href="#" @click.prevent="toggleFilter(item)">
                <span v-t="'filtersMenu.filters.' + item.name"></span>
                <img src="src/assets/icons/check.svg" alt>
              </a>
            </li>
          </ul>
          <ul v-show="groupsMenu" class="app-menu" v-click-outside="getClickOutsideConfig('groups')">
            <li v-for="item in options" :key="item.name" :class="{ active: item.active }">
              <a href="#" style="opacity: 1;" @click.prevent="toggleFlat(item)">
                <span v-t="'groupsMenu.options.' + item.name"></span>
                <img src="src/assets/icons/check.svg" alt>
              </a>
            </li>
            <li v-if="options.length">
              <hr class="divider">
            </li>
            <li class="active" :class="{ disabled: !flat }">
              <a href="#" @click.prevent="addGroup">
                <span v-t="'groupsMenu.buttons.add'"></span>
              </a>
            </li>
          </ul>
        </div>
        <search-filter v-model.trim="tab.props.query" :disabled="tab.name === 'callstack'" :visible.sync="search" />
      </div>
      <component :is="item.component" v-for="item in tabs" v-show="item === tab" ref="tab" :key="item.name" :name="item.name" :class="item.name" v-bind="item.props" />
    </div>
  `,
};
