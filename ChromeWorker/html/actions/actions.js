_A = {  
   "loadcookiesfrombrowser":{  
      "name":"Load cookies from browser",
      "description":"Load cookies from browser",
      "template":"",
      "suggestion":{  
         "en":"autologin",
         "ru":"автовход"
      },
      "group":"httpclient"
   },
   "restrictpopups":{  
      "name":"Restrict popups",
      "description":"Restrict popups",
      "template":"",
      "suggestion":{  
         "en":"ad blocking",
         "ru":"блокировать рекламу"
      },
      "group":"network"
   },
   "allowpopups":{  
      "name":"Allow popups",
      "description":"Allow popups",
      "template":"",
      "group":"network"
   },
   "restrictdownloads":{  
      "name":"Restrict downloads",
      "description":"Restrict downloads",
      "template":"",
      "group":"network"
   },
   "allowdownloads":{  
      "name":"Allow downloads",
      "description":"Allow downloads",
      "template":"",
      "group":"network"
   },
   "loadcookiesfromhttpclient":{  
      "name":"Load cookies from http client",
      "description":"Load cookies from http client",
      "template":"",
      "suggestion":{  
         "en":"autologin",
         "ru":"автовход"
      },
      "group":"network"
   },
   "getthreadnumber":{  
      "name":"Thread index",
      "description":"Thread index",
      "template":"{{Save}}",
      "group":"ScriptStats"
   },
   "closebrowser":{  
      "name":"Close browser",
      "description":"Close browser",
      "template":"",
      "group":"manual"
   },
   "openbrowser":{  
      "name":"Open browser",
      "description":"Open browser",
      "template":"",
      "group":"manual"
   },
   "addtab":{  
      "name":"Add Tab",
      "description":"Add Tab",
      "template":"{{Url}}",
      "suggestion":{  
         "en":"new window, Referrer, Referer",
         "ru":"новое окно, реферрер, реферер"
      },
      "group":""
   },
   "tabinfo":{  
      "name":"Get information about tabs",
      "description":"Get information about tabs",
      "template":" -> {{UrlList}} -> {{CurrentIndex}}",
      "group":""
   },
   "selecttab":{  
      "name":"Select Tab",
      "description":"Select Tab",
      "template":"{{Index}}",
      "group":""
   },
   "closetab":{  
      "name":"Close Tab",
      "description":"Close Tab",
      "template":"{{Index}}",
      "suggestion":{  
         "en":"close window",
         "ru":"закрыть окно"
      },
      "group":""
   },
   "load":{  
      "name":"Load",
      "description":"Load specific url",
      "template":"{{LoadUrl}}",
      "group":"browser",
      "suggestion":{  
         "en":"Referrer, Referer",
         "ru":"реферрер, реферер"
      },
   },
   "navigateback":{  
      "name":"Previous Page",
      "description":"Load previous page",
      "template":"",
      "suggestion":{  
         "en":"browser history",
         "ru":"история браузера"
      },
      "group":"browser"
   },
   "url":{  
      "name":"Current Url",
      "description":"Get current url",
      "template":"{{SaveUrl}}",
      "group":"browser"
   },
   "screen":{  
      "name":"Get resolution and cursor position",
      "description":"Get resolution and cursor position",
      "template":"",
      "suggestion":{  
         "en":"mouse position, get browser size, get curor coordinates, mouse coordinates",
         "ru":"положение мыши, получить размер браузера, получить координаты курсора, координаты мыши"
      },
      "group":"browser"
   },
   "touch":{  
      "name":"Touch screen mode",
      "description":"Touch screen mode",
      "template":"{{Enable}}",
      "suggestion":{  
         "en":"swipe",
         "ru":"свайп, тач"
      },
      "group":"browser"
   },
   "html":{  
      "name":"Page Html",
      "description":"Save page html to variable",
      "template":"{{Save}}",
      "suggestion":{  
         "en":"page source, html code",
         "ru":"html страницы, хтмл код"
      },
      "group":"browser"
   },
   "type":{  
      "name":"Type",
      "description":"Type text on page. For example, put email in input field",
      "template":"{{TypeData}}",
      "suggestion":{  
         "en":"press esc, press escape, press enter, press key, enter key",
         "ru":"печать текста, напечатать текст, ввести текст, нажать кнопку, прокрутка страницы, прокрутить страницу, нажать энтер, нажать клавишу"
      },
      "group":"browser"
   },
   "resize":{  
      "name":"Resize",
      "description":"Change browser size",
      "template":"{{ResizeX}} : {{ResizeY}}",
      "suggestion":{  
         "en":"browser resolution, window resolution, window width, window height, browser width, browser height",
         "ru":"разрешение браузера, ширина браузера, высота браузера, разрешение окна, ширина окна, высота окна"
      },
      "group":"browser"
   },
   "proxy":{  
      "name":"Proxy",
      "description":"Set proxy",
      "template":"{{ProxyText}}",
      "suggestion":{  
         "en":"Change ip, Socks, change browser language, change timezone",
         "ru":"Изменить ип, Сменить ип, Socks, Сокс, изменить язык браузера, изменить часовой пояс"
      },
      "group":"browser"
   },
   "javascript": {  
      "name": "Javascript (deprecated)",
      "description": "Execute javascript",
      "template": "{{Value}} -> {{Save}}",
      "suggestion": {  
         "en": "Execute on page, Access page",
         "ru": "Яваскрипт, Выполнить на странице, Доступ к странице"
      },
      "group": "browser"
   },
   "browserjavascript": {  
      "name": "Javascript",
      "description": "Execute javascript",
      "template":" {{Code}}",
      "suggestion": {  
         "en": "Execute on page, Access page",
         "ru": "Яваскрипт, Выполнить на странице, Доступ к странице"
      },
      "group": "browser"
   },
   "onloadjavascript":{  
      "name":"Execute On Every Page Load In Browser",
      "description":"Execute javascript on every page load in browser",
      "template":"{{Value}}",
      "suggestion":{  
         "en":"Execute on page, Access page, inject code",
         "ru":"Яваскрипт, Выполнить на странице, Доступ к странице, внедрить код"
      },
      "group":"browser"
   },
   "reset":{  
      "name":"Reset",
      "description":"Clear all browser data",
      "template":"",
      "suggestion":{  
         "en":"restart",
         "ru":"перезапуск"
      },
      "group":"browser"
   },
   "openfile":{  
      "name":"Open File Result",
      "description":"Set open file dialog result",
      "template":"{{Value}}",
      "suggestion":{  
         "en":"upload file",
         "ru":"загрузить файл"
      },
      "group":"browser"
   },
   "dragfile":{  
      "name":"Start Drag File",
      "description":"Start Drag File",
      "template":"{{Value}}",
      "group":"browser"
   },
   "promptresult":{  
      "name":"Prompt Result",
      "description":"Set prompt result",
      "template":"{{Value}}",
      "group":"browser"
   },
   "httpauth":{  
      "name":"Http Auth",
      "description":"Set http auth login and password",
      "template":"{{Login}}:{{Password}}",
      "suggestion":{  
         "en":"http authentication",
         "ru":"хттп авторизация"
      },
      "group":"browser"
   },
   "scroll":{  
      "name":"Scroll",
      "description":"Scroll to position",
      "template":"{{X}},{{Y}}",
      "suggestion":{  
         "en":"page scrolling",
         "ru":"прокрутка страницы, скролить страницу"
      },
      "group":"browser"
   },
   "renderscreen":{  
      "name":"Render",
      "description":"Render screen part to variable as png base64 encoded",
      "template":"{{X}},{{Y}},{{Width}},{{Height}} -> {{Save}}",
      "suggestion":{  
         "en":"make screen, make screenshot, render image, render element",
         "ru":"сделать скрин, сделать скриншот, сохранить изображение, сохранить элемент, рендер элемента"
      },
      "group":"browser"
   },
   "clickCaptchaBrowser":{
      "name":"Solve captcha with clicks",
      "description":"Solve captcha with clicks",
      "template":"{{imageData}} -> {{clickCoordinatesList}}",
      "suggestion":{  
         "en":"solve captcha, solve image",
         "ru":"решить капчу, решить картинку"
      },
      "group":"browser"
   },
   "solvecaptcha":{  
      "name":"Solve Captcha",
      "description":"Solve captcha",
      "template":"{{Value}} -> {{Save}}",
      "suggestion":{  
         "en":"solve capcha, solve image",
         "ru":"решить каптчу, решить картинку"
      },
      "group":"browser"
   },
   "solverfailed":{  
      "name":"Captcha Failed",
      "description":"Captcha Failed",
      "template":"",
      "group":"browser"
   },
   "timeout":{  
      "name":"Timeout",
      "description":"Set Timeout",
      "template":"{{Value}}",
      "suggestion":{  
         "en":"max action execution time",
         "ru":"максимальное время ожидания, максимальное время действия"
      },
      "group":"browser"
   },
   "mousesettings":{  
      "name":"Mouse Settings",
      "description":"Mouse Settings",
      "template":"{{Speed}}",
      "suggestion":{  
         "en":"mouse speed",
         "ru":"скорость мыши"
      },
      "group":"browser"
   },
   "notification":{  
      "name":"Notifications",
      "description":"Browser notifications",
      "template":"{{AllowNotifications}}",
      "group":"browser"
   },
   "browsersettings":{  
      "name":"Browser Settings",
      "description":"Browser Settings",
      "template":"",
      "suggestion":{  
         "en":"skip frames, browser options",
         "ru":"пропуск кадров, свойства браузера"
      },
      "group":"browser"
   },
   "getbrowserversionlist":{  
      "name":"Get list of available browser versions",
      "description":"Get list of available browser versions",
      "template":"{{Save}}",
      "suggestion":{  
         "en":"chrome version",
         "ru":"версия chrome"
      },
      "group":"browser"
   },
   "getcurrentbrowserversion":{  
      "name":"Get current browser version",
      "description":"Get current browser version",
      "template":"{{Save2}}",
      "suggestion":{  
         "en":"chrome version",
         "ru":"версия chrome"
      },
      "group":"browser"
   },
   "clickextensionbutton":{  
      "name":"Click extension button",
      "description":"Click extension button",
      "template":"{{ExtensionId}}",
      "suggestion":{  
         "en":"extension",
         "ru":"расширение"
      },
      "group":"browser"
   },
   "if":{  
      "name":"If",
      "description":"Execute conditional actions",
      "template":"{{IfExpression}}",
      "suggestion":{  
         "en":"condition, else, execute if condition",
         "ru":"условие, если, иф, иначе, выполнить если"
      },
      "group":"logic"
   },
   "while":{  
      "name":"While",
      "description":"Execute while loop",
      "template":"{{WhileExpression}}",
      "suggestion":{  
         "en":"cycle, loop, for, until, execute several times",
         "ru":"цикл, фор, выполнять пока не будет выполнено условие, вайл, перебор, выполнить несколько раз"
      },
      "group":"logic"
   },
   "for":{  
      "name":"For",
      "description":"Execute for loop",
      "template":"{{ForFrom}} : {{ForTo}}",
      "suggestion":{  
         "en":"cycle, loop, for, until, execute several times",
         "ru":"цикл, фор, выполнять пока не будет выполнено условие, вайл, перебор, выполнить несколько раз"
      },
      "group":"logic"
   },
   "foreach":{  
      "name":"Foreach",
      "description":"Execute foreach loop",
      "template":"{{ForArray}}",
      "suggestion":{  
         "en":"cycle, loop, for, until, traverse list, iterate list, execute several times",
         "ru":"цикл, фор, выполнять пока не будет выполнено условие, вайл, перебор списка, выполнить несколько раз"
      },
      "group":"logic"
   },
   "break":{  
      "name":"Break",
      "description":"Stop loop",
      "template":"",
      "suggestion":{  
         "en":"stop cycle, cycle interrupt, loop interrupt",
         "ru":"остановить цикл, прервать цикл"
      },
      "group":"logic"
   },
   "continue":{  
      "name":"Continue",
      "description":"Next loop iteration",
      "template":"",
      "suggestion":{  
         "en":"resume loop, resume cycle",
         "ru":"продолжить цикл"
      },
      "group":"logic"
   },
   "success":{  
      "name":"Success",
      "class":"logic",
      "description":"Finish script with success result",
      "template":"{{SuccessMessage}}",
      "suggestion":{  
         "en":"end thread, stop thread",
         "ru":"завершить поток, прервать поток"
      },
      "group":"logic"
   },
   "fail":{  
      "name":"Fail",
      "class":"logic",
      "description":"Finish script with fail result",
      "template":"{{FailMessage}}",
      "suggestion":{  
         "en":"end thread, stop thread",
         "ru":"завершить поток, прервать поток"
      },
      "group":"logic"
   },
   "die":{  
      "name":"End Script",
      "class":"logic",
      "description":"Immediately End Script",
      "template":"{{FailMessage}}",
      "suggestion":{  
         "en":"end script, stop script",
         "ru":"завершить скрипт, прервать скрипт"
      },
      "group":"logic"
   },
   "goto":{  
      "name":"Move to label",
      "class":"logic",
      "description":"Move to label",
      "template":"<~ {{LabelName}}",
      "suggestion":{  
         "en":"goto, move execution point",
         "ru":"переместить точку выполнения"
      },
      "group":"logic"
   },
   "label":{  
      "name":"Set label",
      "description":"Set label",
      "template":"~> {{Label}}",
      "suggestion":{  
         "en":"goto, move execution point",
         "ru":"переместить точку выполнения"
      },
      "group":"logic"
   },
   "ignoreerrors":{  
      "name":"Ignore Errors",
      "description":"Ignore errors",
      "template":"",
      "suggestion":{  
         "en":"catch, try, catch errors",
         "ru":"catch, try, catch errors"
      },
      "group":"logic"
   },
   "setvariable":{  
      "name":"Set Variable",
      "description":"Sets or creates variable",
      "template":"{{SetVariableValue}} -> {{SetVariableName}}",
      "group":"logic"
   },
   "incvariable":{  
      "name":"Increase Variable",
      "description":"Increase Variable",
      "template":"{{IncVariableValue}} -> {{SetVariableName}}",
      "group":"logic"
   },
   "incglobalvariable":{  
      "name":"Increase Global Variable",
      "description":"Increase Global Variable",
      "template":"{{IncVariableValue}} -> {{SetVariableName}}",
      "group":"logic"
   },
   "executefunction":{  
      "name":"Call Function",
      "class":"logic",
      "description":"Call function",
      "template":"{{FunctionName}}",
      "group":"logic"
   },
   "getfunctionparameter":{  
      "name":"Get Function Parameter",
      "class":"logic",
      "description":"Get Function Parameter",
      "template":"{{ParameterName}} -> {{Save}}",
      "group":"logic"
   },
   "recaptchav3":{  
      "name":"Recaptcha V3",
      "class":"browser",
      "description":"Recaptcha V3",
      "template":"{{Action}}",
      "group":"browser"
   },
   "return":{  
      "name":"Return",
      "class":"logic",
      "description":"Return",
      "template":"{{ReturnValue}}",
      "group":"logic"
   },
   "executefunctioninseveralthreads":{  
      "name":"Call function in several threads",
      "class":"logic",
      "description":"Call function in several threads",
      "template":"tr(Function) -> strong(({{FunctionName}}))\ntr(Thread number) -> {{ThreadNumber}}\ntr(Success number) -> {{SuccessNumber}}\ntr(Fail number) -> {{FailNumber}}",
      "group":"logic"
   },
   "log":{  
      "name":"Log",
      "description":"Output string to log",
      "template":"{{LogText}}",
      "suggestion":{  
         "en":"output text, print text, output data",
         "ru":"вывод текста, печать текста, вывод данных"
      },
      "group":"tools"
   },
   "clearlog":{  
      "name":"Clear Log",
      "description":"Clear Log",
      "template":"",
      "suggestion":{  
         "en":"delete log",
         "ru":"удалить лог"
      },
      "group":"tools"
   },
   "result":{  
      "name":"Result",
      "description":"Save result",
      "template":"{{Value}}",
      "suggestion":{  
         "en":"output data",
         "ru":"вывод данных"
      },
      "group":"tools"
   },
   "custom":{  
      "name":"Execute code",
      "description":"Execute custom code",
      "template":"{{Code}}",
      "suggestion":{  
         "en":"execute javascript code, run javascript",
         "ru":"выполнить яваксрипт, код джаваскрипт, выполнить код"
      },
      "group":"tools"
   },
   "globalset":{  
      "name":"Set Global Variable",
      "description":"Set global variable",
      "template":"{{Value}} -> {{Name}}",
      "group":"logic"
   },
   "parseline":{  
      "name":"Parse Line (deprecated)",
      "description":"Parse line like this 'a1:a2:a3' and save to variables",
      "template":"{{Value}} -> {{VariablesList}}",
      "suggestion":{  
         "en":"split line",
         "ru":"разбить строку, разделить строку"
      },
      "group":"tools"
   },
   "randomint":{  
      "name":"Random Number",
      "description":"Generate random integer",
      "template":"{{Save}}",
      "group":"tools"
   },
   "setheader":{  
      "name":"Set Header",
      "description":"Set HTTP Headers, like user agent, accept language, etc",
      "template":"{{Value}} -> {{Name}}",
      "suggestion":{  
         "en":"User agent, Accept Language",
         "ru":"юзер агент"
      },
      "group":"network"
   },
   "getcookiesforurl":{  
      "name":"Get Cookies For Url",
      "description":"Get cookies for url",
      "template":"{{Value}} -> {{Save}}",
      "suggestion":{  
         "en":"autologin",
         "ru":"кукисы, автовход"
      },
      "group":"network"
   },
   "savecookies":{  
      "name":"Save Cookies",
      "description":"Save all cookies to variable",
      "template":"{{Save}}",
      "suggestion":{  
         "en":"autologin",
         "ru":"кукисы, автовход"
      },
      "group":"network"
   },
   "restorecookies":{  
      "name":"Restore Cookies",
      "description":"Restore cookies, which is saved with Save Cookies command",
      "template":"{{Value}}",
      "suggestion":{  
         "en":"autologin",
         "ru":"кукисы, автовход"
      },
      "group":"network"
   },
   "cachemaskallow":{  
      "name":"Cache Mask Allow",
      "description":"Allow save loaded url content to cache",
      "template":"{{Value}}",
      "group":"network"
   },
   "cachemaskdeny":{  
      "name":"Cache Mask Deny",
      "description":"Deny save loaded url content to cache",
      "template":"{{Value}}",
      "group":"network"
   },
   "requestmaskallow":{  
      "name":"Request Mask Allow",
      "description":"Allow load url, which matching mask",
      "template":"{{Value}}",
      "group":"network"
   },
   "requestmaskdeny":{  
      "name":"Request Mask Deny",
      "description":"Deny load url, which matching mask",
      "template":"{{Value}}",
      "suggestion":{  
         "en":"images restrict, stop loading image",
         "ru":"запретить запрос, запросы загружать картинки"
      },
      "group":"network"
   },
   "cleardata":{  
      "name":"Clear Cached Data",
      "description":"Clear loaded cache data",
      "template":"",
      "group":"network"
   },
   "clearmasks":{  
      "name":"Clear Cache Masks",
      "description":"Clear cache masks",
      "template":"",
      "group":"network"
   },
   "getstatus":{  
      "name":"Get Status",
      "description":"Get url load status",
      "template":"{{Value}} -> {{Save}}",
      "group":"network"
   },
   "isloaded":{  
      "name":"Is Loaded",
      "description":"Check if url is loaded",
      "template":"{{Value}} -> {{Save}}",
      "group":"network"
   },
   "getfromcache":{  
      "name":"Get last item from cache",
      "description":"Get base64 encoded cached page",
      "template":"{{Value}} -> {{Save}}",
      "suggestion":{  
         "en":"get request content, get request headers",
         "ru":"получить содержание запроса, получить хедеры запроса"
      },
      "group":"network"
   },
   "getallcache":{  
      "name":"Get all items from cache",
      "description":"Get base64 encoded cached page",
      "template":"{{Value}} -> {{Save}}",
      "suggestion":{  
         "en":"get request content, get request headers",
         "ru":"получить содержание запроса, получить хедеры запроса"
      },
      "group":"network"
   },
   "waitfullload":{  
      "name":"Wait Full Page Load",
      "description":"Wait for full page load",
      "template":"",
      "group":"waiters"
   },
   "waitload":{  
      "name":"Wait Url Loaded",
      "description":"Wait until browser will load url",
      "template":"{{Wait}}",
      "group":"waiters"
   },
   "waiturl":{  
      "name":"Wait Address Bar",
      "description":"Wait address bar will contain url",
      "template":"{{Wait}}",
      "group":"waiters"
   },
   "waitcontent":{  
      "name":"Wait Text",
      "description":"Wait until specific text will be present on page",
      "template":"{{Wait}}",
      "group":"waiters"
   },
   "waitcss":{  
      "name":"Wait Css",
      "description":"Wait until specific css selector will return non empty result",
      "template":"{{Wait}}",
      "group":"waiters"
   },
   "waitfiledownload":{  
      "name":"Wait File Download",
      "description":"Wait file will download",
      "template":"{{Wait}}",
      "suggestion":{  
         "en":"get download file path",
         "ru":"получить путь загруженного файла"
      },
      "group":"waiters"
   },
   "sleep":{  
      "name":"Sleep",
      "description":"Sleep for a specific time",
      "template":"{{sleepfromto}}",
      "suggestion":{  
         "en":"wait, idle",
         "ru":"ждать, бездействие"
      },
      "group":"waiters"
   },
   "httpclientproxy":{  
      "name":"HTTP-Client Proxy",
      "description":"Set proxy for http client",
      "template":"{{ProxyText}}",
      "group":"httpclient"
   },
   "httpclientreset":{  
      "name":"HTTP-Client Reset",
      "description":"Reset http client",
      "template":"",
      "group":"httpclient"
   },
   "httpclientget":{  
      "name":"Get",
      "description":"Make GET request",
      "template":"{{Value}}",
      "group":"httpclient"
   },
   "httpclientpost":{  
      "name":"Post",
      "description":"Make POST request",
      "template":"{{Value}}",
      "group":"httpclient"
   },
   "httpclientdownload":{  
      "name":"Download",
      "description":"Download to file",
      "template":"{{Value}} -> {{File}}",
      "group":"httpclient"
   },
   "httpclienturl":{  
      "name":"Current Url",
      "description":"Get http client current url",
      "template":"{{Save}}",
      "group":"httpclient"
   },
   "httpclientcontent":{  
      "name":"Content",
      "description":"Get http client request result",
      "template":"{{Save}}",
      "group":"httpclient"
   },
   "httpclientstatus":{  
      "name":"Status",
      "description":"Get http client request status",
      "template":"{{Save}}",
      "group":"httpclient"
   },
   "httpclientgetheader":{  
      "name":"Get Header",
      "description":"Get http client header",
      "template":"{{Value}} -> {{Save}}",
      "group":"httpclient"
   },
   "httpclientsetheader":{  
      "name":"HTTP-Client Set Header",
      "description":"Set http client header",
      "template":"{{Value}} -> {{Name}}",
      "group":"httpclient"
   },
   "httpclientclearheader":{  
      "name":"Clear Headers",
      "description":"Clear http client request headers",
      "template":"",
      "group":"httpclient"
   },
   "httpclientgetcookiesforurl":{  
      "name":"Get Cookies For Url",
      "description":"Get cookies for url",
      "template":"{{Value}} -> {{Save}}",
      "group":"httpclient"
   },
   "httpclientsavecookies":{  
      "name":"HTTP-Client Save Cookies",
      "description":"Save all cookies to variable",
      "template":"{{Save}}",
      "group":"httpclient"
   },
   "httpclientrestorecookies":{  
      "name":"HTTP-Client Restore Cookies",
      "description":"Restore cookies, which is saved with Save Cookies command",
      "template":"{{Value}}",
      "group":"httpclient"
   },
   "httpclientfailonerror":{  
      "name":"Set Fail On Error",
      "description":"Fail if error during http request",
      "template":"{{SetFailed}}",
      "group":"httpclient"
   },
   "httpclientiserror":{  
      "name":"Is Error",
      "description":"Check if error",
      "template":"{{Save}}",
      "group":"httpclient"
   },
   "httpclientgeterrorstring":{  
      "name":"Get Error String",
      "description":"Get last error string",
      "template":"{{Save}}",
      "group":"httpclient"
   },
   "httpclientxpathxml":{  
      "name":"Xpath Get Xml",
      "description":"Execute xpath query and get result xml",
      "template":"{{Value}} -> {{Save}}",
      "group":"httpclient"
   },
   "httpclientxpathxmllist":{  
      "name":"Xpath Get Xml List",
      "description":"Execute xpath query and get result xml list",
      "template":"{{Value}} -> {{Save}}",
      "group":"httpclient"
   },
   "httpclientxpathtext":{  
      "name":"Xpath Get Text",
      "description":"Execute xpath query and get result text",
      "template":"{{Value}} -> {{Save}}",
      "group":"httpclient"
   },
   "httpclientxpathtextlist":{  
      "name":"Xpath Get Text List",
      "description":"Execute xpath query and get result text list",
      "template":"{{Value}} -> {{Save}}",
      "group":"httpclient"
   },
   "httpclientxpathcount":{  
      "name":"Xpath Get Count",
      "description":"Execute xpath query and get result count",
      "template":"{{Value}} -> {{Save}}",
      "group":"httpclient"
   },
   "httpclientxpathexists":{  
      "name":"Xpath Get Exists",
      "description":"Execute xpath query and check if exists",
      "template":"{{Value}} -> {{Save}}",
      "group":"httpclient"
   },
   "click":{  
      "name":"Click",
      "class":"browser",
      "description":"Click",
      "template":"{{ClickX}},{{ClickY}}",
      "group":""
   },
   "move":{  
      "name":"Move",
      "class":"browser",
      "description":"Move",
      "template":"{{MoveX}},{{MoveY}}",
      "group":""
   },
   "drag":{  
      "name":"Drag",
      "class":"browser",
      "description":"Drag",
      "template":"{{X}},{{Y}}",
      "group":""
   },
   "drop":{  
      "name":"Drop",
      "class":"browser",
      "description":"Drop",
      "template":"{{X}},{{Y}}",
      "group":""
   },
   "dragelement":{  
      "name":"Drag On Element",
      "class":"browser",
      "description":"Drag On Element",
      "template":"{{PATH}}",
      "group":""
   },
   "dropelement":{  
      "name":"Drop On Element",
      "class":"browser",
      "description":"Drop On Element",
      "template":"{{PATH}}",
      "group":""
   },
   "moveandclick":{  
      "name":"Move And Click",
      "class":"browser",
      "description":"Move And Click",
      "template":"{{X}},{{Y}}",
      "group":""
   },
   "loopelement":{  
      "name":"Start Loop",
      "class":"browser",
      "description":"Start Loop",
      "template":"{{PATH}}",
      "group":""
   },
   "waitelement":{  
      "name":"Wait While Element Exists",
      "class":"browser",
      "description":"Wait While Element Exists",
      "template":"{{PATH}}",
      "group":""
   },
   "xml":{  
      "name":"Get Element Content",
      "class":"browser",
      "description":"Get Element Content",
      "template":"{{PATH}} -> {{SaveXml}}",
      "group":""
   },
   "linkurl":{  
      "name":"Get link URL",
      "class":"browser",
      "description":"Get link URL",
      "template":"{{PATH}} -> {{LinkURL}}",
      "group":""
   },
   "text":{  
      "name":"Get Element Text",
      "class":"browser",
      "description":"Get Element Text",
      "template":"{{PATH}} -> {{SaveText}}",
      "group":""
   },
   "script":{  
      "name":"Execute Javascript On Element (deprecated)",
      "class":"browser",
      "description":"Execute Javascript On Element",
      "template":"{{ScriptValue}} -> {{PATH}}",
      "group":""
   },
   "browserscript":{  
      "name":"Execute Javascript On Element",
      "class":"browser",
      "description":"Execute Javascript On Element",
      "template":"{{ScriptValue}} -> {{PATH}}",
      "group":""
   },
   "clickelement":{  
      "name":"Click On Element",
      "class":"browser",
      "description":"Click On Element",
      "template":"{{PATH}}",
      "group":""
   },
   "moveelement":{  
      "name":"Move On Element",
      "class":"browser",
      "description":"Move On Element",
      "template":"{{PATH}}",
      "group":""
   },
   "moveandclickelement":{  
      "name":"Move And Click On Element",
      "class":"browser",
      "description":"Move And Click On Element",
      "template":"{{PATH}}",
      "group":""
   },
   "clear":{  
      "name":"Clear Edit",
      "class":"browser",
      "description":"Clear Edit",
      "template":"{{PATH}}",
      "group":""
   },
   "typeelement":{  
      "name":"Type Text",
      "class":"browser",
      "description":"Type Text",
      "template":"{{TypeData}} -> {{PATH}}",
      "group":""
   },
   "exist":{  
      "name":"Is Element Exists",
      "class":"browser",
      "description":"Is Element Exists",
      "template":"{{PATH}} -> {{Save}}",
      "group":""
   },
   "style":{  
      "name":"Get Element Style",
      "class":"browser",
      "description":"Get Element Style",
      "template":"{{PATH}} -> {{Save}}",
      "group":""
   },
   "check":{  
      "name":"Set Checkbox",
      "class":"browser",
      "description":"Set Checkbox",
      "template":"{{PATH}}",
      "group":""
   },
   "focus":{  
      "name":"Scroll To Element",
      "class":"browser",
      "description":"Scroll To Element",
      "template":"{{PATH}}",
      "group":""
   },
   "set":{  
      "name":"Set Combobox Value",
      "class":"browser",
      "description":"Set Combobox Value",
      "template":"{{Value}} -> {{PATH}}",
      "group":""
   },
   "set_integer":{  
      "name":"Set Combobox Index",
      "class":"browser",
      "description":"Set Combobox Index",
      "template":"{{Value}} -> {{PATH}}",
      "group":""
   },
   "set_random":{  
      "name":"Set Combobox To Random",
      "class":"browser",
      "description":"Set Combobox To Random",
      "template":"{{PATH}}",
      "group":""
   },
   "attr":{  
      "name":"Get Element Attribute",
      "class":"browser",
      "description":"Get Element Attribute",
      "template":"{{PATH}} -> {{Save}}",
      "group":""
   },
   "set_attr":{  
      "name":"Set Element Attribute",
      "class":"browser",
      "description":"Set Element Attribute",
      "template":"{{Value}} -> {{PATH}}",
      "group":""
   },
   "captcha":{  
      "name":"Solve Captcha",
      "class":"browser",
      "description":"Solve Captcha",
      "template":"{{PATH}} -> {{Save}}",
      "group":""
   },
   "screenshot":{  
      "name":"Screenshot",
      "class":"browser",
      "description":"Screenshot",
      "template":"{{PATH}} -> {{Save}}",
      "group":""
   },
   "coordinates":{  
      "name":"Get Coordinates",
      "class":"browser",
      "description":"Get Coordinates",
      "template":"{{PATH}}",
      "group":""
   },
   "length":{  
      "name":"Get Element Count",
      "class":"browser",
      "description":"Get Element Count",
      "template":"{{PATH}} -> {{Save}}",
      "group":""
   }
}



