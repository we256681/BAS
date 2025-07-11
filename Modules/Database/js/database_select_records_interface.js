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
      <div class="container-fluid" id="database-filters" >
        <div class="col-xs-12">
      
          <div id="database-filters-data" style="margin-bottom:5px">
          </div>
        </div>
      </div>
  </div>

  <div class="container-fluid">

    <%= _.template($('#input_constructor').html())({id:"GroupId", description:tr("Filter by group"), default_selector: "string", disable_int:true,help: {description: tr("Additional filter by group id. It can be obtained from \"Create Group\" action or from \"Get Group List\". Leave blank not to apply filter by group."),examples:[{code:"5b1d2284644bc0759b5dfe15",description: tr("Action will be applied only to exact group and which matches other filters.")},
          {code:tr("Empty string"),description: tr("Action will be applied to records which matches other filters.")}]}}) %>

<div class="container-fluid">
    <div class="col-xs-12">
      <form class="form-horizontal">
        <div class="form-group">
          <div class="col-xs-12">
            <hr style="margin-top:10px;margin-bottom:10px"/>
          </div>
        </div>
      </form>
    </div>
  </div>
    <%= _.template($('#input_constructor').html())({id:"PageNumber", description:tr("Page number"), default_selector: "int", disable_string:true, value_number: 1,help: {description: tr("This value starts from 1 and indicates page number. Page number and page size together set limit and offset of result record list."),examples:[
  {code:tr("Page number - 1, page size - 1000"),description:tr("Gets 1 - 1000 record from database")},
  {code:tr("Page number - 2, page size - 1000"),description:tr("Gets 1001 - 2000 record from database")},
  {code:tr("Page number - 1, page size - 100"),description:tr("Gets 1 - 100 record from database")},
]}
}) %>
    <%= _.template($('#input_constructor').html())({id:"PageSize", description:tr("Page size"), default_selector: "int", disable_string:true, value_number: 1000,help: {description: tr("This value sets maximum list size that you can obtain with this action. Works together with page number, which sets offset. If page number equals one, than this value limits number of results."),examples:[
  {code:tr("Page number - 1, page size - 1000"),description:tr("Gets 1 - 1000 record from database")},
  {code:tr("Page number - 2, page size - 1000"),description:tr("Gets 1001 - 2000 record from database")},
  {code:tr("Page number - 1, page size - 100"),description:tr("Gets 1 - 100 record from database")},
]}
}) %>
  <div class="container-fluid">
    <div class="col-xs-12">
      <form class="form-horizontal">
        <div class="form-group">
          <div class="col-xs-12">
            <hr style="margin-top:10px;margin-bottom:10px"/>
          </div>
        </div>
      </form>
    </div>
  </div>

      <%= _.template($('#input_constructor').html())({id:"SortingType", description:tr("Sorting type"), default_selector: "string", disable_int:true, value_string: "no sorting", variants: ["no sorting", "ascending", "descending"], help: {description: tr("By default sorting is disabled, but you can enable it by changing this field. Don't forget to change database field(\"Sorting column\") for which sorting will be applied."), examples:[
        {code:"no sorting",description:tr("No sorting")},
        {code:"ascending",description:tr("Sort ascending")},
        {code:"descending",description:tr("Sort descending")}


      ]}}) %>

      <div class="col-xs-12">
          <form class="form-horizontal">
            <div class="form-group">
              <div class="col-xs-12">
                <div class="input-group">

                  <% _Schema.forEach(function(t){ %>
                    <span data-preserve="true" style="<%= (t["id"] != _Schema[0]["id"])?"display:none":"" %>" data-table-id="<%= t["id"] %>" data-preserve-type="select" data-preserve-id="SortColumns<%= t["id"] %>">
                      <select class="form-control input-sm" id="SortColumns<%= t["id"] %>" placeholder="Result">
                        <% t["columns"].forEach(function(c){ %>
                          <option value="<%= c.id %>"><%= c.description %></option>
                        <% }) %>
                      </select>
                    </span>
                  <% }) %>

                  <span class="input-group-addon" style="font-size: smaller;">
                    <span class="tr">Sorting column</span>
                    <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-placement="bottom" data-html="true" title="<%= tr('Column for sorting') %>" ></i>

                  </span>
                </div>
              </div>
            </div>
          </form>
        </div>


  <div class="container-fluid">
    <div class="col-xs-12">
      <form class="form-horizontal">
        <div class="form-group">
          <div class="col-xs-12">
            <hr style="margin-top:10px;margin-bottom:10px"/>
          </div>
        </div>
      </form>
    </div>
  </div>

    <%= _.template($('#variable_constructor').html())({id:"RecordList", description:tr("Record list"), default_variable: "RECORD_LIST", help: {description: tr("After action completes, this variable will contain result as a list of strings. Each string describes one record with csv format. You can process list with module \"List\" and parse each line with \"Parse Line\" action. Record columns are components of resulting cvs in a same order as defined in database, plus the last component is always record id."),examples:[
  {code:"[\"Cell11:Cell12:5b1d2284644bc0759b5dfe15\", \"Cell21:Cell22:5b1d2284644bc0759b5dfe1\"]"},
]}}) %>
  </div>

<div class="tooltipinternal">
  <div class="tr tooltip-paragraph-first-fold">Select records from database.</div>
  <div class="tr tooltip-paragraph-fold">This action supports filtering, sorting and paging, if you want to use this features, plaese study corresponding fields. If you don't want to use them, just leave default values, it will just select records from database.</div>
  <div class="tooltip-paragraph-fold"><span class="tr">After action completes, variable will contain result as a list of strings. Each string describes one record with csv format. You can process list with module "List" and parse each line with "Parse Line" action. Use "Foreach" action to iterate over records. Record columns are components of resulting cvs in a same order as defined in database, plus the last component is always record id.</span> (<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=iterateoverrecordsen.png');return false">screen</a>).</div>


  <div class="tooltip-paragraph-fold"><span class="tr">You can also read from database with resources system. You need to define resource of type "Database" and use it in any field.</span> (<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=usedatabaseinanyfield.png');return false">screen</a>)</div>
  <div class="tooltip-paragraph-fold tr">You can also create resource of type "Database" and read all content with "Resource To List" action. Main advantage of resource system is ability to distribute data across threads.</div>
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
