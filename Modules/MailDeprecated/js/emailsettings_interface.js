<div class="container-fluid emailsettings">
      <%= _.template($('#input_constructor').html())({id:"Host", description:tr("Imap Server Host"), default_selector: "string", disable_int:true, help: 
        {
          description: tr("Imap server name. Can be found by searching imap settings for your email hosting, like this: 'gmail imap settings'"), examples:
          [
            {
              code:"imap.gmail.com",description:tr("gmail.com")
            },
            {
              code:"imap.mail.yahoo.com",description:tr("yahoo.com")
            },
            {
              code:"imap-mail.outlook.com",description:tr("outlook.com")
            },
            {
              code:"imap.yandex.ru",description:tr("yandex.ru")
            },
            {
              code:"imap.mail.ru",description:tr("mail.ru")
            }
          ]
        }}) %>
      <%= _.template($('#input_constructor').html())({id:"Port", description:tr("Port"), default_selector: "int", disable_string:true, value_number: 993, help: 
        {
          description: tr("Imap server port. Almost always this value should be equal to 993. If 993 doesn't work, try to search imap settings for your email hosting, like this: 'gmail imap settings'"), examples:
          [
            {
              code:"993",description:tr("Default port for imap server")
            }
          ]
        }}) %>
      <%= _.template($('#input_constructor').html())({id:"Username", description:tr("Username. Can be blank"), default_selector: "string", disable_int:true, help: 
        {
          description: tr("Username, this is almost always the same as you email login"), examples:
          [
            {
              code:"test@gmail.com"
            },
            {
              code:"test",description:tr("Imap login may also not include domain. Check imap settings for your mail service.")
            }
          ]
        }}) %>
      <%= _.template($('#input_constructor').html())({id:"Password", description:tr("Password. Can be blank"), default_selector: "string", disable_int:true, help: 
        {
          description: tr("Password, this is almost always the same as you email password")
        }}) %>
      <div class="col-xs-12">
        <form class="form-horizontal">
          <div class="form-group">
            <div class="col-xs-2">
              <div class="input-group">
              <span data-preserve="true" data-preserve-type="select" data-preserve-id="Select">
                <select class="form-control input-sm" id="Select" placeholder="Encryption">
                    <option value="ssl" selected="selected">ssl</option>
                    <option value="none">none</option>
                </select>
              </span>
              </div>
            </div>
            <label class="control-label text-right tr" style="padding-top:5px !important;">Encryption</label>
          </div>
        </form>
      </div>
      <%= _.template($('#input_constructor').html())({id:"Inbox", description:tr("Inbox name"), default_selector: "string", disable_int:true, value_string:"INBOX", help: 
      {
        description: tr("Mailbox can contain several folders, and you can search into particular folder by changing this parameter. By default it is set to 'INBOX', this folder contains all messages. Each mail hosting provides its own default names for different folders. For example, this is default name of gmail trash folder '[Gmail]/Trash' and spam folder is '[Gmail]/Spam'. You can also create your own folders and use them in this action."), examples:
        [
          {
            code:"INBOX",description:tr("Access all messages")
          },
          {
            code:"[Gmail]/Trash",description:tr("Trash folder, only for gmail")
          },
          {
            code:"[Gmail]/Spam",description:tr("Spam folder, only for gmail")
          }
        ]
      }}) %>
    </div>
    <div class="tooltipinternal">
      <div class="tr tooltip-paragraph-first-fold">Configure imap server access required to read email.</div>
      <div class="tr tooltip-paragraph-fold">This action should be called once before any other action that works with email.</div>
      <div class="tr tooltip-paragraph-fold">You need to fill imap server, your username and password. These settings usually can be found by searching imap settings for your email hosting, like this: 'gmail imap settings'</div>
      <div class="tooltip-paragraph-fold"><span class="tr">For most of providers you also need to allow imap access in account settings. Here are</span> <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://support.google.com/mail/answer/7126229?hl=en');return false">settings</a> <span class="tr">for gmail</span>.</div>
      <div class="tooltip-paragraph-fold"><span class="tr">Gmail also requires to</span> <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://myaccount.google.com/lesssecureapps');return false">allow insecure applications</a> <span class="tr">in order to work with BAS</span>.</div>
      <div class="tr tooltip-paragraph-fold">Login and password are almost always the same as your login and password from mailbox. Login may include or may not include mail domain, check imap settings page for your email hosting.</div>
      <div class="tr tooltip-paragraph-fold">Mail.ru doesn't allow to make search in mailbox.</div>
      <div class="tr tooltip-paragraph-last-fold">Mailbox can contain several folders, and you can search into particular folder by changing 'Folder name' parameter. By default it is set to 'INBOX', this folder contains all messages. Each mail hosting provides its own default names for different folders. For example, this is default name of gmail trash folder '[Gmail]/Trash' and spam folder is '[Gmail]/Spam'. You can also create your own folders and use them in this action.</div>
  </div>
    <%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>