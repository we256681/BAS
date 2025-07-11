<div class="container-fluid executefunction">
    <%= _.template($('#input_constructor').html())({id:"ThreadId", description:tr("Thread id"), default_selector: "expression", disable_int:true, disable_string:true, help: 
{
  description: tr("Thread identifier obtained from 'Call function asynchronously' action. In case if parameter is a list, this action will wait for all async functions in a list to finish."), examples:
  [
    {
      code:"[[THREAD_ID]]",description:tr("Wait for single function.")
    },
    
    {
      code:"[[THREAD_LIST]]",description:tr("Wait for all functions in the list.")
    }
  ]
}}) %>
</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Wait for async function to finish.</div>

    <div class="tr tooltip-paragraph-fold">In order to use this action, function must be started asynchronously with 'Call function asynchronously' action.</div>
    <div class="tr tooltip-paragraph-fold">It is important to understand that async function call returns immediately, and you can obtain its actual result by waiting for function completion with this action.</div>
    <div class="tr tooltip-paragraph-fold">This action may wait for single function or for list of functions. In order to wait for list, place variable with list type into 'Thread id' parameter, usually its name is THREAD_LIST and it is obtained as a result of 'Call function asynchronously' action.</div>
    <div class="tooltip-paragraph-fold"><span class="tr">By default this action will wait infinite amount of time, but you can specify maximum time to wait. If function will not complete by that time, it will be stopped and error will raise. You can process error with "Ignore Errors" action. Setting maximum time to wait is very convinient way to set timeout to any function.</span> (<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?media=browserautomationstudio_2020-02-19_17-03-54.png');return false">screen</a>). </div> 
    <div class="tooltip-paragraph-last-fold"><span class="tr">Correct workflow to wait for several functions may look like</span> <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?media=browserautomationstudio_2020-02-19_17-01-15.png');return false">this</a>.</div>

</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true, use_timeout: true, timeout_value: 100000}) %>