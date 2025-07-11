function FunctionManager()
{
	var EditingFunctionName = "";
	var ParamEditingName = null;
	var MoreDetailsDefault = false;

	//
	//Templates
	//

	var Template = `
		<div id="FunctionManagerMainLabel">
			<img src="icons/functionmanager.png" style="float:left" />
			<span>${tr('Function list')}</span> (<strong><%= GetFunctionList().length %></strong>)
			<a href="#" id="FunctionManagerMainCloseLink">
				<i class="fa fa-times-circle-o FunctionManagerMainClose text-danger" aria-hidden="true" ></i>
			</a>

		</div> 
		<div id="FunctionManagerDescription">${tr('Functions are containers, which holds action list. It helps to group several actions which does same task. For example, there can be function which logins to account, function that checks account balance, etc. It makes your code clear and well organized.')}</div>

		<div class="input-group input-group-sm" style="margin: 10px;">
		  <input type="text" id="FunctionManagerMainLabelSearchInput" class="form-control" placeholder="${tr('Search')}" aria-describedby="FunctionManagerMainLabelSearchLabel" style="border-top-left-radius:2px;border-bottom-left-radius:2px;">
		  
		  <span class="input-group-addon" id="FunctionManagerMainLabelSearchLabel" style="border-top-right-radius: 2px;border-bottom-right-radius: 2px;background-color: #f8f8f8;color: gray;"><i class="fa fa-search" aria-hidden="true"></i></span>
		</div>

		<% _.each(jsort(GetFunctionList()), function(func, index) { %>
            <div class="FunctionManagerItem" data-name="<%= func["name"] %>">
            	<a href="#" data-toggle="tooltip" data-placement="auto" title="${tr('Click to show function in scenario editor')}" class="FunctionManagerItemClick FunctionManagerItem<%= (index%3) + 1 %> <%= (func["name"] == global.function_name) ? "FunctionManagerItemSelected" : "" %>" data-func-name="<%= func["name"] %>" ><%= func["name"] %></a>
				<% if (func["name"] !== "Main") { %>
					<a href="#" class="FunctionManagerItemMenu" data-menu-toggle="false" data-name="<%= func["name"] %>" title="<%= tr('Run this function') %>">
						<i class="fa fa-bolt FunctionManagerItemBar" aria-hidden="true"></i>
					</a>
				<% } %>
				<a href="#" class="FunctionManagerItemMenu" data-menu-toggle="true" data-name="<%= func["name"] %>"><i class="fa fa-bars FunctionManagerItemBar" aria-hidden="true"></i></a>
            </div>
            <div class="FunctionManagerItemMenuContent" data-name="<%= func["name"] %>" data-visible="false">
            	<a href="#" class="FunctionManagerItemLine FunctionManagerItemLine<%= (index%3) + 1 %>">
            		<i class="fa fa-play-circle FunctionManagerItemMenuDescriptionIcon" aria-hidden="true"></i>
            		<span class="FunctionManagerItemLineLeft">
	            		<span class="FunctionManagerItemMenuOpen" data-name="<%= func["name"] %>" style="font-size:small">
	            			<span>${tr('Open')}</span>
	            		</span> 
	            		- 
	            		<span class="FunctionManagerItemMenuDescription">${tr('show function in scenario editor.')}</span>
	            	</span>

           			

            	</a>
            	<% if(func["name"] != "Main") { %>
	            	<a href="#" class="FunctionManagerItemLine FunctionManagerItemLine<%= (index%3) + 1 %>">
	            		<i class="fa fa-pencil FunctionManagerItemMenuDescriptionIcon" aria-hidden="true"></i>
	            		<span class="FunctionManagerItemLineLeft">
		            		<span class="FunctionManagerItemMenuEdit" data-name="<%= func["name"] %>" style="font-size:small">
		            			
		            			<span>${tr('Edit')}</span>
		            		</span> 
		            		- 
		            		<span class="FunctionManagerItemMenuDescription">${tr('edit function name, description, params etc.')}</span>
		            	</span>
	            		
	            	</a>
	            	<a href="#" class="FunctionManagerItemLine FunctionManagerItemLine<%= (index%3) + 1 %>">
	            		<i class="fa fa-bolt FunctionManagerItemMenuDescriptionIcon" aria-hidden="true"></i>
	            		<span class="FunctionManagerItemLineLeft">
		            		<span class="FunctionManagerItemMenuRun" data-name="<%= func["name"] %>" style="font-size:small">
		            			
		            			<span>${tr('Call')}</span>
		            		</span> 
		            		- 
		            		<span class="FunctionManagerItemMenuDescription">${tr('run this function.')}</span>
		            	</span>
	            		
					</a>
					<a href="#" class="FunctionManagerItemLine FunctionManagerItemLine<%= (index%3) + 1 %>">
	            		<i class="fa fa-random FunctionManagerItemMenuDescriptionIcon" aria-hidden="true"></i>
	            		<span class="FunctionManagerItemLineLeft">
		            		<span class="FunctionManagerItemMenuRunAsync" data-name="<%= func["name"] %>" style="font-size:small">
		            			
		            			<span>${tr('Call asynchronously')}</span>
		            		</span> 
		            		- 
		            		<span class="FunctionManagerItemMenuDescription">${tr('run this function in separate thread, wait for results or stop it if necessary.')}</span>
		            	</span>
	            		
	            	</a>
	            	<a href="#" class="FunctionManagerItemLine FunctionManagerItemLine<%= (index%3) + 1 %>">
	            		<i class="fa fa-cubes FunctionManagerItemMenuDescriptionIcon" aria-hidden="true"></i>
	            		<span class="FunctionManagerItemLineLeft">
		            		<span class="FunctionManagerItemMenuRunMultithreaded" data-name="<%= func["name"] %>" style="font-size:small">
		            			
		            			<span>${tr('Call in several threads')}</span>
		            		</span> 
		            		- 
		            		<span class="FunctionManagerItemMenuDescription">${tr('run this function specified number of times with specified number of threads.')}</span>
		            	</span>
	            		
	            	</a>
	            	<a href="#" class="FunctionManagerItemLine FunctionManagerItemLine<%= (index%3) + 1 %>">
	            		<i class="fa fa-times FunctionManagerItemMenuDescriptionIcon" aria-hidden="true"></i>
	            		<span class="FunctionManagerItemLineLeft">
		            		<span class="FunctionManagerItemMenuDelete text-danger" data-name="<%= func["name"] %>" style="font-size:small">
		            			
		            			<span>${tr('Delete')}</span>
		            		</span> 
		            		- 
		            		<span class="FunctionManagerItemMenuDescription">${tr('delete this function.')}</span>
		            	</span>
	            		
					</a>
					<a href="#" class="FunctionManagerItemLine FunctionManagerItemLine<%= (index%3) + 1 %>">
	            		<i class="fa fa-code  FunctionManagerItemMenuDescriptionIcon" aria-hidden="true"></i>
	            		<span class="FunctionManagerItemLineLeft">
		            		<span class="FunctionManagerItemMenuCSharp" data-name="<%= func["name"] %>" style="font-size:small">
		            			
		            			<span>${tr('Run from C#')}</span>
		            		</span> 
		            		- 
		            		<span class="FunctionManagerItemMenuDescription">${tr('run this function from C# code, send parameters and receive result.')}</span>
		            	</span>
	            		
					</a>
					<a href="#" class="FunctionManagerItemLine FunctionManagerItemLine<%= (index%3) + 1 %>">
	            		<i class="fa fa-code  FunctionManagerItemMenuDescriptionIcon" aria-hidden="true"></i>
	            		<span class="FunctionManagerItemLineLeft">
		            		<span class="FunctionManagerItemMenuNodejs" data-name="<%= func["name"] %>" style="font-size:small">
		            			
		            			<span>${tr('Run from Node.js')}</span>
		            		</span> 
		            		- 
		            		<span class="FunctionManagerItemMenuDescription">${tr('run this function from Node.js code, send parameters and receive result.')}</span>
		            	</span>
	            		
					</a>
					<a href="#" class="FunctionManagerItemLine FunctionManagerItemLine<%= (index%3) + 1 %>">
	            		<i class="fa fa-code  FunctionManagerItemMenuDescriptionIcon" aria-hidden="true"></i>
	            		<span class="FunctionManagerItemLineLeft">
		            		<span class="FunctionManagerItemMenuPython" data-name="<%= func["name"] %>" style="font-size:small">
		            			
		            			<span>${tr('Run from Python')}</span>
		            		</span> 
		            		- 
		            		<span class="FunctionManagerItemMenuDescription">${tr('run this function from Python code, send parameters and receive result.')}</span>
		            	</span>
	            		
	            	</a>
	            <% } %>
            </div>
		<% }) %>
        <a href="#" id="FunctionManagerAddTask">+</a>

		

	`

	var TemplateFunction = `
		<div id="FunctionManagerMainLabel">
			<img src="icons/functionmanager.png" style="float:left" />
			<% if(functionname == ''){ %>
				<span>${tr('Adding new function')}</span>
			<% }else{ %>
				<span>${tr('Editing function')}</span> "<strong><%= functionname %></strong>"
			<% } %>
			
			<a href="#" id="FunctionManagerMainCloseLink">
				<i class="fa fa-times-circle-o FunctionManagerMainClose text-danger" aria-hidden="true" ></i>
			</a>
			<a href="#" id="FunctionManagerMainBackLink">
				<i class="fa fa-arrow-circle-o-left FunctionManagerMainBack text-primary" aria-hidden="true" ></i>
			</a>

		</div> 
		<div style="clear:both"></div>



		<div id="FunctionManagerEditData">

			<div class="FunctionManagerEditSection VisibleIfDetailed">
				<i class="fa fa-chevron-right" aria-hidden="true" style="color:#c96055;margin-right:2px;"></i>
				<span>${tr('Function properties:')}</span>
			</div>

			<div id="FunctionManagerEditFunctionNamePanel">
				<div id="FunctionManagerEditFunctionName">
					<span>${tr('Function name:')}</span> 
					<span id="FunctionManagerEditName"></span> 
				</div>
            	<a href="#" class="FunctionManagerEditFunctionLink"><i class="fa fa-pencil FunctionManagerEditFunctionLinkIcon" aria-hidden="true"></i></a>

			</div>

			<div id="FunctionManagerEditFunctionDescriptionPanel" class="VisibleIfDetailed">
				<div id="FunctionManagerEditFunctionDescription">
					<span>${tr('Function description:')}</span>
					<span id="FunctionManagerEditDescription"></span> 
				</div>
            	<a href="#" class="FunctionManagerEditFunctionDescriptionLink"><i class="fa fa-pencil FunctionManagerEditFunctionLinkIcon" aria-hidden="true"></i></a>

			</div>

			<div class="FunctionManagerEditSection VisibleIfDetailed">
				<i class="fa fa-chevron-right" aria-hidden="true" style="color:#c96055;margin-right:2px;"></i>
				<span>${tr('Return value:')}</span>
			</div>

			<div id="FunctionManagerEditHasReturnValuePanel">
				<div id="FunctionManagerEditHasReturnValue">
					<span style="flex-grow: 1;"><span>${tr('Has return value:')}</span> <span id="FunctionManagerEditHasReturnVal">${tr('no')}</span></span>

					<div class="pretty p-switch p-fill" style="margin-right: 21px;">
				        <input type="checkbox" id="FunctionManagerEditHasReturnValCheckbox" />
				        <div class="state p-success">
				            <label></label>
				        </div>
				    </div>
				</div>
			</div>

			<div id="FunctionManagerEditFunctionReturnDescriptionPanel" class="VisibleIfDetailed">
				<div id="FunctionManagerEditFunctionReturnDescription">
					<span>${tr('Return value description:')}</span>
					<span id="FunctionManagerEditReturnDescription"></span> 
				</div>
            	<a href="#" class="FunctionManagerEditFunctionReturnDescriptionLink"><i class="fa fa-pencil FunctionManagerEditFunctionLinkIcon" aria-hidden="true"></i></a>

			</div>

			<div id="FunctionManagerEditFunctionReturnVariablePanel" class="VisibleIfDetailed">
				<div id="FunctionManagerEditFunctionReturnVariable">
					<span>${tr('Default variable name:')}</span>
					<span id="FunctionManagerEditReturnVariable"></span> 
				</div>
            	<a href="#" class="FunctionManagerEditFunctionReturnVariableLink"><i class="fa fa-pencil FunctionManagerEditFunctionLinkIcon" aria-hidden="true"></i></a>

			</div>

			<div class="FunctionManagerEditSection">
				<span style="flex-grow: 1;">
					<i class="fa fa-chevron-right" aria-hidden="true" style="color:#c96055;margin-right:2px;"></i>
					
					
					<span>${tr('Parameters')}</span> <span style="color:gray;font-size:12px;" data-toggle="tooltip" data-placement="auto" title="${tr('Each function can have input parameters. For example, if function logins to some site, then parameters could be account login and account password. Function also can have one output parameter which contains result of its work. Input and output params are optional.')}"><i class="fa fa-question-circle" aria-hidden="true"></i></span> (<strong id="FunctionManagerEditParamsNumber">0</strong>):
				</span>


				<a id="FunctionManagerAddParam" href="#">
					<i class="fa fa-plus-square-o" aria-hidden="true" style="font-style: normal;font-size:14px;margin-right: 3px;"></i>
					<span>${tr('Add')}</span>
				</a>
			</div>

			<div id="FunctionManagerParamList">

				<div id="FunctionManagerParamListEmpty">${tr('Params list is empty')}</div>

			</div>



		</div>
		<div style="height:20px;">&nbsp;</div>
		<a href="#" id="FunctionManagerAddTaskDetailed">${tr('More details')}</a>
		<a href="#" id="FunctionManagerAddTaskCancel">${tr('Cancel')}</a>
		<a href="#" id="FunctionManagerAddTaskResult">${tr('Save changes')}</a>
		

	`
	var TemplateParam = `
	<div data-name="<%- name %>" data-type="<%- type %>" data-description="<%- description %>" data-defaults="<%- defaults %>" class="FunctionManagerParamPanel">
		
		<div class="FunctionManagerEditFunctionParamPanel">
			<div class="FunctionManagerEditFunctionParam">
				<span>` + tr("Name:") + `</span>
				<span class="FunctionManagerEditParam"><%- name %></span> 
				<br/>
				<span>` + tr("Type:") + `</span>
				<span class="FunctionManagerEditParam"><%- type %></span> 
				<br class="VisibleIfDetailed" />
				<span class="VisibleIfDetailed">` + tr("Description:") + `</span>
				<span class="FunctionManagerEditParam VisibleIfDetailed"><%- ((description) ? description : "-") %></span> 
			</div>
        	<div style="float:right;width:40px">
        		<a href="#" class="FunctionManagerEditFunctionParamLink"><i class="fa fa-pencil FunctionManagerEditFunctionLinkIcon" aria-hidden="true"></i></a>
        		<a href="#" class="FunctionManagerEditFunctionDeleteParam"><i class="fa fa-trash FunctionManagerEditFunctionLinkIcon" aria-hidden="true"></i></a>
        	</div>

		</div>
	</div>
	`

	//
	//Rendering
	//

	this.RenderDefault = function()
	{
		this.Render(_TaskCollection,_GobalModel.toJSON())
	}

	this.SendCurrentFunction = function(FunctionName)
	{
		var task = FindTaskById(0)
		var found = false
		while(true)
		{
			if(!task)
				break
			var id = parseInt(task.get("id"))
			if(IsFunctionNode(id) && GetFunctionData(id)["name"] == FunctionName)
			{
				found = true
				break
			}
			task = FindNextTask(id)
		}
		if(found)
		{
			BrowserAutomationStudio_SetCurrentFunction(task.get("fd"))
		}

	}

	this.ChangeFunction = function(FunctionName)
	{
		if(GetFunctionList().find(x=>x.name==FunctionName))
		{
			_MainView.setfunctionname(FunctionName)
	    	this.Hide()
	    	this.SendCurrentFunction(FunctionName)
	    }
	}

	this.Render = function(Data, Global)
	{
		var self = this
        $("#FunctionManagerMain").html(_.template(Template)({data:Data,global:Global}))
        $("#FunctionManagerMainCloseLink").click(function(event){
        	event.preventDefault()
        	self.Hide()
        })
        $(".FunctionManagerItemClick").click(function(event){
        	event.preventDefault()
        	_MainView.setfunctionname($(event.target).attr("data-func-name"))
        	self.Hide()
        	self.SendCurrentFunction($(event.target).attr("data-func-name"))
        })
        $(".FunctionManagerItemLine").click(function(event){
        	event.preventDefault()
        	var Target = $(event.target).closest(".FunctionManagerItemLine")

        	if(Target.find(".FunctionManagerItemMenuOpen").length > 0)
        	{
        		var FuncName = Target.find(".FunctionManagerItemMenuOpen").attr("data-name")
        		_MainView.setfunctionname(FuncName)
        		self.Hide()
        		self.SendCurrentFunction(FuncName)

        	}else if(Target.find(".FunctionManagerItemMenuRun").length > 0)
        	{
				var FuncName = Target.find(".FunctionManagerItemMenuRun").attr("data-name")
        		BrowserAutomationStudio_RunFunction(FuncName);
        		self.Hide()

        	}else if(Target.find(".FunctionManagerItemMenuRunMultithreaded").length > 0)
        	{
				var FuncName = Target.find(".FunctionManagerItemMenuRunMultithreaded").attr("data-name")
        		BrowserAutomationStudio_RunFunctionSeveralThreads(FuncName);
        		self.Hide()
        	}else if(Target.find(".FunctionManagerItemMenuRunAsync").length > 0)
        	{
				var FuncName = Target.find(".FunctionManagerItemMenuRunAsync").attr("data-name")
        		BrowserAutomationStudio_RunFunctionAsync(FuncName);
        		self.Hide()
        	}else if(Target.find(".FunctionManagerItemMenuEdit").length > 0)
        	{
				var FuncName = Target.find(".FunctionManagerItemMenuEdit").attr("data-name")
        		self.RenderFunction(FuncName)
        	}else if(Target.find(".FunctionManagerItemMenuDelete").length > 0)
        	{
				var FuncName = Target.find(".FunctionManagerItemMenuDelete").attr("data-name")
        		_MainView.removefunction(FuncName)
        	}else if(Target.find(".FunctionManagerItemMenuCSharp").length > 0)
        	{
				BrowserAutomationStudio_OpenUrl('https://github.com/CheshireCaat/BASRemote.NET');
				self.Hide()
        	}else if(Target.find(".FunctionManagerItemMenuNodejs").length > 0)
        	{
				BrowserAutomationStudio_OpenUrl('https://github.com/CheshireCaat/bas-remote-node');
				self.Hide()
        	}else if(Target.find(".FunctionManagerItemMenuPython").length > 0)
        	{
				BrowserAutomationStudio_OpenUrl('https://github.com/CheshireCaat/bas-remote-python');
				self.Hide()
        	}
        	
        })
		$("#FunctionManagerMain").contextmenu(function(event) {
			event.preventDefault();
			event.stopPropagation();
			
		});

		$(".FunctionManagerItemMenu").click(function(event){
			event.preventDefault();
			event.stopPropagation();
			var data = $(this).data();

			if (!data.menuToggle) {
				BrowserAutomationStudio_RunFunction(data.name);
				self.Hide();
				return;
			}

			var FunctionName = $(event.target).closest(".FunctionManagerItemMenu").attr("data-name")
			var ContentElement = $(".FunctionManagerItemMenuContent[data-name='" + FunctionName + "']")
			var IsVisible = ContentElement.attr("data-visible") == "true"

			$(".FunctionManagerItemMenuContent").hide()
			$(".FunctionManagerItemMenuContent").attr("data-visible","false")

			$(".FunctionManagerItemMenu").removeClass("FunctionManagerItemMenuCurrent")
			
			if(IsVisible)
			{
				ContentElement.hide().attr("data-visible","false")

			}else
			{
				ContentElement.show().attr("data-visible","true")	
				$(event.target).closest(".FunctionManagerItemMenu").addClass("FunctionManagerItemMenuCurrent")
			}
		})

		$('#FunctionManagerMainLabelSearchInput').off('input').on('input', function() {
			var FilterText = $('#FunctionManagerMainLabelSearchInput').val().toLowerCase()
			$.each( $(".FunctionManagerItem, .FunctionManagerItemMenuContent"), function( Index, Element ){
				Element = $(Element)
				var FunctionName = Element.attr("data-name").toLowerCase()
				if(FilterText.length == 0 || FunctionName.indexOf(FilterText) >= 0)
				{
					Element.removeClass("Filtered")
				}else
				{
					Element.addClass("Filtered")
				}
				
			});
		});

		$("#FunctionManagerAddTask").click(function(){
			self.RenderFunction("")
		})

		tr()

		//$('[data-toggle="tooltip"]').tooltip({trigger:'hover', template:'<div class="mytooltip" role="tooltip"><div class="mytooltip-arrow tooltip-arrow"></div><div class="mytooltip-inner tooltip-inner"></div></div>',delay: { "show": 500, "hide": 100 }})
       
		
	}

	this.RenderFunction = function(FunctionName)
	{
		var self = this
		EditingFunctionName = FunctionName
        $("#FunctionManagerMain").html(_.template(TemplateFunction)({functionname:FunctionName}))
        $("#FunctionManagerMainCloseLink").click(function(event){
        	event.preventDefault()
        	self.Hide()
        })

        $("#FunctionManagerMainBackLink").click(function(event){
        	event.preventDefault()
        	self.RenderDefault()
        })

        $('#FunctionManagerEditHasReturnValCheckbox').change(function() {
        	self.SetHasReturnValue($(this).is(":checked"))
        })

        this.SetFunctionName(FunctionName)
        this.SetFunctionReturnVariable("")

        //Find task and get parse data
        var task = FindTaskById(0)
		var found = false
		while(true)
		{
			if(!task)
				break
			var id = parseInt(task.get("id"))
			if(IsFunctionNode(id) && GetFunctionData(id)["name"] == FunctionName)
			{
				found = true
				break
			}
			task = FindNextTask(id)
		}
		if(found)
		{
			this.ParseData(FunctionName, task.get("fd"))
		}

        /*this.SetFunctionName(FunctionName)
        this.SetFunctionDescription("")
        this.SetFunctionReturnDescription("")
        this.SetFunctionReturnVariable("")
        this.SetHasReturnValue(false)*/


        $(".FunctionManagerEditFunctionLink").click(function(event){
        	event.preventDefault()

	        $("#EditFunctionName").val(self.GetFunctionName())
	        $("#validation").html("")
	        $("#editfunctionmodal").modal({})

        })

        $(".FunctionManagerEditFunctionDescriptionLink").click(function(event){
        	event.preventDefault()
			bootbox.prompt({
			    title: tr("Please input function description : "),
			    inputType: 'textarea',
			    value: self.GetFunctionDescription(),
			    callback: function (result) {
			        if(result === null)
						return
					self.SetFunctionDescription(result); 
			    }
			});

	        

        })

        $(".FunctionManagerEditFunctionReturnDescriptionLink").click(function(event){
        	event.preventDefault()
			bootbox.prompt({
			    title: tr("Please input return value description : "),
			    inputType: 'textarea',
			    value: self.GetFunctionReturnDescription(),
			    callback: function (result) {
			        if(result === null)
						return
					self.SetFunctionReturnDescription(result); 
			    }
			});
        })

        $(".FunctionManagerEditFunctionReturnVariableLink").click(function(event){
        	event.preventDefault()
			bootbox.prompt({
			    title: tr("Please input return variable name : "),
			    value: self.GetFunctionReturnVariable(),
			    callback: function (result) {
			        if(result === null)
						return
					self.SetFunctionReturnVariable(result); 
			    }
			});
        })


        $("#FunctionManagerParamList").off('click', ".FunctionManagerEditFunctionParamLink").on("click", ".FunctionManagerEditFunctionParamLink", function(event){
		    event.preventDefault()

        	var Target = $(event.target).closest(".FunctionManagerParamPanel")
        	var Name = Target.attr("data-name")
        	var Description = Target.attr("data-description")
        	var Type = Target.attr("data-type")
        	var Defaults = Target.attr("data-defaults")
        	$("#EditParameterName").val(Name)
        	$("#EditParameterType").val(Type)
        	$("#EditParameterDescription").val(Description)
        	$("#EditParameterDefaults").val(Defaults)


	        $("#editparametermodal").modal({})
		});

		$("#FunctionManagerParamList").off('click', ".FunctionManagerEditFunctionDeleteParam").on("click", ".FunctionManagerEditFunctionDeleteParam", function(event){
		    event.preventDefault()

        	var Target = $(event.target).closest(".FunctionManagerParamPanel")
        	var Name = Target.attr("data-name")
        	bootbox.confirm(tr("Are you sure that you want to delete") + " '" + Name + "' " + tr("param?"), function(result) {
          		if(result)
        			self.RemoveParam(Name)
        	})

		});

		$("#FunctionManagerParamList").off('click', ".FunctionManagerEditFunctionParamLink").on("click", ".FunctionManagerEditFunctionParamLink", function(event){
		    event.preventDefault()

        	var Target = $(event.target).closest(".FunctionManagerParamPanel")
        	var Name = Target.attr("data-name")
        	var Description = Target.attr("data-description")
        	var Type = Target.attr("data-type")
        	var Defaults = Target.attr("data-defaults")
        	$("#EditParameterName").val(Name)
        	$("#EditParameterType").val(Type)
        	$("#EditParameterDescription").val(Description)
        	$("#EditParameterDefaults").val(Defaults)

        	ParamEditingName = Name

	        $("#validationParam").html("")
	        $("#editparametermodal").modal({})
		});


		$("#FunctionManagerAddParam").click(function(event){
		    event.preventDefault()

		    ParamEditingName = null

        	$("#EditParameterName").val("")
        	$("#EditParameterType").val("String")
        	$("#EditParameterDescription").val("")
        	$("#EditParameterDefaults").val("")
			
	        $("#validationParam").html("")
	        $("#editparametermodal").modal({})

		});


		$("#FunctionManagerAddTaskDetailed").click(function(event){
		    event.preventDefault()
		    if($("#FunctionManagerMain").hasClass("NotDetailed"))
		    {
		    	$("#FunctionManagerMain").removeClass("NotDetailed").removeClass("Detailed").addClass("Detailed")
		    	$("#FunctionManagerAddTaskDetailed").text(tr("Less details"))
		    	$(".EditParameterDescriptionPanel").show()
		    	$(".EditParameterDefaultsPanel").show()
		    	MoreDetailsDefault = true;


		    }else
		    {
		    	$("#FunctionManagerMain").removeClass("NotDetailed").removeClass("Detailed").addClass("NotDetailed")
				$("#FunctionManagerAddTaskDetailed").text(tr("More details"))
				$(".EditParameterDescriptionPanel").hide()
				$(".EditParameterDefaultsPanel").hide()
				MoreDetailsDefault = false;
		    }

		});

		if(MoreDetailsDefault)
		{
			$("#FunctionManagerMain").removeClass("NotDetailed").removeClass("Detailed").addClass("Detailed")
	    	$("#FunctionManagerAddTaskDetailed").text(tr("Less details"))
	    	$(".EditParameterDescriptionPanel").show()
	    	$(".EditParameterDefaultsPanel").show()
		}else
		{
			$("#FunctionManagerMain").removeClass("NotDetailed").removeClass("Detailed").addClass("NotDetailed")
			$("#FunctionManagerAddTaskDetailed").text(tr("More details"))
			$(".EditParameterDescriptionPanel").hide()
			$(".EditParameterDefaultsPanel").hide()

		}

		


		$("#FunctionManagerAddTaskCancel").click(function(event){
		    event.preventDefault()
          	self.RenderDefault()
		});

		$("#FunctionManagerAddTaskResult").click(function(event){
		    event.preventDefault()
		    var name = self.GetFunctionName()
		    if(name.length == 0)
		    {
		    	bootbox.alert(tr("Can't create function with empty name"));
		    	return;
		    }
		    if(FunctionName)
		    {
		    	//Edit existing function
				_GobalModel.set("dontsend",true)

				var task = FindTaskById(0)
				var found = false
				while(true)
				{
					if(!task)
						break
					var id = parseInt(task.get("id"))
					if(IsFunctionNode(id) && GetFunctionData(id)["name"] == FunctionName)
					{
						found = true
						break
					}
					task = FindNextTask(id)
				}
				if(found)
				{
					task.set("name",name)
					task.set("fd", self.SerializeData())
					task.set("code",Normalize(_.template($('#function_code').html())({name:name})))
				}
				//_GobalModel.set("function_name",name)
				//_MainView.SetDefaultInsert()
				_MainView.model.set("dontsend",false)
				BrowserAutomationStudio_SaveToUndoManager()
				BrowserAutomationStudio_AskForVariablesUpdateIfNeeded()
				_MainView.send()
				_MainView.render()
				self.RenderDefault()
		    }else
		    {
		    	//Add new function
		    	var id = Math.floor(Math.random() * (1000000000 - 100)) + 100
          		var code = Normalize(_.template($('#function_code').html())({name:name}))
          		var task = { name: name, fd: self.SerializeData(), code: code, id: id, donotexecuteduringrecord: false, parentid: 0}
          		_TaskCollection.add(task,{at: 1})

		        
          		_GobalModel.set("function_name",name)
          		_MainView.ClearSelection()

         		self.SendCurrentFunction(name)

         		BrowserAutomationStudio_StopSaveToUndoManager = true

         		var i = 1;

          		self.GetParamList().forEach(function(Param){
          			var Id = Math.floor(Math.random() * (1000000000 - 100)) + 100
          			var VarName = self.ClearVariableName(self.Translit(Param.name).replace(/\s/g,"_"))

          			if(VarName.replace(/\_/g,"").length == 0)
          			{
          				VarName	= "FUNCTION_PARAMETER_" + i
          				i++
          			}


          			var Dat = {"s":"getfunctionparameter","v":1,"f":[],"uw":"0","ut":"0","uto":"0","um":"0","d":[{"id":"ParameterName","type":"constr","data":Param.name,"class":"string"},{"id":"Save","type":"var","data":VarName}]}

          			BrowserAutomationStudio_AddTask("\u007b\u0022n\u0022:\u0022" + tr("Obtaining function parameter") + "\u0022,\u0022c\u0022:\u00222\u0022\u007d",`/*Dat:${utf8_to_b64(JSON.stringify(Dat))}*/
      					VAR_${VarName} = _function_argument(${JSON.stringify(Param.name)})`,Id)
					
          		})

          		if(self.GetHasReturnValue())
          		{
          			var Id = Math.floor(Math.random() * (1000000000 - 100)) + 100


					BrowserAutomationStudio_AddTask("\u007b\u0022n\u0022:\u0022" + tr("Finish function and set result") + "\u0022,\u0022c\u0022:\u00221\u0022\u007d",`/*Dat:eyJzIjoicmV0dXJuIiwidiI6MSwiZiI6W10sInV3IjoiMCIsInV0IjoiMCIsInV0byI6IjAiLCJ1bSI6IjAiLCJkIjpbeyJpZCI6IlJldHVyblZhbHVlIiwidHlwZSI6ImNvbnN0ciIsImRhdGEiOiJSZXR1cm4gdmFsdWUiLCJjbGFzcyI6InN0cmluZyJ9XX0=*/
      					_function_return("Return value")`,Id)
          		}


          		BrowserAutomationStudio_StopSaveToUndoManager = false
        		BrowserAutomationStudio_SaveToUndoManager()
        		BrowserAutomationStudio_AskForVariablesUpdateIfNeeded()


          		
          		self.Hide()
		    }
		});
        
		tr()

		//$('[data-toggle="tooltip"]').tooltip({trigger:'hover', template:'<div class="mytooltip" role="tooltip"><div class="mytooltip-arrow tooltip-arrow"></div><div class="mytooltip-inner tooltip-inner"></div></div>',delay: { "show": 500, "hide": 100 }})

    }

    //
    //Params
    //

    this.GetParamList = function()
    {
    	var Res = []
		$.each( $(".FunctionManagerParamPanel"), function( Index, Element ){
			var Name = $(Element).attr("data-name")
			var Type = $(Element).attr("data-type")
			var Description = $(Element).attr("data-description")
			var Defaults = $(Element).attr("data-defaults")
			var El = {name: Name,type: Type,description: Description, defaults: Defaults}
			Res.push(El)
		})
		return Res
    }

    this.FindParam = function(Name)
    {
		var Res = this.GetParamList().find(function(Element) {
			if(Element.name == Name)
				return Element
		});
		if(typeof(Res) == "undefined")
			return null
		return Res
    }

    this.RemoveParam = function(Name)
    {
		$.each( $(".FunctionManagerParamPanel"), function( Index, Element ){
			var CurrentName = $(Element).attr("data-name")
			if(Name == CurrentName)
				$(Element).remove()
		})
	   	this.UpdateParamsInterface()
    }

    this.AddOrChangeParam = function(Name, Type, Description, Defaults)
    {
    	if(Name.length == 0)
    		return "Name is empty"
    	if(ParamEditingName == null)
    	{
    		if(this.FindParam(Name))
    		{
    			return tr("Parameter with this name already exists")
    		}
    	}else
    	{
    		if(Name != ParamEditingName && this.FindParam(Name))
    		{
				return tr("Parameter with this name already exists")
    		}
    		$.each( $(".FunctionManagerParamPanel"), function( Index, Element ){
				var Name = $(Element).attr("data-name")
				if(Name == ParamEditingName)
					$(Element).remove()
			})
    	}
    	$("#FunctionManagerParamList").append(_.template(TemplateParam)({name:Name,type:Type,description:Description,defaults:Defaults}))
    	this.UpdateParamsInterface()
    	return null
    }

    this.UpdateParamsInterface = function()
    {

    	var Items = $('.FunctionManagerParamPanel');
    	$("#FunctionManagerEditParamsNumber").html(Items.length.toString())
    	if(Items.length == 0)
    	{
    		$("#FunctionManagerParamListEmpty").show()
    	}else
    	{
    		$("#FunctionManagerParamListEmpty").hide()
			Items.sort(function(a, b){

			    return $(a).data('name').localeCompare($(b).data('name'));
			});
	    
			Items.appendTo('#FunctionManagerParamList');
		}
    }

    //
    //Tools
    //

    this.ClearVariableName = function(VariableName)
    {
		return VariableName.toUpperCase().replace(/[^a-zA-Z0-9_]/g,"")
    }

    this.Translit = function(Str)
    {
    
	    var ru = {
	        'а': 'a', 'б': 'b', 'в': 'v', 'г': 'g', 'д': 'd', 
	        'е': 'e', 'ё': 'e', 'ж': 'j', 'з': 'z', 'и': 'i', 
	        'к': 'k', 'л': 'l', 'м': 'm', 'н': 'n', 'о': 'o', 
	        'п': 'p', 'р': 'r', 'с': 's', 'т': 't', 'у': 'u', 
	        'ф': 'f', 'х': 'h', 'ц': 'c', 'ч': 'ch', 'ш': 'sh', 
	        'щ': 'shch', 'ы': 'y', 'э': 'e', 'ю': 'u', 'я': 'ya'
	    }, n_str = [];
	    
	    Str = Str.replace(/[ъь]+/g, '').replace(/й/g, 'i');
	    
	    for ( var i = 0; i < Str.length; ++i ) {
	       n_str.push(
	              ru[ Str[i] ]
	           || ru[ Str[i].toLowerCase() ] == undefined && Str[i]
	           || ru[ Str[i].toLowerCase() ].replace(/^(.)/, function ( match ) { return match.toUpperCase() })
	       );
	    }
	    
	    return n_str.join('');
	}
    
	//
	//Data
	//

	this.SerializeData = function()
	{
		var Data = {}
		Data["fd"] = this.GetFunctionDescription()
		Data["hr"] = this.GetHasReturnValue()
		Data["rd"] = this.GetFunctionReturnDescription()
		Data["rv"] = this.GetFunctionReturnVariable()
		Data["p"] = this.GetParamList()

		return JSON.stringify(Data)
	}

	this.ParseData = function(FunctionName, Data)
	{
		var self = this
		try{
			this.SetFunctionName(FunctionName)
			var DataObject = JSON.parse(Data)
			this.SetFunctionDescription(DataObject["fd"])
			this.SetHasReturnValue(DataObject["hr"])
			$('#FunctionManagerEditHasReturnValCheckbox').prop('checked', DataObject["hr"]);
			this.SetFunctionReturnDescription(DataObject["rd"])
			this.SetFunctionReturnVariable(DataObject["rv"])
			ParamEditingName = null
			DataObject["p"].forEach(function(Param){
				self.AddOrChangeParam(Param["name"],Param["type"],Param["description"],Param["defaults"])
			})
		}catch(e)
		{

		}
	}



    this.GetEditingFunctionName = function()
    {
    	return EditingFunctionName
    }

    this.SetHasReturnValue = function(HasReturnValue)
    {
    	if(HasReturnValue)
    	{
    		$("#FunctionManagerEditHasReturnVal").text(tr("yes"))
    		$("#FunctionManagerEditFunctionReturnDescriptionPanel").show()
    		$("#FunctionManagerEditFunctionReturnVariablePanel").show()
    	}else
    	{
			$("#FunctionManagerEditHasReturnVal").text(tr("no"))
			$("#FunctionManagerEditFunctionReturnDescriptionPanel").hide()
    		$("#FunctionManagerEditFunctionReturnVariablePanel").hide()
    	}
    }

    this.GetHasReturnValue = function()
    {
    	return $('#FunctionManagerEditHasReturnValCheckbox').is(":checked")
    }

    this.SetFunctionName = function(FunctionName)
    {
    	if(FunctionName.length == 0)
        {
			$("#FunctionManagerEditName").text(tr("Empty, please set"))
			$("#FunctionManagerEditName").addClass("FunctionManagerEditEmpty")
        }else
        {
        	$("#FunctionManagerEditName").text(FunctionName)
        	$("#FunctionManagerEditName").removeClass("FunctionManagerEditEmpty")
        }
    }

    this.SetFunctionDescription = function(FunctionDescription)
    {
    	if(FunctionDescription.length == 0)
        {
			$("#FunctionManagerEditDescription").text("-")
			$("#FunctionManagerEditDescription").addClass("FunctionManagerEditEmptyNotImportant")
        }else
        {
        	$("#FunctionManagerEditDescription").text(FunctionDescription)
        	$("#FunctionManagerEditDescription").removeClass("FunctionManagerEditEmptyNotImportant")
        }
    }

    this.SetFunctionReturnDescription = function(FunctionReturnDescription)
    {
    	if(FunctionReturnDescription.length == 0)
        {
			$("#FunctionManagerEditReturnDescription").text("-")
			$("#FunctionManagerEditReturnDescription").addClass("FunctionManagerEditEmptyNotImportant")
        }else
        {
        	$("#FunctionManagerEditReturnDescription").text(FunctionReturnDescription)
        	$("#FunctionManagerEditReturnDescription").removeClass("FunctionManagerEditEmptyNotImportant")
        }
    }

    this.SetFunctionReturnVariable = function(FunctionReturnVariable)
    {
    	if(FunctionReturnVariable.length == 0)
        {
			$("#FunctionManagerEditReturnVariable").text(this.ClearVariableName("FUNCTION_RESULT"))
			$("#FunctionManagerEditReturnVariable").addClass("FunctionManagerEditEmptyNotImportant")
        }else
        {
        	$("#FunctionManagerEditReturnVariable").text(this.ClearVariableName(FunctionReturnVariable))
        	$("#FunctionManagerEditReturnVariable").removeClass("FunctionManagerEditEmptyNotImportant")
        }
    }


    this.GetFunctionName = function()
    {
    	if($("#FunctionManagerEditName").hasClass("FunctionManagerEditEmpty"))
    	{
    		return ""
    	}else
    	{
    		return $("#FunctionManagerEditName").text()
    	}
    }

    this.GetFunctionDescription = function()
    {
    	if($("#FunctionManagerEditDescription").hasClass("FunctionManagerEditEmptyNotImportant"))
    	{
    		return ""
    	}else
    	{
    		return $("#FunctionManagerEditDescription").text()
    	}
    }


    this.GetFunctionReturnDescription = function()
    {
    	if($("#FunctionManagerEditReturnDescription").hasClass("FunctionManagerEditEmptyNotImportant"))
    	{
    		return ""
    	}else
    	{
    		return $("#FunctionManagerEditReturnDescription").text()
    	}
    }

    this.GetFunctionReturnVariable = function()
    {
    	if($("#FunctionManagerEditReturnVariable").hasClass("FunctionManagerEditEmptyNotImportant"))
    	{
    		return "FUNCTION_RESULT"
    	}else
    	{
    		return this.ClearVariableName($("#FunctionManagerEditReturnVariable").text())
    	}
    }

    //
    //Visibility
    //
    

	this.Show = function()
	{
		$('#FunctionManagerMain').show()
		$("html").css("overflow","hidden")
		$('#FunctionManagerMain').attr("data-visible","true")
	}

	this.Hide = function()
	{
		$('#FunctionManagerMain').hide()
		$("html").css("overflow","auto")
		$('#FunctionManagerMain').attr("data-visible","false")
	}

	this.IsVisible = function()
	{
		return $("#FunctionManagerMain").attr("data-visible") == "true"
	}

}