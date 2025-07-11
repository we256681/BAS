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

    <%= _.template($('#input_constructor').html())({id:"RecordId", description:tr("Record id"), default_selector: "string", disable_int:true,help: {description: tr("Record id, which will be selected. It can be obtained from resource with type \"Database\" or from list returned from \"Select Records\" action."),examples:[{code:"5b1d2284644bc0759b5dfe15"}]}}) %>

    <%= _.template($('#variable_constructor').html())({id:"RecordString", description:tr("Record"), default_variable: "RECORD_STRING_CSV", help: {description: tr("After action completes, this variable will contain a string in csv format. You can extract different columns of a record with \"Parse Line\" action. Record columns are components of resulting cvs in a same order as defined in database, plus the last component is always record id."),examples:[
  {code:"Cell11:Cell12:5b1d2284644bc0759b5dfe15"},
  {code:tr("Empty string"),description: tr("No record found")}
]}}) %>
    


</div>

<div class="tooltipinternal">
  <div class="tr tooltip-paragraph-first-fold">Select one database record by id.</div>
  
  <div class="tooltip-paragraph-fold tr">This action only selects single record, if you wan't to select more than one or use filters, use "Select Records By Criteria" action.</div>

  <div class="tooltip-paragraph-fold"><span class="tr">After action completes, variable will contain a string in csv format. You can extract different columns of a record with "Parse Line" action. Record columns are components of resulting cvs in a same order as defined in database, plus the last component is always record id.</span> (<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=parsedatabaserecord.png');return false">screen</a>).</div>

    <div class="tooltip-paragraph-fold"><span class="tr">You can also read from database with resources system. You need to define resource of type "Database" and use it in any field.</span> (<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=usedatabaseinanyfield.png');return false">screen</a>)</div>

    <div class="tooltip-paragraph-fold tr">You can also create resource of type "Database" and read all content with "Resource To List" action. Main advantage of resource system is ability to distribute data across threads.</div>

    <div class="tooltip-paragraph-fold tr">It is synchronous, so feel free to use action in several threads.</div>
    <div class="tooltip-paragraph-last-fold tr">Records are displayed and may be edited in database manager.</div>

</div>

<%= _.template($('#back').html())({action:"executeandadd", visible:true, wiki: "tutorial3"}) %>
