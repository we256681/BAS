function BrowserAutomationStudio_OpenSource(Url, LineNumber, ColumnNumber)
{
    window._OpenSource.GetSource(Url, LineNumber, ColumnNumber);
}

function OpenSource()
{
    let Connection = null;
    let IsBusy = false;
    let Timeout = null;


    let NextSourceUrl = null;
    let NextLineNumber = null;
    let NextColumnNumber = null;
    
    let CurrentTaskId = -1;
    let CurrentScriptSourceResult = null;
    let CurrentSourceUrl = null;
    let CurrentLineNumber = null;
    let CurrentColumnNumber = null;
    let CurrentSourceLineNumber = -1;
    let CurrentSourceColumnNumber = -1;
    let IsStoppingDebugger = false;
    let DebuggerCloseRequest = -1;
    let IsFinished = false;
    let IsStoppingDebuggerBecauseSuccess = true;
    let TaskId = 1;


    this.Init = async function(Port)
    {
        let self = this

        let Response = await fetch(`http://127.0.0.1:${Port}/json`);
        let Data = await Response.json();
        let Endpoint = null
        Data.some((EndpointCandidate) => {
            if(EndpointCandidate.url.indexOf("file:///") == 0 || EndpointCandidate.url.indexOf("devtools://") == 0)
            {
                return false
            }
            Endpoint = EndpointCandidate.webSocketDebuggerUrl
            return true
        })

        if(!Endpoint)
            return;

        console.log("Found endpoint ", Endpoint);

        Connection = new WebSocket(Endpoint);

        Connection.onopen = () => {
            IsBusy = false
        }

        Connection.onmessage = (Event) => {
            let o = JSON.parse(Event.data)

            
            if(typeof(o["id"]) == "number")
            {
                let ObtainedId = o["id"];
                {
                    if(IsStoppingDebugger && ObtainedId == DebuggerCloseRequest)
                    {
                        if(IsStoppingDebuggerBecauseSuccess)
                        {
                            console.log("Received disable debugger responce");

                            if(Timeout)
                            {
                                clearTimeout(Timeout);
                                Timeout = null;
                            }
                            IsStoppingDebugger = false;
                            IsBusy = false;
                            IsFinished = true;

                            self.StartNextIfNeeded();
                            return;
                        }else
                        {
                            console.log("Received disable debugger responce after timeout");

                            if(NextSourceUrl)
                            {
                                self.StartNextIfNeeded();
                            }else
                            {
                                IsBusy = false;
                                NextSourceUrl = "";
                                CurrentTaskId = -1;
                                NextLineNumber = -1;
                                NextColumnNumber = -1;
                                CurrentScriptSourceResult = "";
                                CurrentSourceUrl = "";
                                CurrentLineNumber = -1;
                                CurrentColumnNumber = -1;
                                CurrentSourceLineNumber = -1;
                                CurrentSourceColumnNumber = -1;
                                IsStoppingDebugger = false;
                                IsStoppingDebuggerBecauseSuccess = false;
                                DebuggerCloseRequest = -1;
                                NextSourceUrl = "";
                                NextLineNumber = -1;
                                NextColumnNumber = -1;
                                IsFinished = true;
                                return;
                            }
                        }
                    }
                }



                {
                    let IsCurrent = CurrentTaskId == ObtainedId;

                    console.log("Source result from browser, expected = ", CurrentTaskId, ", got = ", ObtainedId);

                    if(IsCurrent)
                    {
                        let Result = o["result"];
                        let ScriptSource = Result["scriptSource"];

                        console.log("Source from browser is same as expected, length = ", ScriptSource.length);
                        console.log("Sending disable debugger request");

                        CurrentScriptSourceResult =
                                CurrentSourceUrl + ":" + CurrentSourceLineNumber + ":" + CurrentSourceColumnNumber + "\n" +
                                ScriptSource;

                        CurrentTaskId = -1;
                        CurrentLineNumber = -1;
                        CurrentColumnNumber = -1;
                        CurrentSourceLineNumber = -1;
                        CurrentSourceColumnNumber = -1;
                        CurrentSourceUrl = "";
                        IsStoppingDebugger = true;
                        IsStoppingDebuggerBecauseSuccess = true;
                        DebuggerCloseRequest = self.Send("Debugger.disable");

                        BrowserAutomationStudio_OpenSourceResult(CurrentScriptSourceResult, true);

                        console.log("Send responce to client, length = ", CurrentScriptSourceResult.length);

                        CurrentScriptSourceResult = "";

                    }

                }

                return;
            }

            if(typeof(o["method"]) == "string")
            {
                let Method = o["method"];

                if(Method == "Debugger.scriptParsed")
                {

                    let Params = o["params"];
                    let Url = Params["url"];

                    if(Url)
                    {
                        let ScriptId = Params["scriptId"];
                        let ReceivedLineStart = Params["startLine"] + 1;
                        let ReceivedColumnStart = Params["startColumn"] + 1;
                        let ReceivedLineEnd = Params["endLine"] + 1;
                        let ReceivedColumnEnd = Params["endColumn"] + 1;

                        console.log("Script parsed for url ", Url, ", script id = ", ScriptId);

                        {
                            if(
                                    //Url matches
                                    (Url == CurrentSourceUrl)
                                    //and
                                    &&
                                    //Source location matches
                                    (
                                        CurrentLineNumber > ReceivedLineStart && CurrentLineNumber < ReceivedLineEnd
                                        ||
                                        CurrentLineNumber == ReceivedLineStart && CurrentColumnNumber >= ReceivedColumnStart
                                        ||
                                        CurrentLineNumber == ReceivedLineEnd && CurrentColumnNumber < ReceivedColumnEnd
                                    )
                            )
                            {
                                CurrentSourceLineNumber = ReceivedLineStart;
                                CurrentSourceColumnNumber = ReceivedColumnStart;
                                let Request = {};
                                Request["scriptId"] = ScriptId;
                                CurrentTaskId = self.SendParams("Debugger.getScriptSource",Request);
                                console.log("Script found, starting request ", CurrentTaskId);
                            }

                        }


                    }
                }
                return;
            }

        };
    }


    this.GetSource = function(Url, LineNumber, ColumnNumber)
    {
        let self = this

        if(IsBusy)
        {
            NextSourceUrl = Url;
            NextLineNumber = LineNumber;
            NextColumnNumber = ColumnNumber;
            console.log("Busy, will try again later.");
        }else
        {
            console.log("Free, starting request.");
            IsBusy = true;
            NextSourceUrl = "";
            CurrentTaskId = -1;
            NextLineNumber = -1;
            NextColumnNumber = -1;
            CurrentScriptSourceResult = "";
            CurrentSourceUrl = Url;
            CurrentLineNumber = LineNumber;
            CurrentColumnNumber = ColumnNumber;
            CurrentSourceLineNumber = -1;
            CurrentSourceColumnNumber = -1;
            IsStoppingDebugger = false;
            DebuggerCloseRequest = -1;
            IsFinished = false;
            IsStoppingDebuggerBecauseSuccess = true;
            if(Timeout)
            {
                clearTimeout(Timeout);
            }
            Timeout = setTimeout(function(){
                self.Timeout();
            }, 5000)

            self.Send("Debugger.enable");
        }
    }

    this.StartNextIfNeeded = function()
    {
        let self = this

        if(NextSourceUrl)
        {
            IsBusy = true;

            NextColumnNumber = -1;
            CurrentScriptSourceResult = "";
            CurrentSourceUrl = NextSourceUrl;
            CurrentLineNumber = NextLineNumber;
            CurrentColumnNumber = NextColumnNumber;
            CurrentSourceLineNumber = -1;
            CurrentSourceColumnNumber = -1;
            IsStoppingDebugger = false;
            DebuggerCloseRequest = -1;
            IsFinished = false;
            NextSourceUrl = "";
            CurrentTaskId = -1;
            NextLineNumber = -1;
            IsStoppingDebuggerBecauseSuccess = false;
            console.log("Another request is pending, url = ", CurrentSourceUrl, ", starting debugger.");
            if(Timeout)
            {
                clearTimeout(Timeout);
            }
            Timeout = setTimeout(function(){
                self.Timeout();
            }, 5000)
            self.Send("Debugger.enable");
        }
    }

    this.Timeout = function()
    {
        let self = this

    
        console.log("Timeout, closing debugger");

        IsStoppingDebugger = true;
        IsStoppingDebuggerBecauseSuccess = false;
        DebuggerCloseRequest = self.Send("Debugger.disable");
        
    }

    this.Send = function(Method)
    {
        let Res = {};
        let SentTask = TaskId;

        Res["id"] = TaskId;
        Res["method"] = Method;

        let Data = JSON.stringify(Res);
        
        TaskId++;

        Connection.send(Data);
        return SentTask;
    }

    this.SendParams = function(Method, Params)
    {

        let Res = {};
        let SentTask = TaskId;

        Res["id"] = TaskId;
        Res["method"] = Method;
        Res["params"] = Params;

        let Data = JSON.stringify(Res);
        
        TaskId++;

        Connection.send(Data);
        return SentTask;
    }


}
