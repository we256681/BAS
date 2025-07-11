

window.SelectNumber = {
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
        $("#EditNumberModalName").text(this.name)
        
        if(typeof(this.data) == "undefined" || !this.data)
          $("#EditNumberModalData").val(10000)
        else if(this.data < 0)
        {
          $("#EditNumberModalData").val(1)
        }else
          $("#EditNumberModalData").val(this.data)
        
        $('#EditNumberModalSave').off('click.number').on('click.number', function() { 
          var NewNumber = parseInt($("#EditNumberModalData").val())
          if(NewNumber)
          {
            self.$emit('change',NewNumber)
            UIkit.modal($('#EditNumberModal')).hide()
          }else
          {
            self.$emit('change',"")
          }
        });
        
        UIkit.modal($('#EditNumberModal')).show()
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
            <span v-if="data < 0">
              1
            </span>
            <span v-else>
              {{data}}
            </span>
          </a>
        </span>
        <span v-show="!data">
          <a href="#" @click.prevent="Select()">
            10000
          </a>
        </span>
      </span>
    `

}