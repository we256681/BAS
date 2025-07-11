<div class="container-fluid emailremove">
      <%= _.template($('#input_constructor').html())({id:"Value", description:tr("Message Id"), default_selector: "string", disable_int:true, help: 
        {
          description: tr("Mail identifier, can be obtained from 'Search First' or from 'Search All' action."), examples:
          [
            {
              code:tr("7571"),description:tr("Remove message with id 7571")
            }
          ]
        }}) %>
    </div>
    <div class="tooltipinternal">
      <div class="tr tooltip-paragraph-first-fold">Delete email with specified id.</div>
      <div class="tr tooltip-paragraph-fold">Mail identifier, can be obtained from 'Search First' or from 'Search All' action.</div>
      <div class="tr tooltip-paragraph-last-fold">In order to execute this action correctly you need to run 'Configure' action first.</div>
    </div>
    <%= _.template($('#back').html())({action:"executeandadd",use_timeout: true, visible:true}) %>