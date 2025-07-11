<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"query", description:tr("SQL query"), default_selector: "string", disable_int:true, disable_editor:true, disable_string:true, use_textarea:true, size: 8, disable_type_chooser:true,textarea_height:80, modal_code_id:"sql-query-help-" + (_K || "en") }) %>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check">
	<input type="checkbox" id="Check" checked="checked"/> <label for="Check" class="tr">Parameterize variables and resources in the query.</label> <i class="fa fa-question-circle help-input" data-toggle="tooltip" data-html="true" title="<%= _.escape(_.template($("#tooltip-input").html())({title:tr("Parameterize variables and resources"),description:tr("If enabled, data from variables and resources will be escaped.") + " " + tr("For different dialects, the result of parameterization may differ."),examples:[{code:tr("Activated"),description:tr("Value")+ " <code style=\"font-size:85%\">SELECT * FROM users WHERE name = [[NAME]]</code> " + tr("will be replaced by") + " <code style=\"font-size:85%\">SELECT * FROM users WHERE name = '" + tr("NAME variable content") + "'</code>"},{code:"<br/>" + tr("Deactivated"),description:tr("Value")+ " <code style=\"font-size:85%\">SELECT * FROM users WHERE name = [[NAME]]</code> " + tr("will be replaced by") + " <code style=\"font-size:85%\">SELECT * FROM users WHERE name = " + tr("NAME variable content") + "</code>"},{code:"<br/>" + tr("Activated"),description:tr("Value")+ " <code style=\"font-size:85%\">SELECT * FROM users WHERE country = {{country}}</code> " + tr("will be replaced by") + " <code style=\"font-size:85%\">SELECT * FROM users WHERE country = '" + tr("country resource content") + "'</code>"},{code:"<br/>" + tr("Deactivated"),description:tr("Value")+ " <code style=\"font-size:85%\">SELECT * FROM users WHERE country = {{country}}</code> " + tr("will be replaced by") + " <code style=\"font-size:85%\">SELECT * FROM users WHERE country = " + tr("country resource content") + "</code>"}]})) %>"></i>
