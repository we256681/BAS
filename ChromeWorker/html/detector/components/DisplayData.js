window.DisplayData = {

    props: ['fpdata'],

    data: function()
    {
      return {
        busy: false,
        selectedBrowserEvent: {},
        selectedFingerprintEvent: {items:[], id:null},
        shownList: [],
        fpDataOld: [],
        score: -1,
        dataprepared: [],
        showDescription: true,
        showItem: null
      }
    },

    watch:
    {
      fpdata: function(fpdata)
      {
        this.UpdateAll(fpdata)
      }

    },

    methods:
    {
      //
      //Helpers
      //

      //Must be used every time the data has changed
      UpdateAll: function(FPData)
      {

        //Prepare data(format it in convinient way).
        this.dataprepared = this.PrepareData(FPData.slice(0))

        //Update shown list: find difference and add to shown list
        var delta = this.dataprepared.filter(x => this.fpDataOld.find(y => y._id == x._id) == null)
        var self = this
        delta.forEach(function(d){
          self.shownList.push(d["_id"])
        })
        this.fpDataOld = this.dataprepared.map(function(x){return {_id: x._id}})

        
        //Prepare score

        //Get all levels
        let levels = this.dataprepared.reduce(
          (all,x) => {
            let levels = x.fingerprints.map(y => y.level);
            return all.concat(levels)
        }, [])
        levels = [...new Set(levels)]
        levels = levels.sort().reverse().slice(0,5)

        var scoreOld = this.score
        this.score = ((levels[0] || 0) * 1.0 + (levels[1] || 0) * 0.15 + (levels[2] || 0) * 0.1 + (levels[3] || 0) * 0.05 + (levels[4] || 0) * 0.05) * 10
        this.score = Math.floor(this.score)
        if(this.score > 99)
          this.score = 99
        if(scoreOld != this.score)
          this.UpdateBar();
      },

      //Convert data to convinient format
      PrepareData: function(FPData)
      {
        //Sort by time
        var Ret = FPData

        //Group
        var RetGrouped = []
        var CurrentItemBrowser = null
        for(var i = 0; i<Ret.length; i++)
        {
          var Item = Ret[i]
          if(Item["type"] == "browser")
          {
            
            CurrentItemBrowser = Item
            CurrentItemBrowser["fingerprints"] = []
            RetGrouped.push(Item)
          }else if(CurrentItemBrowser)
          {
            var Fingerprints = CurrentItemBrowser["fingerprints"]
            var MatchingFingerprint = Fingerprints.find(x => x["id"] == Item["id"] && x["method"] == Item["method"])
            if(MatchingFingerprint)
              MatchingFingerprint["items"].push(Item)
            else
              Fingerprints.push({id: Item["id"],method: Item["method"], items: [Item], level:Item.level, tag: Item.tag})
          }
          
        }

        //Correct fonts
        RetGrouped.forEach(b => {
            b.fingerprints.forEach(f => {
              if(f.id == "Fonts")
              {
                let res = []
                f.items.forEach(i => {
                  
                  if(i.params.name)
                  {
                    if(res.findIndex(i1 => i1.params.name == i.params.name) < 0)
                      res.push(i)
                  }
              
                })
                f.items = res
                
                if(res.length > 100)
                  f.level = 7
                else if(res.length > 55)
                  f.level = 5
              }

          })
          b["fingerprints"] = b["fingerprints"].sort(function(x, y){
            var res = - x["level"] + y["level"]
            if(res == 0 && x["tag"] != y["tag"])
            {
                return x["tag"].localeCompare(y["tag"])
            }

            return res;
          })
            

        })

        RetGrouped = RetGrouped.sort(function(x, y)
          {
            var res = - x['timestamp'] + y['timestamp']
            if(res == 0 && x["type"] != y["type"])
            {
              if(x["type"] == "fingerprint")
                return -1
              if(y["type"] == "fingerprint")
                return 1
            }

            return res;
          })

        /*if(RetGrouped.length > 0)
        {
          var TotalItemsLast = 0
          var LastGroup = RetGrouped[0]
          LastGroup.fingerprints.forEach(x => {
            if(x.items)
            {
              let ToAdd = 0

              x.items.forEach(y => {
                if(y.id != "Fonts")
                {
                  ToAdd++
                }
              })
              TotalItemsLast += ToAdd
            }

          })
          if(TotalItemsLast > 300)
          {
            this.$emit('fpstop')
          }

        }*/



        return RetGrouped
      },

      //Create bar, if not created and update it with current score value
      UpdateBar: function()
      {
        if(typeof(bar) == "undefined")
        {
          bar = new ProgressBar.SemiCircle(containerbar, {
            strokeWidth: 8,
            color: '#FFEA82',
            trailColor: '#eee',
            trailWidth: 5,
            easing: 'easeInOut',
            duration: 1400,
            svgStyle: null,
            text: {
              value: '',
              alignToBottom: false
            },
            from: {color: '#21ba45'},
            to: {color: '#f2711c'},
            step: (state, bar) => {
              bar.path.setAttribute('stroke', this.GetColor(bar.value()));
              var value = Math.round(bar.value() * 100);
              bar.setText(value + " %");

              bar.text.style.color = this.GetColor(bar.value());
            }
          });
          bar.text.style.fontFamily = 'Helvetica';

          bar.text.style.fontSize = '0.95rem';
          bar.text.style.top = '-5px';
          bar.text.style.fontWeight = 'bold';

        }
        bar.animate(this.score / 100.0);
      },

      //
      //Descriptions
      //
      GetName: function(Item)
      {
          return this.$t(Descriptions.GetName(Item))
      },

      GetDescription: function(Item)
      {
          return this.$t(Descriptions.GetDescription(Item))
      },
      GetApiUrl: function(Item)
      {
          return Descriptions.GetApiUrl(Item)
      },
      GetParamsInfo: function(Item, IsShort)
      {
          var self = this
          return Descriptions.GetParamsInfo(Item, IsShort, function(x){return self.$t(x)})
      },


      //
      //Events
      //

      //Show call stack
      ShowCallStack(Stack)
      {
        $('#ModalFingerprint').modal({duration: 0}).modal('hide');

        this.$emit('fpstack', 
          Stack.split(":").slice(0,-2).join(":"), 
          parseInt(Stack.split(":").splice(-2,1).join("")),
          parseInt(Stack.split(":").splice(-1,1).join(""))
        )

      },

      //Show details for browser data
      ShowBrowserDetails: function(item)
      {
        this.selectedBrowserEvent = item
        $('#ModalBrowser').modal({
          centered: false,
          inverted: true,
          allowMultiple: true
        }).modal('show');
      },

      //Show details for fingerprint data
      ShowFingerprintDetails: function(item)
      {
        this.showItem = null
        this.selectedFingerprintEvent = {
          id: item.id, 
          description: this.GetDescription(item), 
          apiurl: this.GetApiUrl(item),
          method: item.method,
          items: item.items.slice(0),
          level: item.level


        }
        if(item.items.length == 1)
          this.showItem = item.items[0]
        $('#ModalFingerprint').modal({
          centered: false,
          inverted: true,
          allowMultiple: true
        }).modal('show');
      },
      
      //Get color for top widget
      GetColor: function(value){
        var hue=((1-value)*120).toString(10);
        return ["hsl(",hue,",90%,40%)"].join("");
      },

      //Get color for item
      GetItemColor: function(value){
        var hue=((1-value)*120).toString(10);
        return ["hsl(",hue,",60%,50%)"].join("");
      },

      //Show menu sidebar
      ShowMenu: function()
      {
        $('.ui.sidebar')
          .sidebar('setting', 'transition', 'overlay')
          .sidebar('toggle');
      },

      //Show all fingerprint activities
      ShowAll: function()
      {
        this.shownList = this.fpdata.filter(x => x.type == "browser").map(x => x._id)
        $('.ui.sidebar')
          .sidebar('toggle');
      },

      //Hide all fingerprint activities
      HideAll: function()
      {
        this.shownList = []
        $('.ui.sidebar')
          .sidebar('toggle');
      },
      
      //Clear all fingerprint activities
      ClearAll: function()
      {
        this.$emit('fpclearall')
        $('.ui.sidebar')
          .sidebar('toggle');
      },

      //Return to first page, don't delete data
      Logout: function()
      {
        this.selectedFingerprintEvent.id = null
        this.$emit('fplogout')
        $('.ui.sidebar')
          .sidebar('toggle');
      },

      //This method is called when moved to current page
      BeforeShow: function()
      {
        if(this.selectedFingerprintEvent.id)
          $('#ModalFingerprint').modal({
            duration: 0,
            centered: false,
            inverted: true,
            allowMultiple: true
          }).modal('show');
        
      },
    },

    updated: function()
    {
    },

    mounted: function()
    {
      this.UpdateAll(this.fpdata);
    },

    template: `<div>
      <div style="position:fixed;top: 0px;left: 0px;right: 0px;bottom: auto;background-color:white;z-index:100;">
        <div style="padding:5px 10px 5px 10px;display: flex;justify-content:space-between;border-bottom:1px solid rgba(34,36,38,.15);">
          
            <div style="display: flex;">
              <div style="display: flex;flex-direction:column;justify-content:center;">
                <div id="containerbar" style="width: 75px;height: 40px;"></div>
              </div>
              <div style="overflow: hidden;white-space: nowrap;display: flex;flex-direction:column;justify-content:center;margin-left:10px" :data-tooltip='$t("0% - no fingerprinting activities. 100% - site definitely fingerprints browser.")' data-position="bottom left">
                {{$t("Overall fingerprinting score.")}}
              </div>
            </div>
            
            <div style="display: flex;flex-direction:column;justify-content:center;">
                <a href="#" @click.prevent="ShowMenu()" style="text-decoration:none;padding:10px 1px 10px 10px;outline: none;color:black;font-size:20px">
                  <i class="angle down icon"></i>
                </a>
            </div>
          
        </div>
      </div>
      <div style="padding-top:50px">
        <template v-if="dataprepared.length > 0">
          <template v-for="br in dataprepared" :key="br._id">

            <template>
              <div style="border-bottom:1px solid rgba(34,36,38,.15);padding:15px 10px 15px 10px;background-color:#f8f8f8;display: flex;max-width: 100vw;justify-content:space-between">
                <div style="display: flex;">
                  <div style="display: flex;flex-direction:column;justify-content:center;">
                    <img v-if="!br.is_image" src="pic/browser.png" style="width:32px;height:32px;padding:6px;margin-right:5px;filter: grayscale(0%);"/>
                    <img v-else :src="'data:image/png;base64,' + br.image" style = "max-width: 100px;max-height: 40px;border-radius: 4px;margin-right:10px;margin-left:6px;" />
                  </div>
                  <div style="display: flex;flex-direction:column;justify-content:center;">
                    <div>
                      <div><h4 class="ui header">{{br["name"]}}</h4></div>
                      <div v-if="!br.is_image" style="font-size:small;color:gray">{{br["description"]}}</div>
                    </div>
                  </div>
                </div>
                <div style="display: flex;flex-direction:column;justify-content:center;">
                  <a href="#" @click.prevent="ShowBrowserDetails(br)" style="color:black;text-decoration:none;padding:5px;outline: none;">
                    <i class="ellipsis vertical icon"></i>
                  </a>
                </div>
              </div>
              <template v-if="shownList.indexOf(br._id) >= 0">
                <template v-for="(fr, index) in br.fingerprints" :key="fr._id">
                  <div class="fp-item" style="display: flex;max-width: 100vw;justify-content:space-between">
                    
                    <div>
                      <div 
                        class="ui horizontal label" 
                        :style="{'background-color':GetItemColor((fr.level + 1)/10.0),'color': 'white'}" 
                        :data-tooltip='$t("1 - not necessarily used for fingerprinting. 10 - definitely used for fingerprinting.")' 
                        data-position="bottom left"
                        >{{fr.level + 1}}</div>
                        
                        <span :style="{'color':GetItemColor((fr.level + 1)/10.0),'font-weight':'900', 'font-size': '16px'}">&laquo;</span> 
                        <span style="font-style:italic;font-weight: bold"> 
                          {{fr.tag}} 
                        </span> 
                        <span v-if="fr.items.length > 1" style="font-size:small;color:gray"> * {{fr.items.length}} </span> 
                        <span :style="{'color':GetItemColor((fr.level + 1)/10.0),'margin-right':'5px','font-weight':'900', 'font-size': '16px'}">&raquo;</span> 

                        </span><span v-html="GetName(fr)"></span>
                    </div>
                    <div style="display: flex;flex-direction:column;justify-content:center;">
                      <a href="#" @click.prevent="ShowFingerprintDetails(fr)" style="color:black;text-decoration:none;padding:0px 5px 0px 5px;outline: none;">
                        <i class="ellipsis vertical icon"></i>
                      </a>
                    </div>
                  </div>
                </template>
              </template>
              <template v-else>
                  <div style="padding:10px;border-bottom:1px solid rgba(34,36,38,.15);font-size:small" v-if="br.fingerprints.length > 0">
                    {{br.fingerprints.length}} {{$t("fingerprint activities hidden")}}, <a herf="#" @click="shownList.push(br._id)" style="cursor:pointer">{{$t("show")}}</a>.
                  </div>
              </template>
            </template>
            

          </template>
        </template>
        <template v-else>
          <div style="padding:20px">
            <div class="ui message">
              <div class="header">
                {{$t("Nothing to display")}}
              </div>
              <p>{{$t("No fingerprint activity detected. Try to load any page.")}}</p>
            </div>
          </div>
          
        </template>
      </div>

      <!-- Menu -->
      <div class="ui sidebar right inverted vertical menu">
        <a class="item" @click="ShowAll()">
          {{$t("Show all")}}
          <i class="eye icon large" style="font-size: 1.1em;"></i>
        </a>
        <a class="item" @click="HideAll()">
          {{$t("Hide all")}}
          <i class="eye slash outline icon large" style="font-size: 1.1em;"></i>
        </a>
        <a class="item"  @click="ClearAll()">
          {{$t("Clear")}}
          <i class="eraser icon large" style="font-size: 1.1em;"></i>
        </a>
        <a class="item"  @click="Logout()">
          {{$t("Logout")}}
          <i class="power off icon large" style="font-size: 1.1em;"></i>
        </a>
      </div>

      <!-- Modal browser event details -->
      <div class="ui modal" id="ModalBrowser">
        
        <div class="ui segments">
          <div class="ui segment" :data-tooltip='$t("Action name and description")' data-position="bottom center" style="background-color:#f8f8f8;">
              <div><strong>{{selectedBrowserEvent.name}}<strong></div>
              <div v-if="!selectedBrowserEvent.is_image" style="font-size:small;color:gray;margin-top:5px">{{selectedBrowserEvent.description}}</div>
              <div v-else style="font-size:small;color:gray;margin-top:5px">{{$t("Element is selected by image:")}}</div>
          </div>
          <div class="ui segment" v-if="selectedBrowserEvent.is_image">
            <img class="ui image centered" :src="'data:image/png;base64,' + selectedBrowserEvent.image" style = "border-radius: 4px" />
          </div>
          <div class="ui segment" v-if="selectedBrowserEvent.comment">
            <div :data-tooltip='$t("Action comment.")' data-position="bottom center"><span style="font-size:small">{{$t("Comment:")}}</span> <span style="color:#337ab7" v-html="selectedBrowserEvent.comment"></span></div>
          </div>
          
          <div class="ui segment">
            <div style="font-size:small;" :data-tooltip='$t("Action timestamp.")' data-position="bottom center">{{moment(selectedBrowserEvent.timestamp).format('Do MMMM, h:mm')}}</div>
          </div>
          <div class="ui segment">
            <div style="font-size:small;" :data-tooltip='$t("Unique action identifier. Use id to find action in scenario tab.")' data-position="bottom center">{{$t("Action id")}} #{{selectedBrowserEvent.id}}</div>
          </div>
          <div class="ui segment" v-if="selectedBrowserEvent.fingerprints && selectedBrowserEvent.fingerprints.length > 0">
            <div class="ui form">
              <div class="ui toggle checkbox" >
                <input type="checkbox" name="hide" id="hide" tabindex="0" :value="selectedBrowserEvent._id" v-model="shownList">
                <label for="hide" style="cursor:pointer">{{$t("Show")}} {{selectedBrowserEvent.fingerprints.length}} {{$t("fingerprint activities")}}</label>
              </div>
            </div>
          </div>
        </div>
      </div>

      <!-- Modal fingerprint event details -->
      <div class="ui modal" id="ModalFingerprint">
        
        <div class="ui segments" style="max-height: calc(90vh);overflow:auto" >
          <div class="ui segment" style="background-color:#f8f8f8;position:relative;top:0px;display: flex;justify-content:space-between;">
              <div><strong>{{selectedFingerprintEvent.id}}<strong></div>
              <div>
                <a href="#" @click.prevent="showDescription = !showDescription" style="text-decoration:none;outline: none;color:black;">
                  <i :class="{angle: true, down: showDescription, left: !showDescription, icon:true}"></i>
                </a>
              </div>
          </div>
          <div class="ui segment" v-if="showDescription">
            <div>
              <img src="pic/question.png" style="float: left;margin-right: 7px;margin-top: 2px;width: 24px;"/>
              {{selectedFingerprintEvent.description}}
            </div>
          </div>

          <div class="ui segment" v-if="selectedFingerprintEvent.apiurl">
            <div>
              <a href="#" @click.prevent="BrowserAutomationStudio_OpenUrl(selectedFingerprintEvent.apiurl)" >{{selectedFingerprintEvent.method}}</a> - {{$t("used method")}}
            </div>
          </div>

          <div class="ui segment">
            <div
                :data-tooltip='$t("1 - not necessarily used for fingerprinting. 10 - definitely used for fingerprinting.")' 
                data-position="bottom center"
                >
              <div 
                class="ui horizontal label" 
                :style="{'margin-right': '2px','background-color':GetItemColor((selectedFingerprintEvent.level + 1)/10.0),'color': 'white'}" 
                
                >{{selectedFingerprintEvent.level + 1}}</div>
                - {{$t("danger level")}}
              
            </div>
          </div>
          <template v-if="showItem == null">

            <div class="ui segment">
              <div>
                {{$t("This technique is used")}} <span class="significant">{{selectedFingerprintEvent.items.length}}</span> {{$t("times")}}:
              </div>
            </div>

            <div class="ui segment" style="padding-top: 8px;padding-bottom: 8px;" v-for="item in selectedFingerprintEvent.items">
              <div style="margin-left:30px;display:flex;">
                <img src="pic/fingerprint.png" style="margin-right:5px" /> 
                <div style="display: flex;flex-direction:column;justify-content:center;">
                  <div>
                    <span v-if="GetParamsInfo(item, true)">
                      <span style="color:gray">
                        {{GetParamsInfo(item, true)}},
                      </span>  
                    </span>
                    <span v-else>
                      At
                      <span style="color:gray">
                        {{moment(item.timestamp).format('Do MMMM, h:mm')}},
                      </span>
                    </span>
                    
                    <a href="#" @click.prevent="showItem = item">{{$t("Inspect")}}</a>
                  </div>
                </div>
              </div>
            </div>
          </template>
          <template v-else>
   
            <div class="ui segment">
              <div style="display:flex;">
                <img src="pic/fingerprint.png" style="margin-right:5px" /> 

                <div style="display: flex;flex-direction:column;justify-content:center;">
                  <div>
                    {{$t("This technique is used at")}} <span style="color:gray">{{moment(showItem.timestamp).format('Do MMMM, h:mm')}}</span>
                    <template v-if="selectedFingerprintEvent.items.length > 1">
                      , <a href="#" @click.prevent="showItem = null" >{{$t("Show all")}}</a>
                    </template>
                  </div>
                </div>
              </div>
            </div>
            <div class="ui segment" v-if="GetParamsInfo(showItem, false)">
              <span v-html="GetParamsInfo(showItem, false)"></span>
            </div>
           <template v-if="showItem.stackprepared && Array.isArray(showItem.stackprepared)">
              <div class="ui segment">
                {{$t("Source code location:")}}
              </div>
              <div class="ui segment" v-for="stack in showItem.stackprepared" style="padding-top:10px;padding-bottom:10px">
                <div style="display: flex;">
                  <div style="margin-left:10px;margin-right:5px;display: flex;flex-direction:column;justify-content:center;">
                     <a href="#" @click.prevent="ShowCallStack(stack)">
                      <i class="search icon"></i>
                     </a>

                  </div>
                  <div style="word-break: break-all;font-size:small;color:gray">
                    <div>File : {{stack.split(":").slice(0,-2).join(":")}}</div>
                    <div style="margin-top:5px">Line : {{stack.split(":").splice(-2,1).join("")}}</div>
                    <div style="margin-top:5px">Column : {{stack.split(":").splice(-1,1).join("")}}</div>
                  </div>
                </div>
              </div>
           </template>
           <template v-else>
              <div class="ui segment">
                {{$t("No information about source code location.")}}
              </div>
           </template> 
          </template>
          
        </div>
      </div>


    <div>`
      
    

}