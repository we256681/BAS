
  

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

    <%= _.template($('#input_constructor').html())({id:"GroupId", description:tr("Group id"), default_selector: "string", disable_int:true,help: {description: tr("Group id, in which record will be moved. It can be obtained from \"Create Group\" action or from \"Get Group List\". Leave blank to delete record from all groups."),examples:[{code:"5b1d2284644bc0759b5dfe15"},
          {code:tr("Empty string"),description: tr("Remove record from all groups.")}]}}) %>

    <%= _.template($('#input_constructor').html())({id:"RecordId", description:tr("Record id"), default_selector: "string", disable_int:true,help: {description: tr("Record id, which will be moved to group. It can be obtained from resource with type \"Database\" or from list returned from \"Select Records\" action."),examples:[{code:"5b1d2284644bc0759b5dfe15"}]}}) %>

    <%= _.template($('#input_constructor').html())({id:"MoveType", description:tr("Record move type"), default_selector: "string", disable_int:true, value_string: "move", variants: ["move", "copy", "reference"], help: {description: tr("Choose a way, how you want to move record."), examples:[
  {code:"move",description:tr("Record will be removed from current group and will be added to a group filled in \"Group id\" field")},
  {code:"copy",description:tr("Record will be copied to a group filled in \"Group id\" field. Old copy will remain, so there will two same records after action completes.")},
  {code:"reference",description:tr("A reference to record will be created. Unlike copy, if you use reference and change record, old one will change too.")},

  ]}}) %>

    <%= _.template($('#variable_constructor').html())({id:"MovedRecordId", description:tr("Record id after move"), default_variable: "MOVED_RECORD_ID",help: {description: tr("This variable will contain id of moved record.")}
}) %>

  </div>

  <div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Move or copy action to other group.</div>
    <div class="tr tooltip-paragraph-fold">Groups are analogous of files. Same as each file may contain several lines, each group may contain several records.</div>
    <div class="tr tooltip-paragraph-fold">You can choose move type by changing field "Record move type". It gives possibility to move or copy or make a reference of existing record.</div>
    <div class="tr tooltip-paragraph-fold">By lefting group id empty you can remove record from all groups. This won't remove record from database, record will still be available in group "All". If you want to remove it completely, use "Delete Record" action.</div>
    <div class="tooltip-paragraph-last-fold"><span class="tr">Groups are displayed and may be edited in database manager</span> (<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=grouplistlocation.png');return false">screen</a>).</div>
  </div>

  <%= _.template($('#back').html())({action:"executeandadd", visible:true, wiki: "tutorial3"}) %>
