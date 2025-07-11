<div class="container-fluid emaillength">
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable To Save"), default_variable: "SAVED_MAILBOX_LENGTH", help: 
    {
      description: tr("This variable will contain integer value with email number.")
    }
  }) %>
</div>
<div class="tooltipinternal">
<div class="tr tooltip-paragraph-first-fold">Get message number inside mailbox.</div>
<div class="tr tooltip-paragraph-last-fold">In order to execute this action correctly you need to run 'Configure' action first.</div>

</div>
<%= _.template($('#back').html())({action:"executeandadd",use_timeout: true, visible:true}) %>