

window.Duration = {
    model: {
      prop: 'seconds',
      event: 'change'
    },
    props: ["seconds", "name", "mode"],
    data: function()
    {
      return {
      }
    },
    computed:
    {
      minute: function()
      {
        var minutes = 0
        if(typeof(this.seconds) != "undefined")
        {
          minutes = Math.floor(this.seconds / 60)
        }
        
        return minutes % 60
      },
      hour: function()
      {
        var minutes = 0
        if(typeof(this.seconds) != "undefined")
        {
          minutes = Math.floor(this.seconds / 60)
        }
        return Math.floor((minutes % 1440) / 60)
      },
      day: function()
      {
        var minutes = 0
        if(typeof(this.seconds) != "undefined")
        {
          minutes = Math.floor(this.seconds / 60)
        }
        return Math.floor(minutes / 1440)
      }
    },
    
    methods: {
      Text: function(){
        var Res = []
        
        if(typeof(this.seconds) == "undefined" || !this.seconds)
        {
          if(this.mode == "infinite")
            return this.$t("Infinite")
          else
            return this.$t("0 seconds")
        }

        if(this.day > 1)
        {
          Res.push(this.day + this.$t(" days"))
        }else if(this.day > 0)
        {
          Res.push(this.day + this.$t(" day"))
        }

        if(this.hour > 1)
        {
          Res.push(this.hour + this.$t(" hours"))
        }else if(this.hour > 0)
        {
          Res.push(this.hour + this.$t(" hour"))
        }

        if(this.minute > 1)
        {
          Res.push(this.minute + this.$t(" minutes"))
        }else if(this.minute > 0)
        {
          Res.push(this.minute + this.$t(" minute"))
        }else if(Res.length == 0)
        {
          Res.push(this.minute + this.$t(" minutes"))
        }



        return Res.join(", ")
      },
      Select: function()
      {
        var self = this
        $("#EditDurationModalName").text(this.name)
        $("#EditDurationDays").val(this.day)
        $("#EditDurationHours").val(this.hour)
        $("#EditDurationMinutes").val(this.minute)

        if(this.mode == "infinite")
        {
          $("#EditDurationInfinite").show()

        }else
        {
          $("#EditDurationInfinite").hide()

        }

        $('#EditDurationModalSave').off('click.duration').on('click.duration', function() { 
          var day = parseInt($("#EditDurationDays").val())
          var hour = parseInt($("#EditDurationHours").val())
          var minute = parseInt($("#EditDurationMinutes").val())
          if(hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59)
          {
            self.seconds = 60 * (minute  + 60 * (hour + 24 * day))
            self.$emit('change',self.seconds)
            UIkit.modal($('#EditDurationModal')).hide()
          }
        });
        $('#EditDurationInfinite').off('click.duration').on('click.duration', function(event) { 
          event.preventDefault();
          self.seconds = 0
          self.$emit('change',self.seconds)

          UIkit.modal($('#EditDurationModal')).hide()
        });

        
        UIkit.modal($('#EditDurationModal')).show()
      }

    },

    updated: function()
    {
      
    },
    

    template: 
    `
      <span>
        <a href="#" @click.prevent="Select()">
          <span v-html="Text()">
            
          </span>
          
        </a>
      </span>
    `

}