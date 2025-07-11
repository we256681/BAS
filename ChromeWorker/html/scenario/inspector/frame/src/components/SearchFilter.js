'use strict';

window.SearchFilter = {
  name: 'SearchFilter',

  props: {
    disabled: {
      type: Boolean,
      default: false,
    },

    visible: {
      type: Boolean,
      default: true,
    },

    value: {
      type: String,
      default: '',
    },
  },

  watch: {
    visible(visible) {
      if (visible) {
        this.$nextTick(() => {
          this.$refs.input.focus();
        });
      }
    },
  },

  methods: {
    clear() {
      if (this.value.length) {
        this.$emit('input', '');
      } else {
        this.$emit('update:visible', false);
      }
    },
  },

  template: /*html*/ `
    <div v-show="visible" class="app-search">
      <input ref="input" type="text" spellcheck="false" :value="value" :disabled="disabled" :placeholder="$t('search.placeholder')" @input="$emit('input', $event.target.value)" @keydown.esc="clear">
      <button type="button" :disabled="disabled" @click="clear">
        <svg width="16" height="16" viewBox="0 0 16 16" xmlns="http://www.w3.org/2000/svg">
          <path d="M2.87348 12.2583L3.93414 13.3189L8 9.25305L12.0659 13.3189L13.1265 12.2583L9.06066 8.19239L13.1265 4.12652L12.0659 3.06586L8 7.13173L3.93414 3.06586L2.87348 4.12652L6.93934 8.19239L2.87348 12.2583Z" />
        </svg>
      </button>
    </div>
  `,
};
