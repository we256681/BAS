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

    <%= _.template($('#input_constructor').html())({id:"GroupId", description:tr("Group id"), default_selector: "string", disable_int:true, help: {description: tr("Group id where record will be inserted. It can be obtained from \"Create Group\" action or from \"Get Group List\". Leave blank to insert in last created group."),examples:[{code:"5b1d2284644bc0759b5dfe15"},{code:tr("Empty string"),description: tr("Insert record in last created group.")}]}
  }) %>

    <%= _.template($('#input_constructor').html())({id:"AddToResources", description:tr("Add record to resources"), default_selector: "string", disable_int:true, value_string: "false", variants: ["true", "false"], help: {description: tr("Select if you want to add new record to resources system besides database. If there is no resource connected to current table, this setting will do nothing."), examples:[
  {code:"true",description:tr("Add record to resources and to database")},
  {code:"false",description:tr("Add record only to database")},
]}}) %>

    <% _Schema.forEach(function(t){ %>
        <% t["columns"].forEach(function(c){ %>
          <span data-table-id="<%= t["id"] %>" style="<%= (t["id"] != _Schema[0]["id"])?"display:none":"" %>" data-id="<%= c["id"] %>">
            <% if(c["type"] == "string"){ %>
              <%= _.template($('#input_constructor').html())({id:"Table_" + t["id"] + "_Column_" + c["id"], description:c["description"], default_selector: "string", disable_int:true, help: {description: tr("Field with type string.")}}) %>          
            <% } %>
            <% if(c["type"] == "int"){ %>
              <%= _.template($('#input_constructor').html())({id:"Table_" + t["id"] + "_Column_" + c["id"], description:c["description"], default_selector: "int", disable_string:true, min_number: -999999, help: {description: tr("Field with type integer.")}}) %>          
            <% } %>
            <% if(c["type"] == "bool"){ %>
              <%= _.template($('#input_constructor').html())({id:"Table_" + t["id"] + "_Column_" + c["id"], description:c["description"], size:4, default_selector: "string", disable_int:true, value_string: "true", variants: ["true","false"], help: {description: tr("Field with type boolean. Could be true or false.")}}) %>          
            <% } %>
            <% if(c["type"] == "date"){ %>
              <%= _.template($('#input_constructor').html())({id:"Table_" + t["id"] + "_Column_" + c["id"], description:c["description"], default_selector: "expression", disable_string:true, help: {description: tr("Field with type date. You can set it by filling this field with milliseconds from 1 january 1970, or leave blank to use current date."), examples:[
                {code:"1528756932119", description:tr("Exact date value")},
                {code:tr("Empty string"), description:tr("Current date")},
                {code:"Date.now()", description:tr("Current date")},
                {code:"Date.now() - 5 * 60 * 1000", description:tr("5 minutes before current date")}

                ]}}) %>          
            <% } %>
          </span>
        <% }) %>      
    <% }) %>

    <%= _.template($('#variable_constructor').html())({id:"Record", description:tr("New Record id"), default_variable: "NEW_RECORD_ID",help: {description: tr("Record id will be written to this variable after action will be completed. You can use this id to interact with this record letter, for example, delete it or change."),examples:[
  {code:"5b1d2284644bc0759b5dfe15"}]}}) %>

    
</div>

<div class="tooltipinternal">
  <div class="tr tooltip-paragraph-first-fold">Insert new record to database.</div>
  <div class="tooltip-paragraph-fold tr">By default this action doesn't synchronize database and resources, if you want to insert record to resources besides database, you need to check "Add record to resources" field.</div>
  <div class="tr tooltip-paragraph-fold">You can also insert record with "Add Element" action from resource module. In fact you can use only resources system to interact with database.</div>
  <div class="tooltip-paragraph-fold tr">This action returns id of inserted record, you can use it to manipulate record later.</div>
  <div class="tooltip-paragraph-fold tr">It is synchronous, so feel free to use action in several threads.</div>
  <div class="tooltip-paragraph-fold tr">Action contains all fields, that you added in database manager. You can fill it, all values will be delivered into database.</div>
  <div class="tooltip-paragraph-last-fold tr">Records are displayed and may be edited in database manager.</div>

</div>


<%= _.template($('#back').html())({action:"executeandadd", visible:true, wiki: "tutorial3"}) %>
