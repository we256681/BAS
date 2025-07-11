

window.Connecting = {

    props: [],
    data: function()
    {
      return {
        showloading: true,
        text: ""
      }
    },
    
    methods: {
      ShowError: function(Error, IsError){
        /*this.text = Error
        this.showloading = true*/
      },
      ShowLogs: function(){
        var Res = ShowApplicationLogs()
        var blob = new Blob([Res], {type: "text/plain;charset=utf-8"});
        saveAs(blob, "scheduler_install_log.txt");
      }
    },

    mounted: function()
    {
      this.text = this.$t("Starting application")
    },
    

    template: 
    `
      <div id="CenterConnecting">
        <div id="CenterConnectingInner">
          <div v-show="showloading">
            <div uk-spinner="ratio: 1.4"></div>
          </div>
          <div id="CenterScreenLogo">
            <div id="CenterScreenLogoText">{{text}} <a href="#" @click.prevent="ShowLogs()" style="font-size:small"><span uk-icon="icon: info; ratio: 0.8"></span></a></div>
            
            
          </div>
        </div>

      </div>
    `

}