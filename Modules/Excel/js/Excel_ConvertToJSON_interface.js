<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"FilePath", description: tr("File path"), default_selector: "string", disable_int:true, value_string: "", help: {description:tr("The path to the excel file."),examples:[{code:"{{excel_file}}"},{code:"C:/test.xlsx"},{code:"C:/Program Files/test1.xlsx"},{code:"C:/Program Files/test2.xlsx"}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description: tr("Variable to save the result"), default_variable: "XLSX_JSON", help: {description: tr("Variable in which, after successful execution of the action, a JSON string will be written."),examples:[{code:"{\"sheets\":[{\"name\":\"S1\",\"data\":[]},{\"name\":\"S2\",\"data\":[]}]}"}]}}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Convert data from excel file to JSON string.</div>
	<div class="tr tooltip-paragraph-fold">This action returns data from an excel file in the form of a JSON string consisting of an object containing a list of objects containing sheet names and their data.</div>
	<div class="tooltip-paragraph-fold"><span class="tr">Example:</span> <code>{"sheets":[{"name":"Sheet1","data":[["a1","b1"],["a2","b2"]]},{"name":"Sheet2","data":[["A1","B1"],["A2","B2"]]}]}</code></div>
	<div class="tr tooltip-paragraph-fold">The resulting JSON string can be processed using actions from the "JSON" module.</div>
	<div class="tr tooltip-paragraph-fold">To get the name of the first sheet, use the JPath query <span style="color:black">$.sheets[0].name</span> in the "Get value" action from the "JSON" module.</div>
	<div class="tr tooltip-paragraph-fold">To get a list of all sheet names, use the JPath query <span style="color:black">$.sheets[*].name</span> in the "Get all values" action from the "JSON" module.</div>
	<div class="tr tooltip-paragraph-fold">To get data for the first sheet, use the JPath query <span style="color:black">$.sheets[0].data</span> in the "Get value" action from the "JSON" module.</div>
	<div class="tr tooltip-paragraph-fold">To get data for a sheet with a name from the [[NAME]] variable, use the JPath query <span style="color:black">$.sheets[?(@.name=="[[NAME]]")].data</span> in the "Get value" action from the "JSON" module.</div>
	<div class="tr tooltip-paragraph-fold">To get the value of the cell with row index 1 and column index 0, use the JPath query <span style="color:black">$.sheets[0].data[1][0]</span> in the "Get value" action from the "JSON" module.</div>
	<div class="tr tooltip-paragraph-fold">This action can be used in conjunction with the "JSON" module and the "Convert from JSON" action to edit an excel file.</div>
	<div class="tr tooltip-paragraph-fold">If a resource is specified in the "File path" parameter, resource location will be used.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd",use_timeout: true, visible:true}) %>