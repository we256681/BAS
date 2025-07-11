'use strict';

(window.locales || (window.locales = {})).en = {
  tabs: {
    variables: 'Variables',
    resources: 'Resources',
    callstack: 'Stack',
    variablesEmpty: 'Variables list is empty',
    resourcesEmpty: 'Resources list is empty',
    callstackEmpty: 'Call stack is empty',
  },
  buttons: {
    filters: 'Filters and sorting',
    groups: 'Group management',
    search: 'Search',
  },
  filtersMenu: {
    sortings: {
      frequency: 'By frequency of use',
      dateModified: 'By date modified',
      dateCreated: 'By date created',
      alphabetically: 'Alphabetically',
    },
    filters: {
      undefined: 'Undefined',
      boolean: 'Boolean',
      object: 'Object',
      string: 'String',
      number: 'Number',
      array: 'Array',
      date: 'Date',
      null: 'Null',
      functions: 'Functions',
      actions: 'Actions',
    },
  },
  groupsMenu: {
    options: {
      groups: 'Enable groups',
    },
    buttons: {
      add: 'Add group',
    },
  },
  search: {
    placeholder: 'Search by name...',
  },
  groups: {
    title: 'This group is empty. You can drag the elements here',
    confirm: 'Do you really want to delete the "{name}" group?',
    toggle: 'Do you really want to disable groups? (all groups will be deleted)',
  },
  params: '{n} parameters | {n} parameter | {n} parameters',
  items: '{n} items | {n} item | {n} items',
};
