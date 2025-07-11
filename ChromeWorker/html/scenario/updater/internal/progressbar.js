(function ($) {
  class ProgressBar {
    constructor (element) {
      this.$element = $(element).data('progress', this);

      this.$inner = $('<div>', {
        'class': 'progress-inner'
      }).appendTo(this.$element);

      this.$label = $('<div>', {
        'class': 'progress-label'
      }).appendTo(this.$element);

      this.reset();
    }

    step(value, animate = true) {
      const max = this.max;
      const min = this.min;
      if (!this.current) this.current = min;
      if (value == null) value = this.current + 1;

      if (value >= min && value <= max) {
        this.$inner.velocity({ width: `${Math.round((value * 100 / max)) || 0}%` }, {
          duration: animate ? 200 : 0,
          easing: 'ease-in',
          queue: false
        });
        this.$label.text(`${value} / ${max}`);
        this.current = value;
      }
    }

    finish(animate = false) {
      this.$inner.velocity('stop');
      this.step(this.max, animate);
    }

    reset(animate = false) {
      this.$inner.velocity('stop');
      this.step(this.min, animate);
    }

    destroy() {
      this.$element.removeData('progress');
      this.$inner.unbind().remove();
      this.$label.unbind().remove();
    }

    get max() {
      const max = this.$element.data('max');
      return max != null ? max : 100;
    }

    get min() {
      const min = this.$element.data('min');
      return min != null ? min : 0;
    }
  };

  $.fn.progressBar = function (option, ...args) {
    return this.each(function () {
      let data = $(this).data('progress') || new ProgressBar(this);

      if (typeof (option) === 'string' && data[option]) {
        if (args.length) {
          data[option].apply(data, args);
        } else {
          data[option].apply(data, []);
        }
      }
    })
  };

  $.fn.progressBar.Constructor = ProgressBar;
})(jQuery);