

window.ScheduleSection = {
    model: {
      prop: 'dat',
      event: 'change'
    },
    props: ["dat", "name", "mode"],
    data: function()
    {
      return {
      }
    },
    
    methods: {
      FormatHour: function(Hour)
      {
        if(Hour < 10)
          return "0" + Hour
        return Hour.toString()
      },
      ItemPartToText: function(El)
      {
        if(El.hasOwnProperty("week") && El.hasOwnProperty("hour") && El.hasOwnProperty("minute"))
        {
          var WeekName = ""
          if(El.week == 1)
          {
            WeekName = this.$t('Monday')
          }
          if(El.week == 2)
          {
            WeekName = this.$t('Tuesday')
          }
          if(El.week == 3)
          {
            WeekName = this.$t('Wednesday')
          }
          if(El.week == 4)
          {
            WeekName = this.$t('Thursday')
          }
          if(El.week == 5)
          {
            WeekName = this.$t('Friday')
          }
          if(El.week == 6)
          {
            WeekName = this.$t('Saturday')
          }
          if(El.week == 7)
          {
            WeekName = this.$t('Sunday')
          }

          return WeekName + ", " + this.FormatHour(El.hour) + ":" + this.FormatHour(El.minute)
          

        }else if(El.hasOwnProperty("day") && El.hasOwnProperty("hour") && El.hasOwnProperty("minute"))
        {
          return El.day.toString() + this.$t(" day of the month, ") + this.FormatHour(El.hour) + ":" + this.FormatHour(El.minute)

        }else if(El.hasOwnProperty("hour") && El.hasOwnProperty("minute"))
        {
          return this.FormatHour(El.hour) + ":" + this.FormatHour(El.minute)
          
        }
        else if(El.hasOwnProperty("minute"))
        {
          if(El.minute == 1)
            return El.minute + this.$t(" minute")
          else
            return El.minute + this.$t(" minutes")
          
        }

        return ""
        
      },
      HasProbability: function(El)
      {
        if(El.start.hasOwnProperty("minute") && El.end.minute != El.start.minute)
        {
          return true
        }

        if(El.start.hasOwnProperty("hour") && El.end.hour != El.start.hour)
        {
          return true
        }

        if(El.start.hasOwnProperty("day") && El.end.day != El.start.day)
        {
          return true
        }

        if(El.start.hasOwnProperty("week") && El.end.week != El.start.week)
        {
          return true
        }

        if(El.probability != 100)
        {
          return true
        }

        return false
      },
      ItemToText: function(El)
      {
        
        var WithProbability = this.HasProbability(El)
        

        if(WithProbability)
        {
          var r = this.$t("from ") + this.ItemPartToText(El.start) + this.$t(" to ") + this.ItemPartToText(El.end) 
          if(El.probability != 100)
            r += this.$t(" with probability ") + El.probability + "%";
          return r
        }else
        {
          return this.$t("at ") + this.ItemPartToText(El.start);
        }

        
      },
      Delete: function(CurrentIndex)
      {
        this.$delete(this.dat, CurrentIndex);
        this.$emit('change',this.dat)

      },
      RemoveAll: function()
      {
        this.dat = [];
        this.$emit('change',this.dat)

      },
      Add: function(CurrentTask, CurrentIndex)
      {
        var self = this
        var ProbabilityShown = false
        $("#EditSchedulerSectionModalName").text(this.name)

        //Update probability range control
        $('#EditSchedulerSectionProbabilityData').off('input.section').on('input.section', function(){
          $('#EditSchedulerSectionProbabilityLabel').text($("#EditSchedulerSectionProbabilityData").val() + "%")
        })

        //Prepare interface

        //Set values
        if(CurrentTask && CurrentTask.hasOwnProperty("start") && CurrentTask.start.hasOwnProperty("day"))
          $("#EditSchedulerSectionDays").val(CurrentTask.start.day)
        else
          $("#EditSchedulerSectionDays").val(1)

        if(CurrentTask && CurrentTask.hasOwnProperty("start") && CurrentTask.start.hasOwnProperty("week"))
          $("#EditSchedulerSectionWeeks").val(CurrentTask.start.week)
        else
          $("#EditSchedulerSectionWeeks").val(1)

        if(CurrentTask && CurrentTask.hasOwnProperty("start") && CurrentTask.start.hasOwnProperty("hour"))
          $("#EditSchedulerSectionHours").val(CurrentTask.start.hour)
        else
          $("#EditSchedulerSectionHours").val(0)

        if(CurrentTask && CurrentTask.hasOwnProperty("start") && CurrentTask.start.hasOwnProperty("minute"))
          $("#EditSchedulerSectionMinutes").val(CurrentTask.start.minute)
        else
          $("#EditSchedulerSectionMinutes").val(0)



        if(CurrentTask && CurrentTask.hasOwnProperty("end") && CurrentTask.end.hasOwnProperty("day"))
          $("#EditSchedulerSectionDaysTo").val(CurrentTask.end.day)
        else
          $("#EditSchedulerSectionDaysTo").val(1)

        if(CurrentTask && CurrentTask.hasOwnProperty("end") && CurrentTask.end.hasOwnProperty("week"))
          $("#EditSchedulerSectionWeeksTo").val(CurrentTask.end.week)
        else
          $("#EditSchedulerSectionWeeksTo").val(1)

        if(CurrentTask && CurrentTask.hasOwnProperty("end") && CurrentTask.end.hasOwnProperty("hour"))
          $("#EditSchedulerSectionHoursTo").val(CurrentTask.end.hour)
        else
          $("#EditSchedulerSectionHoursTo").val(0)

        if(CurrentTask && CurrentTask.hasOwnProperty("end") && CurrentTask.end.hasOwnProperty("minute"))
          $("#EditSchedulerSectionMinutesTo").val(CurrentTask.end.minute)
        else
          $("#EditSchedulerSectionMinutesTo").val(0)

        if(CurrentTask && CurrentTask.hasOwnProperty("probability"))
        {
          $("#EditSchedulerSectionProbabilityData").val(CurrentTask.probability)
          $('#EditSchedulerSectionProbabilityLabel').text(CurrentTask.probability + "%")
        }
        else
        {
          $("#EditSchedulerSectionProbabilityData").val(100)
          $('#EditSchedulerSectionProbabilityLabel').text("100%")

        }


        //Panels visibility
        $("#EditSchedulerSectionValidation").hide()

        $("#EditSchedulerSectionDaysPanel").hide()
        $("#EditSchedulerSectionWeeksPanel").hide()
        $("#EditSchedulerSectionHoursPanel").hide()
        $("#EditSchedulerSectionMinutesPanel").hide()

        $("#EditSchedulerSectionDaysPanelTo").hide()
        $("#EditSchedulerSectionWeeksPanelTo").hide()
        $("#EditSchedulerSectionHoursPanelTo").hide()
        $("#EditSchedulerSectionMinutesPanelTo").hide()

        $("#EditSchedulerSectionProbabilityDataPanel").hide()
        

        $("[for=EditSchedulerSectionDays]").text(this.$t("Day of the month : "))
        $("[for=EditSchedulerSectionWeeks]").text(this.$t("Day of the week : "))
        $("[for=EditSchedulerSectionHours]").text(this.$t("Hour : "))
        $("[for=EditSchedulerSectionMinutes]").text(this.$t("Minute : "))

        if(this.mode == "minute")
        {
          $("#EditSchedulerSectionMinutesPanel").show()
        }
        if(this.mode == "hourminute")
        {
          $("#EditSchedulerSectionMinutesPanel").show()
          $("#EditSchedulerSectionHoursPanel").show()
        }
        if(this.mode == "weekhourminute")
        {
          $("#EditSchedulerSectionMinutesPanel").show()
          $("#EditSchedulerSectionHoursPanel").show()
          $("#EditSchedulerSectionWeeksPanel").show()
        }
        if(this.mode == "monthhourminute")
        {
          $("#EditSchedulerSectionMinutesPanel").show()
          $("#EditSchedulerSectionHoursPanel").show()
          $("#EditSchedulerSectionDaysPanel").show()
        }
        
        



        //Toggle probability
        var ToggleProbabbility = function() { 
          ProbabilityShown = !ProbabilityShown;
          if(ProbabilityShown)
          {
            //Show probability
            $("#EditSchedulerSectionProbability").text(self.$t("Hide probability settings") )

            $("[for=EditSchedulerSectionDays]").text(self.$t("From day of the month : "))
            $("[for=EditSchedulerSectionWeeks]").text(self.$t("From day of the week : "))
            $("[for=EditSchedulerSectionHours]").text(self.$t("From hour : "))
            $("[for=EditSchedulerSectionMinutes]").text(self.$t("From minute : "))


            $("#EditSchedulerSectionDaysPanelTo").hide()
            $("#EditSchedulerSectionWeeksPanelTo").hide()
            $("#EditSchedulerSectionHoursPanelTo").hide()
            $("#EditSchedulerSectionMinutesPanelTo").hide()

            if(self.mode == "minute")
            {
              $("#EditSchedulerSectionMinutesPanelTo").show()
            }
            if(self.mode == "hourminute")
            {
              $("#EditSchedulerSectionMinutesPanelTo").show()
              $("#EditSchedulerSectionHoursPanelTo").show()
            }
            if(self.mode == "weekhourminute")
            {
              $("#EditSchedulerSectionMinutesPanelTo").show()
              $("#EditSchedulerSectionHoursPanelTo").show()
              $("#EditSchedulerSectionWeeksPanelTo").show()
            }
            if(self.mode == "monthhourminute")
            {
              $("#EditSchedulerSectionMinutesPanelTo").show()
              $("#EditSchedulerSectionHoursPanelTo").show()
              $("#EditSchedulerSectionDaysPanelTo").show()
            }

            $("#EditSchedulerSectionProbabilityDataPanel").show()

          }
          else
          {
            //Hide probability
            $("#EditSchedulerSectionProbability").text(self.$t("Run with probability"))

            $("[for=EditSchedulerSectionDays]").text(self.$t("Day of the month : "))
            $("[for=EditSchedulerSectionWeeks]").text(self.$t("Day of the week : "))
            $("[for=EditSchedulerSectionHours]").text(self.$t("Hour : "))
            $("[for=EditSchedulerSectionMinutes]").text(self.$t("Minute : "))


            $("#EditSchedulerSectionDaysPanelTo").hide()
            $("#EditSchedulerSectionWeeksPanelTo").hide()
            $("#EditSchedulerSectionHoursPanelTo").hide()
            $("#EditSchedulerSectionMinutesPanelTo").hide()

            $("#EditSchedulerSectionProbabilityDataPanel").hide()
          }
        };
        if(CurrentTask && this.HasProbability(CurrentTask))
        {
          ToggleProbabbility()
        }
        $('#EditSchedulerSectionProbability').off('click.section').on('click.section', ToggleProbabbility);

                
        $('#EditSchedulerSectionModalSave').off('click.section').on('click.section', function() {
          //Collect data
          var item = {start: {}, end: {}, probability: 100}
          var Validated = true

          if(self.mode == "minute")
          {
            if(ProbabilityShown)
            {
              item.probability = parseInt($("#EditSchedulerSectionProbabilityData").val())
              item.start.minute = parseInt($("#EditSchedulerSectionMinutes").val())
              item.end.minute = parseInt($("#EditSchedulerSectionMinutesTo").val())
              if(item.start.minute < 0 || item.start.minute > 59)
                return;
              if(item.end.minute < 0 || item.end.minute > 59)
                return;

              var Start = 0;
              var End = 0;

              Start += item.start.minute;
              End += item.end.minute;
              
              if(Start > End)
                Validated = false
            }else
            {
              item.start.minute = parseInt($("#EditSchedulerSectionMinutes").val())
              item.end.minute = item.start.minute
              if(item.start.minute < 0 || item.start.minute > 59)
                return;
            }
          }
          if(self.mode == "hourminute")
          {
            if(ProbabilityShown)
            {
              item.probability = parseInt($("#EditSchedulerSectionProbabilityData").val())
              item.start.minute = parseInt($("#EditSchedulerSectionMinutes").val())
              item.end.minute = parseInt($("#EditSchedulerSectionMinutesTo").val())
              item.start.hour = parseInt($("#EditSchedulerSectionHours").val())
              item.end.hour = parseInt($("#EditSchedulerSectionHoursTo").val())
              if(item.start.minute < 0 || item.start.minute > 59)
                return;
              if(item.end.minute < 0 || item.end.minute > 59)
                return;
              if(item.start.hour < 0 || item.start.hour > 23)
                return;
              if(item.end.hour < 0 || item.end.hour > 23)
                return;

              var Start = 0;
              var End = 0;

              Start += item.start.minute;
              End += item.end.minute;

              Start += 1000 * item.start.hour;
              End += 1000 * item.end.hour;
              
              if(Start > End)
                Validated = false
            }else
            {
              item.start.minute = parseInt($("#EditSchedulerSectionMinutes").val())
              item.start.hour = parseInt($("#EditSchedulerSectionHours").val())
              item.end.minute = item.start.minute
              item.end.hour = item.start.hour
              if(item.start.minute < 0 || item.start.minute > 59)
                return;
              if(item.start.hour < 0 || item.start.hour > 23)
                return;
            }
              
          }
          if(self.mode == "weekhourminute")
          {
            if(ProbabilityShown)
            {
              item.probability = parseInt($("#EditSchedulerSectionProbabilityData").val())
              item.start.minute = parseInt($("#EditSchedulerSectionMinutes").val())
              item.end.minute = parseInt($("#EditSchedulerSectionMinutesTo").val())
              item.start.hour = parseInt($("#EditSchedulerSectionHours").val())
              item.end.hour = parseInt($("#EditSchedulerSectionHoursTo").val())
              item.start.week = parseInt($("#EditSchedulerSectionWeeks").val())
              item.end.week = parseInt($("#EditSchedulerSectionWeeksTo").val())

              if(item.start.minute < 0 || item.start.minute > 59)
                return;
              if(item.end.minute < 0 || item.end.minute > 59)
                return;
              if(item.start.hour < 0 || item.start.hour > 23)
                return;
              if(item.end.hour < 0 || item.end.hour > 23)
                return;

              var Start = 0;
              var End = 0;

              Start += item.start.minute;
              End += item.end.minute;

              Start += 1000 * item.start.hour;
              End += 1000 * item.end.hour;

              Start += 1000 * 1000 * item.start.week;
              End += 1000 * 1000 * item.end.week;
              
              if(Start > End)
                Validated = false
            }else
            {
              item.start.minute = parseInt($("#EditSchedulerSectionMinutes").val())
              item.start.hour = parseInt($("#EditSchedulerSectionHours").val())
              item.start.week = parseInt($("#EditSchedulerSectionWeeks").val())
              item.end.minute = item.start.minute
              item.end.hour = item.start.hour
              item.end.week = item.start.week
              if(item.start.minute < 0 || item.start.minute > 59)
                return;
              if(item.start.hour < 0 || item.start.hour > 23)
                return;             
            }
          }
          if(self.mode == "monthhourminute")
          {
            if(ProbabilityShown)
            {
              item.probability = parseInt($("#EditSchedulerSectionProbabilityData").val())
              item.start.minute = parseInt($("#EditSchedulerSectionMinutes").val())
              item.end.minute = parseInt($("#EditSchedulerSectionMinutesTo").val())
              item.start.hour = parseInt($("#EditSchedulerSectionHours").val())
              item.end.hour = parseInt($("#EditSchedulerSectionHoursTo").val())
              item.start.day = parseInt($("#EditSchedulerSectionDays").val())
              item.end.day = parseInt($("#EditSchedulerSectionDaysTo").val())

              if(item.start.minute < 0 || item.start.minute > 59)
                return;
              if(item.end.minute < 0 || item.end.minute > 59)
                return;
              if(item.start.hour < 0 || item.start.hour > 23)
                return;
              if(item.end.hour < 0 || item.end.hour > 23)
                return;
              if(item.start.day < 0 || item.start.day > 31)
                return;
              if(item.end.day < 0 || item.end.day > 31)
                return;


              var Start = 0;
              var End = 0;

              Start += item.start.minute;
              End += item.end.minute;

              Start += 1000 * item.start.hour;
              End += 1000 * item.end.hour;

              Start += 1000 * 1000 * item.start.day;
              End += 1000 * 1000 * item.end.day;
              
              if(Start > End)
                Validated = false
            }else
            {
              item.start.minute = parseInt($("#EditSchedulerSectionMinutes").val())
              item.start.hour = parseInt($("#EditSchedulerSectionHours").val())
              item.start.day = parseInt($("#EditSchedulerSectionDays").val())
              item.end.minute = item.start.minute
              item.end.hour = item.start.hour
              item.end.day = item.start.day
              if(item.start.minute < 0 || item.start.minute > 59)
                return;
              if(item.start.hour < 0 || item.start.hour > 23)
                return;
              if(item.start.day < 0 || item.start.day > 31)
                return;
            }
          }

          if(Validated)
          {
            if(CurrentTask)
            {
              var index = CurrentIndex
              if(index >= 0)
              {
                self.$set(self.dat, index, item)
              }
            }else
            {
              self.dat.push(item)
            }
            self.$emit('change',self.dat)

            UIkit.modal($('#EditSchedulerSectionModal')).hide()
          }else
          {
            $("#EditSchedulerSectionValidation").show()
          }
        });
        
        UIkit.modal($('#EditSchedulerSectionModal')).show()
      }

    },
    

    template: `<span>
          <span v-for="(d, index) of dat" :key="index">
            <span style="color:rgb(102, 102, 102);font-size:16px;display: flex;align-items: center;">
              
              
              <div class="uk-inline" style="margin-right:5px">
                <a href="#"><span uk-icon="icon: pencil"></span></a>

                <div uk-dropdown="mode: click">
                  <ul class="uk-nav uk-dropdown-nav">
                    
                    <li class="uk-active"><a href="#" @click.prevent="Add(d, index)"><span uk-icon="icon: pencil"></span> {{$t('Edit')}}</a></li>
                    <li class="uk-active"><a href="#" @click.prevent="UIkit.modal.confirm($t('Are you sure, that you want to delete this interval?'),{stack: true}).then(function(){Delete(index)})"><span uk-icon="icon: close"></span> {{$t('Remove')}}</a></li>
                    
                  </ul>
                </div>
              </div>

              <span>{{ItemToText(d)}}</span>
              
            </span>
          </span>
          


          <a href="#" @click.prevent="Add(null,-1)" :style="{'display': (dat.length == 0 ? 'inline' : 'inline-block'),'margin-left': (dat.length == 0 ? '3px' : '22px'),'margin-top': '7px'}">
            <span uk-icon="icon: plus-circle"></span>
          </a>

          <a href="#" :uk-tooltip="$t('Remove all intervals')" v-show="dat.length" @click.prevent="UIkit.modal.confirm($t('Are you sure, that you want to delete all intervals?'),{stack: true}).then(function(){RemoveAll()})" :style="{'display': 'inline','margin-left': '5px','margin-top': '7px', 'cursor': 'pointer'}">
            <span uk-icon="icon: trash"></span>
          </a>
        
      </span>`

}