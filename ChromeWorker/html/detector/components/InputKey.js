window.InputKey = {

    props: ['fpkey'],
    data: function()
    {
      return {
        busy: false,
        preload: true
      }
    },
    created: function()
    {
      if(this.fpkey)
        this.CheckKey()
      else
        this.preload = false

    },
    methods: {
      CheckKey: function () {
        var self = this
        self.busy = true
        fetch(`https://fingerprints.bablosoft.com/checkkeyforip?key=${this.fpkey}`)
          .then(function(response) {
            return response.json()
          }).then(function(json) {
            self.preload = false

            setTimeout(function(){
              self.busy = false  
            },1500)
            
            if(json["valid"])
            {
              self.$emit('fplogin',self.fpkey)
            }else
            {
              $.uiAlert({
                textHead: 'Failed to login',
                text: json["message"],
                bgcolor: '#DB2828',
                textcolor: '#fff',
                position: 'bottom-right',
                icon: 'remove circle',
                time: 3,
              })
            }
          });
        
      }
    },

    template: `<div style="background-color:#fbfbfb;height:100vh" v-if="!preload">
      <div style="background-color:rgb(248, 248, 249);display: flex;padding-left:0px;padding-right:20px;padding-top:15px;border-bottom:1px solid rgba(34,36,38,.15)">
        <img src="pic/mask.png" style="width:32px;height:32px;margin:20px;float:left"></img>
        <div>
          <h4 style="margin:5px">{{$t("Now it is easy to detect if site fingerprints you!")}}</h4>
          <div style="margin:5px 5px 20px 5px;">{{$t("FingerprintDetector addon helps to check if site that is opened with BAS uses browser fingerprint techniques to identify you among other users. This includes canvas, audio fingerprinting, font enumeration and many other. You will get detailed report for each fingerprinting activity as a result of its work.")}} <i>{{$t("This tool can be useful if you want to check why your bot is being banned.")}}</i></div>
        </div>

      </div>
      <div>
        <div style="padding-left:0px;padding-right:20px;padding-top:15px;">
          <div style="margin:35px 5px 20px 5px;text-align:center;width:100%">{{$t("In order to start using service you need to")}} <a href="#" onclick="BrowserAutomationStudio_OpenUrl('https://bablosoft.com/shop/FingerprintSwitcher')" >{{$t("buy a key")}}</a> {{$t("and input it here:")}}</div>
          <div class="ui fluid right action left icon focus small input error" style="margin-bottom:55px;margin-left:25px;margin-right:5px">
          <i class="key icon"></i>
          <input type="text" v-model="fpkey">
          <button class="ui primary button" @click="CheckKey()" :disabled="busy" >Login</button>
        </div>
        </div>
      </div>

      <div style="background-color:rgb(248, 248, 249);display: flex;padding-left:0px;padding-right:20px;padding-top:15px;border-bottom:1px solid rgba(34,36,38,.15);border-top:1px solid rgba(34,36,38,.15)">
        <img src="pic/question.png" style="width:32px;height:32px;margin:20px;float:left"></img>
        <div>
          <div style="margin:5px 5px 20px 5px;">{{$t("FingerprintDetector only detects if any fingerprinting techniques is used. In order to change it use")}} <a href="#" onclick="BrowserAutomationStudio_OpenUrl('https://fingerprints.bablosoft.com/')" >FingerprintSwticher</a>{{$t(" service")}}. {{$t("Both projects shares same keys. It means that after buying FingerprintDetector you will automatically get FingerprintSwticher and vice versa.")}}</div>
        </div>

      </div>


    <div>`

}