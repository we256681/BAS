<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"string", description: tr("String"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("The string to insert the substring into."),examples:[{code:tr("Just sample text")},{code:"Test text"},{code:"string"}]} }) %>
<%= _.template($('#input_constructor').html())({id:"index", description:tr("Index") + ". " + tr("Can be blank"), default_selector: "int", disable_string:true, value_number: "", min_number:-999999, max_number:999999, help: {description: tr("The character index after which the substring will be inserted.") + " " + tr("If negative, indicates an offset from the end of the string."),examples:[{code:1, description:tr("After the second character")},{code:0, description:tr("After the first character")},{code:-1, description:tr("After the last character")},{code:tr("Empty string"), description:tr("To the beginning of the string")}]} }) %>
<%= _.template($('#input_constructor').html())({id:"substring", description: tr("Insertable substring"), default_selector: "string", value_string: "", help: {description: tr("The substring to insert."),examples:[{code:tr("not")},{code:"good"},{code:"New"},{code:tr("Empty string"), description:tr("Don't insert a substring")}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "RESULT_STRING", help: {description: tr("Variable in which, after successful execution of the action, the final string will be written."), examples:[{code:tr("Just not sample text")},{code:"Test good text"},{code:"New string"}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Insert a substring into a string.</div>
	<div class="tr tooltip-paragraph-fold">This action will return the string resulting from the substring insertion.</div>
	<div class="tr tooltip-paragraph-fold">For example, if the string <code>"Just string"</code> is specified, the substring <code>"example "</code> and index 5, then the action will return <code>"Just example string"</code>.</div>
	<div class="tr tooltip-paragraph-fold">If the index is a negative number, then it is interpreted as the number of characters from the end of the string.</div>
	<div class="tr tooltip-paragraph-last-fold">If you need to not only insert a substring, but also delete part of the original string, then use the "Remove string part" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
