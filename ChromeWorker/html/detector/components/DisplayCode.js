

window.DisplayCode = {

    props: ['fpcodeloading','fpcodesuccess'],
    data: function()
    {
      return {
        line: 0,
        column: 0,
        source: "",
        filename: "",
        linecurrent: 0,
        columncurrent: 0,
        
      }
    },
    
    methods: {
      Prepare: function(fpcode, fpcurrentline, fpcurrentcolumn)
      {
        let IndexLine = fpcode.indexOf("\n")

        let FirstLine = fpcode.slice(0,IndexLine)
        this.filename = FirstLine.split(":").slice(0,-2).join(":")
        this.line = parseInt(FirstLine.split(":").splice(-2,1).join(""))
        this.column = parseInt(FirstLine.split(":").splice(-1,1).join(""))

        this.source = fpcode.slice(IndexLine + 1)

        let dist = beautifier.js(this.source)
        let sm = js_sourcemap.generator(this.source, dist);
        let smConsumer = js_sourcemap.consumer(sm);

        let line = fpcurrentline - this.line + 1
        let column = fpcurrentcolumn
        
        let locRes = smConsumer.getGenerated({ line: line, column: column })

        this.source = dist
        this.linecurrent = locRes.line + this.line - 1
        this.columncurrent = locRes.column

        /*this.linecurrent = fpcurrentline
        this.columncurrent = fpcurrentcolumn*/
      
      }
    },

    updated: function()
    {
      try
      {
        var right = $("body").prop("clientWidth") - $("pre")[0].scrollWidth - 10
        if(right > 0)
          right = 0
        $("#line-highlight2").css("right",right + "px")
        $("#line-highlight2").css("top",(100 + 20 * (this.linecurrent - this.line)) + "px")
        $("#line-highlight2")[0].scrollIntoViewIfNeeded()
      }catch(e)
      {

      }

      
      /*$("#code").attr("data-line",this.linecurrent)

      Prism.highlightAll();

      $(".line-highlight").css("right", "")

      var width = $("#code code").width() + 53
      if(width < $("body").prop("clientWidth") - 10)
        width = $("body").prop("clientWidth") - 10
      $(".line-highlight").css("width",width + "px")

      if($(".line-highlight").length > 0)
        try{
          $(".line-highlight")[0].scrollIntoViewIfNeeded()
        }catch(e)
        {

        }*/

    },
    

    template: 
    `
      <div>
        <div  style="z-index:1000;position: fixed;top: 0px;left:0px;right:0px;border-bottom:1px solid rgba(34,36,38,.15);padding:15px 10px 15px 10px;background-color:#f8f8f8;display: flex;justify-content:space-between">
          <div style="display: flex;flex-direction:column;justify-content:center;color:gray;font-size:small">
            <div v-if="!fpcodeloading && fpcodesuccess">
              <div style="overflow-wrap: break-word;max-width:calc(100vw - 50px)">{{$t("File")}} : <span >{{filename}}</span> <a href="#" @click.prevent="$emit('fpedit',source)" style="color:gray;text-decoration:none;outline: none;margin-left:5px"><i class="edit icon"></i></a></div>
              <div style="margin-top:5px">{{$t("Line")}} : {{linecurrent}}</div>
              <div style="margin-top:5px">{{$t("Column")}} : {{columncurrent}}</div>
            </div>
            <div v-if="fpcodeloading">
              {{$t("Loading source code")}}...
            </div>
          </div>
          <div style="display: flex;flex-direction:column;justify-content:center;">
            <a href="#" @click.prevent="$emit('fpclose')" style="color:black;text-decoration:none;padding:5px;outline: none;">
              <i class="close icon"></i>
            </a>
          </div>
        </div>
        <div v-if="fpcodeloading" style="position:absolute;top:45%;text-align:center;left:0px;right:0px">
          <div class="ui active inline loader"></div>
        </div>
        <div v-if="!fpcodeloading && !fpcodesuccess" style="position:absolute;top:45%;text-align:center;left:0px;right:0px;color:#de2828">
          {{$t("Loading error, this could happen if source code that you are trying to view was unloaded by browser.")}}
        </div>
        <pre id="code" style="line-height:20px;position:relative;margin: 0px;padding-top: 100px;padding-bottom: 0px;width: 100vw;overflow: auto; height: 100vh;" v-if="!fpcodeloading && fpcodesuccess" class="line-numbers" >{{source}}<div id="line-highlight2" style="background-color:#000bff;position:absolute;height:20px;opacity:0.1;left:0px;right:0px"> </div></pre>
      </div>

    `

}