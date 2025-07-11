'use strict';

window.GroupsList = {
  name: 'GroupsList',

  components: {
    GroupsItem,
    JsonTreeRoot,
  },

  provide() {
    return {
      id: this.id,
    };
  },

  props: {
    filter: {
      type: Function,
      required: true,
    },

    source: {
      type: Object,
      required: true,
    },

    order: {
      type: Array,
      required: true,
    },

    flat: {
      type: Boolean,
      required: true,
    },

    id: {
      type: String,
      required: true,
    },
  },

  data() {
    const { groups } = this.$store.state[this.id];

    if (!groups.length) {
      groups.push({ id: 0, name: 'Main', color: 'brown', expanded: true, items: [] });
    }

    return { groups, counter: Math.max(...groups.map(({ id }) => id)) };
  },

  watch: {
    groups: {
      handler(groups) {
        this.$store.commit('setGroups', { id: this.id, groups });
      },
      deep: true,
    },

    source: {
      handler(source) {
        const keys = Object.keys(source);

        const [{ items }] = this.groups.filter(group => {
          group.items = group.items.filter(item => {
            const index = keys.indexOf(item.key);
            return index > -1 && keys.splice(index, 1);
          });
          return group.id === 0;
        });

        keys.forEach(key => items.push({ key, fixed: false }));
      },
    },

    order: {
      handler() {
        this.groups.forEach(group => this.sortGroup(group));
      },
      immediate: true,
    },

    flat: {
      handler(flat) {
        if (flat) {
          const { groups } = this;

          for (let index = groups.length - 1; index >= 0; index--) {
            if (groups[index].id !== 0) {
              this.removeGroup(index);
            }
          }

          groups[0].expanded = true;
        }
      },
      immediate: true,
    },
  },

  methods: {
    toggleFixed(items, item) {
      item.fixed = !item.fixed;
      this.sortGroup({ items });
    },

    updateGroup(index, data) {
      this.groups[index] = Object.assign(this.groups[index], data);
    },

    removeGroup(index, data) {
      const main = this.groups.find(({ id }) => id === 0);
      const [{ items }] = this.groups.splice(index, 1);
      main.items.push(...items), this.sortGroup(main);
    },

    sortGroup({ items }) {
      const { order } = this;

      return items.sort((a, b) => {
        if (b.fixed || a.fixed) return b.fixed - a.fixed;
        return order.indexOf(a.key) - order.indexOf(b.key);
      });
    },

    addGroup() {
      this.groups.unshift({
        id: ++this.counter,
        name: 'Group',
        color: 'brown',
        expanded: true,
        items: [],
      });
    },
  },

  template: /*html*/ `
    <draggable v-model="groups" :prevent-on-filter="false" :disabled="groups.length === 1" :class="{ flat }" filter="input, span + div, input + div" handle=".group-item-header" class="group-list" tag="ul">
      <groups-item
        v-for="(group, index) in groups"
        :key="group.id"
        :name="group.name"
        :color="group.color"
        :items="group.items"
        :allow-edit="group.id !== 0"
        :allow-remove="group.id !== 0"
        :is-expanded.sync="group.expanded"
        @update="updateGroup(index, $event)"
        @remove="removeGroup(index, $event)"
        @item-added="sortGroup(group)"
        @item-moved="sortGroup(group)"
      >
        <json-tree-root v-for="item in group.items" v-show="filter(item.key, source[item.key])" :key="item.key" :name="item.key" :value="source[item.key]">
          <template #label="{ label }">
            <button type="button" :style="{ opacity: item.fixed ? 1.0 : 0.5 }" @click.stop="toggleFixed(group.items, item)">
              <svg width="16" height="16" viewBox="0 0 16 16" xmlns="http://www.w3.org/2000/svg" style="vertical-align: sub;">
                <path fill-rule="evenodd" clip-rule="evenodd" d="M3.99984 5.1665C3.24839 5.1665 2.52772 5.46501 1.99637 5.99637C1.46501 6.52772 1.1665 7.24839 1.1665 7.99984C1.1665 8.75128 1.46501 9.47195 1.99637 10.0033C2.25947 10.2664 2.57181 10.4751 2.91557 10.6175C3.25932 10.7599 3.62776 10.8332 3.99984 10.8332H6.49984V11.8332H3.99984C3.49644 11.8332 2.99797 11.734 2.53288 11.5414C2.0678 11.3487 1.64522 11.0664 1.28926 10.7104C0.570372 9.99152 0.166504 9.0165 0.166504 7.99984C0.166504 6.98317 0.570372 6.00815 1.28926 5.28926C2.00815 4.57037 2.98317 4.1665 3.99984 4.1665H6.49984V5.1665H3.99984ZM9.49984 4.1665H11.9998C12.5032 4.1665 13.0017 4.26566 13.4668 4.4583C13.9319 4.65094 14.3545 4.9333 14.7104 5.28926C15.0664 5.64522 15.3487 6.0678 15.5414 6.53288C15.734 6.99797 15.8332 7.49644 15.8332 7.99984C15.8332 8.50324 15.734 9.00171 15.5414 9.46679C15.3487 9.93187 15.0664 10.3545 14.7104 10.7104C14.3545 11.0664 13.9319 11.3487 13.4668 11.5414C13.0017 11.734 12.5032 11.8332 11.9998 11.8332H9.49984V10.8332H11.9998C12.3719 10.8332 12.7404 10.7599 13.0841 10.6175C13.4279 10.4751 13.7402 10.2664 14.0033 10.0033C14.2664 9.74021 14.4751 9.42786 14.6175 9.08411C14.7599 8.74035 14.8332 8.37192 14.8332 7.99984C14.8332 7.62776 14.7599 7.25932 14.6175 6.91557C14.4751 6.57181 14.2664 6.25947 14.0033 5.99637C13.7402 5.73327 13.4279 5.52457 13.0841 5.38218C12.7404 5.23979 12.3719 5.1665 11.9998 5.1665H9.49984V4.1665Z" fill="#606060" />
                <path fill-rule="evenodd" clip-rule="evenodd" d="M4.83317 7.49984H11.1665V8.49984H4.83317V7.49984Z" fill="#606060" />
              </svg>
            </button>
            <span>{{ label }}</span>
          </template>
        </json-tree-root>
      </groups-item>
    </draggable>
  `,
};
