<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"FilePath", description: tr("File path"), default_selector: "string", disable_int:true, value_string: "", help: {description:tr("The path to the excel file."),examples:[{code:"{{excel_file}}"},{code:"C:/test.xlsx"},{code:"C:/Program Files/test1.xlsx"},{code:"C:/Program Files/test2.xlsx"}]} }) %>
<%= _.template($('#input_constructor').html())({id:"SheetIndexOrName", description: tr("Sheet index or name"), default_selector: "int", value_number: 0, min_number:0, max_number:999999, help: {description: tr("Index or sheet name in excel file."),examples:[{code:0, description: tr("First sheet index")}, {code:1, description: tr("Second sheet index")}, {code:tr("Sheet1"), description: tr("First sheet name")}, {code:tr("Sheet2"), description: tr("Second sheet name")}]} }) %>
<%= _.template($('#input_constructor').html())({id:"Contains", description: tr("Cells contains"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("The data which will be searched.")} }) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description: tr("Variable to save the result"), default_variable: "XLSX_FOUND_CELLS_LIST", help: {description: tr("Variable in which, after successful execution of the action, the list of addresses of the found cells will be written."),examples:[{code:"[\"4*3\",\"2*6\",\"1*8\"]"},{code:"[ ]", description: tr("Cells not found")}]}}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Find all cells by their content in excel file.</div>
	<div class="tr tooltip-paragraph-fold">This action returns a list of addresses of found cells, cell addresses consist of a column index and a row index separated with "*" symbol. Example: <code>["4*3","2*6","1*8"]</code></div>
	<div class="tr tooltip-paragraph-fold">The resulting list can be processed using actions from the "List" module.</div>
	<div class="tr tooltip-paragraph-fold">Cell addresses can be parsed into column and row indexes by the "*" separator using the "Parse String" action from the "List" module.</div>
	<div class="tr tooltip-paragraph-fold">If a resource is specified in the "File path" parameter, resource location will be used.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd",use_timeout: true, visible:true}) %>