<div class="container-fluid custom">
      <span data-preserve="true" data-preserve-type="constr" data-preserve-id="Code">
        <div id="Code" data-is-code-editor="true" data-force-string="true" style="height:calc(100vh - 100px);border: none; overflow: hidden; outline: none;border:1px solid #e0e0e0;" />
      </span>
      

      <%= "<s" + "cript>" %>
        window.Code = {Editor: null, InitialValue: ""}

        require(['vs/editor/editor.main'], function() {
          window.Code.Editor = createCodeEditor(document.getElementById('Code'), {
            value: window.Code.InitialValue
          });
        });
      <%= "</" + "script>" %>
      <div style="text-align:center">
        <a tabindex="-1" href="#" data-toggle="modal" data-result-target="#Code" class="var" style="margin-right:15px">
          <i class="fa fa-at"></i>
          <span class="tr">Insert variable</span>
        </a>
        <a tabindex="-1" href="#" data-toggle="modal" data-result-target="#Code" class="res">
          <i class="fa fa-database"></i>
            <span class="tr">Load from file, user input, database</span>
        </a>
      </div>
      <span class="short-description tr">Run javascript code in web interface context.</span>
    </div>



    <%= _.template($('#back').html())({action:"executeandadd", visible:true, modal_code_id:"custom-help-interface-" + (_K || "en")}) %> 