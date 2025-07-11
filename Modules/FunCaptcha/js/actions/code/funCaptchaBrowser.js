/*Browser*/
_call_function(BASCaptchaSolver.solveFunCaptcha, {
  proxy: String(<%= sendProxy %>).toLowerCase() == 'true' ? _PROXY : {},
  taskWaitTimeout: <%= taskWaitTimeout %>,
  taskWaitDelay: <%= taskWaitDelay %>,
  serviceName: <%= serviceName %>,
  serviceUrl: <%= serviceUrl %>,
  serviceKey: <%= serviceKey %>,
  userAgent: <%= userAgent %>,
  query: <%= query %>,
  waiter: function () {
    <%= waiter %>
  },
  path: function () {
    return (<%= path %>);
  },
})!