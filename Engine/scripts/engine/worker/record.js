function section_start(name, id, callback)
{
    var DoSend = true;
    if(name === "_execution_point")
    {
        id = Browser.GetCurrentExecutionPointWhenAddingAction()
        if(id == 0)
            DoSend = false
    }

    if(DoSend)
    {
        _sa(id)
        Browser.StartSection(name,id,_get_function_body(callback));
    }else
    {
        Browser.WaitCode(_get_function_body(callback))
    }
}

function _sa(id)
{
    ScriptWorker.SetCurrentAction(id)
}

function section_insert()
{
}
