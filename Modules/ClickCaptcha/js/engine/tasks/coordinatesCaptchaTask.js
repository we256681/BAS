(function (tasks, api, _) {
  const CoordinatesCaptchaTask = _.inherit(tasks.RuCaptchaTask, function (image, params) {
    tasks.RuCaptchaTask.call(this, 'CoordinatesCaptcha', {
      name: 'post',
      rules: {
        textInstructions: { name: 'textinstructions', optional: false },
        imgInstructions: { name: 'imginstructions', optional: true },
      },
      params: params,
      image: image,
    });
    this.data['coordinatescaptcha'] = 1;
  });

  CoordinatesCaptchaTask.prototype.getSolution = function (response) {
    return response.split(':').pop().split(';').map(function (data) {
      const point = data.split(',');

      return [
        parseInt(point[0].split('=').pop()),
        parseInt(point[1].split('=').pop()),
      ]
    });
  };

  api.prototype.CoordinatesCaptchaTask = CoordinatesCaptchaTask;
})(BASCaptchaSolver.tasks, BASCaptchaSolver.RuCaptchaApi, BASCaptchaSolver.utils);