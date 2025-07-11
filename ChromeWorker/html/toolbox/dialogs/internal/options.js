BasDialogsLib.options = {
  resourcesOptions: [
    {
      checked: false,
      id: 'resourceDontDie',
      text: 'Don\'t end application if not exists',
      description: [
        'By default script will stop after resource is finished.',
        'You can use this option to complete not the entire script, but only the thread.'
      ].join(' ')
    },
    {
      checked: true,
      id: 'resourceReuse',
      text: 'Reuse Resource',
      description: [
        'If this setting is enabled, then every time you use resource, it will be replaced with same value.',
        'For example, if you are reading from file with \'Reuse Resource\' enabled, then same line will be used during all thread lifetime.',
        'You need to disable this setting to take new line.'
      ].join(' ')
    }
  ],

  variablesOptions: [],

  functionsOptions: [],
}