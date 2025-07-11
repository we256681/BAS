<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"FilePath", description: tr("File path"), default_selector: "string", disable_int:true, value_string: "", help: {description:tr("The path to the excel file."),examples:[{code:"{{excel_file}}"},{code:"C:/test.xlsx"},{code:"C:/Program Files/test1.xlsx"},{code:"C:/Program Files/test2.xlsx"}]} }) %>
<%= _.template($('#input_constructor').html())({id:"SheetList", description: tr("List of sheets") + ". " + tr("Can be blank"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("List of indexes or names of sheets from excel file, data from which should be imported into resources. As a list, you can use a string consisting of names or indexes of sheets, separated by commas."),examples:[{code:"Sheet1,Sheet2,Sheet5"},{code:"Sheet1, Sheet2, Sheet5"},{code:"[\"Sheet1\", \"Sheet2\", \"Sheet5\"]"},{code:tr("Empty string"), description: tr("All sheets will be imported")}]} }) %>
<%= _.template($('#block_start').html())({id:"Advanced", name: tr("Resources settings"), description: tr("The settings that each resource created by this action will have.")}) %>
<%= _.template($('#input_constructor').html())({id:"SuccessNumber", description:tr("Success Usage"), default_selector: "int", disable_string:true, value_number:1, help: {description: tr("Maximum number of success usages of current resource. Usage of resource is considered successful if thread, which used it is ended with success. For example, if resource holds proxies and script creates accounts, then this parameter indicates how many accounts may be registered for with one proxy.")}}) %>
<%= _.template($('#input_constructor').html())({id:"FailNumber", description:tr("Fail Usage"), default_selector: "int", disable_string:true, value_number:1, help: {description: tr("Maximum number of fail usages of current resource. Usage of resource is considered as fail if thread, which used it is ended with failure. For example, if resource holds proxies and script creates accounts, then this parameter indicates how many bad retries is allowed for one proxy.")}}) %>
<%= _.template($('#input_constructor').html())({id:"SimultaneousUsage", description:tr("Simultaneous Usage"), default_selector: "int", disable_string:true, value_number:1, help: {description: tr("Maximum simultaneous usages. For example, if resource holds proxies, then this parameter indicates how many times single proxy may be used simultaneously.")}}) %>
<%= _.template($('#input_constructor').html())({id:"Interval", description:tr("Interval Between Usage"), default_selector: "int", disable_string:true, value_number:5000, help: {description: tr("Interval between usage in milliseconds."),examples:[{code:"0",description:tr("After resource item is released, it may be acquired again instantly.")},{code:"5000",description:tr("There should be at least 5 seconds gap between usages of every resource item.")}]}}) %>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check">
	<input type="checkbox" id="Check"/> <label for="Check" class="tr">Greedy Usage Algorithm</label>
</span>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check2">
	<input type="checkbox" id="Check2"/> <label for="Check2" class="tr">Wait For Resource</label>
</span>
<%= _.template($('#block_end').html())() %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Import data from excel file into resources.</div>
	<div class="tr tooltip-paragraph-fold">This action creates a resource from each worksheet of the excel file. Resources will be named after worksheets. For example, if book contains 3 worksheets: "Work1", "Work2" and "Work3", then 3 resources with the same names will be created.</div>
	<div class="tr tooltip-paragraph-fold">Data for each resource will be obtained from corresponding worksheet. For example, if worksheet contains 2 lines: A1:B1 and A2:B2, then resource will be created with exact same 2 items.</div>
	<div class="tr tooltip-paragraph-fold">If a resource with the same name already exists, it will be overwritten with new settings and data.</div>
	<div class="tr tooltip-paragraph-fold">Each resource holds container for data, for example, lines from file. Unlike lists, resource system helps to distribute data across threads and also handles thread concurrency for that data.</div>
	<div class="tooltip-paragraph-fold"><span class="tr">It is strongly recommended not to use this action in code which runs in several threads, because each thread will create a new resource and that is probably not your intention. Use it in OnApplicationStart function instead. This function is executed only once for whole script lifetime before any other function and thus is the best place to create resource</span> (
		<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://i.imgur.com/BxLTver.png');return false">screen</a> ).
	</div>
	<div class="tooltip-paragraph-fold"><span class="tr">After custom resource is created and filled with data it can be used as any other resource in any field and in any thread</span>  (
		<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=onapplicationstartuseresource.png');return false">screen</a> ).
	</div>
	<div class="tr tooltip-paragraph-fold">Typical example of using this action would be creation of resource(in OnApplicationStart), which takes data from an excel file.</div>
	<div class="tr tooltip-paragraph-fold">'Wait For Resource' is very important setting, it determines behaviour in case, when resource data list is empty. If prameter is set, then action that acquires resource will wait until data will be added, otherwise action will fail instantly.</div>
	<div class="tr tooltip-paragraph-fold">When 'Greedy Usage Algorithm' is set, resource system will try to acquire same line again and again until that line will exhaust all success or fail usages, only after that next line will be used.</div>
	<div class="tr tooltip-paragraph-fold">If a resource is specified in the "File path" parameter, resource location will be used.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd",use_timeout: true, visible:true}) %>