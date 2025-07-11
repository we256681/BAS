<div style="margin-top:-4px;overflow:hidden;" id="NodeAll">
	<input data-result-target="#NodeInsertVariableHidden"
		id="NodeInsertVariableHidden"
		disable_globals="true"
		disable_locals="false"
		style="display: none"
	/>
	<table style=" height:100vh; width:100vw;position:relative" >
	<tr>
		<td width="200">
		
			<div style="height:100vh;border-right:1px solid #e4e4e4;position:relative;min-width:200px">
				<div class="list-group" style="padding-bottom: 35px;height: 100vh;padding-top: 5px;margin-left: 4px;margin-right: 4px;overflow:auto" id="NodeFileList">
			        <div class="tr" style="margin-left:15px;margin-bottom: 7px;font-size: small;color: gray;">Files:</div>
					<a href="#" class="btn-sm active list-group-item tr function-item" data-function-name="current" data-selected="true">Current</a>
				
				</div>
				<div style="margin-left: 42px;position: absolute; bottom: 3px;">
					<a tabindex="-1" class="btn btn-sm btn-default" id="add-file"><i class="fa fa-plus" aria-hidden="true" style="font-size: 12px;"></i></a>
					<a tabindex="-1" class="btn btn-sm btn-default disabled" id="remove-file"><i class="fa fa-minus" aria-hidden="true" style="font-size: 12px;"></i></a>
					<a tabindex="-1" class="btn btn-sm btn-default disabled" id="edit-file-name"><i class="fa fa-pencil" aria-hidden="true" style="font-size: 12px;"></i></a>

				</div>
			</div>
		</td>
		<td>
			<div style="height:100vh;">
				<div id="NodeJsCode" style="height:100%;width:100%;border: none; overflow: hidden; outline: none;" />
			</div>
		</td>
		<td width="200" style="border-left:1px solid #e4e4e4">
			<div style="height:100vh;position:relative;min-width:200px">
				<div style="position:absolute;top:3px;right:5px;width:190px;">
			
		        	<span data-preserve="true" data-preserve-type="select" data-preserve-id="NodeVersionList">
				  		<select class="form-control input-sm" id="NodeVersionList" placeholder="Result" style="">
		                </select>
			        </span>
			        <div class="tr" style="margin-top:12px;margin-left:15px;margin-bottom: 7px;font-size: small;color: gray;">Tools:</div>
					<div class="list-group">
						<a href="#" class="btn-sm list-group-item tr" id="NodeInsertVariable">Use variable</a>
						<a href="#" class="btn-sm list-group-item tr" id="NodeSynchronize">Synchronize</a>
						<a href="#" class="btn-sm list-group-item tr" id="NodeCallApi">Call API</a>
						<a href="#" class="btn-sm list-group-item tr" id="NodeFail">Fail</a>
						<a href="#" class="btn-sm list-group-item tr" id="NodeUseFile">Use file</a>
						<hr style="margin-top: 5px;margin-bottom: 5px;"/>
						<a href="#" class="btn-sm list-group-item" id="Manual"><i class="fa fa-question-circle" aria-hidden="true" ></i> <span class="tr">Manual</span></a>
					</div>

					
					<span style="display:none">
			        	<%= _.template($('#input_constructor').html())({id:"Name", description:tr("Name"), default_selector: "string", disable_int:true, value_expression: "Math.random().toString(36).substring(2)"}) %>
			        </span>
        
				</div>
				<div style="margin-left:3px;position: absolute; bottom: 3px;">
					<div style="margin-bottom: 7px;margin-left: 5px;">
						<a href="#" tabindex="-1" id="use-timeout" data-value="<%= (typeof(timeout_value) != "undefined" && timeout_value > 0) ? timeout_value : "0" %>" class="<%= (typeof(timeout_value) != "undefined" && timeout_value > 0) ? "" : "use-timeout-disabled" %> trtitle"  data-enabled="<%= (typeof(timeout_value) != "undefined" && timeout_value > 0) ? "true" : "false" %>" data-toggle="tooltip" data-placement="top" title="Maximum time to perform an action."><i class="fa fa-hourglass-end" aria-hidden="true" ><small id="timeout-is-default" class="tr">default</small></i><small id="timeout-val"></i><%= (typeof(timeout_value) != "undefined" && timeout_value > 0) ? ("" + timeout_value) : "" %></small></a>
              		</div>
              		<div>
						<a tabindex="-1" class="btn btn-success tr" id="node-check" style="min-width: 95px;">Ok</a>
						<a tabindex="-1" class="btn btn-success tr" id="ok" style="display:none">Ok</a> <a tabindex="-1" class="btn btn-danger tr" style="min-width: 95px;" href="#!/" id="backtomain">Cancel</a>
					</div>
				</div>
			</div>
		</td>
	</tr>
	</table>
	<span class="short-description tr">Execute custom code using NodeJS.</span>
