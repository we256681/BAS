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

    <%= _.template($('#variable_constructor').html())({id:"GroupList", description:tr("Group list"), default_variable: "GROUP_LIST", help: {description: tr("After action completes, this variable will contain result as a list of strings. Each string describes one group with csv format. You can parse it with \"Parse Line\" action. First component is a group id, second is group name, and third - group description."),examples:[
  {code:"[\"5b1d2284644bc0759b5dfe15:GroupName1:GroupDescription1\", \"5b1d2284644bc0759b5dfe16:GroupName2:GroupDescription2\"]"},
]}
}) %>
  </div>

  <div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Get group list from database.</div>
    <div class="tooltip-paragraph-fold tr">This action can be used to find group id and put records inside that group.</div>
    <div class="tr tooltip-paragraph-fold">Groups are analogous of files. Same as each file may contain several lines, each group may contain several records.</div>
    <div class="tooltip-paragraph-fold"><span class="tr">After action completes, variable will contain result as a list of strings. Each string describes one group with csv format. You can parse it with "Parse Line" action. First component is a group id, second is group name, and third - group description.</span> (<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=iterateovergroups.png');return false">screen</a>).</div>
    <div class="tooltip-paragraph-fold tr">If you have created resource with type database, then you can select one or several groups to set data with which script will be working.</div>
    <div class="tooltip-paragraph-last-fold"><span class="tr">Groups are displayed and may be edited in database manager</span> (<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=grouplistlocation.png');return false">screen</a>).</div>
  </div>
  <%= _.template($('#back').html())({action:"executeandadd", visible:true, wiki: "tutorial3"}) %>
