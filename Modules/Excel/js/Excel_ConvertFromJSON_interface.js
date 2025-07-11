<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"FilePath", description: tr("File path"), default_selector: "string", disable_int:true, value_string: "", help: {description:tr("The path to the excel file."),examples:[{code:"{{excel_file}}"},{code:"C:/test.xlsx"},{code:"C:/Program Files/test1.xlsx"},{code:"C:/Program Files/test2.xlsx"}]} }) %>
<%= _.template($('#input_constructor').html())({id:"Data", description: tr("JSON string"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("JSON string which needs to be converted to excel file."),examples:[{code:"{\"sheets\":[{\"name\":\"S1\",\"data\":[]},{\"name\":\"S2\",\"data\":[]}]}"}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Convert data from JSON string and write it to excel file.</div>
	<div class="tr tooltip-paragraph-fold">This action accepts data in the form of a JSON string consisting of an object containing a list of objects containing sheet names and their data.</div>
	<div class="tooltip-paragraph-fold"><span class="tr">Example:</span> <code>{"sheets":[{"name":"Sheet1","data":[["a1","b1"],["a2","b2"]]},{"name":"Sheet2","data":[["A1","B1"],["A2","B2"]]}]}</code></div>
	<div class="tr tooltip-paragraph-fold">Data in this format can be obtained using the "Convert to JSON" action, or you can create them yourself.</div>
	<div class="tr tooltip-paragraph-fold">This action can be used in conjunction with the "JSON" module and the "Convert to JSON" action to edit an excel file.</div>
	<div class="tr tooltip-paragraph-fold">If the excel file already contains sheets contained in the JSON string, they will be completely overwritten with data from the JSON string.</div>
	<div class="tr tooltip-paragraph-fold">If a resource is specified in the "File path" parameter, resource location will be used.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd",use_timeout: true,use_waiter: true,waiter_enabled: true,waiter_title: "Wait until the file will be written, it may take additional time. In case if this option is checked, error will be thrown if file could not be saved.",visible:true}) %>