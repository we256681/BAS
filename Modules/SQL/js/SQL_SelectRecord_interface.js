<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"table", description:tr("Database table"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("Table name, for which operation will be applied"), examples:[{code:"table"},{code:"users"},{code:"accounts"},{code:"materials"}]} }) %>
<%= _.template($('#input_constructor').html())({id:"where", description:tr("Filter") + ". " + tr("Can be blank"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("A set of conditions for selecting records, presented as a string with SQL syntax. In a SQL query it is used inside a WHERE clause, for example, a query with the filter <code style=\"font-size:85%\">name = 'name'</code> would look like this <code style=\"font-size:85%\">SELECT * FROM users WHERE name = 'name'</code>. The filter will be put in the query without any modification, except for variable and resource substitution."), examples:[{code:"id = 10",description:tr("id equals 10")},{code:"id = [[ID]]",description:tr("id is equal to the value from the variable [[ID]]")},{code:"title = {{title}}",description:tr("title is equal to the value from the resource {{title}}")},{code:"name='john' AND balance > 100",description:tr("name is equal to john and balance is more than one hundred")},{code:"radius BETWEEN 30 AND 90",description:tr("radius from 30 to 90")},{code:"amount IS NULL",description:tr("amount field is empty")},{code:"id IN (4, 12, 58, 67)",description:tr("id is equal to a value from the list 4, 12, 58, 67")},{code:"id IN ([[ID_LIST]])",description:tr("id is equal to the value from the list in the [[ID_LIST]] variable")},{code:tr("Empty string"),description:tr("Do not filter records")}]} }) %>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check" style="padding-left:5px">
	<input type="checkbox" id="Check" checked="checked"/> <label for="Check" class="tr">Parameterize variables and resources in the filter.</label> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title:tr("Parameterize variables and resources"),description:tr("If enabled, data from variables and resources will be escaped.") + " " + tr("For different dialects, the result of parameterization may differ."),examples:[{code:tr("Activated"),description:tr("Value")+ " <code style=\"font-size:85%\">name = [[NAME]]</code> " + tr("will be replaced by") + " <code style=\"font-size:85%\">name = '" + tr("NAME variable content") + "'</code>"},{code:"<br/>" + tr("Deactivated"),description:tr("Value")+ " <code style=\"font-size:85%\">name = [[NAME]]</code> " + tr("will be replaced by") + " <code style=\"font-size:85%\">name = " + tr("NAME variable content") + "</code>"},{code:"<br/>" + tr("Activated"),description:tr("Value")+ " <code style=\"font-size:85%\">country = {{country}}</code> " + tr("will be replaced by") + " <code style=\"font-size:85%\">country = '" + tr("country resource content") + "'</code>"},{code:"<br/>" + tr("Deactivated"),description:tr("Value")+ " <code style=\"font-size:85%\">country = {{country}}</code> " + tr("will be replaced by") + " <code style=\"font-size:85%\">country = " + tr("country resource content") + "</code>"}]})) %>"></i>
