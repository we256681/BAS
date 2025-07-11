(function (window, $) {
  function overlayHandler(event) {
    event.stopPropagation();
    event.preventDefault();
  }

  function detachHandler() {
    const events = ['mousedown', 'mouseup', 'keydown', 'keyup', 'input'];
    $(document).off(events.join(' '), overlayHandler);
  }

  function attachHandler() {
    const events = ['mousedown', 'mouseup', 'keydown', 'keyup', 'input'];
    $(document).on(events.join(' '), overlayHandler);
  }

  window.Toolbox.overlay = {
    hide: function () {
      $('.document-overlay').hide();
      detachHandler();
    },

    show: function () {
      $('.document-overlay').show();
      attachHandler();
    }
  };

  $(() => $('<div>', { 'class': 'document-overlay' }).appendTo('body').hide());
})(window, jQuery);