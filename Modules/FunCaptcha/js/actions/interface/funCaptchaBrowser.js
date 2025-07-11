<div class="container-fluid">
  <%= _.template($('#path').html())({ selector }) %>
  <%= _.template($('#input_constructor').html())({
    variants: [ 'antigate-newapi', 'rucaptcha-newapi', '2captcha-newapi' ],
    description: tr('Service name'),
    value_string: 'rucaptcha-newapi',
    default_selector: 'string',
    disable_int: true,
    id: 'serviceName',
    help: {
      description: tr('Captcha solving service name. You can use service names without the `-newapi` part, the action will work without changes.'),
      examples: [
        { code: 'antigate-newapi', description: tr('Solve captcha using https://anti-captcha.com/ service') },
        { code: 'rucaptcha-newapi', description: tr('Solve captcha using https://rucaptcha.com/ service') },
        { code: '2captcha-newapi', description: tr('Solve captcha using https://2captcha.com/ service') },
      ]
    }
  }) %>
  <%= _.template($('#input_constructor').html())({
    help: { description: tr('Captcha solving service key. You can get it in your personal account of selected captcha solving service') },
    description: tr('Service key'),
    default_selector: 'string',
    disable_int: true,
    id: 'serviceKey'
  }) %>
  <div style="margin-left: 15px">
    <input type="checkbox" id="AdvancedCheck" onchange="$('#Advanced').toggle()" />
    <label for="AdvancedCheck" class="tr">Advanced settings.</label>
  </div>
  <span id="Advanced" style="display: none;">
    <%= _.template($('#input_constructor').html())({
      description: tr('Send current proxy to solver service'),
      variants: [ 'true', 'false' ],
      default_selector: 'string',
      value_string: 'false',
      disable_int: true,
      id: 'sendProxy',
      help: {
        description: tr('Forces person who solves recaptcha use proxy that you setted up with \"Proxy\" action.'),
        examples: [
          { code: 'true', description: tr('Send current proxy to solver service') },
          { code: 'false', description: tr(`Don't send proxy. Default value.`) },
        ]
      }
    }) %>
    <%= _.template($('#input_constructor').html())({
      description: tr('Task solution check interval'),
      default_selector: 'int',
      value_number: 5 * 1000,
      disable_string: true,
      id: 'taskWaitTimeout',
      help: {
        description: tr('Task solution check interval in milliseconds. With this parameter you can control the length of the pause between each new check of the captcha solution.'),
        examples: [
          { code: '600', description: tr('Wait for 600 milliseconds') },
          { code: '10000', description: tr('Wait for 10 seconds') },
          { code: '5000', description: tr('Wait for 5 seconds') }
        ]
      }
    }) %>
    <%= _.template($('#input_constructor').html())({
      description: tr('Task solution check delay'),
      default_selector: 'int',
      value_number: 5 * 1000,
      disable_string: true,
      id: 'taskWaitDelay',
      help: {
        description: tr('Task solution check delay in milliseconds. With this parameter you can control the length of the pause between sending the task to the service and the start of waiting for a solution. Most often, captcha-solving services specify the required waiting time, in other cases you can specify any value convenient for you.'),
        examples: [
          { code: '600', description: tr('Wait for 600 milliseconds') },
          { code: '10000', description: tr('Wait for 10 seconds') },
          { code: '5000', description: tr('Wait for 5 seconds') }
        ]
      }
    }) %>
    <%= _.template($('#input_constructor').html())({
      description: tr('Service URL'),
      default_selector: 'string',
      disable_int: true,
      value_string: '',
      id: 'serviceUrl',
      help: {
        description: tr('Service URL. Can be blank. You can use this option if the service you want is not on the list of available services. In this case, specify the name of the service that works on a similar API and use the address you need.'),
        examples: [
          { code: tr('Empty string'), description: tr('Use default service URL, http://rucaptcha.com for RuCaptcha, etc') },
          { code: 'http://127.0.0.1:8083', description: tr('Use custom service URL with port 8083') },
          { code: 'http://127.0.0.3:8080', description: tr('Use custom service URL with port 8080') },
        ]
      }
    }) %>
    <%= _.template($('#input_constructor').html())({
      value_string: 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.212 Safari/537.36',
      description: tr('User-Agent'),
      default_selector: 'string',
      disable_int: true,
      id: 'userAgent',
      help: { description: tr('User-Agent that will be used by the service for solving captcha. It works only if you use a proxy to solve captcha, i.e. if the `Send current proxy to solver service` option is enabled.') }
    }) %>
  </span>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">This actions solves the <code>FunCaptcha</code> (<a href="#" onclick="BrowserAutomationStudio_OpenUrl('https://i.imgur.com/D7xAa2b.gif'); return false;">screenshot</a>).</div>
  <div class="tr tooltip-paragraph-fold">In order to solve <code>FunCaptcha</code> first you need to perform all actions, after which the captcha appears on the page. For example, fill out the registration form and submit it (if the captcha is already on the page, you can solve it immediately). After that you need to click the left mouse button on the container with captcha or any element inside it (example - <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://i.imgur.com/Gh6MxBf.png');return false">screenshot</a>), select <code>Solve FunCaptcha</code> option from the browser context menu, fill necessary data and click on <code>OK</code> button.</div>
  <div class="tr tooltip-paragraph-fold">This action does not require you to search for the necessary information to solve the <code>FunCaptcha</code> manually, everything happens automatically without your interaction using the source code of the page and other data. Exceptions are optional parameters, such as the use of proxies and the <code>User-Agent</code>.</div>
  <div class="tr tooltip-paragraph-fold">In most cases, you do not need to submit a captcha solution on the site. The action automatically performs <code>FunCaptcha</code> validation, using the response from the service. The only exception is when the site does not use the <code>callback</code> functions - then you will need to proceed manually. For example, press the submit form button or perform some other similar action.</div>
  <div class="tr tooltip-paragraph-fold">Note - the result of the <code>FunCaptcha</code> solution may not be visually visible on the page. It may seem to you that nothing happened and the captcha was not solved. This is not true - after performing this action the site will consider <code>FunCaptcha</code> solved and you can continue working with it.</div>
  <div class="tr tooltip-paragraph-fold"><code>FunCaptcha</code> is solved on the service worker's computer. This means that the site with the captcha can potentially check the IP of the person who solves it and put this IP in the blacklist. There is a special setting <code>Send current proxy to service</code> (under the advanced settings). If you use this option, the worker will solve the captcha through the proxy server that was set using the <code>Proxy</code> action from the browser actions module.</div>
  <div class="tr tooltip-paragraph-fold">When using a proxy, always fill in the field <code>User-Agent</code> or use the default value, otherwise the captcha solving service can give an error. If this parameter is specified, the service worker will solve the captcha using the specified <code>User-Agent</code>.</div>
  <div class="tr tooltip-paragraph-fold">All services requires a service key which which must be obtained on the service website and entered in the <code>Service key</code> field.</div>
  <div class="tr tooltip-paragraph-fold">If the required service is not in the list of available ones, but it works through an API similar to the selected service, then you can specify required server URL in the <code>Service URL</code> field located in the additional settings.</div>
  <div class="tr tooltip-paragraph-fold">If you use programs such as <code>CapMonster</code>, <code>XEvil</code> or similar software, you must fill in the <code>Service URL</code> field in accordance with the documentation for this software.</div>
  <div class="tr tooltip-paragraph-fold">The <code>Task solution check interval</code> parameter is responsible for the frequency of sending requests to the service to check the captcha solution. The more you set the value, the longer BAS will wait before sending the next request. It is recommended to use a delay of at least 5 seconds.</div>
  <div class="tr tooltip-paragraph-fold">The <code>Task solution check delay</code> parameter is responsible for the duration of waiting before the BAS starts checking the captcha. First, the captcha sent to the service, then the BAS will wait for the specified time, after which the task status check itself will begin. Most services recommend waiting at least 5 seconds.</div>
  <div class="tr tooltip-paragraph-last-fold">Detailed documentation for solving <code>FunCaptcha</code> can be found <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://2captcha.com/2captcha-api#solving_funcaptcha_new');return false">here</a>.</div>
</div>
<%= _.template($('#back').html())({ action: 'executeandadd', visible: true, name: tr('Solve FunCaptcha') }) %>