_G = {
   "browser": {icon:  "../icons/browser.png", info: "All actions to work with browser, which do not require a specific element for use. For example, load the url, set proxy, make a screenshot, etc. To click on an element or enter text in a specific field, click on this item and select an action from the menu."},
   "logic": {icon:  "../icons/logic.png", info: "Actions, which defines script logic: loops, conditions, variable declarations."},
   "tools": {icon:  "../icons/tools.png", info: "Useful tools to work with strings and execute code."},
   "network": {icon:  "../icons/network.png", info: "All actions to interact with browser network requests. Deny request to certain urls, set user agent and other headers, access to cache."},
   "waiters": {icon:  "../icons/waiters.png", info: "Wait for certain events or certain time."},
   "email": {icon:  "../icons/email.png", info: "Receive email and search in mailbox."},
   "httpclient": {icon:  "../icons/httpclient.png", info: "HTTP client, very fast alternative to browser."},
   "manual": {icon:  "../icons/manual.png", info: "Running browser is very resource-intensive task. This module can create and destroy browser during thread lifetime and thereby save some memory and processor time."}
}


_AL = 
{
   "Get information about tabs": {"ru": "Получить информацию о вкладках"},
   "Previous Page": {"ru": "Предыдущая Страница"},
   "Clear Log": {"ru": "Очистить лог"},
   "Browser Settings": {"ru": "Настройки Браузера"},
   "Click extension button": {"ru": "Нажать кнопку расширения"},
   "All actions to work with browser, which do not require a specific element for use. For example, load the url, set proxy, make a screenshot, etc. To click on an element or enter text in a specific field, click on this item and select an action from the menu.": {"ru": "Все действия по работе с браузером, которые не требуют конкретного элемента для применения. Например, загрузить урл, установить прокси, сделать скриншот, и т. д. Чтобы кликнуть по элементу или ввести текст в конкретное поле, кликните по этому элементу и выберите действие из меню."},
   "Actions, which defines script logic: loops, conditions, variable declarations.": {"ru": "Действия, которые определяют логику скрипта: циклы, условия, объявления переменных."},
   "Useful tools to work with strings and execute code.": {"ru": "Полезные инструменты для работы со строками и выполнения кода."},
   "All actions to interact with browser network requests. Deny request to certain urls, set user agent and other headers, access to cache.": {"ru": "Все действия для взаимодействия с сетевыми запросами браузера. Отклонить запрос к определенным урл, установить юзер агент и другие заголовки, получить доступ к кешу."},
   "Wait for certain events or certain time.": {"ru": "Ждать определенных событий на странице или спать заданное время."},
   "Receive email and search in mailbox.": {"ru": "Получать электронную почту и искать письма в почтовом ящике."},
   "HTTP client, very fast alternative to browser.": {"ru": "HTTP-клиент, очень быстрая альтернатива браузеру."},
   "Running browser is very resource-intensive task. This module can create and destroy browser during thread lifetime and thereby save some memory and processor time.": {"ru": "Запуск браузера является очень ресурсоемкой задачей. Этот модуль может создавать и удалять браузер в течение работы потока и тем самым экономить часть памяти и процессорного времени."},

   "Get list of available browser versions": {"ru": "Получить список доступных версий браузера"},
   "Get current browser version": {"ru": "Получить текущую версию браузера"},

   "Screenshot": {"ru": "Скриншот"},
   "Thread number": {"ru": "Количество потоков"},
   "Success number": {"ru": "Успешных выполнений"},
   "Fail number": {"ru": "Неудачных выполнений"},
   "Mouse Settings": {"ru": "Настройки Мыши"},
   "Notifications": {"ru": "Уведомления"},
   "Restrict popups": {"ru": "Запретить всплывающие окна"},
   "Allow popups": {"ru": "Разрешить всплывающие окна"},
   "Restrict downloads": {"ru": "Запретить загрузку файлов"},
   "Allow downloads": {"ru": "Разрешить загрузку файлов"},
   "Load cookies from browser": {"ru": "Загрузить cookies из браузера"},
   "Load cookies from http client": {"ru": "Загрузить cookies из http клиента"},
   "Thread index": {"ru": "Номер потока"},
   "Close browser": {"ru": "Закрыть браузер"},
   "Open browser": {"ru": "Открыть браузер"},
   "Open/close browser": {"ru": "Открыть/закрыть браузер"},
   
   "Get Coordinates" : {"ru": "Получить координаты"},
   "Drag" : {"ru": "Начать Перетаскивание"},
   "Drop" : {"ru": "Закончить Перетаскивание"},
   "Drag On Element" : {"ru": "Начать Перетаскивание На Элементе"},
   "Drop On Element" : {"ru": "Закончить Перетаскивание На Элементе"},
   "Add Tab" : {"ru": "Добавить вкладку"},
   "Close Tab" : {"ru": "Закрыть вкладку"},
   "Select Tab" : {"ru": "Переключиться на вкладку"},

   "Load " : {"ru": "Загрузить "},
   "Wait while element ": {"ru": "Ждать пока элемент "},
   " appear on page": {"ru": " появится на странице"},
   "HTTP-Client Proxy": {"ru": "HTTP-Клиент Прокси"},
   "HTTP-Client Reset": {"ru": "HTTP-Клиент Сброс"},
   "HTTP-Client Set Header": {"ru": "HTTP-Клиент Установить Заголовок"},
   "HTTP-Client Save Cookies": {"ru": "HTTP-Клиент Сохранить cookies"},
   "HTTP-Client Restore Cookies": {"ru": "HTTP-Клиент Загрузить cookies"},
   "Label": {"ru": "Метка"},
   "Set label": {"ru": "Установить метку"},
   "Move to label": {"ru": "Перейти к метке"},

   "Click" : {"ru": "Кликнуть"},
   "Move" : {"ru": "Двигать Мышь"},
   "Move And Click" : {"ru": "Двигать Мышь И Кликнуть"},
   "Start Loop" : {"ru": "Начать Цикл"},
   "Wait While Element Exists" : {"ru": "Ждать Появления Элемента"},
   "Get Element Content" : {"ru": "Получить Код Элемента"},
   "Get Element Text" : {"ru": "Получить Текст"},
   "Execute Javascript On Element" : {"ru": "Выполнить Javascript На Элементе"},
   "Execute Javascript On Element (deprecated)" : {"ru": "Выполнить Javascript На Элементе (устаревшее)"},
   "Click On Element" : {"ru": "Кликнуть На Элемент"},
   "Move On Element" : {"ru": "Двигать Мышь Над Элементом"},
   "Move And Click On Element" : {"ru": "Двигать Мышь И Кликнуть На Элемент"},
   "Clear Edit" : {"ru": "Очистить Поле"},
   "Type Text" : {"ru": "Ввод текста"},
   "Is Element Exists" : {"ru": "Проверить Существование"},
   "Get Element Style" : {"ru": "Получить Стиль Элемента"},
   "Set Checkbox" : {"ru": "Установить Чекбокс"},
   "Scroll To Element" : {"ru": "Прокрутка К Элементу"},
   "Set Combobox Value" : {"ru": "Установить Значение Комбобокса"},
   "Set Combobox Index" : {"ru": "Установить Индекс Комбобокса"},
   "Set Combobox To Random" : {"ru": "Выбрать Случайный Элемент Из Комбобокса"},
   "Get Element Attribute" : {"ru": "Получить Атрибут Элемента"},
   "Set Element Attribute" : {"ru": "Установить Атрибут Элемента"},
   "Get Element Count" : {"ru": "Получить Количество Элементов"},

   "Else" : {"ru": "Else"},
   "Initialize" : {"ru": "Инициализация"},
   "Load" : {"ru": "Загрузить"},
   "Current Url" : {"ru": "Текущий Url"},
   "Get resolution and cursor position" : {"ru": "Получить разрешение и положение курсора"},
   "Touch screen mode" : {"ru": "Режим сенсорного экрана"},
   
   "Page Html" : {"ru": "Код страницы"},
   "Type" : {"ru": "Ввод текста"},
   "Resize" : {"ru": "Разрешение"},
   "Proxy" : {"ru": "Прокси"},
   "Javascript" : {"ru": "Яваскрипт"},
   "Javascript (deprecated)" : {"ru": "Яваскрипт (устаревшее)"},
   "Execute On Every Page Load In Browser" :{"ru": "Выполнять при каждой загрузке страницы в браузере"},
   "Reset" : {"ru": "Сброс"},
   "Open File Result" : {"ru": "Диалог Открыть Файл"},
   "Start Drag File" : {"ru": "Начать Перетягивать Файл"},
   "Prompt Result": {"ru": "Окно \"prompt\""},
   "Http Auth": {"ru": "HTTP Аутентификация"},
   "Scroll" : {"ru": "Прокрутка"},
   "Render" : {"ru": "Скриншот"},
   "Solve Captcha" : {"ru": "Решить Капчу"},
   "Captcha Failed": {"ru": "Решено Неверно"},
   "Solve captcha with clicks": {"ru": "Решить капчу кликами"},
   "Timeout": {"ru": "Таймаут"},

   "If" : {"ru": "If"},
   "Get Function Parameter" : {"ru": "Получить Параметр Функции"},
   "While" : {"ru": "While"},
   "For" : {"ru": "For"},
   "Break" : {"ru": "Break"},
   "Success" : {"ru": "Success"},
   "Fail" : {"ru": "Fail"},
   "End Script": {"ru": "Прервать Скрипт"},
   "Ignore Errors": {"ru": "Игнорировать Ошибки"},
   "Set Variable" : {"ru": "Установить переменную"},
   "Increase Variable": {"ru": "Увеличить переменную"},
   "Increase Global Variable": {"ru": "Увеличить глобальную переменную"},
   "Call Function" : {"ru": "Вызов функции"},
   "Call function in several threads" : {"ru": "Вызов функции в несколько потоков"},
   "Log" : {"ru": "Лог"},
   "Result" : {"ru": "Результат"},
   "Execute code" : {"ru": "Выполнить код"},
   "Set Global Variable" : {"ru": "Установить глобальную переменную"},
   "Get Global Variable" : {"ru": "Читать глобальную переменную"},
   "Parse Line (deprecated)" : {"ru": "Парсить строку (устаревшее)"},
   "Random Number": {"ru": "Случайное Число"},

   "Set Header" : {"ru": "Установить заголовок"},
   "Get Cookies For Url" : {"ru": "Получить cookies для url"},
   "Save Cookies" : {"ru": "Сохранить cookies"},
   "Restore Cookies" : {"ru": "Загрузить cookies"},
   "Cache Mask Allow" : {"ru": "Разрешить кэш"},
   "Cache Mask Deny" : {"ru": "Запретить кэш"},
   "Request Mask Allow" : {"ru": "Разрешать запрос"},
   "Request Mask Deny" : {"ru": "Запретить запрос"},
   "Clear Cached Data" : {"ru": "Очистить кэшированные данные"},
   "Clear Cache Masks" : {"ru": "Очистить маски"},
   "Get Status" : {"ru": "Получить статус запроса"},
   "Is Loaded" : {"ru": "Был ли запрос"},
   "Get last item from cache" : {"ru": "Получить последний элемент кэша"},
   "Get all items from cache" : {"ru": "Получить все элементы кэша"},
   "Wait Full Page Load": {"ru": "Ждать Полной Загрузки"},
   "Wait Url Loaded" : {"ru": "Ждать Загрузки Url"},
   "Wait Address Bar" : {"ru": "Ждать адреса браузера"},
   "Wait Text" : {"ru": "Ждать текст"},
   "Wait Css" : {"ru": "Ждать css"},
   "Wait File Download" : {"ru": "Ждать Загрузки Файла"},
   "Sleep" : {"ru": "Спать"},

   "Get" : {"ru": "Get запрос"},
   "Post" : {"ru": "Post запрос"},
   "Download" : {"ru": "Скачать"},
   "Content" : {"ru": "Содержание ответа"},
   "Status" : {"ru": "Статус ответа"},
   "Get Header" : {"ru": "Получить заголовок"},
   "Clear Headers" : {"ru": "Очистить заголовки"},
   "Set Fail On Error" : {"ru": "Завершать работу после ошибки"},
   "Is Error" : {"ru": "Была ли ошибка"},
   "Get Error String" : {"ru": "Получить текст ошибки"},
   "Xpath Get Xml" : {"ru": "Xpath получить xml"},
   "Xpath Get Xml List" : {"ru": "Xpath получить каждый xml"},
   "Xpath Get Text" : {"ru": "Xpath получить текст"},
   "Xpath Get Text List" : {"ru": "Xpath получить каждый текст"},
   "Xpath Get Count" : {"ru": "Xpath получить количество элементов"},
   "Xpath Get Exists" : {"ru": "Xpath проверить существование"},
   "Function" : {"ru": "Функция"},
   "Get link URL" : {"ru": "Получить адрес ссылки"},
   
   "Back" : {"ru": "Назад"},
   "Action interface \"" : {"ru": "Интерфейс действия \""},
   "The module containing " : {"ru": "Модуль, содержащий действие "},
   "\" is missing or damaged." : {"ru": "\" отсутствует или поврежден."},
   " action is not installed, disabled, or damaged" : {"ru": ", не установлен, отключен или поврежден"}
   
}

   function max_length(data,len)
    {
      var res = data
      if(res.length > len)
      {
        res = res.substr(0,len - 4)
        res+=" ..."
      }
      return res

    }


 function prepare_styles(dat, task_name)
 {
   try
   {
      if(task_name == "Else")
       return /*"tooltitle-red"*/ ""

     if(dat === null)
         return ""

     if(_A[dat["s"]]["template"].indexOf("~>")>=0)
      return "textitemtag"

     if(_A[dat["s"]]["template"].indexOf("<~")>=0)
      return "tooltitle-red"


     var Type = _A[dat["s"]]["class"]
     if(typeof(Type) == "undefined")
      return ""
     else if(Type == "browser")
      return /*"tooltitle-blue"*/ ""
     else
      return "tooltitle-red"

   }catch(e)
   {

   }
   return ""
 }

