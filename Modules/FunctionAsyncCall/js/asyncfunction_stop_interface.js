<div class="container-fluid executefunction">
    <%= _.template($('#input_constructor').html())({id:"ThreadId", description:tr("Thread id"), default_selector: "expression", disable_int:true, disable_string:true, help: 
{
  description: tr("Thread identifier obtained from 'Call function asynchronously' action. In case if parameter is a list, this action will stop all async functions in a list."), examples:
  [
    {
      code:"[[THREAD_ID]]",description:tr("Stop single function.")
    },
    
    {
      code:"[[THREAD_LIST]]",description:tr("Stop all functions in the list.")
    }
  ]
}}) %>
</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Stop async function.</div>
    <div class="tr tooltip-paragraph-fold">This action will immediately stop asynchronous function execution.</div>
    <div class="tr tooltip-paragraph-fold">If "Thread id" parameter contains list, each thread from this list will be stopped.</div> 
    <div class="tr tooltip-paragraph-last-fold">This action only works with async functions created with "Call function asynchronously" action.</div>
    
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>