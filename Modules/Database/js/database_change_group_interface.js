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

    <%= _.template($('#input_constructor').html())({id:"GroupId", description:tr("Group id"), default_selector: "string", disable_int:true, 
      help: {
          description: tr("Group id, which will be changed. It can be obtained from \"Create Group\" action or from \"Get Group List\". Leave blank to change last created group."),examples:[
          {code:"5b1d2284644bc0759b5dfe15"},
          {code:tr("Empty string"),description: tr("Change group, which was created last with this script.")}
        ]}
}) %>
    <%= _.template($('#input_constructor').html())({id:"Name", description:tr("Name"), default_selector: "string", disable_int:true,help: {description: tr("New group name. Can be any string. Leave blank not to change the name.")}}) %>
    <%= _.template($('#input_constructor').html())({id:"Description", description:tr("Description"), default_selector: "string", disable_int:true,help: {description: tr("New group description. Can be any string. Leave blank not to change the description.")}}) %>

</div>

<div class="tooltipinternal">
  <div class="tr tooltip-paragraph-first-fold">Change group name and description.</div>
  <div class="tr tooltip-paragraph-fold">Groups are analogous of files. Same as each file may contain several lines, each group may contain several records.</div>
  <div class="tooltip-paragraph-fold"><span class="tr">Possible usecase would be to create group for parsed posts and update parse date in its description with this action.</span> (<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=dateingroupdescription.png');return false">screen</a>).</div>
  <div class="tooltip-paragraph-last-fold"><span class="tr">Groups may be created manually in database manager</span> (<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=grouplist.png');return false">screen</a>).</div>
</div>


<%= _.template($('#back').html())({action:"executeandadd", visible:true, wiki: "tutorial3"}) %>