function prepare_folding(parentid, id, length)
{
   id = parseInt(id)
   if(id > 0)
   {
      var Task = FindTaskById(id)
      var dat = Task.dat()
      if(dat && dat["role"] == "slave") 
      {
         var master = FindTaskById(dat["master"])
         if(master && master.get("is_fold"))
         {
            return false
         }
      }
   }

   var Id = parseInt(parentid)
   if(Id == 0)
      return true

   var len = 0;



   while(true)
   {
      var Task = FindTaskById(Id)
      if(!Task)
         break

      if(len >= length)
         break;

      if(Task.get("is_fold"))
      {
         return false
      }

      var dat = Task.dat()
      if(dat && dat["role"] == "slave") 
      {
         var master = FindTaskById(dat["master"])
         if(master && master.get("is_fold"))
         {
            return false
         }
      }


      len++;

      var ParentId = parseInt(Task.get("parentid"))
      if(ParentId == 0)
         break
      
      Id = ParentId
   }
   return true
}

function prepare_margins(parentid, length, default_value)
{
   var res = ""

   var width = 1;
   var color = "#E8E8E8"

   var Id = parseInt(parentid)

   if(Id == 0)
      return res

   var len = 0;

   var lines = []

   
   if(default_value == "compute")
   {

      while(true)
      {
         var Task = FindTaskById(Id)
         if(!Task)
            break

         if(len >= length)
            break;

         lines.splice(0, 0, Task.get("is_selected"));

         len++;

         var ParentId = parseInt(Task.get("parentid"))
         if(ParentId == 0)
            break
         
         Id = ParentId
      }
   }else if (default_value == "set")
   {
      len = length
      for(var i = 0;i<len;i++)
         lines.push(true);
   }else if (default_value == "clear")
   {
      len = length
      for(var i = 0;i<len;i++)
         lines.push(false);
   }

   if(len > 0)
      res += "background: linear-gradient(to right"

   for(var i = 0;i<len;i++)
   {
      if(lines[i])
      {
         if(i == 0)
         {
            res += ", " + color + " " + width + "px, rgba(0,0,0,0) " + width + "px"
         }else
         {
            
            res += ", rgba(0,0,0,0) " + (20 * i).toString() + "px, " + color + " " + (20 * i).toString() + "px, " + color + " " + (20 * i + width).toString() + "px, rgba(0,0,0,0) " + (20 * i + width).toString() + "px"
         }
      }
   }


   if(len > 0)
      res += ");"

   res += "padding-left:" + (i * 20).toString() + "px;"

   return res

}


