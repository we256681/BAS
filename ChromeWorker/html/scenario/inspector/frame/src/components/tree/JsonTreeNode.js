'use strict';

window.JsonTreeNode = {
  name: 'JsonTreeNode',

  inject: ['id'],

  props: {
    value: {
      required: true,
      validator(value) {
        // prettier-ignore
        return [
          'null',
          'date',
          'array',
          'object',
          'number',
          'string',
          'boolean',
          'undefined',
        ].includes(typeOf(value));
      },
    },

    name: {
      type: String,
      required: true,
    },

    path: {
      type: Array,
      required: true,
    },
  },

  data() {
    const { $store, path, id } = this;
    const expand = $store.state[id].nodes[JSON.stringify(path)] || false;

    return {
      colors: [
        '#dc1600',
        '#ff5200',
        '#eb8200',
        '#a97643',
        '#78893f',
        '#606060',
      ],
      isHovered: false,
      isExpanded: expand,
    };
  },

  computed: {
    filteredKeys() {
      const { type, keys } = this;

      return keys.length <= 100 ? keys : keys.filter((key, idx) => {
        if (type === 'object' && key === '__length__') {
          return false;
        }
        if (type === 'array' && idx === keys.length - 1) {
          return false;
        }
        return true;
      });
    },

    length() {
      const { type, keys, value } = this;

      if (keys.length > 100) {
        if (type === 'object') {
          return value.__length__;
        }
        if (type === 'array') {
          return value.slice(-1)[0];
        }
      }

      return keys.length;
    },

    counter() {
      const pointer = JSON.stringify(this.path);
      const { counters } = this.$store.state;
      return counters[this.id][pointer];
    },

    indent() {
      const depth = this.path.length;
      return `${24 * (depth - 1)}px`;
    },

    color() {
      return this.colors[this.counter];
    },

    keys() {
      const { value, type } = this;
      const keys = value ? Object.keys(value) : [];
      return type === 'array' ? keys.map(key => parseInt(key, 10)) : keys;
    },

    type() {
      return typeOf(this.value);
    },
  },

  watch: {
    keys: {
      handler() {
        const { type, keys } = this;

        if ((type === 'array' || type === 'object') && !keys.length) {
          this.collapse();
        }
      },
      immediate: true,
    },

    type: {
      handler() {
        const { type, keys } = this;
  
        if ((type === 'array' || type === 'object') && !keys.length) {
          this.collapse();
        }
      },
      immediate: true,
    }
  },

  created() {
    this.click = debounce(() => {
      const { type, keys } = this;

      if ((type === 'object' || type === 'array') && keys.length) {
        return this.toggle();
      }

      this.edit();
    }, 250);
  },

  methods: {
    collapse() {
      this.$store.commit('setNodeCollapsed', {
        path: this.path,
        id: this.id,
      });
      this.isExpanded = false;
    },

    expand() {
      this.$store.commit('setNodeExpanded', {
        path: this.path,
        id: this.id,
      });
      this.isExpanded = true;
    },

    toggle() {
      if (this.isExpanded) {
        this.collapse();
      } else {
        this.expand();
      }
    },

    copy() {
      const { id, type, value, length } = this, text = type === 'string' ? value : JSON.stringify(value);

      if (id === 'variables' && ['string', 'object', 'array'].includes(type) && (length > (type === 'string' ? 300 : 100))) {
        post('get', { path: this.path }, ({ value }) => {
          setClipboard(type === 'string' ? value : JSON.stringify(value));
        });
      } else {
        if (value == null) {
          setClipboard(type);
        } else {
          setClipboard(text);
        }
      }
    },

    edit() {
      post('edit', {
        value: this.value,
        type: this.type,
        path: this.path,
        mode: this.id,
      });
    },

    formatDate(date) {
      return dayjs(date).format('YYYY-MM-DD HH:mm:ss [UTC]Z');
    },
  },

  template: /*html*/ `
    <div class="jt-node" :class="[type, { hovered: isHovered, expanded: isExpanded }]" :style="{ '--indent': indent }" @mouseover.stop="isHovered = true" @mouseout.stop="isHovered = false" @click.stop="click">
      <div class="jt-node-inner">
        <span class="jt-node-label" :style="{ color }" @click.stop="edit"><slot name="label" :label="name">{{ name }}</slot>:&nbsp;</span>
        <span class="jt-node-value" style="margin-right: 8px; flex: 1;">
          <template v-if="type === 'undefined' || type === 'null'">{{ type }}</template>
          <template v-else-if="type === 'string'">"{{ value }}"</template>
          <template v-else-if="type === 'object'">
            <span class="jt-node-bracket">{</span>
            <template v-if="!isExpanded">
              <span class="jt-node-preview">{{ $tc('items', length) }}</span>
              <span class="jt-node-bracket">}</span>
            </template>
          </template>
          <template v-else-if="type === 'array'">
            <span class="jt-node-bracket">[</span>
            <template v-if="!isExpanded">
              <span class="jt-node-preview">{{ $tc('items', length) }}</span>
              <span class="jt-node-bracket">]</span>
            </template>
          </template>
          <template v-else-if="type === 'date'">{{ formatDate(value) }}</template>
          <template v-else>{{ value }}</template>
        </span>
        <div class="jt-node-actions" @click.stop>
          <button type="button" @click="copy">
            <icon-copy />
          </button>
          <button type="button" @click="edit">
            <icon-edit />
          </button>
          <button v-if="(type === 'object' || type === 'array') && keys.length" type="button" @click="toggle">
            <icon-chevron :style="{ transform: isExpanded ? '' : 'rotate(180deg)' }" />
          </button>
        </div>
      </div>
      <div v-if="isExpanded && type === 'object'" class="jt-node-nodes">
        <json-tree-node v-for="key in filteredKeys" :key="key" :name="key" :value="value[key]" :path="path.concat(key)" />
        <span v-if="length > 100" class="jt-node-ellipsis">...</span>
        <span class="jt-node-bracket">}</span>
      </div>
      <div v-if="isExpanded && type === 'array'" class="jt-node-nodes">
        <json-tree-node v-for="key in filteredKeys" :key="key" :name="key" :value="value[key]" :path="path.concat(key)" />
        <span v-if="length > 100" class="jt-node-ellipsis">...</span>
        <span class="jt-node-bracket">]</span>
      </div>
    </div>
  `,
};
