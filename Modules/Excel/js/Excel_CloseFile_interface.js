<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"FilePath", description: tr("File path"), default_selector: "string", disable_int:true, value_string: "", help: {description:tr("The path to the excel file."),examples:[{code:"{{excel_file}}"},{code:"C:/test.xlsx"},{code:"C:/Program Files/test1.xlsx"},{code:"C:/Program Files/test2.xlsx"}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Close excel file.</div>
	<div class="tr tooltip-paragraph-fold">If the file is already closed or not open yet, this action will do nothing.</div>
	<div class="tr tooltip-paragraph-fold">This action is optional because file is closed automatically if it is not used for more than 5 seconds.</div>
	<div class="tr tooltip-paragraph-last-fold">If a resource is specified in the "File path" parameter, resource location will be used.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd",use_timeout: true, visible:true}) %>