<div class="container-fluid">
  <%= _.template($('#input_constructor').html())({id:"Service", description:tr("Service"), default_selector: "string", disable_int:true, value_string: "smspva.com", variants: ["smspva.com","sms-activate.ru","sms-reg.com", "getsms.online"]}) %>

  <div class="col-xs-12">
    <form class="form-horizontal">
      <div class="form-group">
        <div class="col-xs-2">
          <div class="input-group">
            <span data-preserve="true" data-preserve-type="select" data-preserve-id="Select">
              <select class="form-control input-sm" id="Select" placeholder="Site">
                <option value="*" selected="selected">*</option>
                <option value="4game">4game</option>
                <option value="AOL">AOL</option>
                <option value="Auto.RU">Auto.RU</option>
                <option value="Avito">Avito</option>
                <option value="Badoo">Badoo</option>
                <option value="Discord">Discord</option>
                <option value="dodopizza.ru">dodopizza.ru</option>
                <option value="Drom.RU">Drom.RU</option>
                <option value="Drug Vokrug">Drug Vokrug</option>
                <option value="Facebook">Facebook</option>
                <option value="Fiverr">Fiverr</option>
                <option value="Fotostrana">Fotostrana</option>
                <option value="G2A.COM">G2A.COM</option>
                <option value="GetTaxi">GetTaxi</option>
                <option value="GMail">GMail</option>
                <option value="HotMail">HotMail</option>
                <option value="ICQ">ICQ</option>
                <option value="Instagram">Instagram</option>
                <option value="LinkedIn">LinkedIn</option>
                <option value="Mail.RU">Mail.RU</option>
                <option value="mamba">mamba</option>
                <option value="MeetMe">MeetMe</option>
                <option value="Naver">Naver</option>
                <option value="OD">OD</option>
                <option value="OLX">OLX</option>
                <option value="Open I Messenger">Open I Messenger</option>
                <option value="Proton Mail">Proton Mail</option>
                <option value="Qiwi">Qiwi</option>
                <option value="SEOsprint.net">SEOsprint.net</option>
                <option value="Sipnet.ru">Sipnet.ru</option>
                <option value="Snapchat">Snapchat</option>
                <option value="Spotify">Spotify</option>
                <option value="Steam">Steam</option>
                <option value="Taxi Maksim">Taxi Maksim</option>
                <option value="Telegram">Telegram</option>
                <option value="Tinder">Tinder</option>
                <option value="Twilio">Twilio</option>
                <option value="Twitter">Twitter</option>
                <option value="Ubank.ru">Ubank.ru</option>
                <option value="Uber">Uber</option>
                <option value="Viber">Viber</option>
                <option value="VK">VK</option>
                <option value="WebMoney">WebMoney</option>
                <option value="WeChat">WeChat</option>
                <option value="Weebly">Weebly</option>
                <option value="WhatsAPP">WhatsAPP</option>
                <option value="Yahoo">Yahoo</option>
                <option value="Yandex">Yandex</option>
                <option value="Zoho">Zoho</option>

              </select>
            </span>
          </div>
        </div>
        <label class="control-label text-right tr" style="padding-top:5px !important;">Site</label>
      </div>
    </form>
  </div>

  <%= _.template($('#input_constructor').html())({id:"Country", description:tr("Country"), default_selector: "string", disable_int:true, value_string: "RU", variants:
  [
    "RU<br/><span style='color:gray'>Russian Federation (smspva.com, sms-activate.ru, sms-reg.com, getsms.online)</span>",
    "KZ<br/><span style='color:gray'>Kazakhstan (smspva.com, sms-activate.ru, sms-reg.com, getsms.online)</span>",
    "UA<br/><span style='color:gray'>Ukraine (smspva.com, sms-activate.ru, sms-reg.com, getsms.online)</span>",
    "EE<br/><span style='color:gray'>Estonia (smspva.com)</span>",
    "FR<br/><span style='color:gray'>France (smspva.com)</span>",
    "ID<br/><span style='color:gray'>Indonesia (smspva.com, sms-activate.ru)</span>",
    "IL<br/><span style='color:gray'>Israel (smspva.com, sms-activate.ru)</span>",
    "KG<br/><span style='color:gray'>Kyrgyzstan (smspva.com, sms-activate.ru)</span>",
    "LV<br/><span style='color:gray'>Latvia (smspva.com)</span>",
    "NL<br/><span style='color:gray'>Netherlands (smspva.com)</span>",
    "PY<br/><span style='color:gray'>Paraguay (smspva.com)</span>",
    "PH<br/><span style='color:gray'>Philippines (smspva.com, sms-activate.ru)</span>",
    "PL<br/><span style='color:gray'>Poland (smspva.com, getsms.online)</span>",
    "RO<br/><span style='color:gray'>Romania (smspva.com)</span>",
    "UK<br/><span style='color:gray'>United Kingdom (smspva.com, getsms.online)</span>",
    "VN<br/><span style='color:gray'>Vietnam (smspva.com, sms-activate.ru)</span>",
    "CN<br/><span style='color:gray'>China (smspva.com, sms-activate.ru)</span>",
    "US<br/><span style='color:gray'>United States (sms-activate.ru)</span>",
    "HK<br/><span style='color:gray'>Kong Kong (sms-activate.ru)</span>",
    "BY<br/><span style='color:gray'>Belarus (getsms.online)</span>",
    "DE<br/><span style='color:gray'>Germany (getsms.online)</span>",
  ]}) %>
  <%= _.template($('#input_constructor').html())({id:"Apikey", description:tr("API key"), default_selector: "string", disable_int:true}) %>
  <%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable To Save"), default_variable: "PHONE_NUMBER"}) %>
</div>
<div class="tooltipinternal">
  <div class="tr tooltip-paragraph-first-fold">Get a phone number from the specified SMS receiving service.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