function prepare_group(dat, task_name, task_id)
{
   try{

      var State = ""


      if(task_name == "Else")
      {
         State = "if"
      }else
      {
         var group = ""
         
         if(dat === null)
            return ""

         State = dat["s"]

      }

      if(_A[State]["class"] && _A[State]["class"] == "browser")
      {
         group = "browser"
      }else
         group = _A[State]["group"] 
      

      if(group.length == 0)
         return ""

      var groupicon = _G["browser"]["icon"]
      if(_G[group])
      {
         groupicon = _G[group]["icon"]
      }

      return "<img id='id72-" + task_id + "' class='toolgroup' src='" + groupicon + "' />"

   }catch(e){return ""}
}
 
 function prepare_title(dat, task_name)
 {
   try
   {
     if(task_name == "Initialize")
       return tr("Initialize")
     if(task_name == "Else")
       return tr("Else")

     
     if(dat === null)
      return ""

     var pref = "" 
     if(_A[dat["s"]]["template"].indexOf("~>")>=0)
      pref = "<i class='fa fa-tag textitemtag' aria-hidden='true'></i> "
     if(_A[dat["s"]]["template"].indexOf("<~")>=0)
      pref = "<i class='fa fa-arrow-circle-right tooltitle-red' aria-hidden='true'></i> "
     
     return pref + tr(_A[dat["s"]]["name"])
   }catch(e)
   {
   }
   return ""
 }

 function prepare_label_data(dat, task_name)
 {
   try
   {
     if(task_name == "Initialize")
       return ""
     if(task_name == "Else")
       return ""

     
     if(dat === null)
      return ""

     var Res = "" 
     if(dat["s"] == "goto")
     {
      Res = " data-is-label-from=\"true\" data-label-name=\"" + quoteattr(dat["d"][0]["data"]) + "\" "
     }else if(dat["s"] == "label")
     {
      Res = " data-is-label-to=\"true\" data-label-name=\"" + quoteattr(dat["d"][0]["data"]) + "\" "
     }
     return Res
     
   }catch(e)
   {
   }
   return ""
 }

 function prepare_title_text(dat, task_name)
 {
   try
   {
     if(task_name == "Initialize")
       return tr("Initialize")
     if(task_name == "Else")
       return tr("Else")

     
     if(dat === null)
      return ""

     
     return tr(_A[dat["s"]]["name"])
   }catch(e)
   {
   }
   return ""
 }

 function prepare_title_tooltip(dat, task_name)
 {
   try
   {
     if(task_name == "Initialize")
       return tr("This is internal action, it is executed first on every thread start")
     
     
     if(dat === null)
      return ""

     
     return tr("Action") + " '" + prepare_title_text(dat,task_name) + "'. " + tr("Click to select. Drag to move it. Double click to edit and see description")
   }catch(e)
   {
   }
   return ""
 }

 function prepare_title_code(code, task_name)
 {
   try
   {
     if(task_name == "Initialize")
       return tr("Initialize")
     if(task_name == "Else")
       return tr("Else")

     var match = code.match("Dat:([^\*]+)")
     if(!match)
       return ""


     var dat = JSON.parse(b64_to_utf8(match[1]))



     var pref = "" 
     if(_A[dat["s"]]["template"].indexOf("~>")>=0)
      pref = "<i class='fa fa-tag textitemtag' aria-hidden='true'></i> "
     if(_A[dat["s"]]["template"].indexOf("<~")>=0)
      pref = "<i class='fa fa-arrow-circle-right tooltitle-red' aria-hidden='true'></i> "
     
     return pref + tr(_A[dat["s"]]["name"])
   }catch(e)
   {
   }
   return ""
 }

 function prepare_edit_body(dat, task_name)
 {
   try
   {
     if(dat === null)
      return ""

     if(!("v" in dat))
     {
         return ""
     }

     return task_name;

   }catch(e)
   {
     return e
   }
   return task_name
 }

 function prepare_description(dat, task_name)
 {
   if(task_name == "Initialize")
      return ""
   if(task_name == "Else")
      return ""

   return prepare_edit_body(dat, task_name).replace(/(?:\r\n|\r|\n)/g, '<br />');
 }

 function prepare_body(dat, task_name)
 {
   try
   {
     if(task_name == "Initialize")
       return tr("Initialize")
     if(task_name == "Else")
       return tr("Else")

    if(dat === null)
      return ""

     return BrowserAutomationStudio_GenerateActionText("", dat, 50, false);

   }catch(e)
   {
     return e
   }
   return task_name
 }




