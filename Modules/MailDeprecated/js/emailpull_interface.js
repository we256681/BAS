<div class="container-fluid emailpull">
      <%= _.template($('#input_constructor').html())({id:"Value", description:tr("Message Id"), default_selector: "string", disable_int:true, help: 
        {
          description: tr("Mail identifier, can be obtained from 'Search First' or from 'Search All' action."), examples:
          [
            {
              code:tr("7571"),description:tr("Fetch message with id 7571")
            }
          ]
        }}) %>

      <%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable To Save"), default_variable: "MAIL_BODY", help: 
        {
          description: tr("This variable will contain mail headers and body after successful execution. You can extract headers or text parts with regular expression."), examples:
          [
            {
              code:"From: sender@gmail.com"
            },
            {
              code:"To: receiver@gmail.com"
            },
            {
              code:"Subject: Mail subject"
            },
            
            {
              code:"&nbsp;"
            },
            {
              code:"Mail body"
            }
            
          ]
        }}) %>
      <%= _.template($('#variable_constructor').html())({id:"VariablesList", description:tr("Variables To Save Links"), default_variable: "LINK1,LINK2,LINK3", append_mode:true, help: 
        {
          description: tr("Links will be automatically extracted from mail text and placed into [[LINK1]], [[LINK2]] and [[LINK3]] variables. You can rename variables or add more variables to extract more links.")
        }}) %>
    </div>
    <div class="tooltipinternal">
      <div class="tr tooltip-paragraph-first-fold">Get mail body by mail id and extract links.</div>
      <div class="tr tooltip-paragraph-fold">Mail identifier, can be obtained from 'Search First' or from 'Search All' action.</div>
      <div class="tr tooltip-paragraph-fold">This action automatically extracts links from email body and saves them to variables.</div>
      <div class="tr tooltip-paragraph-fold">To extract headers like 'SUBJECT' or 'FROM' apply regular expression to [[MAIL_BODY]] variable. For example, to extract 'FROM' header, use regular expression <strong>From\:\s+(.*)</strong></div>
      <div class="tr tooltip-paragraph-last-fold">In order to execute this action correctly you need to run 'Configure' action first.</div>
    </div>
    <%= _.template($('#back').html())({action:"executeandadd",use_timeout: true, visible:true}) %>