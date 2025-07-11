VAR_CYCLE_INDEX = 0
VAR_FOREACH_DATA = ""
LINK_REGEXP = /https?:\/\/(www\.)?[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-z]{2,6}\b([-a-zA-Z0-9@:%_\+.~#?&//=]*)/g
VAR_FOR_EACH_CSS = ">NOTDEFINED>"
VAR_FOR_EACH_MATCH = ">NOTDEFINED>"
VAR_FOR_EACH_XPATH = ">NOTDEFINED>"
IF_ELSE_EXPRESSION = true;

function Cycle(Next,Break)
{
    this.OnFail = "";
    this.OnSuccess = "";

    {
        var c = CYCLES.Current()
        if(c)
        {
            this.OnFail = c.OnFail;
            this.OnSuccess = c.OnSuccess;
        }
    }

    this._Info = {};
    this._Label = [];
    this._Params = {};
    this._Arguments = null;

    this._Break = Break;
    this._Next = Next;
    this._Iterator = 0;
    this.WasNextOrBreak = false;

    this.Next = function(arg)
    {
        if(typeof(arg) === "number")
        {
            if(arg >= CYCLES.Data.length)
            {
                arg = CYCLES.Data.length;
            }
            if(arg > 0)
            {
                for(var i = 0;i<arg - 1;i++)
                {
                    CYCLES.Pop();
                }

                var c = CYCLES.Current();

                if(c)
                {
                    c._Next(c._Iterator++);
                }
            }
        }

        if(typeof(arg) === "undefined")
            this._Next(this._Iterator++);
    }

    this.SetLabel = function(label)
    {
        if(this._Label.indexOf(label)<0)
            this._Label.push(label);
    }

    this.HasLabel = function(label)
    {
        return this._Label.indexOf(label) >= 0;
    }

    this.RemoveLabel = function(label)
    {
        var index = this._Label.indexOf(label);
        if(index>=0)
            this._Label.splice(index,1);
    }

    this.Break = function(arg)
    {

        if(typeof(arg) === "string")
        {
            var c = CYCLES.Current();
            if(!c || c._Label.indexOf(arg)>=0)
                c = null

            while(c && c._Label.indexOf(arg)<0)
            {
                c = CYCLES.Pop();
            }

            if(c)
            {
                c._Break();
            }
        }else
        {
            if(arg >= CYCLES.Data.length)
            {
                arg = CYCLES.Data.length;
            }
            if(arg > 0)
            {
                var c = null
                for(var i = 0;i<arg;i++)
                    c = CYCLES.Pop();
                if(c)
                {
                    c._Break();
                }
            }
        }
    }

    this.Start = function()
    {
        CYCLES.Push(this);
        this.Next();
    }
}

function Cycles()
{
    this.Data = [];
    this.Result = null;
    this.ResultFunction = null;
    this.Push = function(el)
    {
        this.Data.push(el)
        var c = CYCLES.Current()
        if(c)
        {
            ScriptWorker.SetFailFunction(c.OnFail);
            ScriptWorker.SetSuccessFunction(c.OnSuccess);
        }else
        {
            ScriptWorker.SetFailFunction("");
            ScriptWorker.SetSuccessFunction("");
        }
    }
    this.Pop = function()
    {
        var r = this.Data.pop()
        var c = CYCLES.Current()
        if(c)
        {
            ScriptWorker.SetFailFunction(c.OnFail);
            ScriptWorker.SetSuccessFunction(c.OnSuccess);
        }else
        {
            ScriptWorker.SetFailFunction("");
            ScriptWorker.SetSuccessFunction("");
        }

        return r;
    }
    this.Rewind = function(arg)
    {
        var c = CYCLES.Current();
        while(c && c._Label.indexOf(arg)<0)
        {
            CYCLES.Pop();
            c = CYCLES.Current();

        }
    }
    this.Current = function()
    {
        return (this.Data.length>0)? this.Data[this.Data.length - 1] : null;
    }
    this.FindLabelDepth = function(label)
    {
        var res = 1
        for(var i = this.Data.length - 1;i>=0;i--)
        {
            var c = this.Data[i]
            if(c.HasLabel(label))
            {
                return res
            }
            res += 1
        }
        return -1
    }
    this.FindFunctionArgument = function()
    {
        for(var i = this.Data.length - 1;i>=0;i--)
        {
            var c = this.Data[i]
            if(c.HasLabel("argument"))
            {
                if(typeof(c._Arguments) == "undefined")
                    return null
                return c._Arguments;
            }
        }
        return null
    }
}
CYCLES = new Cycles();
_Params = {};


function _next()
{
    var usearg = false;
    var arg = 1;
    if(arguments.length === 1)
    {
        arg = arguments[0];
        usearg = true;
    }
    if(typeof(arg) == "string")
    {
        arg = CYCLES.FindLabelDepth(arg)
        if(arg === -1)
            return
    }

    var c = CYCLES.Current();
    if(c)
    {
        if(usearg)
            c.Next(arg);
        else
            c.Next();
        _stop_subscript_execution();
    }
    else
        success("Ok");
}

function _next_or_section(id, callback)
{
    var c = CYCLES.Current();
    if(c)
    {
        c.Next();
        _stop_subscript_execution();
    }
    else
        section_start("test",id,callback)
}

function _kill_call_stack()
{
    ScriptWorker.SendTaskResult(JSON.stringify({Message: "", Result: null,Success:true}))
    _kill_call_stack_silent()
}

function _kill_call_stack_silent()
{
    VAR_CYCLE_INDEX = 0
    CYCLES = new Cycles();
}


function _break()
{
    var arg = 1;
    if(arguments.length >= 1)
    {
        arg = arguments[0];
    }
    if(typeof(arg) == "string")
    {
        arg = CYCLES.FindLabelDepth(arg)
        if(arg === -1)
            arg = 99999
    }

    var c = CYCLES.Current();
    if(c)
    {
        if(arguments.length >= 2)
            VAR_WAS_ERROR = arguments[1];
        c.Break(arg);
        _stop_subscript_execution();
    }else
    {
        if(arguments.length >= 2)
            VAR_WAS_ERROR = arguments[1];
    }


}

function _iterator()
{

    var c = CYCLES.Current();
    if(c)
    {
        return c._Iterator;
    }
    else
        return 0;

}

function _cycle_param(name)
{

    var c = CYCLES.Current();
    if(c)
    {
        var res = c._Params[name];
        if(typeof(res) == "undefined")
            return ""
        return res;
    }
    else
    {
        var res = _Params[name];
        if(typeof(res) == "undefined")
            return ""
        return res;
    }
}

function _cycle_params()
{
    var c = CYCLES.Current();
    if(c)
    {
        return c._Params;
    }
    else
    {
        return _Params;
    }
}


function _arguments()
{

    var c = CYCLES.Current();
    if(c)
    {
        return c._Arguments;
    }
    else
        return null;

}

function _function_arguments()
{
    return CYCLES.FindFunctionArgument()
}

function _function_argument(p)
{
    var args = _function_arguments()
    try
    {
        if(typeof(args) == "object" && args.hasOwnProperty(p))
        {
            return args[p]
        }
    }catch(e)
    {
    }
    return null
}


function _do(n,b)
{
    var c = new Cycle(n,b);
    c.SetLabel("function");
    c.Start();
}

function _do_with_params(p,n,b)
{
    var c = new Cycle(n,b);
    c.SetLabel("function");
    c._Params = p;
    c.Start();
}

function _repeat(t,n,b)
{
    _do(function(i){
        if(i>t-1)
        {
            _break();
            return;
        }
        n(i);
    },b)
}

function _if(c,f,n)
{
    var expression = ScriptWorker.GetIsRecord() ? _cycle_param('if_else_expression') : null;

    _do(function(i){
        var cc = CYCLES.Current();
        if(cc)
        {
            cc.RemoveLabel("function");
            _set_action_info({ name: "If", expression: expression }, cc);
        }
        if(i>0 || !c)
        {
            _break();
            return;
        }
        f();
    },n);

    if (ScriptWorker.GetIsRecord()) delete _cycle_params().if_else_expression;
}

function _if_else(c, f1, f2,n)
{
    _do(function(i){
        var cc = CYCLES.Current();
        if(cc)
            cc.RemoveLabel("function");
        if(i>0)
        {
            _break();
            return;
        }
        if(c)
            f1();
        else
            f2();

    },n)
}


function _call(f,a,n)
{
    var c = new Cycle(function(i){
        if(i>0)
        {
            _break(1, false);
            return;
        }
        f();

    },n);
    _set_action_info({ name: f.name }, c);
    c.SetLabel("function");
    c._Arguments = a;
    c.Start();
}

function _prepare_function_and_call(f,a,n)
{
    _ARG_PREPARE = [f,a,n]
    if(ScriptWorker.GetIsRecord())
    {
        ScriptWorker.PrepareFunction(f)
    }else
    {
        _prepare_function_and_call_end()
    }
}

function _prepare_function_and_call_end()
{
   _call(function()
   {
    _on_fail(function(){
        if(ScriptWorker.GetEmbeddedExecutingApiCode())
        {
            ScriptWorker.EmbeddedApiResponce(_embedded_prepare_variables(ScriptWorker.GetEmbeddedVariablesList(), _result()));
            _break(1)
        }else
        {
            CYCLES.Pop()
            fail_user(_result())
        }

    })
    CYCLES.Current().RemoveLabel("function")
    _call_function(eval(_ARG_PREPARE[0]),JSON.parse(_ARG_PREPARE[1]),function()
    {
        VAR__BAS_FUNCTION_RESULT_ = _result_function()
        ScriptWorker.SetScript(_get_function_body(_ARG_PREPARE[2]))
        _ARG_PREPARE = null
        ScriptWorker.RunSubScript()
    })

   }, null, function(){})
}

function _call_function(f,a,n)
{
    var c = new Cycle(function(i){
        if(i>0)
        {
            _break(1, false);
            return;
        }
        f();

    },n);
    _set_action_info({ name: f.name }, c);
    c.SetLabel("function");
    c.SetLabel("argument");
    c._Arguments = a;
    c.Start();
}

function _call_task(f,a,n)
{
    var c = new Cycle(function(i){
        if(i>0)
        {
            _break(1, false);
            return;
        }
        f();

    },n);
    _set_action_info({ name: f.name }, c);
    c.SetLabel("function");
    c.SetLabel("argument");
    c.SetLabel("task");
    c._Arguments = a;
    c.Start();
}

function _function_return(v)
{
    if(CYCLES.FindLabelDepth("argument") === -1)
        return


    var TaskDepth = CYCLES.FindLabelDepth("task") - 1
    var ArgumentDepth = CYCLES.FindLabelDepth("argument") - 1

    if(
            //We are calling task from web interface
            TaskDepth > 0
            &&
            //We are not in inner function
            ArgumentDepth == TaskDepth

      )
    {
        _rewind("task")
    }

    if(!CYCLES.Current() || CYCLES.Current().HasLabel("task"))
        ScriptWorker.SendTaskResult(JSON.stringify({Message: "", Result: v,Success:true}))
    CYCLES.ResultFunction = v;
    _break("argument")
}

function _call_section(func,threads,success_number,fail_number,callback)
{
    _ARG = arguments
    sleep(1000, function(){
        ScriptWorker.SubstageCall(_ARG[0].name,_ARG[1],_ARG[2],_ARG[3],_get_function_body(_ARG[4]))
    })

}

function _result()
{
    return CYCLES.Result
}

function _set_result(val)
{
    CYCLES.Result = val;
}

function _result_function()
{
    var res = CYCLES.ResultFunction
    CYCLES.ResultFunction = null
    return res
}

function _return(val)
{
    CYCLES.Result = val
    _stop_subscript_execution()
}


function _set_label(label)
{
    var c = CYCLES.Current();
    if(c)
        c.SetLabel(label);
}

function _rewind(arg)
{
    CYCLES.Rewind(arg);
}

function _goto(label, callback)
{

    if(typeof(_BAS_GOTO_DATA) == "undefined" || !(label in _BAS_GOTO_DATA))
    {
        fail("Goto to non existing label " + label)
        return
    }
    _kill_call_stack()
    ScriptWorker.SetScript(_get_function_body(_BAS_GOTO_DATA[label]))
    ScriptWorker.RunSubScript()
}

function _fast_goto(label, callback)
{

    if(typeof(_BAS_GOTO_DATA) == "undefined" || !(label in _BAS_GOTO_DATA))
    {
        fail("Goto to non existing label " + label)
        return
    }
    ScriptWorker.SetScript(_get_function_body(_BAS_GOTO_DATA[label]))
    ScriptWorker.RunSubScript()
}

function _long_goto(label, offset, reverse, callback)
{
    if(typeof(_BAS_GOTO_DATA) == "undefined" || !(label in _BAS_GOTO_DATA))
    {
        fail("Goto to non existing label " + label)
        return
    }
    if(offset<0)
        _kill_call_stack()
    else
    {
        for(var i = 0;i<offset;i++)
            CYCLES.Pop();
    }
    var len = reverse.length
    for(var i = len - 1;i>=0;i--)
    {
        var f = reverse[i]
        if(!f)
            f = function(){}
        else
            f = _BAS_GOTO_DATA[f]
        CYCLES.Push(new Cycle(function(){_break();},f));
    }


    ScriptWorker.SetScript(_get_function_body(_BAS_GOTO_DATA[label]))
    ScriptWorker.RunSubScript()
}

function _set_action_info(info, cycle) {
    if (ScriptWorker.GetIsRecord() && info) {
        info.id = ScriptWorker.GetCurrentAction();

        if (["if", "for", "while", "foreach"].indexOf(info.name.toLowerCase()) < 0) {
            info.type = "function";
        } else {
            info.type = "action";
        }

        cycle = cycle || CYCLES.Current();

        if (cycle) {
            Object.keys(info).forEach(function (key) {
                if (key === "id" && key in cycle._Info) {
                    return;
                }
                cycle._Info[key] = info[key];
            });
        }
    }
}

function _set_if_expression(expression) {
    if (ScriptWorker.GetIsRecord()) {
        _cycle_params().if_else_expression = base64_decode(expression);
    }
}