</span>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "SQL_QUERY_RESULTS", help: {description: tr("Variable in which, after successful execution of the action, the results of the query will be written.") + " " + tr("If a SELECT query is executed, the format of the saved data depends on the \"Data format\" setting.")}}) %>
<%= _.template($('#input_constructor').html())({id:"data_format", description: tr("Data format"), default_selector: "string", variants:["CSV list<br/><span style='color:gray;font-size:small'>[\"1:test1:true\",\"2:test2:false\",\"3:test3:false\"]</span>","CSV string<br/><span style='color:gray;font-size:small'>1:tes1:true<br/>2:test2:false<br/>3:test3:false</span>","2D list<br/><span style='color:gray;font-size:small'>[[1,\"test1\",true],[2,\"test2\",false],[3,\"test3\",false]]</span>","Object list<br/><span style='color:gray;font-size:small'>[{\"id\":1,\"name\":\"test1\",\"active\":true},{\"id\":2,\"name\":\"test2\",\"active\":false},{\"id\":3,\"name\":\"test3\",\"active\":false}]</span>"], disable_int:true, value_string: "CSV list", help: {description: tr("The format in which the received data will be stored in a variable if a SELECT query is executed."),examples:[{code:"CSV list", description:"[\"1:test1:true\",\"2:test2:false\",\"3:test3:false\"]"},{code:"CSV string", description:"1:tes1:true<br/>2:test2:false<br/>3:test3:false"},{code:"2D list", description:"[[1,\"test1\",true],[2,\"test2\",false],[3,\"test3\",false]]"},{code:"Object list", description:"[{\"id\":1,\"name\":\"test1\",\"active\":true},{\"id\":2,\"name\":\"test2\",\"active\":false},{\"id\":3,\"name\":\"test3\",\"active\":false}]"}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Execute an arbitrary query against an SQL database.</div>
	<div class="tr tooltip-paragraph-fold">In order to execute this action correctly you need to run "Setup connection" action first.</div>
	<div class="tooltip-paragraph-fold"><span class="tr">A query can have multiple lines, you can learn more about SQL in</span> <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://www.w3schools.com/sql');return false">tutorial</a><span>.</span></div>
	<div class="tr tooltip-paragraph-fold">If the parameter "Parameterize variables and resources" is activated, then data from variables and resources will be escaped.</div>
	<div class="tr tooltip-paragraph-fold">For example, this query <code>SELECT * FROM users WHERE name = [[NAME]]</code> will be replaced with this one <code>SELECT * FROM users WHERE name = 'NAME variable content'</code>. In case if parameter "Parameterize variables and resources" is disabled, query will be following <code>SELECT * FROM users WHERE name = NAME variable content</code>.</div>
	<div class="tr tooltip-paragraph-fold">For different dialects, the result of parameterization may differ.</div>
	<div class="tr tooltip-paragraph-fold">If a SELECT query is executed, the format of the saved data depends on the "Data format" setting.</div>
	<div class="tooltip-paragraph-fold"><span class="tr">CSV list - a list that consists of CSV lines and CSV line consists of field values separated with ":" symbol.</span> <span class="tr">Example:</span> <code>["1:test1:true","2:test2:false","3:test3:false"]</code></div>
	<div class="tooltip-paragraph-fold"><span class="tr">CSV string - this CSV lines separated by a line break, and consisting of field values separated with ":" symbol.</span> <span class="tr">Example:</span> <code><br/>1:tes1:true<br/>2:test2:false<br/>3:test3:false</code></div>
	<div class="tooltip-paragraph-fold"><span class="tr">2D list - a list that consists of lists containing the field values.</span> <span class="tr">Example:</span> <code>[[1,"test1",true],[2,"test2",false],[3,"test3",false]]</code></div>
	<div class="tr tooltip-paragraph-fold">Object list - a list that consists of objects, in the object the key is the name of the field, and the value is the content of the field.</div>
	<div class="tooltip-paragraph-fold"><span class="tr">Example:</span> <code>[{"id":1,"name":"test1","active":true},{"id":2,"name":"test2","active":false},{"id":3,"name":"test3","active":false}]</code></div>
	<div class="tr tooltip-paragraph-fold">CSV lines can be parsed using the "Parse Line" action from the "Tools" module.</div>
	<div class="tr tooltip-paragraph-fold">Lists can be processed using actions from the "List" module.</div>
	<div class="tr tooltip-paragraph-fold">Objects can be processed using actions from the "JSON" module.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd",use_timeout: true, visible:true}) %>

<script type="text/template" id="sql-query-help-en">
	<span style="font-size:small">
		This field should contain the SQL query to be executed against the database. A query can have multiple lines, you can learn more about SQL in <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://www.w3schools.com/sql');return false">tutorial</a>.<br/><br/>
		
		Example #1. Insert new record into "users" table, the "name" field is "John", the "age" field is 25
		<pre>INSERT INTO users (name, age)
VALUES ('John', 25)</pre>

		Example #2.1. Insert new record into "users" table, where field values are obtained from variables.<br/><code>The parameter "Parameterize variables and resources" must be enabled, as variables are not escaped.</code>
		<pre>INSERT INTO users (name, surname, age)
VALUES ([[NAME]], [[SURNAME]], [[AGE]])</pre>

		Example #2.2. Insert new record into "users" table, where field values are obtained from variables.<br/><code>The parameter "Parameterize variables and resources" must be disabled, since the variables are already escaped.</code>
		<pre>INSERT INTO users (name, surname, age)
VALUES ('[[NAME]]', '[[SURNAME]]', [[AGE]])</pre>

		Example #3. Delete from table "users" records with "id" equal to 10
		<pre>DELETE FROM users
WHERE id = 10</pre>

		Example #4. Delete from table "users" records in which "age" is empty field
		<pre>DELETE FROM users
WHERE age IS NULL</pre>

		Example #5. Select all records from "users" table
		<pre>SELECT * FROM users</pre>

		Example #6.1. Select all records from "users" table in which the "name" field is equal to the value from the [[NAME]] variable.<br/><code>The parameter "Parameterize variables and resources" must be enabled, since the variable is not escaped.</code>
		<pre>SELECT * FROM users
WHERE name = [[NAME]]</pre>

		Example #6.2. Select all records from "users" table in which the "name" field is equal to the value from the [[NAME]] variable.<br/><code>The parameter "Parameterize variables and resources" must be disabled, since the variable is already escaped.</code>
		<pre>SELECT * FROM users
WHERE name = '[[NAME]]'</pre>
	</span>
</script>

<script type="text/template" id="sql-query-help-ru">
	<span style="font-size:small">
		Это поле должно содержать SQL запрос, который нужно выполнить к базе данных. Запрос может иметь несколько строк, подробнее о SQL вы можете узнать в <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://www.w3schools.com/sql');return false">руководстве</a>.<br/><br/>
		
		Пример #1. Вставить новую запись в таблицу "users", поле "name" равно "John", поле "age" равно 25
		<pre>INSERT INTO users (name, age)
VALUES ('John', 25)</pre>

		Пример #2.1. Вставить новую запись в таблицу "users", где значения полей получены из переменных.<br/><code>Параметр "Параметризировать переменные и ресурсы" должен быть включен, так как переменные не экранированы.</code>
		<pre>INSERT INTO users (name, surname, age)
VALUES ([[NAME]], [[SURNAME]], [[AGE]])</pre>

		Пример #2.2. Вставить новую запись в таблицу "users", где значения полей получены из переменных.<br/><code>Параметр "Параметризировать переменные и ресурсы" должен быть отключен, так как переменные уже экранированы.</code>
		<pre>INSERT INTO users (name, surname, age)
VALUES ('[[NAME]]', '[[SURNAME]]', [[AGE]])</pre>

		Пример #3. Удалить из таблицы "users" записи в которых "id" равен 10
		<pre>DELETE FROM users
WHERE id = 10</pre>

		Пример #4. Удалить из таблицы "users" записи, в которых "age" пустое поле
		<pre>DELETE FROM users
WHERE age IS NULL</pre>

		Пример #5. Получить все записи из таблицы "users"
		<pre>SELECT * FROM users</pre>

		Пример #6.1. Получить все записи из таблицы "users", в которых поле "name" равно значению из переменной [[NAME]].<br/><code>Параметр "Параметризировать переменные и ресурсы" должен быть включен, так как переменная не экранирована.</code>
		<pre>SELECT * FROM users
WHERE name = [[NAME]]</pre>

		Пример #6.2. Получить все записи из таблицы "users", в которых поле "name" равно значению из переменной [[NAME]].<br/><code>Параметр "Параметризировать переменные и ресурсы" должен быть отключен, так как переменная уже экранирована.</code>
		<pre>SELECT * FROM users
WHERE name = '[[NAME]]'</pre>
	</span>
</script>