</span>
<%= _.template($('#block_start').html())({id:"Additional", name: tr("Additional settings"), description: ""}) %>
<%= _.template($('#input_constructor').html())({id:"included_columns", description:tr("List of included columns"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("List of column names to be included in the final results.") + " " + tr("As a list, you can use a string consisting of column names, separated by commas.") + " " + tr("If the \"List of excluded columns\" parameter is specified, the value of this parameter is ignored."),examples:[{code:"id,title,url,amount"},{code:"id, title, url, amount"},{code:"[\"id\", \"title\", \"url\", \"amount\"]"},{code:tr("Empty string"), description:tr("Include all columns")}]} }) %>
<%= _.template($('#input_constructor').html())({id:"excluded_columns", description:tr("List of excluded columns"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("List of column names to be excluded from the final results.") + " " + tr("As a list, you can use a string consisting of column names, separated by commas.") + " " + tr("If this parameter is specified, the value of the \"List of included columns\" parameter is ignored."),examples:[{code:"id,title,url,amount"},{code:"id, title, url, amount"},{code:"[\"id\", \"title\", \"url\", \"amount\"]"},{code:tr("Empty string"), description:tr("Do not exclude columns")}]} }) %>
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
<%= _.template($('#input_constructor').html())({id:"order_direction", description:tr("Sorting type"), default_selector: "string", variants: ["no sorting<br/><span style='color:gray;font-size:small'>" + tr("No sorting") + "</span>", "ascending<br/><span style='color:gray;font-size:small'>" + tr("Sort ascending") + "</span>", "descending<br/><span style='color:gray;font-size:small'>" + tr("Sort descending") + "</span>"], disable_int:true, value_string: "no sorting", help: {description: tr("By default sorting is disabled, but you can enable it by changing this field. Don't forget to change database field(\"Sorting column\") for which sorting will be applied."), examples:[{code:"no sorting",description:tr("No sorting")},{code:"ascending",description:tr("Sort ascending")},{code:"descending",description:tr("Sort descending")}]} }) %>
<%= _.template($('#input_constructor').html())({id:"order_column", description:tr("Sorting column"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("Column for sorting"),examples:[{code:"id"},{code:"amount"}]} }) %>
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
<%= _.template($('#input_constructor').html())({id:"offset", description:tr("Offset"), default_selector: "int", disable_string:true, value_number: "", min_number:0, max_number:999999, help: {description: tr("Number of records to skip."),examples:[{code:tr("Empty string"), description:tr("Don't skip records")}]} }) %>
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
<%= _.template($('#block_end').html())() %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "SQL_RECORD", help: {description: tr("Variable in which, after successful execution of the action, the received record will be written.") + " " + tr("The format of the saved data depends on the \"Data format\" setting.")}}) %>
<%= _.template($('#input_constructor').html())({id:"data_format", description: tr("Data format"), default_selector: "string", variants:["CSV<br/><span style='color:gray;font-size:small'>1:test1:true</span>","List<br/><span style='color:gray;font-size:small'>[1,\"test1\",true]</span>","Object<br/><span style='color:gray;font-size:small'>{\"id\":1,\"name\":\"test1\",\"active\":true}</span>"], disable_int:true, value_string: "CSV", help: {description: tr("The format in which the received data will be stored in a variable."),examples:[{code:"CSV", description:"1:test1:true"},{code:"List", description:"[1,\"test1\",true]"},{code:"Object", description:"{\"id\":1,\"name\":\"test1\",\"active\":true}"}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Select single record from SQL database.</div>
	<div class="tr tooltip-paragraph-fold">In order to execute this action correctly you need to run "Setup connection" action first.</div>
	<div class="tooltip-paragraph-fold"><span class="tr">The filter has SQL WHERE clause syntax, you can learn more about SQL WHERE in</span> <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://www.w3schools.com/sql/sql_where.asp');return false">tutorial</a><span>.</span></div>
	<div class="tr tooltip-paragraph-fold">If the parameter "Parameterize variables and resources in the filter" is activated, then data from variables and resources in the filter will be escaped.</div>
	<div class="tr tooltip-paragraph-fold">For example, this filter <code>name = [[NAME]]</code> will be replaced with this one <code>name = 'NAME variable content'</code>. In case if parameter "Parameterize variables and resources" is disabled, filter will be following <code>name = NAME variable content</code>.</div>
	<div class="tr tooltip-paragraph-fold">For different dialects, the result of parameterization may differ.</div>
	<div class="tooltip-paragraph-fold"><span class="tr">CSV - a string consisting of field values separated by ":".</span> <span class="tr">Example:</span> <code>1:test1:true</code></div>
	<div class="tooltip-paragraph-fold"><span class="tr">List - a list consists the field values.</span> <span class="tr">Example:</span> <code>[1,"test1",true]</code></div>
	<div class="tooltip-paragraph-fold"><span class="tr">Object - an object, in which the key - a field name and value - the content of the field.</span> <span class="tr">Example:</span> <code>{"id":1,"name":"test1","active":true}</code></div>
	<div class="tr tooltip-paragraph-fold">CSV lines can be parsed using the "Parse Line" action from the "Tools" module.</div>
	<div class="tr tooltip-paragraph-fold">Lists can be processed using actions from the "List" module.</div>
	<div class="tr tooltip-paragraph-fold">Objects can be processed using actions from the "JSON" module.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd",use_timeout: true, visible:true}) %>
