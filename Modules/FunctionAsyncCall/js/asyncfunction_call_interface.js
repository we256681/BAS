<div class="container-fluid executefunction">
    <div class="col-xs-12" style="margin-bottom: 5px;">
    
    <div class="input-group">
        
        <span data-preserve="true" data-preserve-type="text" data-preserve-id="FunctionName">
        <input class="form-control function-input" type="text" id="FunctionName" placeholder="Function name" />
        <ul class="dropdown-menu input-field-dropdown dropdown-align dropdown-black">
            <li>
            <a tabindex="-1" href="#" class="func" data-result-target="#FunctionName">
                <span class="tr">Select function</span>
            </a>
            </li>
        </ul>
        </span>        

        <span class="input-group-addon input-description tr">Function name</span>
    </div>
    
    </div>
    <div id="FunctionParametersNoReturn">
    </div>
    <%= _.template($('#variable_constructor').html())({id:"ResultThreadId", description:tr("Result thread id"), default_variable: "THREAD_ID", help: { description: tr("Instead of function result, this variable contains identifier, which can be used to obtain function result when function will finish. It also allows communicating with function, with help of it you can: obtain function result, wait for function completion or stop function.")}}) %>

    <div style="margin-left: 20px;">
  	    <input type="checkbox" id="AdvancedCheck" onchange="$('#Advanced').toggle()" /> 
  	    <label for="AdvancedCheck" class="tr" >Advanced settings.</label>
    </div>
    <span id="Advanced" style="display:none">
        <%= _.template($('#input_constructor').html())({id:"DesiredThreadId", description:tr("Desired thread id"), default_selector: "int", disable_string:true, value_number:0, min_number:0, max_number:999999, variants: ["0<br/><span style='color:gray;font-size:small'>" + tr("Start in a new thread") + "</span>"], help: 
{
  description: tr("Identifier for thread, where function will start. By default function will be started in new thread, but you can change that behavior by modifying this value."), examples:
  [
    {
      code:"0",description:tr("Start task in a new thread. New thread id will be returned in 'Result thread id' parameter.")
    },
    
    {
      code:"[[THREAD_ID]]",description:tr("Start task in old thread. THREAD_ID variable must be obtained as a result of previously called 'Call function asynchronously' action. Previous action call must set 'Close thread after function finish' parameter to false, otherwise thread will be stopped.")
    }
  ]
}}) %>
        <%= _.template($('#input_constructor').html())({id:"CloseThreadAfterFunctionEnds", description:tr("Close thread after function finish"), default_selector: "string", disable_int:true, value_string: "true", variants: ["true<br/><span style='color:gray;font-size:small'>" + tr("Close thread after function will finish") + "</span>", "false<br/><span style='color:gray;font-size:small'>" + tr("Don't close thread<br/>Next function can be called in this thread") + "</span>"], help: 
{
  description: tr("If thread must be stopped after this function will finish execution. Default value is true, which helps to prevent hanging threads. Set it to false only if you want to use this thread later."), examples:
  [
    {
      code:"true",description:tr("Close thread after function will finish")
    },
    
    {
      code:"false",description:tr("Don't close thread. Next function can be called in this thread. Use 'Desired thread id' to start new function in same thread.")
    }
  ]
}}) %>

    <%= _.template($('#input_constructor').html())({id:"OnIdle", description:tr("On idle"), default_selector: "string", disable_int:true, value_string: "wait", variants: ["wait", "start postponded function"], help: 
{
  description: tr("This setting defines behavior, which will be performed on idle, ie when function will finish but thread will remain active. By using 'start postponded function' option you can achieve such behavior, when all functions will be executed in same pool of threads. This will help to save resources needed to restrat threads and browsers. Don't forget to set 'Close thread after function finish' to false, otherwise threads will be closed automatically, idle state won't be achieved and effect will be neglected."), examples:
  [
    {
      code:"wait",description:tr("Default. When thread has no work to do, it will wait to new function call.")
    },
    
    {
      code:"start postponded function",description:tr("In idle state thread will look for postdonded functions, thoose, which was queued due to 'Maximum async tasks executed simultaneously' setting. If such function will be found, it will be executed in current thread.")
    }
  ]
}}) %>

        <%= _.template($('#input_constructor').html())({id:"MaxThreads", description:tr("Maximum async tasks executed simultaneously"), default_selector: "int", disable_string:true, value_number: 30, min_number:0, max_number:999999, help: 
{
  description: tr("Maximum simultaneously running threads with async functions per application. If function will be started beyond the limit, it will be postponed and will be started immediately after some other async function will finish and the limit will not be exceeded. This is very powerful setting, it allows you to start as many threads as you want and let BAS balance thread number."), examples:
  [
    {
      code:"30",description:tr("Limit the number of threads to 30. Default.")
    },
    
    {
      code:"1",description:tr("Execute async functions sequentially.")
    },
    
    {
      code:"100",description:tr("Limit the number of threads to 100.")
    }
  ]
}}) %>
        <%= _.template($('#variable_constructor').html())({id:"ThreadList", description:tr("List where thread id will be added"), default_variable: "THREAD_LIST", help: { description: tr("List, where thread id will be added. This variable has a list type and can be processed with \"List\" module. It accumulates all started threads so, you can perform bulk actions on it, for example, stop them immediately all together. If variable doesn't exist, it will be created.")}}) %>
    </span>



