

window.Scheduler = {

    props: ["data", "validation", "apiendpoint", "visibility"],
    model: {
      prop: 'data',
      event: 'change'
    },
    data: function()
    {
      return {
        more: false,
        predictionData: null,
        predictionDataPrepared: null,
        predictionDataIsSuccess: false,
        predictionDataMessage: "",
        predictionDataHasMore: false,
        mainTabIndex: 0

      }
    },

    watch: {
      visibility: function()
      {
        if(this.visibility)
        {
          UIkit.tab("#SchedulerTabMain").show(0);
          var self = this
          fetch(this.apiendpoint + "/tasks/predict", {
              method: "POST",
              cache: "no-cache",
              redirect: "follow",
              referrer: "no-referrer",
              body: JSON.stringify(this.data)
            })
            .then(response => response.json())
            .then(response => {
              self.predictionData = response
            })
        }
        
      },
      predictionData: function (){
        var self = this

        self.predictionDataPrepared = null
        self.predictionDataIsSuccess = false
        self.predictionDataMessage = ""
        self.predictionDataHasMore = false

        if(!self.predictionData)
          return

        var res1 = JSON.parse(JSON.stringify(self.predictionData.list)).map(x => parseInt(x))

        var res1 = res1.sort(function(a,b){
          return - a + b
        })
        
        var LastDate = ""
        var CurrentItem = null
        var res = []
        res1.forEach(function(element){
          
          var CurrentDate = moment(parseInt(element)).format('dddd, MMMM Do')

          if(CurrentDate != LastDate)
          {
            CurrentItem = {date: CurrentDate, data: []}
            res.push(CurrentItem)
            LastDate = CurrentDate

          }
          CurrentItem.data.push(element)
        })

        self.predictionDataPrepared = res
        self.predictionDataIsSuccess = self.predictionData.success
        self.predictionDataMessage = self.predictionData.message
        self.predictionDataHasMore = self.predictionData.has_more
      },

    },
    
    
    methods: {
      Change: function(){
        var self = this
        fetch(this.apiendpoint + "/tasks/predict", {
            method: "POST",
            cache: "no-cache",
            redirect: "follow",
            referrer: "no-referrer",
            body: JSON.stringify(this.data)
          })
          .then(response => response.json())
          .then(response => {
            self.predictionData = response
          })
        this.$emit('change',this.data)
        

      }, 
      Example: function(Index)
      {

        `<div class="SchedulerSelectContainer" style="display: flex;align-items: center;">
                <span class="Circle"></span>
                <span>Start in <i>10 minutes</i> </span>
                <a href="#" @click.prevent="Example(1)" uk-tooltip="Use this example" style="cursor: pointer">
                  <span  uk-icon="icon: sign-in; ratio: 1.0"></span>
                </a>
              </div>
              <div class="SchedulerSelectContainer" style="display: flex;align-items: center;">
                <span class="Circle"></span>
                <span>Start in <i>1 hour</i></span>
                <a href="#" @click.prevent="Example(2)" uk-tooltip="Use this example" style="cursor: pointer">
                  <span  uk-icon="icon: sign-in; ratio: 1.0"></span>
                </a>
              </div>
              <div class="SchedulerSelectContainer" style="display: flex;align-items: center;">
                <span class="Circle"></span>
                <span>Start periodically with <i>1 hour</i> interval between launches </span>
                <a href="#" @click.prevent="Example(3)" uk-tooltip="Use this example" style="cursor: pointer">
                  <span  uk-icon="icon: sign-in; ratio: 1.0"></span>
                </a>
              </div>
              <div class="SchedulerSelectContainer" style="display: flex;align-items: center;">
                <span class="Circle"></span>
                <span>Start each hour <i>at 00:30</i></span> 
                <a href="#" @click.prevent="Example(4)" uk-tooltip="Use this example" style="cursor: pointer">
                  <span  uk-icon="icon: sign-in; ratio: 1.0"></span>
                </a>
              </div>
              <div class="SchedulerSelectContainer" style="display: flex;align-items: center;">
                <span class="Circle"></span>
                <span>Start every day <i>at 23:59</i> </span>
                <a href="#" @click.prevent="Example(5)" uk-tooltip="Use this example" style="cursor: pointer">
                  <span  uk-icon="icon: sign-in; ratio: 1.0"></span>
                </a>
              </div>
              <div class="SchedulerSelectContainer" style="display: flex;align-items: center;">
                <span class="Circle"></span>
                <span>Start every week <i>at Monday at 13:00</i> </span>
                <a href="#" @click.prevent="Example(6)" uk-tooltip="Use this example" style="cursor: pointer">
                  <span  uk-icon="icon: sign-in; ratio: 1.0"></span>
                </a>
              </div>
              <div class="SchedulerSelectContainer" style="display: flex;align-items: center;">
                <span class="Circle"></span>
                <span>Start on working days evenings and weekends with probability </span>
                <a href="#" @click.prevent="Example(7)" uk-tooltip="Use this example" style="cursor: pointer">
                  <span  uk-icon="icon: sign-in; ratio: 1.0"></span>
                </a>
              </div>
              <div class="SchedulerSelectContainer" style="display: flex;align-items: center;">
                <span class="Circle"></span>
                <span>Start <i>10 times</i> from now without interval </span>
                <a href="#" @click.prevent="Example(8)" uk-tooltip="Use this example" style="cursor: pointer">
                  <span uk-icon="icon: sign-in; ratio: 1.0"></span>
                </a>
              </div>`
        if(Index == 1)
        {
          this.data = {type: "at", interval: 30 * 60, date_start: (Date.now() + 1000 * 60 * 10).toString(), date_start_list: [], sections_hour: [], sections_day: [], sections_week: [], sections_month: []}
        }

        if(Index == 2)
        {
          this.data = {type: "at", interval: 30 * 60, date_start: (Date.now() + 1000 * 60 * 60).toString(), date_start_list: [], sections_hour: [], sections_day: [], sections_week: [], sections_month: []}
        }

        if(Index == 3)
        {
          this.data = {"type":"interval","interval":3600,"date_start_list":[],"sections_hour":[],"sections_day":[],"sections_week":[],"sections_month":[],"consider_execution_time":true}
        }

        if(Index == 4)
        {
          this.data = {"type":"hour","interval":1800,"date_start_list":[],"sections_hour":[{"start":{"minute":30},"end":{"minute":30},"probability":100}],"sections_day":[],"sections_week":[],"sections_month":[]}
        }
        
        if(Index == 5)
        {
          this.data = {"type":"day","interval":1800,"date_start_list":[],"sections_hour":[],"sections_day":[{"start":{"minute":59,"hour":23},"end":{"minute":59,"hour":23},"probability":100}],"sections_week":[],"sections_month":[]}
        }

        if(Index == 6)
        {
          this.data = {"type":"week","interval":1800,"date_start_list":[],"sections_hour":[],"sections_day":[],"sections_week":[{"start":{"minute":0,"hour":13,"week":1},"end":{"minute":0,"hour":14,"week":1},"probability":100}],"sections_month":[]}
        }

        if(Index == 7)
        {
          this.data = {"type":"week","interval":1800,"date_start_list":[],"sections_hour":[],"sections_day":[],"sections_week":[{"start":{"minute":0,"hour":18,"week":1},"end":{"minute":0,"hour":22,"week":1},"probability":70},{"start":{"minute":0,"hour":18,"week":2},"end":{"minute":0,"hour":22,"week":2},"probability":70},{"start":{"minute":0,"hour":18,"week":3},"end":{"minute":0,"hour":22,"week":3},"probability":70},{"start":{"minute":0,"hour":18,"week":4},"end":{"minute":0,"hour":22,"week":4},"probability":70},{"start":{"minute":0,"hour":18,"week":5},"end":{"minute":0,"hour":22,"week":5},"probability":70},{"start":{"minute":0,"hour":12,"week":6},"end":{"minute":0,"hour":18,"week":6},"probability":50},{"start":{"minute":0,"hour":12,"week":7},"end":{"minute":0,"hour":18,"week":7},"probability":50}],"sections_month":[]}
        }

        if(Index == 8)
        {
          this.data = {"type":"interval","interval":0,"date_start_list":[],"sections_hour":[],"sections_day":[],"sections_week":[],"sections_month":[],"max_start_number":10}
        }

        if(Index == 9)
        {
          this.data = {type: "", interval: 30 * 60, date_start_list: [], sections_hour: [], sections_day: [], sections_week: [], sections_month: []}
        }

        this.Change()
        UIkit.tab("#SchedulerTabMain").show(0);
      }
    },

    mounted: function () {
      var self = this
      UIkit.util.on('#SchedulerTabSwitcher', 'shown', function(event, area) {
        if($("#SchedulerTabMain>*").first().hasClass("uk-active"))
        {
          self.mainTabIndex = 0
        }else
        {
          self.mainTabIndex = 1
        }
      });
      
    },
    

    template: `<div id="SchedulerContent">
        <div id="SchedulerAll" :style="{'width' : (predictionDataPrepared && !validation && mainTabIndex == 0) ? '60%' : '100%', 'float': 'left'}">
          <ul uk-tab id="SchedulerTabMain">
            <li><a href="#">{{$t('Select schedule type')}}</a></li>
            <li><a href="#">{{$t('Examples')}}</a></li>
          </ul>

          <ul id="SchedulerTabSwitcher" class="uk-switcher uk-margin">
            <li>
              <div class="uk-form-controls">
                <div style="width:50%;max-width:550px;float:left">
                  <div class="SchedulerSelectContainer">
                    <label><input @change="Change()" class="uk-radio" value="now" type="radio" name="SchedulerSelect" v-model="data.type"> {{$t('Run now')}}</label>
                    <div class="SchedulerSelectSection">
                      <div class="ScheduleTypeDescription" v-show="data.type == 'now'">
                        {{$t('Run task instantly and only once after creation.')}}
                      </div>
                      
                    </div>
                    <div class="ScheduleTypeValidation" v-if="validation && validation.type == 'now'">
                      <span uk-icon="icon: warning; ratio: 0.8"></span> <span class="ScheduleTypeValidationText">{{validation.message}}</span>
                    </div>

                  </div>
                  <div class="SchedulerSelectContainer">
                    <label><input @change="Change()" class="uk-radio" value="at" type="radio" name="SchedulerSelect" v-model="data.type"> {{$t('Run at')}}</label>
                    <div class="SchedulerSelectSection">
                      <div class="ScheduleTypeDescription" v-show="data.type == 'at'">
                        {{$t('Run task only once at selected date and time.')}}
                      </div>
                      <div class="ScheduleTypeInterface" v-show="data.type == 'at'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('Select date, when task will start')">{{$t('Start date')}}</span>: 
                        <selectdate @change="Change()" v-model="data.date_start" :name="$t('Start date')"></selectdate>
                      </div>
                      
                    </div>
                    <div class="ScheduleTypeValidation" v-if="validation && validation.type == 'at'">
                      <span uk-icon="icon: warning; ratio: 0.8"></span> <span class="ScheduleTypeValidationText">{{validation.message}}</span>
                    </div>
                  
                  </div>
                  <div class="SchedulerSelectContainer">
                    <label><input @change="Change()" class="uk-radio" value="interval" type="radio" name="SchedulerSelect" v-model="data.type"> {{$t('Run with interval')}}</label>
                    <div class="SchedulerSelectSection">
                      <div class="ScheduleTypeDescription" v-show="data.type == 'interval'">
                        {{$t('Run task periodically with selected interval between each run.')}}
                      </div>
                      <div class="ScheduleTypeInterface" v-show="data.type == 'interval'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('Intarval between each script start')">{{$t('Interval between each task start')}}</span>: 
                        <duration :mode="'zero'" @change="Change()" v-model="data.interval" :name="$t('Interval between each task start')"></duration>
                      </div>
                      <div class="ScheduleTypeInterface" v-show="data.type == 'interval'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('If this option is checked, then time when task is being executed is a part of interval. For example, if task execution time is 7 minutes and interval is 10 minutes, then next run will be scheduled in 3 minutes in case if option is checked and in 10 in other case.')">{{$t('Task execution time is part of interval')}}</span>: 
                        <input @change="Change()" class="uk-checkbox" type="checkbox" v-model="data.consider_execution_time">
                      </div>
                      
                    </div>
                    <div class="ScheduleTypeValidation" v-if="validation && validation.type == 'interval'">
                      <span uk-icon="icon: warning; ratio: 0.8"></span> <span class="ScheduleTypeValidationText">{{validation.message}}</span>
                    </div>
                  </div>
                  <div class="SchedulerSelectContainer">
                    <label><input @change="Change()" class="uk-radio" value="hour" type="radio" name="SchedulerSelect" v-model="data.type"> {{$t('Start every hour')}}</label>

                    <div class="SchedulerSelectSection">
                      <div class="ScheduleTypeDescription" v-show="data.type == 'hour'">
                          {{$t('Run task every hour at selected minute.')}}
                      
                      </div>
                      <div class="ScheduleTypeInterface" v-show="data.type == 'hour'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('List of minutes when task will be started every hour')">{{$t('Start every hour at')}}</span>: 
                        <schedulesection :mode="'minute'" @change="Change()" v-model="data.sections_hour" :name="$t('Start every hour at')"></schedulesection>
                      </div>
                      
                    </div>
                    <div class="ScheduleTypeValidation" v-if="validation && validation.type == 'hour'">
                      <span uk-icon="icon: warning; ratio: 0.8"></span> <span class="ScheduleTypeValidationText">{{validation.message}}</span>
                    </div>
                  </div>
                  <div class="SchedulerSelectContainer">
                    <label><input @change="Change()" class="uk-radio" value="day" type="radio" name="SchedulerSelect" v-model="data.type"> {{$t('Start every day')}}</label>
                    
                    <div class="SchedulerSelectSection">
                      <div class="ScheduleTypeDescription" v-show="data.type == 'day'">
                        {{$t('Run task every day at selected time.')}}
                        
                      </div>
                      <div class="ScheduleTypeInterface" v-show="data.type == 'day'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('List of hours and minutes when task will be started every day')">{{$t('Start every day at')}}</span>: 
                        <schedulesection :mode="'hourminute'" @change="Change()" v-model="data.sections_day" :name="$t('Start every day at')"></schedulesection>
                      </div>
                      
                    </div>
                    <div class="ScheduleTypeValidation" v-if="validation && validation.type == 'day'">
                      <span uk-icon="icon: warning; ratio: 0.8"></span> <span class="ScheduleTypeValidationText">{{validation.message}}</span>
                    </div>
                  </div>
                  <div class="SchedulerSelectContainer">
                    <label><input @change="Change()" class="uk-radio" value="week" type="radio" name="SchedulerSelect" v-model="data.type"> {{$t('Start every week')}}</label>
                    
                    <div class="SchedulerSelectSection">
                      <div class="ScheduleTypeDescription" v-show="data.type == 'week'">
                        {{$t('Run task every week at selected day of the week and time.')}}
                        
                      </div>
                      <div class="ScheduleTypeInterface" v-show="data.type == 'week'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('List of timestamps when task will be started every week')">{{$t('Start every week at')}}</span>: 
                        <schedulesection :mode="'weekhourminute'" @change="Change()" v-model="data.sections_week" :name="$t('Start every week at')"></schedulesection>
                      </div>
                      
                    </div>
                    <div class="ScheduleTypeValidation" v-if="validation && validation.type == 'week'">
                      <span uk-icon="icon: warning; ratio: 0.8"></span> <span class="ScheduleTypeValidationText">{{validation.message}}</span>
                    </div>
                  </div>
                  <div class="SchedulerSelectContainer">
                    <label><input @change="Change()" class="uk-radio" value="month" type="radio" name="SchedulerSelect" v-model="data.type"> {{$t('Start every month')}}</label>
                    
                    <div class="SchedulerSelectSection">
                      <div class="ScheduleTypeDescription" v-show="data.type == 'month'">
                        {{$t('Run task every month at selected day of the month and time.')}}
                        
                      </div>
                      <div class="ScheduleTypeInterface" v-show="data.type == 'month'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('List of timestamps when task will be started every month')">{{$t('Start every month at')}}</span>: 
                        <schedulesection :mode="'monthhourminute'" @change="Change()" v-model="data.sections_month" :name="$t('Start every month at')"></schedulesection>
                      </div>
                      
                    </div>
                    <div class="ScheduleTypeValidation" v-if="validation && validation.type == 'month'">
                      <span uk-icon="icon: warning; ratio: 0.8"></span> <span class="ScheduleTypeValidationText">{{validation.message}}</span>
                    </div>
                  </div>
                  <div class="SchedulerSelectContainer">
                    <label><input @change="Change()" class="uk-radio" value="list" type="radio" name="SchedulerSelect" v-model="data.type"> {{$t('Set start time list')}}</label>
                    <div class="SchedulerSelectSection">
                      <div class="ScheduleTypeDescription" v-show="data.type == 'list'">
                        {{$t('Select several timestamps to run current task.')}}
                      </div>
                      <div class="ScheduleTypeInterface" v-show="data.type == 'list'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('List of timestamps when task will be started')">{{$t('Start date list')}}</span>: 
                        <datelist @change="Change()" v-model="data.date_start_list" :name="$t('Add start script date')"></datelist>
                      </div>

                    </div>
                    <div class="ScheduleTypeValidation" v-if="validation && validation.type == 'list'">
                      <span uk-icon="icon: warning; ratio: 0.8"></span> <span class="ScheduleTypeValidationText">{{validation.message}}</span>
                    </div>

                  </div>

                  <div class="SchedulerSelectContainer">
                    <label><input @change="Change()" class="uk-radio" value="event" type="radio" name="SchedulerSelect" v-model="data.type"> {{$t('Sequence')}}</label>
                    <div class="SchedulerSelectSection">
                      <div class="ScheduleTypeDescription" v-show="data.type == 'event'">
                        {{$t('Run this task immediately after completing another task.')}}
                      </div>
                      <div class="ScheduleTypeInterface" v-show="data.type == 'event'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('Task name after completing which current task will start')">{{$t('Task name to run after')}}</span>: 
                        <selecttask @change="Change()" v-model="data.run_other_task_name" :name="$t('Task name to run after')"></selecttask>
                      </div>

                    </div>
                    <div class="ScheduleTypeValidation" v-if="validation && validation.type == 'event'">
                      <span uk-icon="icon: warning; ratio: 0.8"></span> <span class="ScheduleTypeValidationText">{{validation.message}}</span>
                    </div>

                  </div>
                  
                </div>

                <div style="width:50%;float:left">
                  <label class="ScheduleTypeMore" v-show="data.type == 'now'"><input class="uk-checkbox" type="checkbox" v-model="more"> {{$t('More settings')}}</label>
                  <div class="SchedulerSelectSection2">
                    <span v-if="more">
                      <div class="ScheduleTypeInterface" v-show="data.type == 'now'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('If task will run longer than selected time, it will be forced to stop')">{{$t('Maximum running time')}}</span>: 
                        <duration :mode="'infinite'" @change="Change()" v-model="data.max_running_time" :name="$t('Maximum running time')"></duration>
                      </div>
                    </span>
                  </div>

                  <label class="ScheduleTypeMore" v-show="data.type == 'at'"><input class="uk-checkbox" type="checkbox" v-model="more"> {{$t('More settings')}}</label>
                  <div class="SchedulerSelectSection2">
                    <span v-if="more">
                      <div class="ScheduleTypeInterface" v-show="data.type == 'at'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('If task will run longer than selected time, it will be forced to stop')">{{$t('Maximum running time')}}</span>: 
                        <duration :mode="'infinite'" @change="Change()" v-model="data.max_running_time" :name="$t('Maximum running time')"></duration>
                      </div>
                    </span>
                  </div>


                  <label class="ScheduleTypeMore" v-show="data.type == 'interval'"><input class="uk-checkbox" type="checkbox" v-model="more"> {{$t('More settings')}}</label>
                  <div class="SchedulerSelectSection2">
                    <span v-if="more">
                      <div class="ScheduleTypeInterface" v-show="data.type == 'interval'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('If single task will run longer than selected time, it will be forced to stop')">{{$t('Maximum running time for single task')}}</span>: 
                        <duration :mode="'infinite'" @change="Change()" v-model="data.max_running_time" :name="$t('Maximum running time for single task')"></duration>
                      </div>
                      <div class="ScheduleTypeInterface" v-show="data.type == 'interval'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('Maximum number of times the task can be run')">{{$t('Maximum run number')}}</span>: 
                        <selectnumber :mode="'infinite'" @change="Change()" v-model="data.max_start_number" :name="$t('Maximum run number')"></selectnumber>
                      </div>
                      <div class="ScheduleTypeInterface" v-show="data.type == 'interval'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('Select date, when task sequence will start. If no date is selected, sequence will start immediately.')">{{$t('Sequence start date')}}</span>: 
                        <selectdate @change="Change()" v-model="data.date_start" :name="$t('Start date')"></selectdate>
                      </div>
                      <div class="ScheduleTypeInterface" v-show="data.type == 'interval'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('Select date, when task sequence will stop. If no date is selected, sequence will stop only when maximum run number is reached.')">{{$t('Sequence end date')}}</span>: 
                        <selectdate @change="Change()" v-model="data.date_end" :name="$t('End date')"></selectdate>
                      </div>
                    </span>
                  </div>

                  <label class="ScheduleTypeMore" v-show="data.type == 'hour'"><input class="uk-checkbox" type="checkbox" v-model="more"> {{$t('More settings')}}</label>
                  <div class="SchedulerSelectSection2">
                    <span v-if="more">
                      <div class="ScheduleTypeInterface" v-show="data.type == 'hour'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('If single task will run longer than selected time, it will be forced to stop')">{{$t('Maximum running time for single task')}}</span>: 
                        <duration :mode="'infinite'" @change="Change()" v-model="data.max_running_time" :name="$t('Maximum running time for single task')"></duration>
                      </div>
                      <div class="ScheduleTypeInterface" v-show="data.type == 'hour'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('Maximum number of times the task can be run')">{{$t('Maximum run number')}}</span>: 
                        <selectnumber :mode="'infinite'" @change="Change()" v-model="data.max_start_number" :name="$t('Maximum run number')"></selectnumber>
                      </div>
                      <div class="ScheduleTypeInterface" v-show="data.type == 'hour'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('Select date, when task sequence will start. If no date is selected, sequence will start immediately.')">{{$t('Sequence start date')}}</span>: 
                        <selectdate @change="Change()" v-model="data.date_start" :name="$t('Start date')"></selectdate>
                      </div>
                      <div class="ScheduleTypeInterface" v-show="data.type == 'hour'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('Select date, when task sequence will stop. If no date is selected, sequence will stop only when maximum run number is reached.')">{{$t('Sequence end date')}}</span>: 
                        <selectdate @change="Change()" v-model="data.date_end" :name="$t('End date')"></selectdate>
                      </div>
                    </span>
                  </div>

                  <label class="ScheduleTypeMore" v-show="data.type == 'day'"><input class="uk-checkbox" type="checkbox" v-model="more"> {{$t('More settings')}}</label>
                  <div class="SchedulerSelectSection2">
                    <span v-if="more">
                      <div class="ScheduleTypeInterface" v-show="data.type == 'day'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('If single task will run longer than selected time, it will be forced to stop')">{{$t('Maximum running time for single task')}}</span>: 
                        <duration :mode="'infinite'" @change="Change()" v-model="data.max_running_time" :name="$t('Maximum running time for single task')"></duration>
                      </div>
                      <div class="ScheduleTypeInterface" v-show="data.type == 'day'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('Maximum number of times the task can be run')">{{$t('Maximum run number')}}</span>: 
                        <selectnumber :mode="'infinite'" @change="Change()" v-model="data.max_start_number" :name="$t('Maximum run number')"></selectnumber>
                      </div>
                      <div class="ScheduleTypeInterface" v-show="data.type == 'day'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('Select date, when task sequence will start. If no date is selected, sequence will start immediately.')">{{$t('Sequence start date')}}</span>: 
                        <selectdate @change="Change()" v-model="data.date_start" :name="$t('Start date')"></selectdate>
                      </div>
                      <div class="ScheduleTypeInterface" v-show="data.type == 'day'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('Select date, when task sequence will stop. If no date is selected, sequence will stop only when maximum run number is reached.')">{{$t('Sequence end date')}}</span>: 
                        <selectdate @change="Change()" v-model="data.date_end" :name="$t('End date')"></selectdate>
                      </div>
                    </span>
                  </div>

                  <label class="ScheduleTypeMore" v-show="data.type == 'week'"><input class="uk-checkbox" type="checkbox" v-model="more"> {{$t('More settings')}}</label>
                  <div class="SchedulerSelectSection2">
                    <span v-if="more">
                      <div class="ScheduleTypeInterface" v-show="data.type == 'week'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('If single task will run longer than selected time, it will be forced to stop')">{{$t('Maximum running time for single task')}}</span>: 
                        <duration :mode="'infinite'" @change="Change()" v-model="data.max_running_time" :name="$t('Maximum running time for single task')"></duration>
                      </div>
                      <div class="ScheduleTypeInterface" v-show="data.type == 'week'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('Maximum number of times the task can be run')">{{$t('Maximum run number')}}</span>: 
                        <selectnumber :mode="'infinite'" @change="Change()" v-model="data.max_start_number" :name="$t('Maximum run number')"></selectnumber>
                      </div>
                      <div class="ScheduleTypeInterface" v-show="data.type == 'week'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('Select date, when task sequence will start. If no date is selected, sequence will start immediately.')">{{$t('Sequence start date')}}</span>: 
                        <selectdate @change="Change()" v-model="data.date_start" :name="$t('Start date')"></selectdate>
                      </div>
                      <div class="ScheduleTypeInterface" v-show="data.type == 'week'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('Select date, when task sequence will stop. If no date is selected, sequence will stop only when maximum run number is reached.')">{{$t('Sequence end date')}}</span>: 
                        <selectdate @change="Change()" v-model="data.date_end" :name="$t('End date')"></selectdate>
                      </div>
                    </span>
                  </div>

                  <label class="ScheduleTypeMore" v-show="data.type == 'month'"><input class="uk-checkbox" type="checkbox" v-model="more"> {{$t('More settings')}}</label>
                  <div class="SchedulerSelectSection2">
                    <span v-if="more">
                      <div class="ScheduleTypeInterface" v-show="data.type == 'month'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('If single task will run longer than selected time, it will be forced to stop')">{{$t('Maximum running time for single task')}}</span>: 
                        <duration :mode="'infinite'" @change="Change()" v-model="data.max_running_time" :name="$t('Maximum running time for single task')"></duration>
                      </div>
                      <div class="ScheduleTypeInterface" v-show="data.type == 'month'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('Maximum number of times the task can be run')">{{$t('Maximum run number')}}</span>: 
                        <selectnumber :mode="'infinite'" @change="Change()" v-model="data.max_start_number" :name="$t('Maximum run number')"></selectnumber>
                      </div>
                      <div class="ScheduleTypeInterface" v-show="data.type == 'month'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('Select date, when task sequence will start. If no date is selected, sequence will start immediately.')">{{$t('Sequence start date')}}</span>: 
                        <selectdate @change="Change()" v-model="data.date_start" :name="$t('Start date')"></selectdate>
                      </div>
                      <div class="ScheduleTypeInterface" v-show="data.type == 'month'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('Select date, when task sequence will stop. If no date is selected, sequence will stop only when maximum run number is reached.')">{{$t('Sequence end date')}}</span>: 
                        <selectdate @change="Change()" v-model="data.date_end" :name="$t('End date')"></selectdate>
                      </div>
                    </span>
                  </div>

                  <label class="ScheduleTypeMore" v-show="data.type == 'list'"><input class="uk-checkbox" type="checkbox" v-model="more"> {{$t('More settings')}}</label>
                  <div class="SchedulerSelectSection2">
                    <span v-if="more">
                      <div class="ScheduleTypeInterface" v-show="data.type == 'list'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('If single task will run longer than selected time, it will be forced to stop')">{{$t('Maximum running time for single task')}}</span>: 
                        <duration :mode="'infinite'" @change="Change()" v-model="data.max_running_time" :name="$t('Maximum running time for single task')"></duration>
                      </div>
                    </span>
                  </div>

                  <label class="ScheduleTypeMore" v-show="data.type == 'event'"><input class="uk-checkbox" type="checkbox" v-model="more"> {{$t('More settings')}}</label>
                  <div class="SchedulerSelectSection2">
                    <span v-if="more">
                      <div class="ScheduleTypeInterface" v-show="data.type == 'event'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('If single task will run longer than selected time, it will be forced to stop')">{{$t('Maximum running time for single task')}}</span>: 
                        <duration :mode="'infinite'" @change="Change()" v-model="data.max_running_time" :name="$t('Maximum running time for single task')"></duration>
                      </div>

                      <div class="ScheduleTypeInterface" v-show="data.type == 'event'">
                        <span class="ScheduleTypeName">{{$t('Minimum interval between each task run')}}</span>: 
                        <duration :mode="'zero'" @change="Change()" v-model="data.interval_event" :name="$t('Minimum interval between each task run')"></duration>
                      </div>
                      <div class="ScheduleTypeInterface" v-show="data.type == 'event'">
                        <span class="ScheduleTypeName" :uk-tooltip="$t('If this option is checked, then time when task is being executed is a part of interval. For example, if task execution time is 7 minutes and interval is 10 minutes, then next run will be scheduled in 3 minutes in case if option is checked and in 10 in other case.')">{{$t('Task execution time is part of interval')}}</span>: 
                        <input class="uk-checkbox" type="checkbox" v-model="data.consider_execution_time">
                      </div>
                      
                    </span>
                  </div>

                  <div class="SchedulerSelectContainer" style="display: flex;align-items: center;margin-top:30px;" v-show="data.type">
                    <button class="uk-button uk-button-default uk-button-small" @click="Example(9)"> {{$t('Restore Defaults')}}</button>
                  </div>
                </div>


                
              </div>

            </li>
            <li>

              <div class="SchedulerSelectContainer" style="display: flex;align-items: center;">
                <span class="Circle"></span>
                <span>{{$t('Start in')}} <i>{{$t('10 minutes')}}</i> </span>
                <a href="#" @click.prevent="Example(1)" :uk-tooltip="$t('Use this example')" style="cursor: pointer">
                  <span uk-icon="icon: sign-in; ratio: 1.0"></span>
                </a>
              </div>
              <div class="SchedulerSelectContainer" style="display: flex;align-items: center;">
                <span class="Circle"></span>
                <span>{{$t('Start in')}} <i>{{$t('1 hour')}}</i></span>
                <a href="#" @click.prevent="Example(2)" :uk-tooltip="$t('Use this example')" style="cursor: pointer">
                  <span uk-icon="icon: sign-in; ratio: 1.0"></span>
                </a>
              </div>
              <div class="SchedulerSelectContainer" style="display: flex;align-items: center;">
                <span class="Circle"></span>
                <span>{{$t('Start periodically with')}} <i>{{$t('1 hour')}}</i> {{$t('interval between launches')}} </span>
                <a href="#" @click.prevent="Example(3)" :uk-tooltip="$t('Use this example')" style="cursor: pointer">
                  <span uk-icon="icon: sign-in; ratio: 1.0"></span>
                </a>
              </div>
              <div class="SchedulerSelectContainer" style="display: flex;align-items: center;">
                <span class="Circle"></span>
                <span>{{$t('Start each hour')}} <i>{{$t('at 00:30')}}</i></span> 
                <a href="#" @click.prevent="Example(4)" :uk-tooltip="$t('Use this example')" style="cursor: pointer">
                  <span uk-icon="icon: sign-in; ratio: 1.0"></span>
                </a>
              </div>
              <div class="SchedulerSelectContainer" style="display: flex;align-items: center;">
                <span class="Circle"></span>
                <span>{{$t('Start every day')}} <i>{{$t('at 23:59')}}</i> </span>
                <a href="#" @click.prevent="Example(5)" :uk-tooltip="$t('Use this example')" style="cursor: pointer">
                  <span uk-icon="icon: sign-in; ratio: 1.0"></span>
                </a>
              </div>
              <div class="SchedulerSelectContainer" style="display: flex;align-items: center;">
                <span class="Circle"></span>
                <span>{{$t('Start every week')}} <i>{{$t('at Monday between 13:00 and 14:00')}}</i> </span>
                <a href="#" @click.prevent="Example(6)" :uk-tooltip="$t('Use this example')" style="cursor: pointer">
                  <span uk-icon="icon: sign-in; ratio: 1.0"></span>
                </a>
              </div>
              <div class="SchedulerSelectContainer" style="display: flex;align-items: center;">
                <span class="Circle"></span>
                <span>{{$t('Start on working days evenings and weekends with probability')}} </span>
                <a href="#" @click.prevent="Example(7)" :uk-tooltip="$t('Use this example')" style="cursor: pointer">
                  <span uk-icon="icon: sign-in; ratio: 1.0"></span>
                </a>
              </div>
              <div class="SchedulerSelectContainer" style="display: flex;align-items: center;">
                <span class="Circle"></span>
                <span>{{$t('Start')}} <i>{{$t('10 times')}}</i> {{$t('from now without interval')}} </span>
                <a href="#" @click.prevent="Example(8)" :uk-tooltip="$t('Use this example')" style="cursor: pointer">
                  <span uk-icon="icon: sign-in; ratio: 1.0"></span>
                </a>
              </div>
              


            </li>

          </ul>
        </div>
        <div id="SchedulerTesting" style="width: calc(40% - 41px);float: left;margin-left:40px;" v-show="predictionDataPrepared && !validation && mainTabIndex == 0">
          <ul uk-tab>
            <li><a href="#">{{$t('Estimated launch schedule')}}</a></li>
            
          </ul>

          <ul class="uk-switcher uk-margin">
            <li>
              <div style="text-align:center">
                <span :uk-tooltip="$t('And more items')" uk-icon="icon: more; ratio: 1.5" v-if="predictionDataHasMore" style="margin-bottom: 5px;margin-top: -20px;"></span>
              </div>

              <div v-if="!predictionDataIsSuccess" style="color: #f0506e;font-size: .875rem;display: flex;align-items: center;">
                <span uk-icon="icon: warning; ratio: 0.8"></span> 
                <span style="margin-left:5px;">{{predictionDataMessage}}</span>
              </div>

              <div v-for="itemDate in predictionDataPrepared" :key="itemDate.date">
                <div style="font-size: 15px;font-style: italic;">{{itemDate.date}}</div>
                  <ul class="uk-list uk-list-bullet" v-show="itemDate.data.length > 0" style="margin-left:10px;margin-top:0px;margin-bottom:10px">
                    <div v-for="item in itemDate.data" :key="item" style="font-size: small;color: gray;">
                      {{moment(item).format('HH:mm')}}
                    </div>
                  </ul> 
                </div>
              </li></ul></div>
              
              
            
          
        </div>
      `

}