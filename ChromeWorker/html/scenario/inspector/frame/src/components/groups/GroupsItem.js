'use strict';

window.GroupsItem = {
  name: 'GroupsItem',

  props: {
    allowRemove: {
      type: Boolean,
      default: false,
    },

    allowEdit: {
      type: Boolean,
      default: false,
    },

    isExpanded: {
      type: Boolean,
      default: false,
    },

    color: {
      type: String,
      required: true,
    },

    items: {
      type: Array,
      required: true,
    },

    name: {
      type: String,
      required: true,
    },
  },

  data() {
    return {
      newColor: this.color,
      newName: this.name,
      isEditing: false,
      colors: {
        green: '142, 196, 143',
        brown: '192, 189, 155',
        blue: '154, 203, 230',
        gray: '207, 207, 207',
        red: '246, 155, 147',
      },
    };
  },

  computed: {
    style() {
      return {
        '--color-rgb': this.colors[this.newColor],
      };
    },
  },

  methods: {
    onMove({ to, from, relatedContext, draggedContext }) {
      const related = relatedContext.element;
      const dragged = draggedContext.element;

      if (to === from && related && dragged) {
        return related.fixed === dragged.fixed;
      }
    },

    update(accept) {
      if (!this.isEditing) return;

      if (accept && this.newName) {
        this.$emit('update', {
          color: this.newColor,
          name: this.newName,
        });
      } else {
        this.newColor = this.color;
        this.newName = this.name;
      }

      this.isEditing = false;
    },

    toggle(event) {
      if (this.isEditing) return;
      this.$emit('update:is-expanded', !this.isExpanded);
    },

    remove() {
      post('confirm', { message: this.$t('groups.confirm', { name: this.name }) }, ({ result }) => {
        if (result) this.$emit('remove');
      });
    },

    cancel() {
      this.update(false);
    },

    accept() {
      this.update(true);
    },

    edit() {
      this.isEditing = true;
      this.$nextTick(() => {
        this.$refs.input.focus();
      });
    },
  },

  template: /*html*/ `
    <li class="group-item" :style="style">
      <div class="group-item-header" @click="toggle">
        <svg width="14" height="14" viewBox="0 0 16 16" xmlns="http://www.w3.org/2000/svg">
          <path d="M6 3.5v-2H0v12H16v-10H6Z" stroke="#606060" />
        </svg>
        <div class="group-item-inputs" v-click-outside="accept">
          <input v-if="isEditing" ref="input" v-model.trim="newName" maxlength="30" spellcheck="false" type="text" @keydown.enter="accept" @keydown.esc="cancel">
          <span v-else>{{ name }}</span>
          <div class="group-item-controls" @click.stop>
            <template v-if="isEditing">
              <ul class="group-item-swatches">
                <li v-for="(value, key) in colors" :key="key" :style="{ borderColor: newColor === key ? 'rgb(' + value + ')' : 'transparent' }" class="group-item-swatch" @click="newColor = key">
                  <svg width="12" height="12" viewBox="0 0 12 12" xmlns="http://www.w3.org/2000/svg">
                    <circle cx="6" cy="6" r="6" :fill="'rgb(' + value + ')'" />
                  </svg>
                </li>
              </ul>
              <button type="button" @click="accept">
                <svg width="16" height="16" viewBox="0 0 16 16" xmlns="http://www.w3.org/2000/svg">
                  <path d="m6.25 10.6002-3.55-3.55-.7.7 3.55 3.55.7.7 7.05-7.05-.7-.75-6.35 6.4Z" />
                </svg>
              </button>
            </template>
            <template v-else>
              <button v-if="allowRemove" type="button" @click="remove">
                <icon-delete />
              </button>
              <button v-if="allowEdit" type="button" @click="edit">
                <icon-edit />
              </button>
              <button type="button" @click="$emit('update:is-expanded', !isExpanded)">
                <icon-chevron :style="{ transform: isExpanded ? '' : 'rotate(180deg)' }" />
              </button>
            </template>
          </div>
        </div>
      </div>
      <draggable v-show="isExpanded" :style="{ '--title': JSON.stringify($t('groups.title')) }" class="group-item-content" handle=".jt-root > .jt-node > .jt-node-inner > .jt-node-label" group="items" :list="items" :move="onMove" @add="$emit('item-added')" @update="$emit('item-moved')">
        <slot></slot>
      </draggable>
    </li>
  `,
};