</div>

<script>
	$(function() {

		

		
		if(typeof(window.NodeJsResizeAttached) == "undefined")
		{
			window.NodeJsResizeAttached = true

			/* Autoresize code panel */
			$(window).resize(function(){
				$("#NodeJsCode").css("width",($(window).width() - 400) + "px")
			});	

			/* Paste BAS actions */
			document.addEventListener('paste', function(event) {
				var text = event.clipboardData.getData('Text')
				if(text.indexOf("BAS:") == 0)
				{
					text = text.slice(4)
					var text = eval(text)
					
					var all = ""
					text.forEach((el) => {
						var code = el["code"]
						code = code.replace(/\/\*Dat:[^\*]+\*\/\s*/g,"")
						code = code.replace(/\/\*Browser\*\/\s*/g,"")
						if(all.length > 0)
						{
							all += "\n;\n"
						}
						all += code
					})

					setTimeout(function(){
						window.NodeJsGlobal.Editor.trigger('', 'undo');
						var HexRes = Array.from(new TextEncoder("UTF-8").encode(all));
						HexRes = HexRes.map(x => x < 16 ? "0" + x.toString(16) : x.toString(16));
						HexRes = JSON.stringify("_HEX:" + HexRes.join(""));
						InsertText("await BAS_API(" + HexRes + ");");
					},100)

				
				}
				
    			
			});

		}
		

		/* Init model */
		window.NodeJsGlobal = {Editor: null, InitialValue: "", FileList: {}, Selected: "current"}
		
		window.NodeJsGlobal.FileList = _EmbeddedModel.GetAllFiles()

		window.NodeJsGlobal.FileList.forEach((data) => {
			$('#NodeFileList').append($('<a href="#" class="btn-sm list-group-item tr function-item" data-function-name="' + data["data_name"] + '" data-selected="false">' + data["data_name"] + '</a>'));
		})

		
		/* Add new file */
		$("#add-file").click(function(){
			bootbox.prompt(tr("Add new file"), function(result){ 
				if(!result)
					return;
				if(result.length > 0 && result != "current" &&typeof(window.NodeJsGlobal.FileList.find((el) => {return el["data_name"] == result})) == "undefined")
				{
					var data_template = "module.exports = async function(){\n\n}"
					window.NodeJsGlobal.FileList.push({data: data_template, data_name: result, data_type: 1, language_name: "Node",language_version: $("#NodeVersionList").val()})
					var el = $('<a href="#" class="btn-sm list-group-item tr function-item" data-function-name="' + result + '" data-selected="false">' + result + '</a>')
					el.click(FileClick)
					$('#NodeFileList').append(el);
				}
				
			});
		})

		/* Change current file */
		$(".function-item").click(FileClick)

		function FileClick(event){
			event.preventDefault();
			var el = $(this)
			var name = el.attr("data-function-name")
			$(".function-item").removeClass("active")
			el.addClass("active")
			var prev_code = window.NodeJsGlobal.Editor.getValue()

			if(window.NodeJsGlobal.Selected == "current")
			{
				window.NodeJsGlobal.InitialValue = prev_code
			}else
			{
				var found = window.NodeJsGlobal.FileList.find((el) => {return el["data_name"] == window.NodeJsGlobal.Selected})
				if(typeof(found) != "undefined")
				{
					found["data"] = prev_code
				}
				
			}

			window.NodeJsGlobal.Selected = name

			if(name == "current")
			{
				window.NodeJsGlobal.Editor.setValue(window.NodeJsGlobal.InitialValue)

				$("#add-file").removeClass("disabled")
				$("#remove-file").removeClass("disabled").addClass("disabled")
				$("#edit-file-name").removeClass("disabled").addClass("disabled")
			}else
			{
				window.NodeJsGlobal.Editor.setValue(window.NodeJsGlobal.FileList.find((el) => {return el["data_name"] == name})["data"])
				$("#add-file").removeClass("disabled")
				$("#remove-file").removeClass("disabled")
				$("#edit-file-name").removeClass("disabled")
			}
		}

		/* Remove file */
		$("#remove-file").click(function(){
			bootbox.confirm(tr("Are you sure, that you want to remove this file?"), function(result){ 
				if(result)
				{
					$('*[data-function-name="' + window.NodeJsGlobal.Selected + '"]').remove()
					window.NodeJsGlobal.FileList = window.NodeJsGlobal.FileList.filter((el) => {return el["data_name"] != window.NodeJsGlobal.Selected})
					$('*[data-function-name="current"]')[0].click()
				}
			})
		})

		/* Edit file name */
		$("#edit-file-name").click(function(){
			bootbox.prompt(
				{
					title: tr("Edit file name"), 
					value: window.NodeJsGlobal.Selected, 
					callback: function(result){ 
						if(result && result.length > 0 && result != "current" && typeof(window.NodeJsGlobal.FileList.find((el) => {return el["data_name"] == result})) == "undefined")
						{
							var el = $('*[data-function-name="' + window.NodeJsGlobal.Selected + '"]')
							el.attr("data-function-name", result)
							el.text(result)
							var dat = window.NodeJsGlobal.FileList.find((el) => {return el["data_name"] == window.NodeJsGlobal.Selected})
							dat["data_name"] = result
							window.NodeJsGlobal.Selected = result
						}
					}
				}
			)
		})
		


		/* Load code editor */
		require(['vs/editor/editor.main'], function() {
			window.NodeJsGlobal.Editor = createCodeEditor(document.getElementById('NodeJsCode'), {
				value: window.NodeJsGlobal.InitialValue
			});
		});

		/* Save results */
		function CollectData()
		{
			var res = []
			var current = {data_name: $("#Name").val(), data_type: 0, language_name: "Node", language_version: $("#NodeVersionList").val()}
			if(window.NodeJsGlobal.Selected == "current")
			{
				current["data"] = window.NodeJsGlobal.Editor.getValue()
			}else
			{
				current["data"] = window.NodeJsGlobal.InitialValue
			}
			res.push(current)

			window.NodeJsGlobal.FileList.forEach(function(data){
				if(window.NodeJsGlobal.Selected == data["data_name"])
				{
					data["data"] = window.NodeJsGlobal.Editor.getValue()
				}
				res.push(data)				
			})

			
			return res;
		}

		$("#node-check").click(function(){

			bootbox.dialog({ message: '<div class="text-center"><i class="fa fa-spin fa-spinner"></i> ' + tr("Updating") + '...</div>', closeButton: false, animate: false})

			_EmbeddedModel.InsertOrReplaceData(CollectData(), "Node", $("#NodeVersionList").val(), function(is_success, error_string){
    			bootbox.hideAll()

	    		if(is_success)
	    		{
	    			$("#ok").click()
	    		}else
	    		{
					var el = $("<pre style='overflow:scroll;max-height: 80vh;'/>")
					el.text(tr("Failed to update") + ":\n\n\n" + error_string)
					bootbox.dialog({ message: el, animate: false, size: "large", onEscape: true, backdrop: true})
	    		}

	    	})
		})



		if(_EmbeddedModel.GetVersionsForLanguages("Node").length == 0 && !_GobalModel.get("isedit"))
		{
			$("#NodeAll").html('<div style="margin-top:50px;text-align:center">' + tr("Node.js is not enabled, use ") + '<a href="#" onclick="BrowserAutomationStudio_OpenUrl(\'settings://settings\'); return false;">' + tr("settings") + '</a> ' + tr("to enable it") + '.</div> <br/> <div style="text-align:center"><a tabindex="-1" class="btn btn-danger standartbutton tr" href="#!/" id="backtomain">Cancel</a></div>')
		}else
		{

			/* Init versions */
			if(_EmbeddedModel.GetVersionsForLanguages("Node").length == 0)
			{
				try
				{
					var version = CurrentDat["d"].find((el) => {return el["id"] == "NodeVersionList"})["data"]
					$('#NodeVersionList').append($('<option>', {
					    value: version,
					    text: "Node.js " + version
					}));
				}catch(e)
				{
					
				}
				
			}else
			{
			    _EmbeddedModel.GetVersionsForLanguages("Node").forEach((version) => {

					$('#NodeVersionList').append($('<option>', {
					    value: version,
					    text: "Node.js " + version
					}));
				})
			}

			/* Load data to current item */
			$("#Name").change(function() {
				var val = _EmbeddedModel.GetFunctionCode($("#Name").val())
				if(window.NodeJsGlobal.Editor)
				{
					window.NodeJsGlobal.Editor.setValue(val);
				}
				else
				{
					window.NodeJsGlobal.InitialValue = val
				}
			});
		}

		/* Process tools */
		function InsertText(text)
		{
			var line = window.NodeJsGlobal.Editor.getPosition();
			console.log(line)
			var range = new monaco.Range(line.lineNumber, line.column, line.lineNumber, line.column);
			var id = { major: 1, minor: 1 };             
			var op = {identifier: id, range: range, text: text, forceMoveMarkers: true};
			window.NodeJsGlobal.Editor.executeEdits("my-source", [op]);
		}

		$("#NodeFail").click(function(event){
			event.preventDefault()
			InsertText("\nthrow \"Stop reason\";\n");
		})

		$("#NodeInsertVariable").click(function(event){
			event.preventDefault()
			if(window.NodeJsGlobal.Selected != "current")
			{
				bootbox.alert(tr("Variables are not allowed inside files"))
				return
			}
			$("#NodeInsertVariableHidden").val("");
			BasVariablesDialog.create($("#NodeInsertVariableHidden"));
		})

		$("#NodeInsertVariableHidden").change(function(){
			event.preventDefault()
			InsertText($("#NodeInsertVariableHidden").val());
		})

		$("#NodeSynchronize").click(function(event){
			event.preventDefault()
			InsertText("\nawait (new Promise((resolve, reject) => {\n\t\/*Place your code here and call resolve to proceed*\/\n\tresolve()\n}));\n");
		})

		$("#NodeCallApi").click(function(event){
			event.preventDefault()
			bootbox.alert(
				tr("You can call any BAS action from node.js code, including working with browser. To do that, you need just copy and paste any action right into code editor.")
				+ "<br/>"
				+ tr("See following") + " <a href='#' onclick='BrowserAutomationStudio_OpenUrl(\"https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=callapi.gif\");return false;' >" + tr("video") + "</a> " + tr("for more info.")
				+ "<br/>"
				+ "<br/>"
				+ tr("You can also call BAS functions and obtain result.")
				+ `<pre>var Result = await BAS_FUNCTION(
	"FunctionName", 
	{ParamName1: "ParamValue1", ParamName2: "ParamValue2"}
)</pre>`
				 + "<a href='#' onclick='BrowserAutomationStudio_OpenUrl(\"https://wiki.bablosoft.com/doku.php?id=node.js#calling_bas_functions\");return false;' >" + tr("More info") + "</a>"

			)
		})

		$("#Manual").click(function(event){
			event.preventDefault()
			BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/doku.php?id=node.js');
		})

		$("#NodeUseFile").click(function(event){
			event.preventDefault()
			var options = window.NodeJsGlobal.FileList.map((el) => {return {text: el["data_name"], value: el["data_name"]}})
			if(options.length == 0)
			{
				bootbox.alert(tr("File list is empty, please create one."));
			}else
				bootbox.prompt({
	    			title: tr("Select file"),
	    			inputType: 'select',
	    			inputOptions: options,
	    			callback: function (result) {
						if(result && result.length > 0)
						{
							var FolderName = ""
							if(window.NodeJsGlobal.Selected == "current")
							{
								FolderName = "..";
							}else
							{
								FolderName = ".";
							}
							
							
							InsertText("\nawait require(\"" + FolderName + "/" + result + "\")();\n");
						}
	    			}
				});
			
		})

	

	});
	

	
</script>