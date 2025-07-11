<span id="action-add">
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
      
      <div class="container-fluid" id="database-filters" style="margin-top:10px">
        <div class="col-xs-12">
          <button type="button" class="btn btn-default btn-sm" id="database-filters-add"><i class="fa fa-filter" aria-hidden="true"></i> <span class="tr">Add filter</span></button>
          <button type="button" class="btn btn-default btn-sm" id="database-filters-clear"><i class="fa fa-eraser" aria-hidden="true"></i> <span class="tr">Remove all filters</span></button>
          
        </div>
      </div>
      <div class="container-fluid" id="database-filters" style="margin-bottom:5px" >
        <div class="col-xs-12">
      
          <div id="database-filters-data">
          </div>
        </div>
      </div>

  <%= _.template($('#input_constructor').html())({id:"GroupId", description:tr("Filter by group"), default_selector: "string", disable_int:true,help: {description: tr("Additional filter by group id. It can be obtained from \"Create Group\" action or from \"Get Group List\". Leave blank not to apply filter by group."),examples:[{code:"5b1d2284644bc0759b5dfe15",description: tr("Action will be applied only to exact group and which matches other filters.")},
          {code:tr("Empty string"),description: tr("Action will be applied to records which matches other filters.")}]}}) %>

      <%= _.template($('#input_constructor').html())({id:"DeleteResources", description:tr("Delete resource"), default_selector: "string", disable_int:true, value_string: "false", variants: ["true", "false"], help: {description: tr("Select if you want to delete records from resources system besides database. If there is no resource connected to current table, this setting will do nothing."), examples:[
      {code:"true",description:tr("Delete records from resources and from database")},
      {code:"false",description:tr("Delete records only from database")},
    ]}}) %>

</div>

  <div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Delete records from database.</div>

    <div class="tooltip-paragraph-fold tr">This action deletes records by criteria, if you want to delete single record, use "Delete One Record" action.</div>
   
    <div class="tooltip-paragraph-fold tr">This action affects all records matching criteria, be carefull, if you forget to set criteria, all records in database will be deleted.</div>

    <div class="tooltip-paragraph-fold tr">By clicking on "Add filter" button you can modify filters. You can add as many filters as you like. Only records that matches all filters will be affected by this action.</div>

    <div class="tooltip-paragraph-fold tr">Filters may also include groups. If you fill group id, this action will be applied to records in that group and which matches other filters.</div>

    <div class="tooltip-paragraph-fold tr">By default this action doesn't synchronize database and resources, if you want to delete record from resources besides database, you need to check "Delete resource" field.</div>

    <div class="tooltip-paragraph-fold tr">It may take a lot of time if you are deleting a lot of records.</div>
    <div class="tooltip-paragraph-fold tr">It is synchronous, so feel free to use action in several threads.</div>
    <div class="tooltip-paragraph-last-fold tr">Records are displayed and may be edited in database manager.</div>

  </div>
  <%= _.template($('#back').html())({action:"executeandadd", visible:true, wiki: "tutorial3"}) %>
</span>
<span id="filter-add" style="display:none">

<div class="container-fluid">
  <div class="col-xs-12">

    <span class="tr">Field</span>:
    <select class="form-control input-sm" id="FilterFieldInput">
    </select>
    <span class="tr">Filter type</span>:
    <select class="form-control input-sm" id="FilterTypeInput">
    </select>
  </div>
</div>
<div style="margin-top:10px" id="FiledValueInput">
    <%= _.template($('#input_constructor').html())({id:"Value", description:tr("Value"), default_selector: "string"}) %>
</div>

<div class="container-fluid" style="margin-top:0px">
  <div class="col-xs-12">

    <button type="button" class="btn btn-primary filterok standartbutton tr" style="margin-top:5px" >Ok</button>
    <button type="button" class="btn btn-default filtercancel standartbutton tr" style="margin-top:5px" >Cancel</button>
  </div>
</div>

</span>