function BrowserAutomationStudio_GenerateActionText(action, data, max_len, no_html)
{
   var State = data["s"]

   if("v" in data && action.length > 0)
   {
      return max_length(action,max_len)
   }
   
   if(typeof _A[State]==="undefined")
      return tr("The module containing ") + State + tr(" action is not installed, disabled, or damaged")

   var Data = data["d"]
   var Template = _A[State]["template"]

   if(Template.length == 0)
      return tr(_A[State]["name"])

   Template = Template.replace(new RegExp("tr\\(([^\\)]+)\\)","g"), function(m1,m2){return tr(m2)})
   var Split = Template.split(/\{\{(.*?)\}\}/g)
   var Res = ""
   var LastArrow = false
   var LastTag = false
   var LastGoto = false
   var IsEmpty = true
   for(var i = 0;i<Split.length;i++)
   {
      if(i%2 == 0)
      {
         if(no_html)
         {
            Res += tr(Split[i])
         }else
         {
            if(Split[i].length > 0)
               IsEmpty = false

            LastArrow = Split[i].indexOf("->") >= 0
            if(LastArrow)
               Res += _.escape(tr(Split[i])).replace("-&gt;","<i class='fa fa-arrow-circle-right ' aria-hidden='true'></i>")

            LastTag = Split[i].indexOf("~>") >= 0
            if(LastTag)
               Res += _.escape(tr(Split[i])).replace("~&gt;","")

            LastGoto = Split[i].indexOf("<~") >= 0
            if(LastGoto)
            {
               Res += _.escape(tr(Split[i])).replace("&lt;~","")
            }

            if(!LastGoto && !LastTag && !LastArrow)
            {
               Res += _.escape(tr(Split[i]))
            }
         }
      }else
      {
         var Class = ""
         if(LastArrow)
            Class = "textitemsignificant"
         if(LastTag)
         {
            Class = "textitemtagtext"
         }

         if(LastGoto)
         {
            Class = "textitemtagtext"
         }
         
         if(Split[i] == "PATH")
         {
            if(data["p"]["is_image"])
            {
               if(!no_html)
               {
                  Res += "<img class='pathimg' draggable='false' src='data:image/png;base64," + data["d"].find(function(el){return el["id"] == "ImageBase64"})["data"] + "'/>"

                  IsEmpty = false
               }
            }else
            {
               var Current = data["p"]["current"]
               var DataName = ""
               if(Current != "none")
               {
                  DataName = data["p"][Current] 
               }else
               {
                  DataName = "body"
               }


               if(no_html)
               {
                  Res += DataName
               }else
               {
                  if(DataName.length > 0)
                     IsEmpty = false

                  Res += "<span class=" + Class + ">" + _.escape(max_length(DataName,max_len)) + "</span>"
               }
            }
         }else
         {
            var El = Data.filter(function(el){return el["id"] == Split[i]})
            if(El.length > 0)
            {
               var DataName = El[0]

               if(no_html)
               {
                  Res += DataName["data"].replace(/(?:\r\n|\r|\n)/g," ")
               }else
               {
                  if(DataName["data"].length > 0)
                     IsEmpty = false
                  var InsertData =  _.escape(max_length(DataName["data"].replace(/(?:\r\n|\r|\n)/g," "),max_len))
                  if(Split[i] == "FunctionName")
                  {
                     Res += "<a href='#' class='movetofunction' onclick='_FunctionManager.ChangeFunction(" + JSON.stringify(DataName["data"]) + ");return false'>" + InsertData + "</a>"
                  }else
                  {
                     Res += "<span class=" + Class + ">" + InsertData + "</span>"
                  }
               }
            }        
         }

         
      }
      
   }

   if(no_html)
   {
      return Res
   }

   if(IsEmpty)
      Res = tr(_A[State]["name"])

   return Res.replace(new RegExp("\\n","g"),"<br/>").replace(new RegExp("strong\\(\\((.+)\\)\\)","g"), function(m1,m2){return "<strong>" + m2 + "</strong>"})

   

   
}