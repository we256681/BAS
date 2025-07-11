<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"FilePath", description: tr("File path"), default_selector: "string", disable_int:true, value_string: "", help: {description:tr("The path to the excel file."),examples:[{code:"{{excel_file}}"},{code:"C:/test.xlsx"},{code:"C:/Program Files/test1.xlsx"},{code:"C:/Program Files/test2.xlsx"}]} }) %>
<%= _.template($('#input_constructor').html())({id:"SheetName", description: tr("Sheet name"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("New sheet name."),examples:[{code:tr("Sheet1")}, {code:tr("Sheet2")}, {code:tr("New Sheet")}]} }) %>
<%= _.template($('#input_constructor').html())({id:"SheetIndex", description: tr("Sheet index") + ". " + tr("Can be blank"), default_selector: "int", value_number: "", min_number:0, max_number:999999, help: {description: tr("The index where the sheet should be placed, or the name of the sheet before which it should be placed."),examples:[{code:tr("Numbеr"), description: tr("The sheet will have the specified index")}, {code:0, description: tr("First sheet index")}, {code:1, description: tr("Second sheet index")}, {code:tr("String"), description: tr("The sheet will be added before the specified sheet")}, {code:tr("Empty string"), description: tr("The sheet will be added to the end of the book")}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Add new sheet to excel file.</div>
	<div class="tr tooltip-paragraph-fold">If the "Sheet index" parameter is empty, the sheet will be added to the end of the book.</div>
	<div class="tr tooltip-paragraph-fold">In the "Sheet index" parameter, you can specify the name of an existing sheet, then a new sheet will be added before it.</div>
	<div class="tr tooltip-paragraph-fold">When specifying a sheet name in the "Sheet index" parameter, the field type must be <code>string</code>.</div>
	<div class="tr tooltip-paragraph-fold">If a resource is specified in the "File path" parameter, resource location will be used.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd",use_timeout: true,use_waiter: true,waiter_enabled: true,waiter_title: "Wait until the file will be written, it may take additional time. In case if this option is checked, error will be thrown if file could not be saved.",visible:true}) %>