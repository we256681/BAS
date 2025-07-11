((Backbone) => {
  _.extend(Backbone.Events, {
    once: function (events, callback, context) {
      const boundOff = _.bind(this.off, this);
      const oneOffCallback = _.once(function () {
        boundOff(events, oneOffCallback);
        callback.apply(context, arguments);
      });
      return this.on(events, oneOffCallback, context);
    },

    unbind: Backbone.Events.unbind,

    bind: Backbone.Events.bind,

    off: Backbone.Events.off,

    on: Backbone.Events.on,
  });

  _.each(['Model', 'Collection', 'Router', 'View', 'History'], (kind) => _.extend(Backbone[kind].prototype, Backbone.Events));
})(Backbone);