<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"string", description: tr("String"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("The string from which to extract email addresses."),examples:[{code:tr("Suggestions for cooperation you can send to email: cooperation@marketplace.com")},{code:tr("</br>Test text - test@gmail.com. Just example text - example@gmail.com.")},{code:tr("</br>Proposals for advertising, you can send to email: ad@site.com</br>Suggestions for improving the site, you can send to email: feedback@site.com</br>To get help in solving problems with the site, you can write to the email: support@site.com")}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "EMAILS_LIST", help: {description: tr("Variable in which, after successful execution of the action, the list of email addresses will be written."), examples:[{code:"[\"cooperation@marketplace.com\"]"},{code:"[\"test@gmail.com\", \"example@gmail.com\"]"},{code:"[\"ad@site.com\", \"feedback@site.com\", \"support@site.com\"]"}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Extract all email addresses from string.</div>
	<div class="tr tooltip-paragraph-fold">This action will return a list containing the email addresses obtained from the string.</div>
	<div class="tr tooltip-paragraph-fold">For example, if the string <code>"Test text - test@gmail.com. Just example text - example@gmail.com."</code> is specified, then the action will return <code>["test@gmail.com", "example@gmail.com"]</code>.</div>
	<div class="tr tooltip-paragraph-last-fold">The resulting list can be processed using actions from the "List" module.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
