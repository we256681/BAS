function GetCallStackForTask(Task)
{
    var TasksParents = []
    while(true)
    {
        var parentid = parseInt(Task.get("parentid"))
        if(parentid == 0)
            break

        
        Task = FindTaskById(parentid)
        if(!Task)
            break

        TasksParents.unshift(parseInt(Task.get("id")))

        if(IsFunctionNode(parentid))
            break

    }
    return TasksParents;
}

function GenerateCodeForMovingExecutionPoint(IdFrom, IdTo)
{
    var TaskFrom = FindTaskById(parseInt(IdFrom))

    if(!TaskFrom)
        return "_kill_call_stack_silent()"

    var TaskFromStack = GetCallStackForTask(TaskFrom)  

    var TaskTo = FindTaskById(parseInt(IdTo))

    if(!TaskTo)
        return "_kill_call_stack_silent()"

    var TaskToStack =  GetCallStackForTask(TaskTo)  

    var CurrentFromStackValue = 0
    var CurrentToStackValue = 0
    var CurrentFromStackIndex = 0
    var CurrentToStackIndex = 0

    if(TaskFromStack.length > 0)
        CurrentFromStackValue = TaskFromStack[0]
    
    if(TaskToStack.length > 0)
        CurrentToStackValue = TaskToStack[0]

    if(CurrentFromStackValue != CurrentToStackValue)
        return "_kill_call_stack_silent()"

    while(true)
    {
        if(CurrentFromStackIndex + 1 >= TaskFromStack.length)
            break
        if(CurrentToStackIndex + 1 >= TaskToStack.length)
            break
        
        CurrentFromStackValue = TaskFromStack[CurrentFromStackIndex + 1] 
        CurrentToStackValue = TaskToStack[CurrentToStackIndex + 1] 

        if(CurrentFromStackValue != CurrentToStackValue)
            break;

        CurrentFromStackIndex++;
        CurrentToStackIndex++;
    }

    var RemoveNumber = TaskFromStack.length - CurrentFromStackIndex - 1;
    var AddList = [];

    for(var i = CurrentToStackIndex + 1;i<TaskToStack.length;i++)
    {
        var TaskTo = TaskToStack[i]
        var NextTask = FindNextSibling(TaskTo)
        if(NextTask)
        {
            AddList.push(NextTask.get("id"))
        }else
        {
            AddList.push(null)
        }
    }

    if(RemoveNumber <= 0 && AddList.length <= 0)
    {
        return ""
    }

    var Code = ""

    if(RemoveNumber > 0)
    {
        Code += "for(var i = 0;i<" + RemoveNumber + ";i++)CYCLES.Pop();\n"
    }

    if(AddList.length > 0)
    {
        for(var i = 0;i<AddList.length;i++)
        {
            var Add = AddList[i]
            if(Add === null)
            {
                Code += `
                    CYCLES.Push
                    (
                        new Cycle(
                            function(){
                                _break();
                            },
                            (
                                function()
                                {
                                    var res = function(){};
                                    return res;
                                }
                            )()
                        )
                    )
                `
                
            }else
                Code += `
                CYCLES.Push
                (
                    new Cycle(
                        function(){
                            _break();
                        },
                        (
                            function()
                            {
                                var res = function(){section_start("test", ${Add}, function(){})};
                                return res;
                            }
                        )()
                    )
                )
            `
        }
    }

    
        

    return Code
}