</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Execute function asynchronously without waiting for its completion.</div>

    <div class="tooltip-paragraph-fold"><span class="tr">Function must be defined beforehand. BAS allows define functions through "Function Manager"</span> (<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=browserautomationstudio_2020-02-19_12-28-39.png');return false">screen</a>)</div>
    <div class="tr tooltip-paragraph-fold">Functions are containers, which holds action list. It helps to group several actions which does same task. For example, there can be function which logins to account, function that checks account balance, etc. It makes your code clear and well organized.</div> 
    <div class="tr tooltip-paragraph-fold">Another advantage is that one function may be executed as many times as you want from any place in the script. This helps to avoid code duplication. If you want to update function, you need to do that only once, not in every place where it used.</div>
    <div class="tr tooltip-paragraph-fold">This action will start a new thread and execute function in it with specified parameters.</div>
    <div class="tr tooltip-paragraph-fold">It is important to understand that because of function will be started in a new thread, all local variables won't be available inside function. Function will use completely different browser. It means that, for example, authorization on sites performed in thread that called function won't be available inside function.</div>
    <div class="tr tooltip-paragraph-fold">Sharing data. You can pass data to function by setting input parameters, results from function may be obtained by using "Get async function result" action. Results are available only after function finished working.</div>
    <div class="tooltip-paragraph-fold"><span class="tr">Execution of this action will finish immediately upon invocation even if function execution itself takes significant time.</span> (<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?media=browserautomationstudio_2020-02-19_17-08-03.png');return false">screen</a>)</div>
    <div class="tooltip-paragraph-fold"><span class="tr">You can wait for one or several asynchronous functions to finish by using "Wait for async function completion" action.</span> (<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?media=browserautomationstudio_2020-02-19_17-10-57.png');return false">screen</a>)</div>

    <div class="tr tooltip-paragraph-fold">This action behaves differently in record and run mode. In run mode it creates a new thread and parallelizes workflow. In record mode it works almost the same as regular function call, because record mode only works with single thread.</div>
    <div class="tooltip-paragraph-fold"><span class="tr">This action returns thread id inside THREAD_ID variable. This identifier allows communicating with function, with help of it you can: obtain function result, wait for function completion or stop function.</span> (<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?media=browserautomationstudio_2020-02-19_14-57-58.png');return false">screen</a>)</div>
    <div class="tooltip-paragraph-fold"><span class="tr">Use 'Wait for async function completion' and then 'Get async function result' to get async function result</span>. (<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?media=browserautomationstudio_2020-02-19_16-12-55.png');return false">screen</a>)</div>

    <div class="tr tooltip-paragraph-fold">It also adds thread identifier to THREAD_LIST variable. This variable has a list type and can be processed with "List" module. It accumulates all started threads so, you can perform bulk actions on it, for example, stop them immediately all together.</div>
    <div class="tooltip-paragraph-fold"><span class="tr">Script execution won't finish until at least one asynchronous function is working. You can use that behavior to stop script automatically when all tasks are done. For example, if main thread starts two functions, one takes 1 minute and second takes 2 minutes. In that case, script will finish in 2 minutes even if main thread will finish immediately after start.</span> (<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?media=browserautomationstudio_2020-02-19_14-53-55.png');return false">screen</a>) </div>
    <div class="tooltip-paragraph-fold"><span class="tr">You can limit maximum number of simultaneously running threads with async functions by setting "Maximum async tasks executed simultaneously" parameter. If function will be started beyond the limit, it will be postponed and will be started immediately after some other async function will finish and the limit will not be exceeded. This is very powerful setting, it allows you to start as many threads as you want and let BAS balance thread number.</span> (<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?media=browserautomationstudio_2020-02-19_14-51-19.png');return false">screen</a>) </div>
    <div class="tr tooltip-paragraph-fold">By default, function will be started in a new thread, and after function will finish, thread will be deleted. However, you can keep thread running and even execute new function in it after old is finished. Next function will share same context - same browser and local variables. Consider following example, there are two functions: SetProxy and DoParsing, if you execute them in different threads, parsing will be performed with real ip, and if you execute them in same thread sequentially, parsing will be performed through proxy. In order to start function in old thread, set "Desired thread id" to old thread id, you need also to set "Close thread after function finish" parameter to false.</div>
    <div class="tooltip-paragraph-last-fold"><span class="tr">If new function will be started in thread, that already has function started, new function will be postponed. This new function will be started immediately after old one gets finished.</span> (<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?media=browserautomationstudio_2020-02-20_20-12-47.png');return false">screen</a>) </div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:false}) %>