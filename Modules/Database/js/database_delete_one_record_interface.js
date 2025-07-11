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

    <%= _.template($('#input_constructor').html())({id:"RecordId", description:tr("Record id"), default_selector: "string", disable_int:true,help: {description: tr("Record id, which will be deleted. It can be obtained from resource with type \"Database\" or from list returned from \"Select Records\" action."),examples:[{code:"5b1d2284644bc0759b5dfe15"},{code:tr("Empty string"), description:tr("Delete last record obtained as resource.")}]}}) %>

    <%= _.template($('#input_constructor').html())({id:"DeleteResources", description:tr("Delete resource"), default_selector: "string", disable_int:true, value_string: "false", variants: ["true", "false"], help: {description: tr("Select if you want to delete record from resources system besides database. If there is no resource connected to current table, this setting will do nothing."), examples:[
      {code:"true",description:tr("Delete record from resources and from database")},
      {code:"false",description:tr("Delete record only from database")},
    ]}}) %>

    

    
</div>

<div class="tooltipinternal">
  <div class="tr tooltip-paragraph-first-fold">Delete record from database.</div>
  
  <div class="tooltip-paragraph-fold tr">This action only deletes single record, if you wan't to delete more than one, use "Delete Records By Criteria" action.</div>
  <div class="tooltip-paragraph-fold tr">You can also delete record from database obtained with resource by using "Delete Current Element" action.</div>

  <div class="tooltip-paragraph-fold tr">By default this action doesn't synchronize database and resources, if you want to delete record from resources besides database, you need to check "Delete resource" field.</div>
  <div class="tooltip-paragraph-fold tr">It is synchronous, so feel free to use action in several threads.</div>
  <div class="tooltip-paragraph-last-fold tr">Records are displayed and may be edited in database manager.</div>

</div>

<%= _.template($('#back').html())({action:"executeandadd", visible:true, wiki: "tutorial3"}) %>
