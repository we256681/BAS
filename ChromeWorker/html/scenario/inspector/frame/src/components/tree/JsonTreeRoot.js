'use strict';

window.JsonTreeRoot = {
  name: 'JsonTreeRoot',

  components: {
    JsonTreeNode,
  },

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
  },

  template: /*html*/ `
    <div class="jt-root">
      <json-tree-node :name="name" :path="[name]" :value="value">
        <template #label="{ label }">
          <slot name="label" :label="name">{{ name }}</slot>
        </template>
      </json-tree-node>
    </div>
  `,
};
