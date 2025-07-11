

window.SelectTask = {
    model: {
      prop: 'data',
      event: 'change'
    },
    props: ["data", "name"],
    data: function()
    {
      return {
      }
    },
    
    methods: {
      Select: function()
      {
        var self = this
        $("#EditTaskSequenceModalName").text(this.name)

        var List = window.App.$refs.tasklistpage.tasks.map(t => t.name)

        var ListFiltered = [...new Set(List)].sort();

        if(ListFiltered.length > 0)
        {
          $("#EditTaskSequenceModalData").show()
          $("#EditTaskSequenceNoData").hide()
        }else
        {
          $("#EditTaskSequenceModalData").hide()
          $("#EditTaskSequenceNoData").show()
        }


        $("#EditTaskSequenceModalData").html("")


        var Default = ""
        ListFiltered.forEach(function(x, index){
          var el = $("<option>")
            .val(x)
            .text(x)
            if(index == 0)
            {
              Default = x
            }
          $("#EditTaskSequenceModalData").append(el);
        })

        if(typeof(self.data) != "undefined" && self.data)
          $("#EditTaskSequenceModalData").val(self.data)
        else
          $("#EditTaskSequenceModalData").val(Default)
        
        $('#EditTaskSequenceModalSave').off('click.tasks').on('click.tasks', function() { 
          var NewValue = $("#EditTaskSequenceModalData").val()
          self.$emit('change',NewValue)
          UIkit.modal($('#EditTaskSequenceModal')).hide()
        });
        
        UIkit.modal($('#EditTaskSequenceModal')).show()
      }

    },

    updated: function()
    {
      
    },
    

    template: 
    `
      <span>
        <span v-show="data">
          <a href="#" @click.prevent="Select()">
            {{data}}
          </a>
        </span>
        <span v-show="!data">
          <a href="#" @click.prevent="Select()">
            {{$t('Task not set')}}
          </a>
        </span>
      </span>
    `

}