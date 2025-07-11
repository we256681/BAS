<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({id:"Host", description:tr("SMTP Server Host"), default_selector: "string", disable_int:true}) %>
	<%= _.template($('#input_constructor').html())({id:"Port", description:tr("Port"), default_selector: "int", disable_string:true, value_number: 465, variants: ["465","587","25"]}) %>
	<%= _.template($('#input_constructor').html())({id:"Username", description:tr("Username. Can be blank"), default_selector: "string", disable_int:true}) %>
	<%= _.template($('#input_constructor').html())({id:"Password", description:tr("Password. Can be blank"), default_selector: "string", disable_int:true}) %>
	<div class="col-xs-12">
    <form class="form-horizontal">
      <div class="form-group">
        <div class="col-xs-2">
          <div class="input-group">
            <span data-preserve="true" data-preserve-type="select" data-preserve-id="Select">
              <select class="form-control input-sm" id="Select" placeholder="Encryption">
                <option value="ssl" selected="selected">ssl</option>
					      <option value="tls">tls</option>
                <option value="none">none</option>
              </select>
            </span>
          </div>
        </div>
        <label class="control-label text-right tr" style="padding-top:5px !important;">Encryption</label>
      </div>
    </form>
  </div>
</div>
<div class="tooltipinternal">
   <div class="tr tooltip-paragraph-first-fold">Configure SMTP server access required to send email.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>