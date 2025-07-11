
  

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

    <%= _.template($('#input_constructor').html())({id:"GroupId", description:tr("Group id"), default_selector: "string", disable_int:true,help: {description: tr("Group id, which will be deleted. It can be obtained from \"Create Group\" action or from \"Get Group List\""),examples:[{code:"5b1d2284644bc0759b5dfe15"}]}}) %>

    <%= _.template($('#input_constructor').html())({id:"DeleteData", description:tr("Delete data from group"), default_selector: "string", disable_int:true, value_string: "true", variants: ["true", "false"], help: {description: tr("Choose if all rows from selected group will be deleted with group."), examples:[
  {code:"true",description:tr("Delete group with data")},
  {code:"false",description:tr("Delete group without data")},
]}
                  }) %>

  </div>

  <div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Delete group from database.</div>
    <div class="tr tooltip-paragraph-fold">Groups are analogous of files. Same as each file may contain several lines, each group may contain several records.</div>
    <div class="tr tooltip-paragraph-fold">You can select, if you want to delete group with or without data.</div>
    <div class="tooltip-paragraph-last-fold"><span class="tr">Groups are displayed and may be edited in database manager</span> (<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=grouplistlocation.png');return false">screen</a>).</div>
  </div>

  <%= _.template($('#back').html())({action:"executeandadd", visible:true, wiki: "tutorial3"}) %>
