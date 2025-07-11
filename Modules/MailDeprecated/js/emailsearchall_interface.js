<div class="container-fluid emailsearchall">
      <%= _.template($('#input_constructor').html())({id:"Sender", description:tr("Sender Query"), default_selector: "string", disable_int:true, help: 
        {
          description: tr("Search by 'from' field(mail sender). Put here part of sender address to filter by mail sender. Leave blank not to filter by sender."), examples:
          [
            {
              code:tr("Empty string"),description:tr("Don't filter by sender.")
            },
            {
              code:"@twitter.com",description:tr("Search for all messages from mail with twitter domain")
            },
            {
              code:"info@twitter.com",description:tr("Search for all mails sent from info@twitter.com")
            }
          ]
        }}) %>
      <%= _.template($('#input_constructor').html())({id:"To", description:tr("Recipient"), default_selector: "string", disable_int:true, help: 
        {
          description: tr("Search by 'to' field(mail recepient). Put here part of recepient address to filter by mail receiver. Leave blank not to filter by recepient. This parameter is usefull if you collect mail from several mailboxes."), examples:
          [
            {
              code:tr("Empty string"),description:tr("Don't filter by recepient.")
            },
            {
              code:"test@yourdomain.com",description:tr("Search for all messages which is sent to test@yourdomain.com mailbox")
            }
          ]
        }}) %>
      <%= _.template($('#input_constructor').html())({id:"Subject", description:tr("Subject Query"), default_selector: "string", disable_int:true, help: 
        {
          description: tr("Search email by subject. This could be combined with other queries."), examples:
          [
            {
              code:tr("Empty string"),description:tr("Don't filter by mail subject")
            },
            {
              code:tr("Subject string part"),description:tr("Search all messages with subject that contains specified string")
            }
          ]
        }}) %>
      <%= _.template($('#input_constructor').html())({id:"Body", description:tr("Body Query"), default_selector: "string", disable_int:true, help: 
        {
          description: tr("Search email by message text. This could be combined with other queries."), examples:
          [
            {
              code:tr("Empty string"),description:tr("Don't filter by mail body")
            },
            {
              code:tr("Body string part"),description:tr("Search all messages with text that contains specified string")
            }
          ]
        }}) %>
      <%= _.template($('#variable_constructor').html())({id:"Save", description:tr("List with found mail ids"), default_variable: "MAIBOX_SEARCH_RESULT_LIST", help: 
{
  description: tr("This variable will contain list with mail identifiers. Mail identifiers is not the same as mail text or mail headers, but can be used to obtain them with 'Get message' action. Mail identifiers can also be used to delete messages with 'Remove Message' action. Use 'Foreach' to iterate over them."), examples:
  [
    {
      code:"[]",description:tr("No messages found.")
    },
    {
      code:"[\"1\", \"2\", \"3\"]",description:tr("Found 3 messages.")
    }
  ]
}
}) %>
   </div>
   <div class="tooltipinternal">
      <div class="tr tooltip-paragraph-first-fold">Search for all emails matching criterias, save mail ids to variable with list type.</div>
      <div class="tr tooltip-paragraph-fold">If several queries will be set, this action will return only mails matching all criterias.</div>
      <div class="tr tooltip-paragraph-fold">If you run this action without queries, then all mails in mailbox will be fetched.</div>
      <div class="tr tooltip-paragraph-fold">If you want to select first message, use 'Search First' action.</div>
      <div class="tooltip-paragraph-fold"><span class="tr">This action returns variable with type list, use 'List' module to manipulate it, or 'Foreach' action to iterate over mail ids</span> <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('http://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=fetchmails.png');return false">screen</a>. </div>
      <div class="tr tooltip-paragraph-last-fold">In order to execute this action correctly you need to run 'Configure' action first.</div>
    </div>
    <%= _.template($('#back').html())({action:"executeandadd",use_timeout: true, visible:true}) %>