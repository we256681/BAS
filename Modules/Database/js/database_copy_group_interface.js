
  

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

    <%= _.template($('#input_constructor').html())({id:"GroupId", description:tr("Group id"), default_selector: "string", disable_int:true,help: {description: tr("Group id, which will be duplicated. It can be obtained from \"Create Group\" action or from \"Get Group List\". Leave blank to copy last created group."),examples:[{code:"5b1d2284644bc0759b5dfe15"},
          {code:tr("Empty string"),description: tr("Copy group, which was created last with this script.")}]}}) %>

    <%= _.template($('#variable_constructor').html())({id:"DuplicatedGroupId", description:tr("Result group id"), default_variable: "DUPLICATED_GROUP_ID",help: {description: tr("Group id will be written to this variable after action will be completed. You can use this id to interact with this group letter, for example, delete it or change."),examples:[
  {code:"5b1d2284644bc0759b5dfe15"}]}}) %>

  </div>

  <div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Duplicate group name, description and data.</div>
    <div class="tr tooltip-paragraph-fold">Groups are analogous of files. Same as each file may contain several lines, each group may contain several records.</div>
    <div class="tr tooltip-paragraph-fold">Duplicated group will have same name and description as original, but you can change it later with "Change Group" action.</div>
    <div class="tooltip-paragraph-last-fold"><span class="tr">Groups are displayed and may be edited in database manager</span> (<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=grouplistlocation.png');return false">screen</a>).</div>
  </div>

  <%= _.template($('#back').html())({action:"executeandadd", visible:true, wiki: "tutorial3"}) %>
