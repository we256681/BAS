<div class="container-fluid emailsearchfirst">
<%= _.template($('#input_constructor').html())({id:"Sender", description:tr("Sender Query"), default_selector: "string", disable_int:true
  , help: 
  {
    description: tr("Search by 'from' field(mail sender). Put here part of sender address to filter by mail sender. Leave blank not to filter by sender."), examples:
    [
      {
        code:tr("Empty string"),description:tr("Don't filter by sender.")
      },
      {
        code:"@twitter.com",description:tr("Search for message from mail with twitter domain")
      },
      {
        code:"info@twitter.com",description:tr("Search for email sent from info@twitter.com")
      }
    ]
  }
  })
%>
<%= _.template($('#input_constructor').html())({id:"To", description:tr("Recipient"), default_selector: "string", disable_int:true, help: 
  {
    description: tr("Search by 'to' field(mail recepient). Put here part of recepient address to filter by mail receiver. Leave blank not to filter by recepient. This parameter is usefull if you collect mail from several mailboxes."), examples:
    [
      {
        code:tr("Empty string"),description:tr("Don't filter by recepient.")
      },
      {
        code:"test@yourdomain.com",description:tr("Search for message which is sent to test@yourdomain.com mailbox")
      }
    ]
  }
  }) %>
<%= _.template($('#input_constructor').html())({id:"Subject", description:tr("Subject Query"), default_selector: "string", disable_int:true, help: 
  {
    description: tr("Search email by subject. This could be combined with other queries."), examples:
    [
      {
        code:tr("Empty string"),description:tr("Don't filter by mail subject")
      },
      {
        code:tr("Subject string part"),description:tr("Search message with subject that contains specified string")
      }
    ]
  }
  }) %>
<%= _.template($('#input_constructor').html())({id:"Body", description:tr("Body Query"), default_selector: "string", disable_int:true, help: 
  {
    description: tr("Search email by message text. This could be combined with other queries."), examples:
    [
      {
        code:tr("Empty string"),description:tr("Don't filter by mail body")
      },
      {
        code:tr("Body string part"),description:tr("Search message with text that contains specified string")
      }
    ]
  }}) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Mail text"), default_variable: "MAIL_BODY", help: 
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
  }
  }) %>
<%= _.template($('#variable_constructor').html())({id:"MailId", description:tr("Id of found message"), default_variable: "MAIL_ID", help: 
  {
    description: tr("Found mail identifier, can be used to delete message later with action 'Remove Message'."), examples:
    [
      {
        code:tr("7571"),description:tr("Found message has id 7571")
      }
    ]
  }}) %>
<%= _.template($('#variable_constructor').html())({id:"VariablesList", description:tr("Variables To Save Links"), default_variable: "LINK1,LINK2,LINK3", append_mode:true, help: 
  {
    description: tr("Links will be automatically extracted from mail text and placed into [[LINK1]], [[LINK2]] and [[LINK3]] variables. You can rename variables or add more variables to extract more links.")
  }}) %>
</div>

<div class="tooltipinternal">
<div class="tr tooltip-paragraph-first-fold">Search for first email matching all criterias, save mail body and headers to specified variable.</div>
<div class="tr tooltip-paragraph-fold">If you run this action without queries, then first mail will be fetched.</div>
<div class="tr tooltip-paragraph-fold">This action automatically extracts links from email body and saves them to variables.</div>
<div class="tr tooltip-paragraph-fold">If you want to select several messages, use 'Search All' action.</div>
<div class="tr tooltip-paragraph-fold">To extract headers like 'SUBJECT' or 'FROM' apply regular expression to [[MAIL_BODY]] variable. For example, to extract 'FROM' header, use regular expression <strong>From\:\s+(.*)</strong></div>
<div class="tr tooltip-paragraph-last-fold">In order to execute this action correctly you need to run 'Configure' action first.</div>
</div>

<%= _.template($('#back').html())({action:"executeandadd",use_timeout: true, visible:true}) %>