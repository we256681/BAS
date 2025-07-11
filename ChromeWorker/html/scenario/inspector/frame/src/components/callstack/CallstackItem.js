'use strict';

window.CallstackItem = {
  name: 'CallstackItem',

  props: {
    options: {
      type: Object,
      required: true,
    },

    type: {
      type: String,
      required: true,
    },

    name: {
      type: String,
      required: true,
    },

    id: {
      type: Number,
      required: true,
    },
  },

  data() {
    return {
      preview: true,
      overflow: false,
      overflowWidth: -1,
      overflowHeight: -1,
    };
  },

  computed: {
    hasArguments() {
      return this.size > 0;
    },

    isFunction() {
      return this.type === 'function';
    },

    isAction() {
      return this.type === 'action';
    },

    size() {
      const args = this.options.arguments;
      return Object.keys(args).length;
    },
  },

  mounted() {
    this.$nextTick().then(() => this.handleResize());
  },

  created() {
    window.addEventListener('resize', this.handleResize);
  },

  destroyed() {
    window.removeEventListener('resize', this.handleResize);
  },

  methods: {
    handleResize() {
      if (this.isAction) {
        const { offsetHeight, scrollHeight } = this.$refs.preview;
        if (offsetHeight < scrollHeight && this.preview) {
          this.overflowHeight = scrollHeight;
        }

        const { offsetWidth, scrollWidth } = this.$refs.preview;
        if (offsetWidth < scrollWidth && this.preview) {
          this.overflowWidth = scrollWidth;
        }

        this.overflow = offsetHeight < this.overflowHeight || offsetWidth < this.overflowWidth;
        if (!this.overflow) this.preview = true;
      }
    },

    focusAction() {
      post('focusAction', {
        id: this.id,
      });
    },

    toggle() {
      if (this.hasArguments || this.overflow) {
        this.preview = !this.preview;
      }
    },
  },

  template: /*html*/ `
    <li class="callstack-item" :class="{ preview, action: isAction, function: isFunction }">
      <div class="callstack-item-title" @click="focusAction">
        <img :src="'src/assets/icons/' + (isAction ? 'gear' : 'flash') + '.svg'" alt>
        <span class="callstack-item-name">
          <span>{{ name }}&nbsp;</span>
          <span style="font-size: 10px; font-weight: 400; color: #606060a6;">({{ id }})</span>
          <span>{{ hasArguments || isAction ? ':' : '' }}</span>
        </span>
        <span ref="preview" class="callstack-item-data">
          <span v-if="isAction">{{ name === 'If' ? options.expression : options.iterator }}</span>
          <span v-else-if="hasArguments" v-show="preview">[{{ $tc('params', size) }}]</span>
        </span>
        <button v-show="hasArguments || overflow" type="button" class="callstack-item-toggle" @click.stop="toggle">
          <icon-chevron :style="{ transform: preview ? 'rotate(180deg)' : '' }" />
        </button>
      </div>
      <ul v-if="hasArguments" v-show="!preview" class="callstack-item-params">
        <li v-for="(value, name) in options.arguments" :key="name" class="callstack-item-param">
          <span class="callstack-item-param-name">{{ name }}:</span>
          <span class="callstack-item-param-value">{{ value }}</span>
        </li>
      </ul>
    </li>
  `,
};
