<div class="container-fluid">
  <div class="col-xs-12">
      <form class="form-horizontal">
        <div class="form-group">
          <div class="col-xs-12">
            <div class="input-group">
              <span data-preserve="true" data-preserve-type="select" data-preserve-id="Table">
                <select class="form-control input-sm" id="Table" placeholder="Result">
                    <% _Schema.forEach(function(t){ %>
                     <option value="<%= t.id %>"><%= t.description %></option>
                    <% }) %>
                </select>
              </span>
              <span class="input-group-addon" style="font-size: smaller;">
                <span class="tr">Database table</span>
                <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-placement="bottom" data-html="true" title="<%= tr('Table name, for which operation will be applied') %>" ></i>

              </span>
            </div>
          </div>
        </div>
      </form>
    </div>

    <%= _.template($('#input_constructor').html())({id:"RecordId", description:tr("Record id"), default_selector: "string", disable_int:true,help: {description: tr("Record id, which will updated. It can be obtained from resource with type \"Database\" or from list returned from \"Select Records\" action."),examples:[{code:"5b1d2284644bc0759b5dfe15"},{code:tr("Empty string"), description:tr("Update last record obtained as resource.")}]}}) %>

    <%= _.template($('#input_constructor').html())({id:"UpdateResources", description:tr("Update resources"), default_selector: "string", disable_int:true, value_string: "false", variants: ["true", "false"], help: {description: tr("Select if you want to update record in resources system besides database. If there is no resource connected to current table, this setting will do nothing."), examples:[
      {code:"true",description:tr("Update record in resources and in database")},
      {code:"false",description:tr("Update record only in database")},
    ]}}) %>

    <% _Schema.forEach(function(t){ %>
        <% t["columns"].forEach(function(c){ %>
          <span data-table-id="<%= t["id"] %>" style="<%= (t["id"] != _Schema[0]["id"])?"display:none":"" %>" data-id="<%= c["id"] %>">
            <% if(c["type"] == "string"){ %>
              <%= _.template($('#input_constructor').html())({id:"Table_" + t["id"] + "_Column_" + c["id"], description:c["description"], default_selector: "string", disable_int:true, help: {description: tr("Field with type string. Leave it blank to not update that field. If you want to update this field with empty string, change field type to \"expression\" and fill it with two quotes \"\"")}}) %>          
            <% } %>
            <% if(c["type"] == "int"){ %>
              <%= _.template($('#input_constructor').html())({id:"Table_" + t["id"] + "_Column_" + c["id"], description:c["description"], default_selector: "expression",value_string: "", disable_string:true, min_number: -999999, help: {description: tr("Field with type integer. Leave it blank to not update that field.")}}) %>          
            <% } %>
            <% if(c["type"] == "bool"){ %>
              <%= _.template($('#input_constructor').html())({id:"Table_" + t["id"] + "_Column_" + c["id"], description:c["description"], size:4, default_selector: "string", disable_int:true, variants: ["true","false"], help: {description: tr("Field with type boolean. Could be true or false. Leave it blank to not update that field.")}}) %>          
            <% } %>
            <% if(c["type"] == "date"){ %>
              <%= _.template($('#input_constructor').html())({id:"Table_" + t["id"] + "_Column_" + c["id"], description:c["description"], default_selector: "expression", disable_string:true, help: {description: tr("Field with type date. Leave it blank to not update that field. You can set it by filling this field with milliseconds from 1 january 1970"), examples:[
                {code:"1528756932119", description:tr("Exact date value")},
                {code:"Date.now()", description:tr("Current date")},
                {code:"Date.now() - 5 * 60 * 1000", description:tr("5 minutes before current date")}

                ]}}) %>          
            <% } %>
          </span>
        <% }) %>      
    <% }) %>


    
</div>

<div class="tooltipinternal">
  <div class="tr tooltip-paragraph-first-fold">Update record from database.</div>
  
  <div class="tooltip-paragraph-fold tr">If you want to update only certain fields, leave other fields blank. Those that will be blank won't be updated. If you want to update field of string type with empty string, change field type to "expression" and fill it with two quotes "".</div>

  <div class="tooltip-paragraph-fold tr">This action only updates single record, if you wan't to update several fields, use "Update Records By Criteria" action.</div>

  <div class="tooltip-paragraph-fold tr">By default this action doesn't synchronize database and resources, if you want to update record in resources besides database, you need to check "Update resources" field.</div>
  <div class="tooltip-paragraph-fold tr">This action doesn't change id of updated record.</div>
  <div class="tooltip-paragraph-fold tr">It is synchronous, so feel free to use action in several threads.</div>
  <div class="tooltip-paragraph-fold tr">Action contains all fields, that you added in database manager. You can fill it, all values will be delivered into database.</div>
  <div class="tooltip-paragraph-last-fold tr">Records are displayed and may be edited in database manager.</div>

</div>

<%= _.template($('#back').html())({action:"executeandadd", visible:true, wiki: "tutorial3"}) %>
