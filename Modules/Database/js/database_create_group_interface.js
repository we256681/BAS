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

    <%= _.template($('#input_constructor').html())({id:"Name", description:tr("Name"), default_selector: "string", disable_int:true,help: {description: tr("Group name, which you are creating. Can be any string.")}}) %>
    <%= _.template($('#input_constructor').html())({id:"Description", description:tr("Description"), default_selector: "string", disable_int:true,help: {description: tr("Group description, which you are creating. Can be any string.")}}) %>
    <%= _.template($('#variable_constructor').html())({id:"GroupId", description:tr("Group id"), default_variable: "NEW_GROUP_ID",help: {description: tr("Group id will be written to this variable after action will be completed. You can use this id to interact with this group letter, for example, delete it or change."),examples:[
  {code:"5b1d2284644bc0759b5dfe15"}
 
]}}) %>
</div>

<div class="tooltipinternal">
  <div class="tr tooltip-paragraph-first-fold">Add new group to database.</div>
  <div class="tr tooltip-paragraph-fold">Groups are analogous of files. Same as each file may contain several lines, each group may contain several records.</div>
  <div class="tooltip-paragraph-fold tr">After this action is finished, you can use group identifier returned as variable to interact with group, for example, add records in group.</div>
  <div class="tooltip-paragraph-fold tr">If you have created resource with type database, then you can select one or several groups to set data with which script will be working.</div>
  <div class="tooltip-paragraph-fold tr">If script outputs some data like user accounts or parsed posts, a good practice would be to create group during script start(in function OnApplicationStart) and place all data inside this group. This will help to discern data for different script lauches.</div>
  <div class="tooltip-paragraph-last-fold"><span class="tr">Groups may be created manually in database manager</span> (<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=grouplist.png');return false">screen</a>).</div>
</div>


<%= _.template($('#back').html())({action:"executeandadd", visible:true, wiki: "tutorial3"}) %>
