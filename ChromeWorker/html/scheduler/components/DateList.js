

window.DateList = {
    model: {
      prop: 'dat',
      event: 'change'
    },
    props: ["dat", "name"],
    data: function()
    {
      return {
      }
    },
    
    methods: {
      Upload: function(event)
      {
        if (!window.File || !window.FileReader || !window.FileList || !window.Blob) {
          return;
        }
        var self = this

        var input = event.target;
        var length = input.files.length
        if(length > 0)
        {
          var file = input.files[0];
          var fr = new FileReader();
          fr.onload = function(){
            
            self.dat = fr.result.match(/[^\r\n]+/g).filter(l => /^\d{13}$/.test(l) );
            self.$emit('change',self.dat)    

            event.target.type = "text";
            event.target.type = "file";
            
          }
          fr.readAsText(file);
        }
      },
      ItemToText: function(El)
      {
        return moment(parseInt(El)).format('dddd, MMMM Do, HH:mm')
      },
      RemoveAll: function()
      {
        this.dat = [];
        this.$emit('change',this.dat)

      },
      Delete: function(CurrentIndex)
      {
        this.$delete(this.dat, CurrentIndex);
        this.$emit('change',this.dat)

      },
      Add: function(CurrentDate, CurrentIndex)
      {
        var self = this
        var lastDate = null
        $("#EditDateModalName").text(this.name)
        $("#EditDateModal .datepicker-here").datepicker({
           todayButton: new Date(),
           onSelect: function(formattedDate, date, inst) {
            lastDate = date;
          },
          language: window.CurrentLang
        })
        
        if(CurrentDate)
          $("#EditDateModal .datepicker-here").data('datepicker').selectDate(new Date(CurrentDate));
        else
          $("#EditDateModal .datepicker-here").data('datepicker').selectDate(new Date());
          
        
        $('#EditDateModalSave').off('click.date').on('click.date', function() { 
          
          if(lastDate)
          {
            var NewTimestamp = lastDate.getTime().toString();
            if(CurrentDate)
            {
              var index = CurrentIndex
              if(index >= 0)
              {
                self.$set(self.dat, index, NewTimestamp)
              }
            }else
            {
              self.dat.push(NewTimestamp)
            }
            self.$emit('change',self.dat)
          }

          

          UIkit.modal($('#EditDateModal')).hide()
        });
        
        UIkit.modal($('#EditDateModal')).show()
      }

    },
    

    template: `<span>
          <span v-for="(d, index) of dat" :key="index">
            <span style="color:rgb(102, 102, 102);font-size:16px;display: flex;align-items: center;">
              
              
              <div class="uk-inline" style="margin-right:5px">
                <a href="#"><span uk-icon="icon: pencil"></span></a>

                <div uk-dropdown="mode: click">
                  <ul class="uk-nav uk-dropdown-nav">
                    
                    <li class="uk-active"><a href="#" @click.prevent="Add(parseInt(d), index)"><span uk-icon="icon: pencil"></span> {{$t('Edit')}}</a></li>
                    <li class="uk-active"><a href="#" @click.prevent="UIkit.modal.confirm($t('Are you sure, that you want to delete this date?'),{stack: true}).then(function(){Delete(index)})"><span uk-icon="icon: close"></span> {{$t('Remove')}}</a></li>
                    
                  </ul>
                </div>
              </div>

              <span>{{ItemToText(d)}}</span>
              
            </span>
          </span>
          


          <a href="#" @click.prevent="Add(null,-1)" :style="{'display': (dat.length == 0 ? 'inline' : 'inline-block'),'margin-left': (dat.length == 0 ? '3px' : '22px'),'margin-top': '7px'}">
            <span uk-icon="icon: plus-circle"></span>
          </a>

          <a href="#" :uk-tooltip="$t('Remove all dates')" v-show="dat.length" @click.prevent="UIkit.modal.confirm($t('Are you sure, that you want to delete all dates?'),{stack: true}).then(function(){RemoveAll()})" :style="{'display': 'inline','margin-left': '5px','margin-top': '7px', 'cursor': 'pointer'}">
            <span uk-icon="icon: trash"></span>
          </a>

          <div uk-form-custom :uk-tooltip="$t('Upload date list from file, each line must contain milliseconds elapsed since the epoch.')">
              <input type="file" @change="Upload">
              <a href="#" :style="{'display': 'inline','margin-left': '5px','margin-top': '7px', 'cursor': 'pointer'}">
                <span uk-icon="icon: upload"></span>
              </a>
              
          </div>
          
        
      </span>`

}