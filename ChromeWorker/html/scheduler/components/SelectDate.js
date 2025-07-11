

window.SelectDate = {
    model: {
      prop: 'timestamp',
      event: 'change'
    },
    props: ["timestamp", "name"],
    data: function()
    {
      return {
      }
    },
    
    methods: {
      Select: function()
      {
        var self = this
        var lastDate = null
        $("#EditDateModalName").text(this.name)
        $("#EditDateModal .datepicker-here").datepicker({
           todayButton: new Date(),
           onSelect: function(formattedDate, date, inst) {
            lastDate = date;
          },
          clearButton: true,
          language: window.CurrentLang

        })
        
        if(typeof(this.timestamp) == "undefined" || !this.timestamp)
          $("#EditDateModal .datepicker-here").data('datepicker').selectDate(new Date());
        else
          $("#EditDateModal .datepicker-here").data('datepicker').selectDate(new Date(parseInt(this.timestamp)));
        
        $('#EditDateModalSave').off('click.date').on('click.date', function() { 
          
          if(lastDate)
          {
            var NewTimestamp = lastDate.getTime().toString();
            self.$emit('change',NewTimestamp)
          }else
          {
            self.$emit('change',"")
          }
          UIkit.modal($('#EditDateModal')).hide()
        });
        
        UIkit.modal($('#EditDateModal')).show()
      }

    },

    updated: function()
    {
      
    },
    

    template: 
    `
      <span>
          <span v-show="timestamp">
            <a href="#" @click.prevent="Select()">
              {{moment(parseInt(timestamp)).format('dddd, MMMM Do, HH:mm')}}
            </a>
          </span>
          <span v-show="!timestamp">
            <a href="#" @click.prevent="Select()">
              {{$t('Date not set')}}
            </a>
          </span>
        
      </span>
    `

}