<div class="container-fluid">
  <%= _.template($('#path').html())({ selector }) %>
  <%= _.template($('#input_constructor').html())({ id: 'serviceName', description: tr('Service name'), variants: [ 'rucaptcha', '2captcha' ], value_string: 'rucaptcha', default_selector: 'string', disable_int: true, help: {
    description: tr('Captcha solving service name'),
    examples: [
      { code: 'rucaptcha', description: tr('Solve captcha using https://rucaptcha.com/ service') },
      { code: '2captcha', description: tr('Solve captcha using https://2captcha.com/ service') },
    ]
  }}) %>
  <%= _.template($('#input_constructor').html())({ id: 'serviceKey', description: tr('Service key'), disable_int: true, default_selector: 'string', help: {
    description: tr('Captcha solving service key. You can get it in your personal account of selected captcha solving service') 
  }}) %>
  <%= _.template($('#input_constructor').html())({ description: tr('Captcha description. Can be empty.'), value_string: '', default_selector: 'string', disable_int: true, id: 'textInstructions', help: {
    description: tr('If captcha has any description, it can be added with this field.'),
    examples: [
      { code: tr('Select all images where there is a plane') },
      { code: tr('Select all images where there is a boat') }
    ]
  }}) %>
  <div style="margin-left: 15px">
    <input type="checkbox" id="AdvancedCheck" onchange="$('#Advanced').toggle()" />
    <label for="AdvancedCheck" class="tr">Advanced settings.</label>
  </div>
  <div id="Advanced" style="display: none;">
    <div style="margin-left: 15px;">
      <span data-preserve="true" data-preserve-type="check" data-preserve-id="emulateMouse">
        <input type="checkbox" id="emulateMouse" checked />
        <label for="emulateMouse"><%= tr('Emulate mouse movements') %></label>
      </span>
    </div>
    <%= _.template($('#input_constructor').html())({ description: tr('Task solution check interval'), default_selector: 'int', value_number: 5000, disable_string: true, id: 'taskWaitTimeout', help: {
      description: tr('Task solution check interval in milliseconds. With this parameter you can control the length of the pause between each new check of the captcha solution.'),
      examples: [
        { code: '600', description: tr('Wait for 600 milliseconds') },
        { code: '10000', description: tr('Wait for 10 seconds') },
        { code: '5000', description: tr('Wait for 5 seconds') }
      ]
    }}) %>
    <%= _.template($('#input_constructor').html())({ description: tr('Task solution check delay'), default_selector: 'int', value_number: 5000, disable_string: true, id: 'taskWaitDelay', help: {
      description: tr('Task solution check delay in milliseconds. With this parameter you can control the length of the pause between sending the task to the service and the start of waiting for a solution. Most often, captcha-solving services specify the required waiting time, in other cases you can specify any value convenient for you.'),
      examples: [
        { code: '600', description: tr('Wait for 600 milliseconds') },
        { code: '10000', description: tr('Wait for 10 seconds') },
        { code: '5000', description: tr('Wait for 5 seconds') }
      ]
    }}) %>
    <%= _.template($('#input_constructor').html())({ description: tr('Service URL'), default_selector: 'string', disable_int: true, value_string: '', id: 'serviceUrl', help: {
      description: tr('Service URL. Can be blank. You can use this option if the service you want is not on the list of available services. In this case, specify the name of the service that works on a similar API and use the address you need.'),
      examples: [
        { code: tr('Empty string'), description: tr('Use default service URL, http://rucaptcha.com for RuCaptcha, etc') },
        { code: 'http://127.0.0.1:8083', description: tr('Use custom service URL with port 8083') },
        { code: 'http://127.0.0.3:8080', description: tr('Use custom service URL with port 8080') },
      ]
    }}) %>
  </div>
</div>
<div class="tooltipinternal">
  <div class="tr tooltip-paragraph-first-fold">Solve any type of captcha that requires clicking on images.</div>
  <div class="tr tooltip-paragraph-fold">For <code>ReCaptcha</code> and <code>HCaptcha</code> solving it is recommended to use the <code>Solve Recaptcha 2.0</code> and <code>Solve HCaptcha</code> actions from the browser context menu, if it's possible. It is easier to use, and the captcha can be solved faster. Also, these actions can perform automatic submit of the solution. Use this action if other options don't suit you for some reason.</div>
  <div class="tr tooltip-paragraph-fold">This action allows you to solve any captcha that requires you to click on images according to specific instructions. These can be <code>ReCaptcha</code>, <code>HCaptcha</code> and other types. To solve such captchas, you need to send an image to the service containing elements to be clicked. If there is a description on the captcha or next to it, it is recommended to send it along with the image to avoid incorrect solutions.</div>
  <div class="tr tooltip-paragraph-fold">All services requires a service key which which must be obtained on the service website and entered in the <code>Service key</code> field.</div>
  <div class="tr tooltip-paragraph-fold">This action, unlike the <code>Solve captcha with clicks</code> action from the <code>Browser</code> module, performs clicks automatically, and you do not need to manually calculate the coordinates for clicks relative to the position of the element on the page. It is recommended to use this action.</div>
  <div class="tr tooltip-paragraph-fold">An important point - this action does not automatically confirm the solution of the captcha. You have to do it yourself. For example, if you solve <code>ReCaptcha2</code> using this method, you should click the checkbox yourself, and after clicking on the coordinates, click the <code>Next</code> or <code>Done</code> button.</div>
  <div class="tr tooltip-paragraph-fold">The <code>Task solution check interval</code> parameter is responsible for the frequency of sending requests to the service to check the captcha solution. The more you set the value, the longer BAS will wait before sending the next request. It is recommended to use a delay of at least 5 seconds.</div>
  <div class="tr tooltip-paragraph-fold">The <code>Task solution check delay</code> parameter is responsible for the duration of waiting before the BAS starts checking the captcha. First, the captcha sent to the service, then the BAS will wait for the specified time, after which the task status check itself will begin. Most services recommend waiting at least 5 seconds.</div>
  <div class="tr tooltip-paragraph-fold">If the required service is not in the list of available ones, but it works through an API similar to the selected service, then you can specify required server URL in the <code>Service URL</code> field located in the additional settings.</div>
  <div class="tr tooltip-paragraph-fold">If you use programs such as <code>CapMonster</code>, <code>XEvil</code> or similar software, you must fill in the <code>Service URL</code> field in accordance with the documentation for this software.</div>
  <div class="tr tooltip-paragraph-last-fold">Detailed documentation for solving such captchas can be found <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://2captcha.com/2captcha-api#coordinates');return false">here</a>.</div>
</div>
<%= _.template($('#back').html())({ action: 'executeandadd', visible: true, name: tr('Solve captcha with clicks') }) %>