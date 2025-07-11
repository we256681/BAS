<div class="container-fluid executefunction">
    <%= _.template($('#input_constructor').html())({id:"ThreadId", description:tr("Thread id"), default_selector: "expression", disable_int:true, disable_string:true, help: {description: tr("Thread identifier obtained from 'Call function asynchronously' action.")}}) %>
    <%= _.template($('#variable_constructor').html())({id:"Result", description:tr("Result"), default_variable: "THREAD_RESULT", help: { description: tr("Result of thread execution. In order to set this value from executed function, you need to use 'Result' action. If there was no call of 'Result' action inside function, null will be returned.")}}) %>
    <%= _.template($('#variable_constructor').html())({id:"IsRunnig", description:tr("Is running"), default_variable: "THREAD_IS_RUNNING", help: { description: tr("Boolean value(true/false) indicates if function is still running.")}}) %>
    <%= _.template($('#variable_constructor').html())({id:"IsSuccess", description:tr("Is success"), default_variable: "THREAD_IS_SUCCESS", help: { description: tr("Boolean value(true/false) indicates if last function execution was successful.")}}) %>
    <%= _.template($('#variable_constructor').html())({id:"Error", description:tr("Error string"), default_variable: "THREAD_ERROR", help: { description: tr("Error string in case if last function call was not successful.")}}) %>




</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Obtain asynchronous function result.</div>
    <div class="tr tooltip-paragraph-fold">In order to use this action, function must be started asynchronously with 'Call function asynchronously' action.</div>

    <div class="tr tooltip-paragraph-fold">It is important to understand that async function call returns immediately without result, and you can obtain its actual result only by calling this action later, when function will finish.</div>

    <div class="tr tooltip-paragraph-fold">This action obtains result of function execution, it also helps to obtain function state, if it is still running and if there was error during execution.</div>
    <div class="tr tooltip-paragraph-fold">It makes no sense to check THREAD_RESULT until THREAD_IS_RUNNING is set to true, ie until function is still running.</div>
    <div class="tooltip-paragraph-fold"><span class="tr">You can use THREAD_IS_RUNNING to wait for function result, but it is much easier to use 'Wait for async function completion' action.</span> (<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?media=browserautomationstudio_2020-02-19_16-06-37.png');return false">screen</a>) </div>
    <div class="tr tooltip-paragraph-last-fold">If there was error during function call, use THREAD_IS_SUCCESS and THREAD_ERROR to check it and obtain error message.</div>
    <div class="tooltip-paragraph-fold"><span class="tr">Correct workflow to get async function result may look like</span> <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?media=browserautomationstudio_2020-02-19_16-12-55.png');return false">this</a>.</div>

</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>