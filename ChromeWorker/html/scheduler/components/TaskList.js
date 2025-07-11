

window.TaskList = {

    props: ["tasks", "scheduler_log", "application_log", "apiendpoint", "streamendpoint", "bablosoftendpoint", "is_local", "minbasversion", "manual"],
    data: function()
    {
      return {
        sorting: "added",
        isasc: true,
        selectedTask: "",
        reportTask: null,
        schedulerLogSearch: "",
        schedulerLogSearchVisibility: false,
        applicationLogSearch: "",
        applicationLogSearchVisibility: false,
        showArchived: false,
        isLogsVisible: false ,
        isApplicationLogClear: true,
        taskEdit: {id:0},
        taskEditStage: 0,
        taskEditCheckRemoteProject: "",
        taskEditCheckRemoteSuccess: false,
        taskEditValidated: false,
        taskEditHasResources: true,
        inputScriptType: 0,
        inputScriptId: "",
        inputScriptResources: null,
        inputScriptInterface: null,
        inputScriptError: "",
        updateVersion: "",
        schedulerInputs: window.SchedulerInputs

      }
    },
    watch: {
      applicationLogSearch: function () {
        this.FilterApplicationLog()
      },
      applicationLogSearchVisibility: function () {
        this.FilterApplicationLog()
      },
      application_log: function () {
        if(this.application_log.length > 0)
        {
          this.PrependApplicationElements(this.application_log)
          this.isApplicationLogClear = false
        }
      }
    },
    mounted: function()
    {
      var self = this
      setInterval(function(){
        if(self.reportTask && self.reportTask.status == "executing")
        {
          fetch(self.apiendpoint + "/tasks/report/" + self.reportTask.id)
        }
      }, 10000);
      setTimeout(function(){
        self.isLogsVisible = window.innerWidth >= 900
      },1000)

      setInterval(function(){
        fetch(self.apiendpoint + '/check_update', {
          method: "GET",
          cache: "no-cache",
          redirect: "follow",
          referrer: "no-referrer"
        })
        .then(response => response.text())
        .then(text => {
        	self.updateVersion = text
    	})
      },10000)
    },
    methods: {
	  UseRecentBas: function()
	  {
	  	var self = this
		var Endpoint = self.apiendpoint + '/download?file=' + encodeURIComponent(self.schedulerInputs.ini)
        fetch(Endpoint, {
          method: "GET",
          cache: "no-cache",
          redirect: "follow",
          referrer: "no-referrer"
        })
        .then(response => response.text())
        .then(text => {
			var RecentFile = GetRecents(text)
			if(RecentFile.length == 0)
			{
				UIkit.modal.alert(self.$t('Failed to fetch project from BAS.'),{stack: true})
			}else
			{
				var Endpoint = self.apiendpoint + '/download?file=' + encodeURIComponent(RecentFile)
		        fetch(Endpoint, {
		          method: "GET",
		          cache: "no-cache",
		          redirect: "follow",
		          referrer: "no-referrer"
		        })
		        .then(response => response.text())
		        .then(text => {
		        	self.ClearProject()
		        	self.FinalizeTaskUpload(text)

		        })

			}
        })
	  },
      UpdateSchedulerVersion: function()
      {
      	var self = this
      	fetch(this.apiendpoint + '/restart', {
          method: "GET",
          cache: "no-cache",
          redirect: "follow",
          referrer: "no-referrer"
        })
        .then(function() {
        	RestartApplication(self.updateVersion, JSON.stringify(self.schedulerInputs))
    	}).catch(function() {
        	RestartApplication(self.updateVersion, JSON.stringify(self.schedulerInputs))
    	});
    	
      },
      Uninstall: function()
      {
      	fetch(this.apiendpoint + '/uninstall', {
          method: "GET",
          cache: "no-cache",
          redirect: "follow",
          referrer: "no-referrer"
        })
        .then(function() {
        	CloseApplication()
    	}).catch(function() {
        	CloseApplication()
    	});
      },
      Translate: function(Lang)
      {
        window.App.$i18n.locale = Lang
        window.CurrentLang = Lang
        moment.locale(Lang)

        UIkit.offcanvas($("#TaskListMenu")).hide();
      },
      DownloadSchedulerLog: function()
      {
      
        fetch(this.apiendpoint + '/get_scheduler_log', {
          method: "GET",
          cache: "no-cache",
          redirect: "follow",
          referrer: "no-referrer"
        })
        .then(response => response.text())
        .then(text => {
          var blob = new Blob([text], {type: "text/plain;charset=utf-8"});
          saveAs(blob, "scheduler-log.txt");
        })
      },
      DownloadFile: function(Path)
      {
        var Endpoint = this.apiendpoint + '/download?file=' + encodeURIComponent(Path)
        fetch(Endpoint, {
          method: "GET",
          cache: "no-cache",
          redirect: "follow",
          referrer: "no-referrer"
        })
        .then(response => response.text())
        .then(text => {
          var blob = new Blob([text], {type: "text/plain;charset=utf-8"});
          var Split = Path.split(/[\/\\]/)
          saveAs(blob, Split[Split.length - 1]);
        })
      },
      LoadAllTasks: function()
      {
        if (!window.File || !window.FileReader || !window.FileList || !window.Blob) {
          return;
        }
        var self = this

        var input = document.getElementById('LoadAllTasks');
        var length = input.files.length
        if(length > 0)
        {
          var file = input.files[0];
          var fr = new FileReader();
          fr.onload = function(){
            
            var List = JSON.parse(fr.result)
            var Execute = Promise.resolve();
            
            for(var i = 0;i<List.length;i++)
            {

              let Task = List[i]
              Execute = Execute.then(function(){
                return new Promise(function(resolve, reject) {
                  var EndPoint;
                  if(Task.archived)
                  {
                    EndPoint = self.apiendpoint + "/tasks/insertarchived"
                  }else
                  {
                    EndPoint = self.apiendpoint + "/tasks/insert"
                  }
                  delete Task["archived"]
                  var Data = JSON.stringify(Task)
                  fetch(EndPoint, {
                      method: "POST",
                      cache: "no-cache",
                      redirect: "follow",
                      referrer: "no-referrer",
                      headers: {
                        'Accept': 'application/json',
                        'Content-Type': 'application/json'
                      },
                      body: Data
                    }).then(function(response){
                      return response.blob()
                    }).then(function() {
                      resolve()
                    })
                })
              });

            }

            document.getElementById('LoadAllTasks').type = "text";
            document.getElementById('LoadAllTasks').type = "file";
            
          }
          fr.readAsText(file);
        }
        UIkit.offcanvas($("#TaskListMenu")).hide();
      },
      SaveAllTasks: function()
      {
        var self = this
        var Res = self.tasks.map(function(x){
          return {
            name: x.name, 
            script_name: x.script_name,
            script_login: x.script_login,
            script_pass: x.script_pass,
            script_data: x.script_data,
            schedule: x.schedule,
            settings: x.settings,
            archived: (x.status == "archived")
          }
        })
        var blob = new Blob([JSON.stringify(Res)], {type: "text/plain;charset=utf-8"});
        saveAs(blob, "scheduler-tasks.json");
        UIkit.offcanvas($("#TaskListMenu")).hide();
      },
      AcceptCaptcha: function(captcha)
      {
        if(captcha.type == "Browser" && !this.is_local)
        {
          UIkit.modal.alert(this.$t('Manual browser control is only available if you run scheduler locally.'),{stack: true})
          return 
        }
        var Value = $("[data-captcha-id=" + captcha.id + "]").val()
        var formData = new FormData();
        formData.append('id', captcha.id);
        if(captcha.type != "Browser")
          formData.append('data', Value);
        formData.append('success', "true");

        fetch(this.apiendpoint + "/manual/answer", {
          method: "POST",
          cache: "no-cache",
          redirect: "follow",
          referrer: "no-referrer",
          body: formData
        })
      },
      RejectCaptcha: function(captcha)
      {
        var Value = $("[data-captcha-id=" + captcha.id + "]").val()
        var formData = new FormData();
        formData.append('id', captcha.id);
        formData.append('data', "");
        formData.append('success', "false");

        fetch(this.apiendpoint + "/manual/answer", {
          method: "POST",
          cache: "no-cache",
          redirect: "follow",
          referrer: "no-referrer",
          body: formData
        })
      },
      ShowManualActions: function()
      {
        UIkit.modal($('#ManualModal')).show()
      },
      ScheduleTypeFormat: function(schedule)
      {
        if(schedule.type == "now")
        {
          return this.$t("Run now")
        }
        if(schedule.type == "at")
        {
          return this.$t("Run at exact time")
        }
        if(schedule.type == "interval")
        {
          return this.$t("Run with interval")
        }
        if(schedule.type == "hour")
        {
          return this.$t("Start every hour")
        }
        if(schedule.type == "day")
        {
          return this.$t("Start every day")
        }
        if(schedule.type == "week")
        {
          return this.$t("Start every week")
        }
        if(schedule.type == "month")
        {
          return this.$t("Start every month")
        }
        if(schedule.type == "list")
        {
          return this.$t("Start time list")
        }
        if(schedule.type == "event")
        {
          return this.$t("Run after ") + schedule.run_other_task_name
        }
      },
      SaveTaskEdit: function()
      {
        var self = this
        var Id = null
        var TaskOriginal = null

        if(typeof(this.taskEdit.id) != "undefined" && this.taskEdit.id)
        {
          Id = this.taskEdit.id
        }
        delete this.taskEdit["id"]

        var Data = JSON.stringify(this.taskEdit)
        var Endpoint;
        var Result = null

        if(Id)
        {
          
          var TaskOriginal = this.tasks.find((t) => t["id"] == Id)
          if(typeof(TaskOriginal) != "object")
            return

          var Status = TaskOriginal.status

          if(Status == "scheduled_wait_for_execution")
          {
            if(JSON.stringify(TaskOriginal.schedule) != JSON.stringify(this.taskEdit.schedule))
            {
              Result = new Promise(function(resolve, reject) {
                UIkit.modal.confirm(self.$t('The task is scheduled at ') + moment(parseInt(TaskOriginal.run_next_date)).format('dddd, MMMM Do, hh:mm A') + self.$t(', do you want to update schedule?'), {labels: {'ok': self.$t('Yes'), 'cancel': self.$t('No')}, stack: true}).then(
                  function(){resolve(self.apiendpoint + '/tasks/updatehard/' + Id)}
                  ,function(){resolve(self.apiendpoint + '/tasks/updatesoft/' + Id)}
                )
                
              });
            }
            else
              Result = Promise.resolve(self.apiendpoint + "/tasks/updatesoft/" + Id);

          }else if(Status == "downloading" || Status == "executing")
          {
            if(
              TaskOriginal.script_name != this.taskEdit.script_name ||
              TaskOriginal.script_login != this.taskEdit.script_login ||
              TaskOriginal.script_pass != this.taskEdit.script_pass ||
              TaskOriginal.script_data != this.taskEdit.script_data ||
              TaskOriginal.settings != this.taskEdit.settings
              )
            {
              Result = new Promise(function(resolve, reject) {
                UIkit.modal.confirm(self.$t('The task is already running do you want to restart it in order to apply new settings?'), {labels: {'ok': self.$t('Yes'), 'cancel': self.$t('No')}, stack: true}).then(
                  function(){resolve(self.apiendpoint + '/tasks/updatehard/' + Id)}
                  ,function(){resolve(self.apiendpoint + '/tasks/updatesoft/' + Id)}
                )
                
              });
            }else
            {
              Result = Promise.resolve(self.apiendpoint + "/tasks/updatesoft/" + Id);              
            }
          }else
          {
            Result = Promise.resolve(self.apiendpoint + "/tasks/updatesoft/" + Id);
          }

          

        }else
        {
          Result = Promise.resolve(this.apiendpoint + "/tasks/insert");
        }

        
        Result.then(function(Endpoint){
          fetch(Endpoint, {
            method: "POST",
            cache: "no-cache",
            redirect: "follow",
            referrer: "no-referrer",
            headers: {
              'Accept': 'application/json',
              'Content-Type': 'application/json'
            },
            body: Data
          })
          UIkit.modal($('#EditTaskModal')).hide()

        })        

        

      },
      AutogenerateFrameCreated: function(){
        var Resources = {}
        var Data = null
        var DataGenerated = null

        if(this.inputScriptType == 2)
          Data = this.taskEdit.script_data
        else if(this.inputScriptType == 4)
          Data = this.inputScriptResources
        else if(this.inputScriptType == 5)
          DataGenerated = this.inputScriptInterface

        if(this.taskEdit.settings)
          Resources = ResourcesToHash(this.taskEdit.settings)
        else
        {
          if(this.inputScriptType == 2)
            Resources = ResourcesToHash(this.taskEdit.script_data)
          else if(this.inputScriptType == 4 || this.inputScriptType == 5)
          {
            Resources = ResourcesToHash(this.inputScriptResources)
          }
        }
        
        var SendData = {message: "init", data: Data, data_generated: DataGenerated, apiendpoint: this.apiendpoint, resources: Resources}
        document.getElementById("InterfaceAutogenerate").contentWindow.postMessage(JSON.stringify(SendData), "*")
      },
      AutogenerateFrameReceived: function(Data)
      {
        var Res;
        if(this.inputScriptType == 2)
          Res = HashToResources(this.taskEdit.script_data, Data);
        else
          Res = HashToResources(this.inputScriptResources, Data);

        this.inputScriptType = 0
        this.inputScriptId = ""
        this.inputScriptError = ""
        
        this.taskEdit.settings = Res
      },
      CheckVersion: function(Version)
      {
        var split = this.minbasversion.split(".")
        if(split.length != 3)
          return false
        var MinVersion1 = parseInt(split[0])
        var MinVersion2 = parseInt(split[1])
        var MinVersion3 = parseInt(split[2])

        split = Version.split(".")
        if(split.length != 3)
          return false

        var Version1 = parseInt(split[0])
        var Version2 = parseInt(split[1])
        var Version3 = parseInt(split[2])
        if(Version1 > MinVersion1)
          return true

        if(Version1 < MinVersion1)
          return false

        if(Version2 > MinVersion2)
          return true

        if(Version2 < MinVersion2)
          return false


        if(Version3 > MinVersion3)
          return true

        if(Version3 < MinVersion3)
          return false

        return true

      },
      InputData: function()
      {
        var self = this

        this.inputScriptId = "resources_" + Math.random().toString(36).substring(2, 15)
        this.inputScriptType = 0
        this.inputScriptError = ""

        
        if(this.taskEdit.script_data)
        {
          // Starting local script
          if(this.is_local)
          {
            // Running application
            this.inputScriptType = 1
            
            var formData = new FormData();
            formData.append('script', this.taskEdit.script_data);
            formData.append('actual', this.taskEdit.settings);

            fetch(this.apiendpoint + "/tasks/resources_edit_local/" + this.inputScriptId, {
              method: "POST",
              cache: "no-cache",
              redirect: "follow",
              referrer: "no-referrer",
              body: formData
            })
          }else
          {
            // Autogenerate interface, because running from remote
            this.inputScriptType = 2
            $("#InterfaceAutogenerateParent").html('')
            $("#InterfaceAutogenerateParent").html('<iframe src="inputdatagenerated.html" id="InterfaceAutogenerate"></iframe>')
            
          }
        }else
        {
          // Starting remote script

          // Checking if script has web interface
          var formData = new FormData();
          formData.append('email', this.taskEdit.script_login);
          formData.append('pass', this.taskEdit.script_pass);
          fetch(this.bablosoftendpoint + "/apps/" + this.taskEdit.script_name + "/maininterface", {
            method: "POST",
            cache: "no-cache",
            redirect: "follow",
            referrer: "no-referrer",
            body: formData
          })
          .then(response => response.json())
          .then(response => {
            var UsesInterface = response["uses_interface"];
            
            if(UsesInterface)
            {
              // Script has its own web interface, so need to start it
              this.inputScriptType = 5
              this.inputScriptInterface = response["main_interface_html"];
              this.inputScriptResources = response["resources"];
              $("#InterfaceAutogenerateParent").html('')
              $("#InterfaceAutogenerateParent").html('<iframe src="inputdatagenerated.html" id="InterfaceAutogenerate"></iframe>')
            }else
            {
              //Script doesn't have remote interface
              if(self.is_local)
              {
                // Starting remote script

                self.inputScriptType = 3
                var formData = new FormData();
                formData.append('script_name', this.taskEdit.script_name);
                formData.append('script_login', this.taskEdit.script_login);
                formData.append('script_pass', this.taskEdit.script_pass);
                formData.append('actual', this.taskEdit.settings);
                fetch(self.apiendpoint + "/tasks/resources_edit_remote/" + self.inputScriptId, {
                  method: "POST",
                  cache: "no-cache",
                  redirect: "follow",
                  referrer: "no-referrer",
                  body: formData
                })
              }else
              {
                //Autogenerating web interface
                this.inputScriptType = 4
                this.inputScriptResources = response["resources"];
                $("#InterfaceAutogenerateParent").html('')
                $("#InterfaceAutogenerateParent").html('<iframe src="inputdatagenerated.html" id="InterfaceAutogenerate"></iframe>')
                
              }
            }
            
          })
        }
        
      },
      ClearProject: function()
      {
        this.taskEditCheckRemoteSuccess = false
        this.taskEditCheckRemoteProject = ""
        this.taskEditHasResources = true;
        this.taskEdit.settings = "";
        this.taskEdit.script_data = "";
        this.taskEdit.script_name = ""        
        this.taskEdit.script_pass = ""
        this.taskEdit.script_login = ""
        this.inputScriptType = 0;
        this.inputScriptId = "";
        this.inputScriptError = "";
        this.taskEditValidated = false;
      },
      CheckRemoteProject: function()
      {
        var formData = new FormData();
        formData.append('email', this.taskEdit.script_login);
        formData.append('pass', this.taskEdit.script_pass);
        var self = this
        
        this.taskEditCheckRemoteSuccess = false
        this.taskEditCheckRemoteProject = ""
        this.taskEditHasResources = true;
        this.taskEdit.settings = "";
        this.taskEdit.script_data = "";
        this.inputScriptType = 0;
        this.inputScriptId = "";
        this.inputScriptError = "";
        this.taskEditValidated = false;
        
        fetch(this.bablosoftendpoint + "/apps/" + this.taskEdit.script_name + "/maininterface", {
            method: "POST",
            cache: "no-cache",
            redirect: "follow",
            referrer: "no-referrer",
            body: formData
        })
        .then(response => response.json())
        .then(response => {
          if(response["success"] && !self.CheckVersion(response["version"]))
          {
            response["success"] = false
            response["message"] = self.$t("Project version must be at least ") + self.minbasversion

          }
          if(!response["success"])
          {
            self.taskEditCheckRemoteSuccess = false
            self.taskEditCheckRemoteProject = response["message"]
            self.taskEditHasResources = true;
            $("#SaveProjectNotification").stop( true, true ).show().fadeOut(3000)
          }else
          {
            self.taskEditValidated = true;
            self.taskEditCheckRemoteSuccess = true
            self.taskEditCheckRemoteProject = self.$t("Authentication succeeded")
            self.taskEditHasResources = !response["is_empty_data"];
            $("#SaveProjectNotification").stop( true, true ).show().fadeOut(3000)
          }
        })
      },
      TaskEditDataUpdate: function()
      {
        this.ClearProject()

        if (!window.File || !window.FileReader || !window.FileList || !window.Blob) {
          return;
        }
        var self = this

        var input = document.getElementById('TaskData');
        var length = input.files.length
        if(length > 0)
        {
          var file = input.files[0];
          var fr = new FileReader();
          fr.onload = function(){
            
            self.FinalizeTaskUpload(fr.result)

            document.getElementById('TaskData').type = "text";
            document.getElementById('TaskData').type = "file";
            
          }
          fr.readAsText(file);
        }else
        {
          $("#SaveProjectNotificationLocal").removeClass("uk-text-success").removeClass("uk-text-danger").addClass("uk-text-danger").html(self.$t("Project is empty")).stop( true, true ).show().fadeOut(3000)          
        }
      },

      FinalizeTaskUpload: function(Text)
      {
      	var self = this
      	var MatchVersion = /\<EngineVersion\>([^\<]+)\<\/EngineVersion\>/.exec(Text)
        var ValidationResult = true;
        var ValidationMessage = "";

        if(Text.indexOf("<BrowserAutomationStudioProject>") < 0)
        {
          ValidationResult = false
          ValidationMessage = self.$t("Project doesn't have correct format")
        }else if(!MatchVersion)
        {
          ValidationResult = false
          ValidationMessage = self.$t("No information about version")
        }else if(!self.CheckVersion(MatchVersion[1]))
        {
          ValidationResult = false
          ValidationMessage = self.$t("Project version must be at least ") + self.minbasversion
        }


        if(ValidationResult)
        {
          self.taskEdit.script_data = Text;
          self.taskEditHasResources = Text.indexOf("<Model>") >= 0;
          self.taskEditValidated = true;
          $("#SaveProjectNotificationLocal").removeClass("uk-text-success").removeClass("uk-text-danger").addClass("uk-text-success").html(self.$t("Project saved")).stop( true, true ).show().fadeOut(3000)
        }else
        {
          self.taskEdit.script_data = "";
          self.taskEditHasResources = true;
          $("#SaveProjectNotificationLocal").removeClass("uk-text-success").removeClass("uk-text-danger").addClass("uk-text-danger").html(ValidationMessage).stop( true, true ).show().fadeOut(3000)               
        }

      },
      
      AddTask: function()
      {
          this.taskEditStage = 0;
          this.inputScriptType = 0;
          this.inputScriptId = "";
          this.inputScriptError = "";
          this.taskEditValidated = false;
          document.getElementById('TaskData').type = "text";
          document.getElementById('TaskData').type = "file";
          this.taskEditCheckRemoteSuccess = false;
          this.taskEditCheckRemoteProject = "";
          this.taskEditHasResources = true;
          this.taskEdit = {id: 0, script_data: "", 
            schedule: 
              {type: "", interval: 30 * 60, date_start_list: [], sections_hour: [], sections_day: [], sections_week: [], sections_month: []}
          };
          $('.SwitcherProject').hide();
          $('#SwitcherProjectLocal').show();
          $('input[name=ProjectLocalRemoteSwitch]').prop('checked', false);
          $($('input[name=ProjectLocalRemoteSwitch]').get(0)).prop('checked', true);

          if(this.schedulerInputs.method == "Hidden")
          {
          	//This means autoinput
			this.taskEditValidated = true;
			this.taskEdit.script_name = this.schedulerInputs.script_name
			this.taskEdit.script_login = this.schedulerInputs.script_login
			this.taskEdit.script_pass = this.schedulerInputs.script_pass
			this.taskEdit.script_data = this.schedulerInputs.script_data
	        this.RecheckTaskEdit()

          }
          UIkit.modal($('#EditTaskModal')).show()
      },
      EditTask: function(task)
      {
          this.taskEditStage = 0;
          this.inputScriptType = 0;
          this.inputScriptId = "";
          this.inputScriptError = "";
          this.taskEditValidated = true;
          document.getElementById('TaskData').type = "text";
          document.getElementById('TaskData').type = "file";
          this.taskEditCheckRemoteSuccess = false;
          this.taskEditCheckRemoteProject = "";
          this.taskEditHasResources = true;
          this.taskEdit = JSON.parse(JSON.stringify(task))
          $('.SwitcherProject').hide();
          this.RecheckTaskEdit()
          UIkit.modal($('#EditTaskModal')).show()
      },
      RecheckTaskEdit: function()
      {
		if(this.taskEdit.script_name)
		{
			$('#SwitcherProjectRemote').show()
			$('#SwitcherProjectLocal').hide()


			$('input[name=ProjectLocalRemoteSwitch]').prop('checked', false);
			$($('input[name=ProjectLocalRemoteSwitch]').get(1)).prop('checked', true);

			//Need to understand if script has resources
			var formData = new FormData();

			formData.append('email', this.taskEdit.script_login);
			formData.append('pass', this.taskEdit.script_pass);
			var self = this

			fetch(this.bablosoftendpoint + "/apps/" + this.taskEdit.script_name + "/maininterface", {
			  method: "POST",
			  cache: "no-cache",
			  redirect: "follow",
			  referrer: "no-referrer",
			  body: formData
			})
			.then(response => response.json())
			.then(response => {
			  self.taskEditHasResources = !response["is_empty_data"];
			})
		}
		else
		{
			$('#SwitcherProjectLocal').show()
			$('#SwitcherProjectRemote').hide()

			$('input[name=ProjectLocalRemoteSwitch]').prop('checked', false);
			$($('input[name=ProjectLocalRemoteSwitch]').get(0)).prop('checked', true);
			this.taskEditHasResources = this.taskEdit.script_data.indexOf("<Model>") >= 0;
		}
      },
      ClearAllTasks: function()
      {
        this.tasks = this.tasks.splice(0, this.tasks.length);

        fetch(this.apiendpoint + "/tasks/clearall") 
      },
      DeleteArchived: function()
      {
        var i = 0;
        while(true)
        {
          if(i >= this.tasks.length)
          {
            break;
          }
          var Task = this.tasks[i]
          if(Task.status == "archived")
          {
            this.tasks.splice(i,1)
          }else
          {
            i++
          }
        }
        
        fetch(this.apiendpoint + "/tasks/remove_archived")
      },
      AnimateLog: function(node)
      {
        var animationName = "fadeIn"
        node.classList.add('animated', animationName)
        /*function handleAnimationEnd() {
            node.classList.remove('animated', animationName)
            node.removeEventListener('animationend', handleAnimationEnd)
        }

        node.addEventListener('animationend', handleAnimationEnd)*/
      },
      AnimateTask: function(id)
      {
        var node = $("[data-progress-id='" + id + "']")[0]
        var animationName = "bounceIn"
        if(!node.classList.contains('animated'))
        {
          node.classList.add('animated', animationName)
          function handleAnimationEnd() {
              node.classList.remove('animated', animationName)
              node.removeEventListener('animationend', handleAnimationEnd)
          }

          node.addEventListener('animationend', handleAnimationEnd)
        }
      },
      Escape: function(Text)
      {
        return String(Text).replace(/[&<>"'`=\/]/g, function (s) {
          if(s == '&')
          {
            return '&amp;'
          }else if(s == '<')
          {
            return '&lt;'
          }else if(s == '>')
          {
            return '&gt;'
          }else if(s == '"')
          {
            return '&quot;'
          }else if(s == "'")
          {
            return '&#39;'
          }else if(s == '/')
          {
            return '&#x2F;'
          }else if(s == '`')
          {
            return '&#x60;'
          }else if(s == '=')
          {
            return '&#x3D;'
          }
          return entityMap[s];
        });
      },
      FilterApplicationLogItem: function(Item, Search)
      {
        var SubItems = Item.childNodes;
        var KnownProperties = ['data-task-name', 'data-action-id', 'data-result-name', 'data-prefix', 'data-message']
        var IsVisible = new Set();
        for(var j = SubItems.length - 1;j>=0;j--) 
        {
          var SubItem = SubItems[j];
          if(SubItem.classList)
          {
            for(var k = 0; k < KnownProperties.length; k++)
            {
              var Prop = KnownProperties[k]
              if(SubItem.classList.contains(Prop))
              {
                var Dat = Item.getAttribute(Prop)
                var DatEscape = this.Escape(Dat)
                if(!this.applicationLogSearchVisibility)
                {
                  if(SubItem.innerHTML != DatEscape)
                    SubItem.innerHTML = DatEscape
                }else
                {
                  var IsVisibleCurrent = this.ContainsSearch(DatEscape.toLowerCase(),Search)
                  if(IsVisibleCurrent.length >= 0)
                  {
                    SubItem.innerHTML = this.ReplaceSearch(DatEscape, Search, "<span class='SelectedText'>", "</span>")
                    IsVisibleCurrent.forEach(item => IsVisible.add(item))
                  }else
                  {
                    if(SubItem.innerHTML != DatEscape)
                      SubItem.innerHTML = DatEscape
                  }
                }
                break
              }
            }
          }
        }
        if(!this.applicationLogSearchVisibility)
        {
          Item.classList.remove("Hidden");
        }else
        {
          var IsVisibleFinal = true
          Search.forEach(function(el,index){
            if(!IsVisible.has(index))
              IsVisibleFinal = false
          })
          if(IsVisibleFinal)
          {
            Item.classList.remove("Hidden");
          }else
          {
            Item.classList.add("Hidden");
          }

        }

      },
      FilterApplicationLog: function()
      {
          var self = this
          var Search = this.applicationLogSearch.toLowerCase().split(";").map(function(el){return self.Escape(el)})

          var ItemList = document.getElementById("ApplicationLogList");
          var Items = ItemList.childNodes;
          for(var i = Items.length - 1;i>=0;i--)
          {
            var Item = Items[i];
            this.FilterApplicationLogItem(Item, Search)
          }
      },
      PrepareApplicationLogElement: function(Item)
      {
        var prefix = ""
        var self = this

        var found = Item.message.match(/^\s*\[[^\]]+\]\s*((Thread|Поток)[^\:]+\:)?/);
        if(found)
        {
          prefix = found[0]
          Item.message = Item.message.slice(found[0].length)
        }

        var e = document.createElement("div");
        this.AnimateLog(e)
        
        e.classList.add("ApplicationLogListItem");

        var a = document.createElement("a");
        a.setAttribute("href","#")
        a.addEventListener("click", function(){
          self.applicationLogSearch = Item.task_name
          self.applicationLogSearchVisibility = true
        }); 
        a.classList.add("ApplicationLogTask");
        a.classList.add("data-task-name");

        e.setAttribute("data-task-name", Item.task_name)
        a.innerHTML = this.Escape(Item.task_name)
        e.appendChild(a)

        if(Item.action_id)
        {
          e.appendChild(document.createTextNode(" "))
          if(Item.result_index.length == 0)
          {

            var s = document.createElement("span");
            s.innerHTML = this.Escape("[" + Item.action_id + "]")
            s.classList.add("data-action-id");
            e.setAttribute("data-action-id", "[" + Item.action_id + "]")

            e.appendChild(s)
          }else 
          {
            var s = document.createElement("span");
            s.innerHTML = this.Escape(Item.action_id)
            s.classList.add("ResultApplicationLog");
            s.classList.add("data-result-name");
            e.setAttribute("data-result-name", Item.action_id)
            e.appendChild(s)
          }
        }

        e.appendChild(document.createTextNode(" "))
        

        var s1 = document.createElement("span");
        s1.innerHTML = this.Escape(prefix)
        s1.classList.add("data-prefix");
        e.setAttribute("data-prefix", prefix)

        e.appendChild(s1)

        var s2 = document.createElement("span");
        s2.innerHTML = this.Escape(Item.message)

        if(Item.type == 'fail')
        {
          s2.className = "uk-text-danger";
        }
        if(Item.type == 'success')
        {
          s2.className = "uk-text-success";
        }
        s2.classList.add("data-message");
        e.setAttribute("data-message", Item.message)


        e.appendChild(s2)



        return e

      },
      PrependApplicationElements: function(Items)
      {
          $("#ApplicationLogList").show()
          $("#ApplicationLogNoMessages").hide()

          var Fragment = document.createDocumentFragment();
          var self = this
          var Search = this.applicationLogSearch.toLowerCase().split(";").map(function(el){return self.Escape(el)})

          for(var i = 0;i<Items.length;i++)
          {
            if(i>1000)
              break
            var Item = this.PrepareApplicationLogElement(Items[i]);
            this.FilterApplicationLogItem(Item, Search)
            Fragment.appendChild(Item);
            this.isApplicationLogClear = false

          }
          var ItemList = document.getElementById("ApplicationLogList");
          ItemList.prepend(Fragment);
          while(true)
          {
            if(ItemList.childElementCount <= 1000)
              break
            ItemList.removeChild(ItemList.lastChild)
          }
      },
      ReplaceSearch: function(Text, From, WrapLeft, WrapRight)
      {
        var Res = Text
        From.forEach(function(from){
          if(from.length > 0)
          {
            Res = Res.replace(new RegExp(from.replace(/([.*+?^=!:${}()|\[\]\/\\])/g, "\\$1"), 'gi'), function(T,offset,all){
              for(var i = offset + T.length;i<all.length;i++)
              {
                if(all[i] == ">")
                  return T

                if(all[i] == "<")
                  break;
              }
              for(var i = offset - 1;i>=0;i--)
              {
                if(all[i] == ">")
                  break

                if(all[i] == "<")
                  return T
              }

              return WrapLeft + T + WrapRight
            });
          }
        })
        return Res;
      },
      ContainsSearch: function(Text, From)
      {
        var res = []
        From.forEach(function(from, index){

          if(from.length == 0)
            res.push(index)

          if(Text.indexOf(from) >= 0)
            res.push(index)
        })
        return res
      },
      OnMessage: function(data)
      {
        var self = this

        if(data["type"] == "add_task")
        {
          this.tasks.push(data["params"])
        }

        if(data["type"] == "summary")
        {
          this.tasks = data["params"].tasks.filter(function(t){return typeof(t.name) == "string"})
          this.manual = data["params"].manual_actions
          this.scheduler_log = data["params"].scheduler_log
          setTimeout(function(){
            self.application_log = data["params"].application_log 
          },1)
        }

        if(data["type"] == "resources_edit")
        {
          //this.tasks.push(data["params"])
          if(data["params"]["id"] == this.inputScriptId)
          {
            this.inputScriptType = 0
            this.inputScriptId = ""
            if(!data["params"]["success"])
              this.inputScriptError = data["params"]["message"]
            else
              this.inputScriptError = ""
            if(data["params"]["data"])
              this.taskEdit.settings = data["params"]["data"]
          }
        }

        if(data["type"] == "add_scheduler_log")
        {
          this.scheduler_log.push(data["params"])
        }

        if(data["type"] == "manual_add")
        {
          this.manual.push(data["params"])
        }

        if(data["type"] == "manual_remove")
        {
          for(var i = 0; i < this.manual.length; i++)
          { 
            if(this.manual[i].id === data["params"]["id"])
            {
              this.manual.splice(i, 1); 
              break;
            }
          }
          if(this.manual.length == 0)
            UIkit.modal($('#ManualModal')).hide()
        }

        if(data["type"] == "add_application_log")
        {
          var NewLog = data["params"]
          this.PrependApplicationElements(NewLog)
        }

        if(data["type"] == "update_task")
        {
          var object = this.tasks.find((t) => t["id"] == data["params"]["id"])
          if(typeof(object) == "object")
          {
            Object.assign(object, data["params"]);
            if(data["params"].hasOwnProperty("success_current") && data["params"]["success_current"] > 0)
            {
              this.AnimateTask(data["params"]["id"])
            }

          }
        }

        if(data["type"] == "remove_task")
        {
          for(var i = 0; i < this.tasks.length; i++)
          { 
            if(this.tasks[i].id === data["params"]["id"])
            {
              this.tasks.splice(i, 1); 
              break;
            }
          }
        }
      },
      ShowReport: function(Id)
      {
        var object = this.tasks.find((t) => t["id"] == Id)
        if(typeof(object) == "object")
        {
          this.reportTask = object
          if(this.reportTask.status == "executing")
            this.reportTask.report = ""
          UIkit.modal($('#ReportModal')).show()
        }
      },
      ClearSchedulerLog: function()
      {
        this.scheduler_log.splice(0,this.scheduler_log.length)

        UIkit.offcanvas($("#SchedulerLogMenu")).hide();
        fetch(this.apiendpoint + "/scheduler_log/clear/")
        
      },
      ClearApplicationLog: function()
      {
        //this.application_log.splice(0,this.application_log.length)
        $("#ApplicationLogList").html("")
        $("#ApplicationLogNoMessages").show()
        $("#ApplicationLogList").hide()

        UIkit.offcanvas($("#ApplicationLogMenu")).hide();
        fetch(this.apiendpoint + "/application_log/clear/")

        this.isApplicationLogClear = true


      },
      SchedulerLogSearch: function()
      {
        this.schedulerLogSearch = "";
        this.schedulerLogSearchVisibility = !this.schedulerLogSearchVisibility
        UIkit.offcanvas($("#SchedulerLogMenu")).hide();
        $("#SchedulerLogSearch").focus();
      },
      ApplicationLogSearch: function()
      {
        this.applicationLogSearch = "";
        this.applicationLogSearchVisibility = !this.applicationLogSearchVisibility
        UIkit.offcanvas($("#ApplicationLogMenu")).hide();
        $("#ApplicationLogSearch").focus();
      },
      SchedulerLogSearchClose: function(){
          this.schedulerLogSearch = "";
          this.schedulerLogSearchVisibility = false
      },
      ApplicationLogSearchClose: function(){
          this.applicationLogSearch = "";
          this.applicationLogSearchVisibility = false
      },

      ShowMenuScheduler: function()
      {
        UIkit.offcanvas($("#SchedulerLogMenu")).show();
      },

      ShowMenuApplication: function()
      {
        UIkit.offcanvas($("#ApplicationLogMenu")).show();
      },

      ShowTaskListMenu: function()
      {
        UIkit.offcanvas($("#TaskListMenu")).show();
      },
      SelectTask: function(Name)
      {
        if(this.selectedTask == Name)
          this.selectedTask = ""
        else
          this.selectedTask = Name
      },
      MaxChars: function(Text, Len)
      {
        if(Text.length < Len)
          return Text
        return Text.substring(0,Len) + " ..."
      },
      Sorting: function(Name)
      {
        if(Name == this.sorting)
        {
          this.isasc = !this.isasc
        }else
        {
          this.sorting = Name
        }

      },
      HumanReadableStatus: function(Status)
      {
        if(Status == "need_schedule")
          return this.$t("Scheduling")

        if(Status == "scheduled_wait_for_execution")
          return this.$t("Scheduled")
        
        if(Status == "downloading")
          return this.$t("Downloading")
        
        if(Status == "executing")
          return this.$t("Executing")

        if(Status == "archived")
          return this.$t("Archived")
      },
      DescriptionStatus: function(Status)
      {
        if(Status == "need_schedule")
          return this.$t("Task is waiting to be scheduled.")

        if(Status == "scheduled_wait_for_execution")
          return this.$t("Task is scheduled, waiting for execution.")
        
        if(Status == "downloading")
          return this.$t("Setting up application, this may take several minutes.")
        
        if(Status == "executing")
          return this.$t( "Task is executing right now.")

        if(Status == "archived")
          return this.$t("Task was executed desired number of times and archived right now. This means, that it won't be scheduled any more without user interaction.")
      },
      TaskApiNoReturn: function(Endpoint, Task)
      {
        fetch(this.apiendpoint + "/tasks/" + Endpoint + "/" + Task.id)
      }


    },
    computed: {
      validateSchedule: function()
      {
        if(typeof(this.taskEdit) == "undefined")
          return {type: "general", message: "No object"}

        if(!this.taskEdit.hasOwnProperty("schedule"))
          return {type: "general", message: "No object"}

        var Schedule = this.taskEdit.schedule
        if(!Schedule.hasOwnProperty("type"))
          return {type: "general", message: "No object"}

        var Type = Schedule.type
        if(!Type)
          return {type: "general", message: "No object"}

        if(Type == "at")
        {
          if(!Schedule.hasOwnProperty("date_start") || !Schedule.date_start)
            return {type: "at", message: this.$t("Start date is not selected")}
        }
        if(Type == "interval")
        {
          if(
            (Schedule.hasOwnProperty("date_start") && Schedule.date_start) && 
            (Schedule.hasOwnProperty("date_end") && Schedule.date_end) &&
            parseInt(Schedule.date_end) < parseInt(Schedule.date_start)
          )
            return {type: "interval", message: this.$t("Start date is greater than end date")}
        }
        if(Type == "hour")
        {
          if(
            (Schedule.hasOwnProperty("date_start") && Schedule.date_start) && 
            (Schedule.hasOwnProperty("date_end") && Schedule.date_end) &&
            parseInt(Schedule.date_end) < parseInt(Schedule.date_start)
          )
            return {type: "hour", message: this.$t("Start date is greater than end date")}
          
          if(Schedule.sections_hour.length == 0)
            return {type: "hour", message: this.$t("'Start every hour at' is empty")}
        }
        if(Type == "day")
        {
          if(
            (Schedule.hasOwnProperty("date_start") && Schedule.date_start) && 
            (Schedule.hasOwnProperty("date_end") && Schedule.date_end) &&
            parseInt(Schedule.date_end) < parseInt(Schedule.date_start)
          )
            return {type: "day", message: this.$t("Start date is greater than end date")}
          
          if(Schedule.sections_day.length == 0)
            return {type: "day", message: this.$t("'Start every day at' is empty")}
        }
        if(Type == "week")
        {
          if(
            (Schedule.hasOwnProperty("date_start") && Schedule.date_start) && 
            (Schedule.hasOwnProperty("date_end") && Schedule.date_end) &&
            parseInt(Schedule.date_end) < parseInt(Schedule.date_start)
          )
            return {type: "week", message: this.$t("Start date is greater than end date")}
          
          if(Schedule.sections_week.length == 0)
            return {type: "week", message: this.$t("'Start every week at' is empty")}
        }
        if(Type == "month")
        {
          if(
            (Schedule.hasOwnProperty("date_start") && Schedule.date_start) && 
            (Schedule.hasOwnProperty("date_end") && Schedule.date_end) &&
            parseInt(Schedule.date_end) < parseInt(Schedule.date_start)
          )
            return {type: "month", message: this.$t("Start date is greater than end date")}
          
          if(Schedule.sections_month.length == 0)
            return {type: "month", message: this.$t("'Start every month at' is empty")}
        }
        if(Type == "list")
        {
          
          if(Schedule.date_start_list.length == 0)
            return {type: "list", message: this.$t("'Start date list' is empty")}
        }
        if(Type == "event")
        {
          
          if(typeof(Schedule.run_other_task_name) == "undefined" || Schedule.run_other_task_name.length == 0)
            return {type: "event", message: this.$t("Task name not set")}
        }
        return null;
      },
      /*applicationLogPrepared: function (){
        if(this.application_log.length > 1000)
          this.application_log.splice(1000)
        var self = this
        var clone = []
        this.application_log.forEach(function(el){
          clone.push(jQuery.extend(true, {}, el))
        })
        var res = clone.filter(function(x){
          x.task_name_highlight = x.task_name
          var element = x

          x.prefix = ""

          var found = x.message.match(/^\s*\[[^\]]+\]\s*((Thread|Поток)[^\:]+\:)?/);
          if(found)
          {
            x.prefix = found[0]
            x.message = x.message.slice(found[0].length)
          }
          
          if(!self.applicationLogSearchVisibility || self.applicationLogSearch == '')
            return true
          var Search = self.applicationLogSearch.toLowerCase().split(";")
          var IsVisible = new Set();
          var IsVisibleCurrent = self.ContainsSearch(x.message.toLowerCase(),Search)
          if(IsVisibleCurrent.length >= 0)
          {
            x.message = self.ReplaceSearch(x.message, Search, "<span class='SelectedText'>", "</span>")
            IsVisibleCurrent.forEach(item => IsVisible.add(item))
          }

          IsVisibleCurrent = self.ContainsSearch(x.prefix.toLowerCase(),Search)
          if(IsVisibleCurrent.length >= 0)
          {
            x.prefix = self.ReplaceSearch(x.prefix, Search, "<span class='SelectedText'>", "</span>")
            IsVisibleCurrent.forEach(item => IsVisible.add(item))
          }

          IsVisibleCurrent = self.ContainsSearch(x.task_name.toLowerCase(),Search)
          if(IsVisibleCurrent.length >= 0)
          {
            x.task_name_highlight = self.ReplaceSearch(x.task_name_highlight, Search, "<span class='SelectedText'>", "</span>")
            IsVisibleCurrent.forEach(item => IsVisible.add(item))
          }

          IsVisibleCurrent = self.ContainsSearch(x.action_id.toLowerCase(),Search)
          if(IsVisibleCurrent.length >= 0)
          {
            x.action_id = self.ReplaceSearch(x.action_id, Search, "<span class='SelectedText'>", "</span>")
            IsVisibleCurrent.forEach(item => IsVisible.add(item))                         
          }
          var IsVisibleFinal = true
          Search.forEach(function(el,index){
            if(!IsVisible.has(index))
              IsVisibleFinal = false
          })
          return IsVisibleFinal
        })
        return res
      },*/

      schedulerLogPrepared: function (){
        var self = this
        if(this.scheduler_log.length > 300)
          this.scheduler_log.splice(0,this.scheduler_log.length - 300)
        var Search = self.schedulerLogSearch.toLowerCase().split(";").map(function(el){return self.Escape(el)})
        var res1 = JSON.parse(JSON.stringify(this.scheduler_log)).filter(function(x){
          var element = x
          
          if(!x.task_name)
            x.task_name = ""

          x.task_name_highlight = self.Escape(x.task_name)

          element.message = self.$t(element.message)

          if(element.message.indexOf("<date>") >= 0)
          {
            element.message = element.message.replace("<date>","<i>" + moment(parseInt(element.date_replace)).format('dddd, MMMM Do, hh:mm A') + "</i>")
          }
          if(element.message.indexOf("<a data-fail>") >= 0)
          {
            var el = $(element.message)
            el.attr("style","color:#f0506e;text-decoration:none;")
            el.attr("data-script",element.task_name)
            el.attr("data-data",element.scheduler_log)
            el.attr("href","#")
            el.attr("onclick","$('#FailStartModalData').html($(this).attr('data-data'));$('#FailStartModalScript').html($(this).attr('data-script'));UIkit.modal($('#FailStartModal')).show();return false")
            el.prepend($('<span style="margin-right:3px" uk-icon="icon: link; ratio: 0.75"></span>'))
            element.message = $('<div>').append(el.clone()).html();
          }

          if(element.message.indexOf("<a data-success>") >= 0)
          {
            var el = $(element.message)
            el.attr("style","color:#32d296;text-decoration:none;")
            el.attr("href","#")
            el.attr("onclick","window.App.GetTaskList().ShowReport('" + element.task_id + "');return false")
            el.prepend($('<span style="margin-right:3px" uk-icon="icon: link; ratio: 0.75"></span>'))
            element.message = $('<div>').append(el.clone()).html();
          }

          
          

          if(!self.schedulerLogSearchVisibility || self.schedulerLogSearch == '')
            return true
          
          var IsVisible = new Set();
          var IsVisibleCurrent = self.ContainsSearch(x.message.toLowerCase(),Search)
          if(IsVisibleCurrent.length >= 0)
          {
            x.message = self.ReplaceSearch(x.message, Search, "<span class='SelectedText'>", "</span>")
            IsVisibleCurrent.forEach(item => IsVisible.add(item)) 
          }
          IsVisibleCurrent = self.ContainsSearch(x.task_name_highlight.toLowerCase(),Search)
          if(IsVisibleCurrent.length >= 0)
          {
            x.task_name_highlight = self.ReplaceSearch(x.task_name_highlight, Search, "<span class='SelectedText'>", "</span>")
            IsVisibleCurrent.forEach(item => IsVisible.add(item)) 
          }
          var IsVisibleFinal = true
          Search.forEach(function(el,index){
            if(!IsVisible.has(index))
              IsVisibleFinal = false
          })
          return IsVisibleFinal
        })
        var res1 = res1.sort(function(a,b){

          return - parseInt(a["date"]) + parseInt(b["date"])  
        })
        
        //var CurrentItem = {date: "", data: []}
        var LastDate = ""
        var CurrentItem = null
        var res = []
        res1.forEach(function(element){
          
          var CurrentDate = moment(parseInt(element.date)).format('dddd, MMMM Do')

          if(CurrentDate != LastDate)
          {
            CurrentItem = {date: CurrentDate, data: []}
            res.push(CurrentItem)
            LastDate = CurrentDate

          }
          CurrentItem.data.push(element)
        })
        return res
      },

      tasksPrepared: function (){
        var self = this
        var res = []
        if(this.sorting == "added")
          res = this.tasks.sort(function(a,b){
            if(a["status"] == b["status"])
            {
              var res = - parseInt(a["date_created"]) + parseInt(b["date_created"])  
              if(self.isasc)
                return res
              else
                return -res

            }else
            {
              var ai = -1;
              if(a["status"] == "archived")
                ai = 0
              if(a["status"] == "need_schedule")
                ai = 1
              if(a["status"] == "scheduled_wait_for_execution")
                ai = 2
              if(a["status"] == "downloading")
                ai = 3
              if(a["status"] == "executing")
                ai = 4

              var bi = -1;
              if(b["status"] == "archived")
                bi = 0
              if(b["status"] == "need_schedule")
                bi = 1
              if(b["status"] == "scheduled_wait_for_execution")
                bi = 2
              if(b["status"] == "downloading")
                bi = 3
              if(b["status"] == "executing")
                bi = 4
              
              return - ai + bi
            }
            
          })

        if(this.sorting == "scheduled")
          res = this.tasks.sort(function(a,b){
            if(a["status"] == b["status"])
            {
              var res = - parseInt(a["run_next_date"]) + parseInt(b["run_next_date"])  
              if(self.isasc)
                return res
              else
                return -res

            }else
            {
              var ai = -1;
              if(a["status"] == "archived")
                ai = 0
              if(a["status"] == "need_schedule")
                ai = 1
              if(a["status"] == "scheduled_wait_for_execution")
                ai = 2
              if(a["status"] == "downloading")
                ai = 3
              if(a["status"] == "executing")
                ai = 4

              var bi = -1;
              if(b["status"] == "archived")
                bi = 0
              if(b["status"] == "need_schedule")
                bi = 1
              if(b["status"] == "scheduled_wait_for_execution")
                bi = 2
              if(b["status"] == "downloading")
                bi = 3
              if(b["status"] == "executing")
                bi = 4
              
              return - ai + bi
            }
            
          })

        if(this.sorting == "name")
          res = this.tasks.sort(function(a,b){
            if(a["status"] == b["status"])
            {
              var res = - a["name"].localeCompare(b["name"])
              if(self.isasc)
                return res
              else
                return -res
            }else
            {
              var ai = -1;
              if(a["status"] == "archived")
                ai = 0
              if(a["status"] == "need_schedule")
                ai = 1
              if(a["status"] == "scheduled_wait_for_execution")
                ai = 2
              if(a["status"] == "downloading")
                ai = 3
              if(a["status"] == "executing")
                ai = 4

              var bi = -1;
              if(b["status"] == "archived")
                bi = 0
              if(b["status"] == "need_schedule")
                bi = 1
              if(b["status"] == "scheduled_wait_for_execution")
                bi = 2
              if(b["status"] == "downloading")
                bi = 3
              if(b["status"] == "executing")
                bi = 4
              
              return - ai + bi
            }
            
          })

        var archive_number = 0;
        for(var i = 0;i<res.length;i++)
        {
          var Item = res[i]
          if(Item.status == "archived")
          {
            archive_number++
          }
        }
        for(var i = 0;i<res.length;i++)
        {
          var Item = res[i]
          var ItemPrev = null
          if(i >= 1)
          {
            ItemPrev = res[i - 1]
          }
          if(Item.status == "archived" && (!ItemPrev || ItemPrev.status != "archived"))
          {
            Item.archive_number = archive_number
          }else
          {
            Item.archive_number = 0
          }
        }
        return res
      }
    },

    updated: function()
    {
      
    },
    

    template: `<div :class="{'LogVisible': isLogsVisible && (schedulerLogPrepared.length > 0 || !isApplicationLogClear),'LogInvisible': !(isLogsVisible && (schedulerLogPrepared.length > 0 || !isApplicationLogClear))}">

      <div id="ApplicationLogHeader">
        <div class="PanelHeader">
          <span v-show="!applicationLogSearchVisibility">
            <span uk-icon="icon: table; ratio: 1.4" class="PanelHeaderIcon" style="margin-right:10px;"></span>
            <span class="PanelHeaderText">{{$t('Application log')}}</span>
          </span>
          <span v-show="applicationLogSearchVisibility">
            <span uk-icon="icon: search; ratio: 1.4" class="PanelHeaderIcon" style="margin-right:10px;float: left;margin-top: 3px"></span>
            <input v-model="applicationLogSearch" :uk-tooltip="$t('Use ; symbol to search for several values. For example Task name;Thread #3')" @keydown.esc="ApplicationLogSearchClose()" id="ApplicationLogSearch" class="uk-input PanelHeaderSearch" type="text" :placeholder="$t('What to search ?')" style="cursor:auto">
            <a href="#" @click.prevent="ApplicationLogSearchClose()">
              <span uk-icon="icon: close; ratio: 1.4" class="PanelHeaderIcon" style="margin-left:10px;margin-right:10px;float: left;margin-top: 3px"></span>
            </a>
          </span>
          <a href="#" @click.prevent="ShowMenuApplication()">
            <span uk-icon="icon: grid; ratio: 1.3" class="PanelHeaderGrid"></span>
          </a>
        </div>
      </div>


      <div id="SchedulerLogHeader">
        <div class="PanelHeader">
          <span v-show="!schedulerLogSearchVisibility">
            <span uk-icon="icon: table; ratio: 1.4" class="PanelHeaderIcon" style="margin-right:10px;"></span>
            <span class="PanelHeaderText">{{$t('Scheduler log')}}</span>
          </span>
          <span v-show="schedulerLogSearchVisibility">
            <span uk-icon="icon: search; ratio: 1.4" class="PanelHeaderIcon" style="margin-right:10px;float: left;margin-top: 3px"></span>
            <input v-model="schedulerLogSearch" @keydown.esc="SchedulerLogSearchClose()" :uk-tooltip="$t('Use ; symbol to search for several values. For example Task name;Thread #3')" id="SchedulerLogSearch" class="uk-input PanelHeaderSearch" type="text" :placeholder="$t('What to search ?')" style="cursor:auto">
            <a href="#" @click.prevent="SchedulerLogSearchClose()">
              <span uk-icon="icon: close; ratio: 1.4" class="PanelHeaderIcon" style="margin-left:10px;margin-right:10px;float: left;margin-top: 3px"></span>
            </a>
          </span>
          <a href="#" @click.prevent="ShowMenuScheduler()">
            <span uk-icon="icon: grid; ratio: 1.3" class="PanelHeaderGrid"></span>
          </a>
        </div>
      </div>

      <div id="SchedulerLog" data-simplebar data-simplebar-auto-hide="false" data-simplebar-scrollbar-min-size="50">
        <div v-show="schedulerLogPrepared.length == 0" class="NoMessagesScheduler">
          {{$t('No scheduler log messages')}}
        </div>

        <div>
          <div v-for="itemDate in schedulerLogPrepared" :key="itemDate.date">
            <div class="SchedulerLogDate animated fadeIn">{{itemDate.date}} : </div>
            <ul class="uk-list uk-list-striped" v-show="itemDate.data.length > 0" style="margin:0px">
              <li v-for="item in itemDate.data" :key="item.id" style="border-bottom: 1px solid #e5e5e5;border-top:none;" class="animated fadeIn">
                <a href="#" @click.prevent="schedulerLogSearchVisibility = true;schedulerLogSearch = item.task_name" style="font-weight:bold;color:rgb(102, 102, 102)" v-if="item.task_name_highlight" v-html="item.task_name_highlight"></a> 
                [{{moment(parseInt(item.date)).format('HH:mm:ss')}}] <span :class="{'uk-text-danger':item.status == 'Error','uk-text-success':item.status == 'Success'}" v-html="item.message"></span>
              </li>
            </ul> 
            
          </div>
        </div>
      </div>


      <div id="ApplicationLog" data-simplebar data-simplebar-auto-hide="false" data-simplebar-scrollbar-min-size="50">
        <div id="ApplicationLogNoMessages" class="NoMessagesApplication">
          {{$t('No applicaition log messages')}}
        </div>

        <div>
          <div id="ApplicationLogList" style="display:none">
          </div>
        </div>
      </div>


      <div id="TaskListHeader">
        <div class="PanelHeader">
          <span uk-icon="icon: calendar; ratio: 1.3" class="PanelHeaderIcon"></span>
          <span class="PanelHeaderText">{{$t('Task list')}}</span>
          <a href="#" class="HideLogs" v-if="isLogsVisible && (schedulerLogPrepared.length > 0 || !isApplicationLogClear)" @click.prevent="isLogsVisible = false">
            <span uk-icon="icon: chevron-right; ratio: 1.3"></span>
          </a>
          <a href="#" class="HideLogs" v-if="!isLogsVisible && (schedulerLogPrepared.length > 0 || !isApplicationLogClear)" @click.prevent="isLogsVisible = true">
            <span uk-icon="icon: chevron-left; ratio: 1.3"></span>
          </a>

          
          <a href="#" @click.prevent="ShowTaskListMenu()">
            <span uk-icon="icon: grid; ratio: 1.3" class="PanelHeaderGrid"></span>
          </a>

          <a href="#" @click.prevent="ShowManualActions()" class="ManualIconPanel" v-if="manual && manual.length > 0">
            <span uk-icon="icon: image; ratio: 1.3" class="ManualIcon"></span>
            <span class="ManualIconNumber">
              {{ (manual.length > 99) ? "99+" : manual.length }}
            </span>
          </a>

          <a href="#" :uk-tooltip="$t('New version is available')" @click.prevent="UIkit.modal.confirm($t('Version ') + updateVersion + $t(' is available. Are you sure, that you want to update scheduler? All tasks will be stopped during update.')).then(function(){UpdateSchedulerVersion()})" class="UpdateIconPanel" v-if="updateVersion.length > 0">
            <span uk-icon="icon: cloud-download; ratio: 1.3" class="UpdateIcon"></span>
            <span class="UpdateIconNumber">
              {{ updateVersion }}
            </span>
          </a>

        </div>
      </div>

      <div id="TaskList" data-simplebar data-simplebar-auto-hide="false">
        
        <div v-show="tasksPrepared.length == 0" class="NoTasks">
          {{$t('No tasks created yet.')}}
        </div>
        <a href="#" id="AddTask" @click.prevent="AddTask()">
          <span uk-icon="icon: plus"></span>
        </a>

        <div id="TaskListBody" v-show="tasksPrepared.length > 0">
            <div v-for="task in tasksPrepared" :key="task.id">
              <div v-if="task.archive_number" class="ArchivedNumber">
                {{$t('There are')}} {{task.archive_number}} {{$t('archived records.')}} 
                <a href="#" @click.prevent="showArchived=true" v-if="!showArchived">{{$t('Show')}}</a>
                <a href="#" @click.prevent="showArchived=false" v-else>{{$t('Hide')}}</a>
                <a href="#" @click.prevent="UIkit.modal.confirm($t('Are you sure, that you want to delete all archived records?')).then(function(){DeleteArchived()})" class="uk-text-danger DeleteArchived"> {{$t('Remove all archived')}}</a>
              </div>
              
                <div class="TaskItem" v-show="showArchived || task.status != 'archived'">
                
                  <span :data-progress-id="task.id" v-bind:class="{ Danger: (task.status === 'need_schedule' || task.status === 'scheduled_wait_for_execution'), Warning: task.status === 'downloading', Success: task.status === 'executing', Muted: task.status === 'archived', TaskStatusItem: true }" class="TaskStatusItem Danger">
                    <span :uk-tooltip="$t('Task is scheduled, waiting for execution.')" v-if="task.status === 'need_schedule' || task.status === 'scheduled_wait_for_execution'" uk-icon="icon: clock; ratio: 1.0"></span>
                    <span :uk-tooltip="$t('Setting up application, this may take several minutes.')" v-if="task.status === 'downloading'" uk-icon="icon: download; ratio: 1.0"></span>
                    <span :uk-tooltip="$t('Task is executing right now.')" v-if="task.status === 'executing'" uk-icon="icon: play-circle; ratio: 1.0"></span>
                    <span :uk-tooltip="$t('Task was executed desired number of times and archived right now. This means, that it wont be scheduled any more without user interaction.')" v-if="task.status === 'archived'" uk-icon="icon: ban; ratio: 1.0"></span>

                  </span>
                  
                  <span class="TaskItemName TaskRowBig">{{ MaxChars(task.name,40) }}</span>
                  
                  <span v-if="task.status === 'need_schedule'" class="uk-text-muted TaskItemState TaskRow">
                    ( <span class="TaskStatusLabel" :uk-tooltip="$t('Task is waiting to be scheduled.')">{{$t('scheduling')}}</span> )  
                  </span>
                  <span v-if="task.status === 'scheduled_wait_for_execution'" class="uk-text-muted TaskItemState TaskRow">
                    ( <span class="TaskStatusLabel" :uk-tooltip="$t('Task is scheduled, waiting for execution.')">{{$t('scheduled')}}</span> )  
                  </span>
                  <span v-if="task.status === 'downloading'" class="uk-text-muted TaskItemState TaskRow">
                    ( <span class="TaskStatusLabel" :uk-tooltip="$t('Setting up application, this may take several minutes.')">{{$t('downloading')}}</span> )  
                  </span>
                  <span v-if="task.status === 'executing'" class="uk-text-muted TaskItemState TaskRow">
                    ( <span class="TaskStatusLabel" :uk-tooltip="$t('Task is executing right now.')">{{$t('executing')}}</span> )  
                  </span>
                  <span v-if="task.status === 'archived'" class="uk-text-muted TaskItemState TaskRow">
                    ( <span class="TaskStatusLabel" :uk-tooltip="$t('Task was executed desired number of times and archived right now. This means, that it wont be scheduled any more without user interaction.')">{{$t('archived')}}</span> )  
                  </span>


                  <template v-if="task.status === 'executing'">            
                    <div class="TaskItemVertical TaskRowBig ProgressBar" :uk-tooltip="$t('Each script has total success and fail number allowed. When either success number or fail number are exceeded, script will stop. What exactly counts as success depends on the script, it could be registration number or posts count or anything else.')">
                      <div class="TaskItemVertical1 Green">
                        {{$t('Successes')}} {{ task.success_current }} / {{ task.success_total }}
                      </div>
                      <div class="TaskItemVertical2">
                        
                        <div class="progress-bar-green">
                          <span class="progress-green" :style="{ width: Math.floor(100.0 * task.success_current / task.success_total) + '%' }"></span>
                        </div>

                      </div>
                    </div>
                    <div class="TaskItemVertical TaskRowBig ProgressBar" :uk-tooltip="$t('Each script has total success and fail number allowed. When either success number or fail number are exceeded, script will stop. What exactly counts as fail depends on the script, it could be any error during script execution.')">
                      <div class="TaskItemVertical1 Red">
                        {{$t('Fails')}} {{ task.fail_current }} / {{ task.fail_total }}
                      </div>
                      <div class="TaskItemVertical2">
                        <div class="progress-bar-red">
                          <span class="progress-red" :style="{ width: Math.floor(100.0 * task.fail_current / task.fail_total) + '%' }"></span>
                        </div>
                      </div>
                    </div>
                  </template>


                  <span v-show="task.status === 'scheduled_wait_for_execution'" class="TaskItemTime TaskRowBig">
                    {{$t('Will start')}} <span class="Date" :data-livestamp="Math.ceil(parseInt(task.run_next_date) / 1000)"></span>
                  </span>

                  <span v-show="task.status === 'downloading'" class="TaskItemTime TaskRowBig">
                    {{$t('Downloading started')}} <span class="Date" :data-livestamp="Math.ceil(parseInt(task.run_last_date) / 1000)"></span>
                  </span>

                  <span v-show="task.status === 'executing'" class="TaskItemTime TaskRowBig">
                    {{$t('Execution started')}} <span class="Date" :data-livestamp="Math.ceil(parseInt(task.run_last_date) / 1000)"></span>
                  </span>

                  <a href="#" @click.prevent="SelectTask(task.id)" class="TaskItemDetails"><span uk-icon="more-vertical"></span></a>

                </div>
              
                <div v-show="selectedTask == task.id && (showArchived || task.status != 'archived')">

                  <div class="TaskItemMore">
                    <div class="TaskDetails">
                      <div>
                        <span class="TaskDetailsKey">{{$t('Name :')}}</span> <span class="TaskDetailsValue">"{{task.name}}"</span>
                      </div>
                    </div>
                    <div class="TaskDetails">
                      <div>
                        <span class="TaskDetailsKey">{{$t('Status :')}}</span> <span style="cursor:help" :uk-tooltip="DescriptionStatus(task.status)" class="TaskDetailsValue">{{HumanReadableStatus(task.status)}}</span>
                      </div>
                    </div>
                    <div class="TaskDetails">
                      <div>
                        <span class="TaskDetailsKey">{{$t('State :')}}</span> <span v-bind:class="{TaskDetailsValue: true, 'uk-text-success': (task.status == 'executing'), 'uk-text-danger': (task.status != 'executing')}">{{(task.status == "executing") ? $t("Running") : $t("Stopped")}}</span>
                      </div>
                    </div>

                    <div class="TaskDetails">
                      <div :uk-tooltip="$t('Total number of the number of times the script has been started and completed.')">
                        <span class="TaskDetailsKey">{{$t('Total number script finished :')}}</span> <span class="TaskDetailsValue">{{task.number_execute}}</span>
                      </div>
                    </div>


                    <div class="TaskDetails" v-show="task.status == 'executing'">
                      <div :uk-tooltip="$t('Number of threads currently running')">
                        <span class="TaskDetailsKey">{{$t('Threads :')}}</span> <span class="TaskDetailsValue">{{task.threads}}</span>
                      </div>
                    </div>


                    <div class="TaskDetails" v-show="task.status == 'executing'">
                      <div :uk-tooltip="$t('Number of browsers currently running')">
                        <span class="TaskDetailsKey">{{$t('Browsers number :')}}</span> <span class="TaskDetailsValue">{{task.browsers_count}}</span>
                      </div>
                    </div>

                    
                    <div class="TaskDetails" v-show="task.status == 'executing'">
                      <div :uk-tooltip="$t('Each script has total success and fail number allowed. When either success number or fail number are exceeded, script will stop. What exactly counts as success depends on the script, it could be registration number or posts count or anything else.')">
                        <span class="TaskDetailsKey">{{$t('Total successes :')}}</span> <span class="TaskDetailsValue uk-text-success">{{task.success_current}} / {{task.success_total}}</span>
                      </div>
                    </div>

                    <div class="TaskDetails" v-show="task.status == 'executing'">
                      <div :uk-tooltip="$t('Each script has total success and fail number allowed. When either success number or fail number are exceeded, script will stop. What exactly counts as fail depends on the script, it could be any error during script execution.')">
                        <span class="TaskDetailsKey">{{$t('Total fails :')}}</span> <span class="TaskDetailsValue uk-text-danger">{{task.fail_current}} / {{task.fail_total}}</span>
                      </div>
                    </div>

                    <div class="TaskDetails">
                      <div v-if="task.run_last_date.length > 0" :uk-tooltip="$t('Date and time when the script was last started')">
                        <span class="TaskDetailsKey">{{$t('Last start date :')}}</span> <span class="TaskDetailsValue">{{moment(parseInt(task.run_last_date)).format('dddd, MMMM Do, hh:mm A')}}</span>
                      </div>
                      <div v-else :uk-tooltip="$t('Date and time when the script was last started')">
                        <span class="TaskDetailsKey">{{$t('Last start date :')}}</span> <span class="TaskDetailsValue">?</span>
                      </div>
                    </div>

                    <div class="TaskDetails" v-show="task.status == 'scheduled_wait_for_execution'">
                      <div :uk-tooltip="$t('Date and time when the script will be started next time')">
                        <span class="TaskDetailsKey">{{$t('Scheduled date :')}}</span> <span class="TaskDetailsValue">{{moment(parseInt(task.run_next_date)).format('dddd, MMMM Do, hh:mm A')}}</span>
                      </div>
                    </div>

                    <div class="TaskDetails">
                      <div>
                        <span class="TaskDetailsKey">{{$t('Scheduled type :')}}</span> <span class="TaskDetailsValue">{{ScheduleTypeFormat(task.schedule)}} 
                          <a href="#" @click.prevent="EditTask(task);taskEditStage=3"><span uk-icon="icon: search"></span></a>

                        </span>
                      </div>
                    </div>

                  </div>
                  <div class="TaskItemMoreAdditional" v-show="task.status == 'executing' || task.log_path">
                    <h4 class="uk-heading-divider ">{{$t('Advanced information')}}</h4>
                    <div v-show="task.log_path">
                      <a href="#" @click.prevent="DownloadFile(task.log_path)" class="uk-text-small">
                        {{$t('Download log')}}</a> - <span class="uk-text-small uk-text-muted">{{$t('download log file produced by last or current run.')}}
                      </span>
                    </div>
                    <div v-show="task.report || task.status == 'executing'">
                      <a href="#" @click.prevent="ShowReport(task.id)" class="uk-text-small">{{$t('Show report')}}</a> - 
                      <span class="uk-text-small uk-text-muted">{{$t('show script report, it contains information about problems during last script run, running time, number of successful thread executions and other.')}}</span>
                    </div>
                    <div v-show="task.result1_active && task.result1_active != '0' && task.result1_number > 0">
                      <a href="#" @click.prevent="DownloadFile(task.result1_path)" class="uk-text-small">
                        {{task.result1_name}} ({{task.result1_number}})</a> - <span class="uk-text-small uk-text-muted">{{$t('download file with results, like registered users.')}}
                      </span>
                    </div>
                    <div v-show="task.result2_active && task.result2_active != '0' && task.result2_number > 0">
                      <a href="#" @click.prevent="DownloadFile(task.result2_path)"  class="uk-text-small">
                        {{task.result2_name}} ({{task.result2_number}})</a> - <span class="uk-text-small uk-text-muted">{{$t('download file with results, like registered users.')}}
                      </span>
                    </div>
                    <div v-show="task.result3_active && task.result3_active != '0' && task.result3_number > 0">
                      <a href="#" @click.prevent="DownloadFile(task.result3_path)"  class="uk-text-small">
                        {{task.result3_name}} ({{task.result3_number}})</a> - <span class="uk-text-small uk-text-muted">{{$t('download file with results, like registered users.')}}
                      </span>
                    </div>
                    <div v-show="task.result4_active && task.result4_active != '0' && task.result4_number > 0">
                      <a href="#" @click.prevent="DownloadFile(task.result4_path)"  class="uk-text-small">
                        {{task.result4_name}} ({{task.result4_number}})</a> - <span class="uk-text-small uk-text-muted">{{$t('download file with results, like registered users.')}}
                      </span>
                    </div>
                    <div v-show="task.result5_active && task.result5_active != '0' && task.result5_number > 0">
                      <a href="#" @click.prevent="DownloadFile(task.result5_path)"  class="uk-text-small">
                        {{task.result5_name}} ({{task.result5_number}})</a> - <span class="uk-text-small uk-text-muted">{{$t('download file with results, like registered users.')}}
                      </span>
                    </div>
                    <div v-show="task.result6_active && task.result6_active != '0' && task.result6_number > 0">
                      <a href="#" @click.prevent="DownloadFile(task.result6_path)"  class="uk-text-small">
                        {{task.result6_name}} ({{task.result6_number}})</a> - <span class="uk-text-small uk-text-muted">{{$t('download file with results, like registered users.')}}
                      </span>
                    </div>
                    <div v-show="task.result7_active && task.result7_active != '0' && task.result7_number > 0">
                      <a href="#" @click.prevent="DownloadFile(task.result7_path)"  class="uk-text-small">
                        {{task.result7_name}} ({{task.result7_number}})</a> - <span class="uk-text-small uk-text-muted">{{$t('download file with results, like registered users.')}}
                      </span>
                    </div>
                    <div v-show="task.result8_active && task.result8_active != '0' && task.result8_number > 0">
                      <a href="#" @click.prevent="DownloadFile(task.result8_path)"  class="uk-text-small">
                        {{task.result8_name}} ({{task.result8_number}})</a> - <span class="uk-text-small uk-text-muted">{{$t('download file with results, like registered users.')}}
                      </span>
                    </div>
                    <div v-show="task.result9_active && task.result9_active != '0' && task.result9_number > 0">
                      <a href="#" @click.prevent="DownloadFile(task.result9_path)"  class="uk-text-small">
                        {{task.result9_name}} ({{task.result9_number}})</a> - <span class="uk-text-small uk-text-muted">{{$t('download file with results, like registered users.')}}
                      </span>
                    </div>
                  </div>
                  <div class="TaskItemMoreAdditional">
                    <h4 class="uk-heading-divider ">{{$t('Control this schedule')}}</h4>

                    <div v-show="task.status == 'executing' || task.status == 'downloading'" class="IconAndText">
                      <a href="#" @click.prevent="TaskApiNoReturn('stop',task)" class="uk-text-small" style="display:inline-block">
                        <span class="IconAndText">
                          <span uk-icon="icon: close; ratio: 0.8" style="margin-right:4px"></span> 
                          {{$t('Stop')}}
                        </span>
                      </a>
                      <span>&nbsp;</span>-<span>&nbsp;</span>
                      <span class="uk-text-small uk-text-muted">{{$t('stop this task instantly.')}}</span>
                    </div>

                    <div v-show="task.status == 'scheduled_wait_for_execution' || task.status == 'archived'" class="IconAndText">
                      <a href="#" @click.prevent="TaskApiNoReturn('start',task)" class="uk-text-small" style="display:inline-block">
                        <span class="IconAndText">
                          <span uk-icon="icon: play-circle; ratio: 0.8" style="margin-right:4px"></span> 
                          {{$t('Start')}}
                        </span>
                      </a>
                      <span>&nbsp;</span>-<span>&nbsp;</span>
                      <span class="uk-text-small uk-text-muted">{{$t('start this task instantly.')}}</span>
                    </div>

                    <div v-show="task.status != 'archived'" class="IconAndText">
                      <a href="#" @click.prevent="TaskApiNoReturn('archive',task)" class="uk-text-small" style="display:inline-block">
                        <span class="IconAndText">
                          <span uk-icon="icon: history; ratio: 0.8" style="margin-right:4px"></span> 
                          {{$t('Archive')}}
                        </span>
                      </a>
                      <span>&nbsp;</span>-<span>&nbsp;</span>
                      <span class="uk-text-small uk-text-muted">{{$t('archived tasks wont be scheduled, but wont be deleted also, so you can run it later.')}}</span>
                    </div>

                    <div v-show="task.status == 'archived'" class="IconAndText">
                      <a href="#" @click.prevent="TaskApiNoReturn('restore',task)" class="uk-text-small" style="display:inline-block">
                        <span class="IconAndText">
                          <span uk-icon="icon: future; ratio: 0.8" style="margin-right:4px"></span> 
                          {{$t('Restore from archive')}}
                        </span>
                      </a>
                      <span>&nbsp;</span>-<span>&nbsp;</span>
                      <span class="uk-text-small uk-text-muted">{{$t('task will be restored from archive and scheduled again.')}}</span>
                    </div>

                    <div class="IconAndText">
                      <a href="#" @click.prevent="EditTask(task)" class="uk-text-small" style="display:inline-block">
                        <span class="IconAndText">
                          <span uk-icon="icon: pencil; ratio: 0.8" style="margin-right:4px;margin-left: -2px;"></span> 
                          {{$t('Edit')}}
                        </span>
                      </a>
                      <span>&nbsp;</span>-<span>&nbsp;</span>
                      <span class="uk-text-small uk-text-muted">{{$t('edit current task, change name, schedule, etc.')}}</span>
                    </div>

                    <div class="IconAndText" v-show="task.status == 'executing' && is_local">
                      <a href="#" @click.prevent="TaskApiNoReturn('togglevisibility',task)" class="uk-text-small" style="display:inline-block">
                        <span class="IconAndText">
                          <span uk-icon="icon: search; ratio: 0.8" style="margin-right:4px;margin-left: -2px;"></span> 
                          {{$t('Toggle visibility')}}
                        </span>
                      </a>
                      <span>&nbsp;</span>-<span>&nbsp;</span>
                      <span class="uk-text-small uk-text-muted">{{$t('show or hide script interface.')}}</span>
                    </div>

                    <div class="IconAndText">
                      <a href="#" @click.prevent="UIkit.modal.confirm($t('Are you sure, that you want to delete this task?')).then(function(){TaskApiNoReturn('delete',task)})" class="uk-text-small uk-text-danger" style="display:inline-block">
                        <span class="IconAndText">
                          <span uk-icon="icon: ban; ratio: 0.8" style="margin-right:4px;margin-left: -2px;"></span> 
                          {{$t('Delete')}}
                        </span>
                      </a>
                      <span>&nbsp;</span>-<span>&nbsp;</span>
                      <span class="uk-text-small uk-text-muted">{{$t('delete current task, if you planning to use it later, better use archive.')}}</span>
                    </div>

                  </div>
                </div>
            </div>


        </div>
      </div>

      <div id="TaskListMenu" uk-offcanvas="overlay: true; flip: true">
        <div class="uk-offcanvas-bar">

          <button class="uk-offcanvas-close" type="button" uk-close></button>

          <ul class="uk-nav uk-nav-default">
              
              <li class="uk-nav-header">{{$t('Task list')}}</li>
              <li>
                <a href="#" @click.prevent="Sorting('added')">
                  <span v-if="sorting == 'added' && isasc" class="uk-margin-small-right" uk-icon="icon: arrow-up"></span> 
                  <span v-if="sorting == 'added' && !isasc" class="uk-margin-small-right" uk-icon="icon: arrow-down"></span> 
                  {{$t('Sort by date added')}}
                </a>
              </li>
              <li>
                <a href="#" @click.prevent="Sorting('scheduled')">
                  <span v-if="sorting == 'scheduled' && isasc" class="uk-margin-small-right" uk-icon="icon: arrow-up"></span> 
                  <span v-if="sorting == 'scheduled' && !isasc" class="uk-margin-small-right" uk-icon="icon: arrow-down"></span> 
                  {{$t('Sort by date scheduled')}}
                </a>
              </li>
              <li>
                <a href="#" @click.prevent="Sorting('name')">
                  <span v-if="sorting == 'name' && isasc" class="uk-margin-small-right" uk-icon="icon: arrow-up"></span> 
                  <span v-if="sorting == 'name' && !isasc" class="uk-margin-small-right" uk-icon="icon: arrow-down"></span> 
                  {{$t('Sort by name')}}
                </a>
              </li>
              <li class="uk-nav-divider"></li>
              <li><a href="#" @click.prevent="UIkit.modal.confirm($t('Are you sure, that you want to delete all records?')).then(function(){ClearAllTasks()})">{{$t('Clear all tasks')}}</a></li>
              <li class="uk-nav-divider"></li>
              <li><a href="#" @click.prevent="SaveAllTasks()"><span class="uk-margin-small-right" uk-icon="icon: download"></span> {{$t('Save all tasks')}}</a></li>

              <input type="file" @change="LoadAllTasks()" id="LoadAllTasks" class="LoadAllTasksLink" style="display:none">
              <li><a href="#" @click.prevent="$('#LoadAllTasks')[0].click()"><span class="uk-margin-small-right" uk-icon="icon: upload"></span> {{$t('Load tasks')}}</a></li>
              <li class="uk-nav-divider"></li>
              <li><a href="#" @click.prevent="Translate('en')"> {{$t('Translate to en')}}</a></li>
              <li><a href="#" @click.prevent="Translate('ru')"> {{$t('Translate to ru')}}</a></li>
              <li class="uk-nav-divider"></li>
              <li>
              	<a 
              		href="#" 
              		@click.prevent="UIkit.modal.confirm($t('Are you sure, that you want to disable the scheduler? This will stop all current tasks, and new tasks will not be planned after PC reboot.')).then(function(){Uninstall()})"
              	>
              		<span class="uk-margin-small-right uk-text-danger" uk-icon="icon: trash"></span> <span class="uk-text-danger">{{$t('Disable scheduler')}}</span></a>
              	</li>
              
          </ul>
        </div>
      </div>

      <div id="SchedulerLogMenu" uk-offcanvas="overlay: true; flip: true">
        <div class="uk-offcanvas-bar">

          <button class="uk-offcanvas-close" type="button" uk-close></button>

          <ul class="uk-nav uk-nav-default">
              
              <li class="uk-nav-header">{{$t('Scheduler log')}}</li>
              <li style="margin-top:30px">
                <a href="#" @click.prevent="ClearSchedulerLog()">
                  <span class="uk-margin-small-right" uk-icon="icon: trash"></span> 

                  {{$t('Clear log')}}
                </a>
              </li>
              <li style="margin-top:10px">
                <a href="#" @click.prevent="SchedulerLogSearch()">
                  <span class="uk-margin-small-right" uk-icon="icon: search"></span> 

                  {{$t('Search')}}
                </a>
              </li>
              <li style="margin-top:10px">
                <a href="#" @click.prevent="DownloadSchedulerLog()" @click="UIkit.offcanvas($('#SchedulerLogMenu')).hide();">
                  <span class="uk-margin-small-right" uk-icon="icon: download"></span> 
                    {{$t('Download full log')}}
                </a>
              </li>
              

          </ul>
        </div>
      </div>


      <div id="ApplicationLogMenu" uk-offcanvas="overlay: true; flip: true">
        <div class="uk-offcanvas-bar">

          <button class="uk-offcanvas-close" type="button" uk-close></button>

          <ul class="uk-nav uk-nav-default">
              
              <li class="uk-nav-header">{{$t('Application log')}}</li>
              <li style="margin-top:30px">
                <a href="#" @click.prevent="ClearApplicationLog()">
                  <span class="uk-margin-small-right" uk-icon="icon: trash"></span> 

                  {{$t('Clear log')}}
                </a>
              </li>
              <li style="margin-top:10px">
                <a href="#" @click.prevent="ApplicationLogSearch()">
                  <span class="uk-margin-small-right" uk-icon="icon: search"></span> 

                  {{$t('Search')}}
                </a>
              </li>
              <li style="margin-top:10px;color: gray;">
                {{$t('You can download application log for each task from task menu.')}}
              </li>
          </ul>
        </div>
      </div>

      <div id="EditTaskModal" class="uk-modal-full" uk-modal="stack: true; esc-close: false">
        <div class="uk-modal-dialog">
          <button @click="reportTask = null" class="uk-modal-close-full uk-close-large" type="button" uk-close style="margin-top:5px"></button>
          <div uk-height-viewport>
            <div style="padding:15px;height:40px;border-bottom:1px solid rgb(241, 241, 241)">
              <div style="margin-bottom:0px;line-height: 40px;font-size: 18px;">
                <span v-if="!taskEdit.id">{{$t('Creating new task')}}</span>
                <span v-else>{{$t('Editing task')}}</span>
              </div>
            </div>

            <div style="margin-top:0px;border:none;height:calc(100vh - 170px);display:flex;align-items: center;justify-content: center;flex-direction: column;" id="ScriptReportResult">

              <div class="md-stepper-horizontal" style="width:100%">
                <a href="#" @click.prevent="taskEditStage=0" :class="{'md-step':true, 'active': taskEdit.name, 'passive': !taskEdit.name, 'current': taskEditStage == 0}">
                  <div class="md-step-circle">
                    <span uk-icon="check" v-if="taskEdit.name"></span>
                    <span class="circle" v-else></span>
                  </div>
                  <div class="md-step-title">{{$t('Name')}}</div>
                  <div class="md-step-optional">{{$t('Task name')}}</div>
                  <div class="md-step-bar-left"></div>
                  <div class="md-step-bar-right"></div>
                </a>
                <a href="#" @click.prevent="taskEditStage=1" v-show="SchedulerInputs.method != 'Hidden'" :class="{'md-step':true, 'active': ((taskEdit.script_name || taskEdit.script_data) && taskEditValidated), 'passive': !((taskEdit.script_name || taskEdit.script_data) && taskEditValidated), 'current': taskEditStage == 1}">
                  <div class="md-step-circle">
                    <span uk-icon="check" v-if="((taskEdit.script_name || taskEdit.script_data) && taskEditValidated)"></span>
                    <span class="circle" v-else></span>
                  </div>
                  <div class="md-step-title">{{$t('Project')}}</div>
                  <div class="md-step-optional">{{$t('Upload project')}}</div>
                  <div class="md-step-bar-left"></div>
                  <div class="md-step-bar-right"></div>
                </a>
                <a href="#" @click.prevent="taskEditStage=2" :class="{'md-step':true, 'active': (taskEdit.settings || !taskEditHasResources), 'passive': !(taskEdit.settings || !taskEditHasResources), 'current': taskEditStage == 2}">
                  <div class="md-step-circle">
                    <span uk-icon="check" v-if="taskEdit.settings || !taskEditHasResources"></span>
                    <span class="circle" v-else></span>
                  </div>
                  <div class="md-step-title">{{$t('Data')}}</div>
                  <div class="md-step-optional">{{$t('Select data for script')}}</div>
                  <div class="md-step-bar-left"></div>
                  <div class="md-step-bar-right"></div>
                </a>
                <a href="#" @click.prevent="taskEditStage=3" :class="{'md-step':true, 'active': validateSchedule == null, 'passive': validateSchedule != null, 'current': taskEditStage == 3}">
                  <div class="md-step-circle">
                    <span uk-icon="check" v-if="validateSchedule == null"></span>
                    <span class="circle" v-else></span>
                  </div>
                  <div class="md-step-title">{{$t('Schedule')}}</div>
                  <div class="md-step-optional">{{$t('Schedule task')}}</div>
                  <div class="md-step-bar-left"></div>
                  <div class="md-step-bar-right"></div>
                </a>
              </div>
              <div style="flex: 1;overflow:auto;width:100%">
                <div id="Stage0" v-show="taskEditStage==0">
                  <form class="uk-form-stacked">
                    <div class="uk-margin">
                        <label class="uk-form-label" for="TaskEditName" style="margin-bottom: 15px;">{{$t('Task label, set it to something meaningful in order to remember its purpose:')}}</label>
                        <div class="uk-form-controls">
                            <input v-model="taskEdit.name" :class="{'uk-input': true,'uk-form-danger': !taskEdit.name}" id="TaskEditName" type="text" :placeholder="$t('Task label')" style="margin-left: 15px;">
                        </div>
                    </div>
                  </form>
                </div>
                <div id="Stage1" v-show="taskEditStage==1">
                  <div class="uk-form-label">{{$t('Please select project source:')}}</div>

                  <div class="uk-grid-small uk-child-width-auto uk-grid" style="margin-top: 25px !important">
                      
                      <label style="padding-left: 30px;"><input onchange="$('.SwitcherProject').hide();$('#SwitcherProjectLocal').show()" class="uk-radio" type="radio" name="ProjectLocalRemoteSwitch" checked> {{$t('Load project from file')}}</label>

                      <label style="padding-left: 30px;"><input onchange="$('.SwitcherProject').hide();$('#SwitcherProjectRemote').show()" class="uk-radio" type="radio" name="ProjectLocalRemoteSwitch"> {{$t('Use project from server')}}</label>
                  </div>
                  
                  <div style="margin-top: 30px;margin-left: 15px;">
                    <div id="SwitcherProjectLocal" class="SwitcherProject">
                      <div class="uk-form-stacked">
                        <div style="margin-bottom:30px;color: #333;font-size: .875rem;">
                          <span v-if="typeof(taskEdit.script_data) != 'string' || taskEdit.script_data.length == 0">
                            {{$t('No project uploaded yet.')}}
                          </span>
                          <span v-else style="color: #333;font-size: .875rem;">
                            {{$t('Project uploaded, size')}} {{humanFileSize(taskEdit.script_data.length, true)}}. <a href="#" @click.prevent="ClearProject()">{{$t('Remove')}}</a>
                          </span>
                        </div>
                        <div uk-form-custom="target: true">
                          <input type="file" @change="TaskEditDataUpdate()" id="TaskData">
                          <button class="uk-button uk-button-default">{{$t('Upload')}}</button> 
                        </div>
                        <span v-show="schedulerInputs.method == 'AnyAndRecent'" class="UseRecentBas" >
                        	{{$t(', or')}} 
                        	<a href="#" style="cursor:pointer !important" @click.prevent="UseRecentBas()" :uk-tooltip="$t('This feature will work only if you saved project in BAS. All unsaved changes wont be fetched.')">{{$t('use recent BAS project')}}</a>.
                        </span>
                        <span id="SaveProjectNotificationLocal" class="uk-text-success" style="display:none"></span>
                      </div>
                    </div>
                    <div id="SwitcherProjectRemote" class="SwitcherProject" style="display:none">
                      <div class="uk-form-stacked">
                        <div class="uk-margin">
                            <label class="uk-form-label">{{$t('The script name, must be exactly the same as what was entered during script creation.')}} <a href="https://bablosoft.com/bas/scripts" target="_blank">{{$t('Upload new')}}</a>.</label>
                            <div class="uk-form-controls">
                                <input v-model="taskEdit.script_name" :class="{'uk-input': true}" type="text" :placeholder="$t('Script name')">

                            </div>
                        </div>
                        <div class="uk-margin">
                            <label class="uk-form-label">{{$t('User login which has access for current script:')}}</label>
                            <div class="uk-form-controls">
                                <input v-model="taskEdit.script_login" :class="{'uk-input': true}" type="text" :placeholder="$t('Login')">
                            </div>
                        </div>
                        <div class="uk-margin">
                            <label class="uk-form-label">{{$t('User password which has access for current script:')}}</label>
                            <div class="uk-form-controls">
                                <input type="password" v-model="taskEdit.script_pass" :class="{'uk-input': true}" :placeholder="$t('Password')">
                            </div>
                        </div>
                        <button class="uk-button uk-button-default" @click="CheckRemoteProject()">{{$t('Save')}}</button> <span id="SaveProjectNotification" :class="{'uk-text-success': taskEditCheckRemoteSuccess, 'uk-text-danger': !taskEditCheckRemoteSuccess}">{{taskEditCheckRemoteProject}}</span>
                      </div>  
                    </div>
                  </div>
                </div>
                <div id="Stage2" v-show="taskEditStage==2" :class="{'CustomInterfaceVisible': inputScriptType == 2 || inputScriptType == 4 || inputScriptType == 5}">
                  <div v-show="inputScriptType == 1 || inputScriptType == 3" class="uk-form-label TaskEditStage2">

                    <div>
                      {{$t('Interface is starting and will be shown in another window. Check in task bar if you dont see it.')}}
                        <a href="#" @click.prevent="fetch(apiendpoint + '/tasks/cancel/' + inputScriptId)">
                          {{$t('Cancel.')}}
                        </a>
                    </div>
                    

                  </div>

                  <div v-show="inputScriptType == 2 || inputScriptType == 4 || inputScriptType == 5" class="uk-form-label" style="height:100%;overflow: hidden;">
                    <span style="padding:4px;display:block;text-align:right;background-color: #1e87f0;">
                      <a href="#" style="color: white;" @click.prevent="$('#InterfaceAutogenerateParent').html('');inputScriptType=0">
                        <span uk-icon="icon: close; ratio: 1.2"></span>
                      </a>
                    </span>
                    <span id="InterfaceAutogenerateParent">
                      
                    </span>
                  </div>

                  <div v-show="inputScriptType == 0 && taskEditValidated">
                    <form class="uk-form-stacked" v-show="!taskEditHasResources">
                      <div class="uk-margin">
                          <label class="uk-form-label" style="margin-bottom: 15px;">{{$t('This script doesnt require to input any data.')}}</label>
                      </div>
                    </form>
                    <form class="uk-form-stacked" v-show="taskEditHasResources">
                      <div class="uk-margin">
                          <label class="uk-form-label" style="margin-bottom: 15px;">
                            <span v-show="taskEdit.settings">
                              {{$t('Data prepared.')}} <a href="#" @click.prevent="InputData()">{{$t('Change data')}}</a>.
                            </span>
                            <span v-show="!taskEdit.settings">
                              {{$t('This script requires data in order to work correctly.')}}
                              <a href="#" @click.prevent="InputData()">{{$t('Input data')}}</a>.
                              <pre class="uk-text-danger" v-if="inputScriptError">{{inputScriptError}}</pre>
                            </span>

                          </label>
                      </div>
                    </form>
                  </div>

                  <div v-show="inputScriptType == 0 && !taskEditValidated">
                    <form class="uk-form-stacked">
                      <div class="uk-margin">
                          <label class="uk-form-label" style="margin-bottom: 15px;">{{$t('No project uploaded yet.')}} <a href="#" @click.prevent="taskEditStage = 1">{{$t('Upload')}}</a>.</label>
                      </div>
                    </form>
                  </div>

                </div>
                <div id="Stage3" v-show="taskEditStage==3">
                  <scheduler :visibility="taskEditStage==3" :apiendpoint="apiendpoint" :validation="validateSchedule" v-model="taskEdit.schedule" v-if="typeof(taskEdit) == 'object' && typeof(taskEdit.schedule) == 'object'">
                  </scheduler>
                </div>
              </div>

            </div>    
            <div id="TaskEditBottom">
              <button class="uk-button uk-button-primary" id="EditTaskSave" @click="SaveTaskEdit()" v-show="taskEdit.name && ((taskEdit.script_name || taskEdit.script_data) && taskEditValidated) && (taskEdit.settings || !taskEditHasResources) && validateSchedule == null">{{$t('Save and close')}}</button>
              <button class="uk-button uk-button-danger" id="EditTaskCancel" @click="UIkit.modal($('#EditTaskModal')).hide()">{{$t('Cancel')}}</button>
              <button class="uk-button uk-button-default" id="EditTaskPrev" @click="if(SchedulerInputs.method == 'Hidden' && taskEditStage == 0){taskEditStage+=2}else{taskEditStage++}" v-show="taskEditStage<3">{{$t('Next')}}</button>
              <button class="uk-button uk-button-default" id="EditTaskNext" @click="if(SchedulerInputs.method == 'Hidden' && taskEditStage == 2){taskEditStage-=2}else{taskEditStage--}" v-show="taskEditStage>0">{{$t('Previous')}}</button>
            </div>
          </div>
        </div>
      </div>

      <div id="ReportModal" class="uk-modal-full" uk-modal="stack: true">
        <div class="uk-modal-dialog">
          <button @click="reportTask = null" class="uk-modal-close-full uk-close-large" type="button" uk-close style="margin-top:5px"></button>
          <div uk-height-viewport>
            <div style="padding:15px;border-bottom:1px solid rgb(221, 221, 221);height:40px">
              <div style="margin-bottom:0px;line-height: 40px;font-size: 18px;">{{$t('Report for')}} <span style="font-weight: 600;">{{reportTask ? reportTask.name : ""}}</span> {{$t('script')}}</div>
            </div>
            <pre style="margin-top:0px;border:none;padding:20px;height:calc(100vh - 110px);overflow:auto" id="ScriptReportResult" data-simplebar data-simplebar-auto-hide="false" data-simplebar-scrollbar-min-size="50">{{((reportTask && reportTask.report) ? reportTask.report : "Loading ...")}}</pre>
          </div>
        </div>
      </div>


      


      <div id="FailStartModal" class="uk-modal-full" uk-modal="stack: true">
        <div class="uk-modal-dialog">
          <button @click="reportTask = null" class="uk-modal-close-full uk-close-large" type="button" uk-close style="margin-top:5px"></button>
          <div uk-height-viewport>
            <div style="padding:15px;border-bottom:1px solid rgb(221, 221, 221);height:40px">
              <div style="margin-bottom:0px;line-height: 40px;font-size: 18px;">{{$t('Report for')}} <span style="font-weight: 600;" id="FailStartModalScript"></span> {{$t('script')}}</div>
            </div>
            <pre style="margin-top:0px;border:none;padding:20px;height:calc(100vh - 110px);overflow:auto" id="FailStartModalData" data-simplebar data-simplebar-auto-hide="false" data-simplebar-scrollbar-min-size="50"></pre>
          </div>
        </div>
      </div>


      <div id="ManualModal" class="uk-modal-full" uk-modal="stack: true">
        <div class="uk-modal-dialog">
          <button @click="reportTask = null" class="uk-modal-close-full uk-close-large" type="button" uk-close style="margin-top:5px"></button>
          <div uk-height-viewport>
            <div style="padding:15px;border-bottom:1px solid rgb(221, 221, 221);height:40px">
              <div style="margin-bottom:0px;line-height: 40px;font-size: 18px;">{{$t('Manual activities')}}</div>
            </div>

            <div style="margin-top:0px;border:none;height:calc(100vh - 72px);overflow:auto">

              <template v-for="item in manual" :key="item.id">
                <div uk-grid class="uk-grid CaptchaGrid" v-if="item.type != 'Browser'">
                  
                  <div class="uk-width-1-2@s uk-flex uk-flex-middle uk-flex-center uk-first-column">

                    <img style="padding-bottom: 10px;" :src="'data:image/png;base64,' + item.data" v-if="item.type == 'Image'">
                    <div style="padding-bottom: 10px;" v-if="item.type == 'Text'">{{item.data}}</div>
                  </div>

                  <div class="uk-width-1-2@s">
                    <div style="margin-right: -2px;">
                      <input type="text" :placeholder="$t('Captcha result')" class="uk-input CaptchaResult" :data-captcha-id="item.id">
                    </div>
                  
                    <div style="padding: 10px 0 10px 0;display: flex;justify-content: flex-start;align-items: stretch;flex-wrap: nowrap;">
                      <div style="min-height: 25px;flex-basis: 50%;flex-grow: 0;flex-shrink: 0;padding: 0 2px 0 0;">
                        <button class="uk-button uk-button-small uk-button-default uk-width-1-1" style="margin: 0 10px 0 0;" @click="AcceptCaptcha(item)">Ok</button>
                      </div>
                      <div style="min-height: 25px;flex-basis: 50%;flex-grow: 0;flex-shrink: 0;padding: 0 2px 0 0;">
                        <button class="uk-button uk-button-small  uk-button-default uk-width-1-1" style="margin: 0px;" @click="RejectCaptcha(item)">{{$t('Cancel')}}</button>
                      </div>
                    </div>
                  </div>
                </div>

                <div v-if="item.type == 'Browser'" style="text-align:center;border-bottom: 1px solid rgb(221, 221, 221);padding: 20px !important;">
                  {{$t('Browser is controlled manually with message: ')}}<strong>{{item.data}}</strong>'. <a href="#" @click.prevent="AcceptCaptcha(item)">{{$t('Show browser')}}</a>.
                </div>
              </template>

            </div>

          </div>
        </div>
      </div>

    </div>`

}