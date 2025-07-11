<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"string", description: tr("String"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("The string from which to extract phone numbers."),examples:[{code:tr("Hello, my number is +1 323 123-4567")},{code:tr("</br>Business enquiries/press: Alexandra +7 900 000-00-00")},{code:tr("</br>PR: Maria +7(900)000-00-01 | Agent 1: Valeria +7(900)000-00-02 | Agent 2: Ekaterina +7(900)00-00-03")}]} }) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "PHONE_NUMBERS_LIST", help: {description: tr("Variable in which, after successful execution of the action, the list of phone numbers will be written."), examples:[{code:"[\"+1 323 123-4567\"]"},{code:"[\"+7 900 000-00-00\"]"},{code:"[\"+7(900)000-00-01\", \"+7(900)000-00-02\", \"+7(900)00-00-03\"]"}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Extract all phone numbers from the string.</div>
	<div class="tr tooltip-paragraph-fold">This action will return a list containing the phone numbers obtained from the string.</div>
	<div class="tr tooltip-paragraph-fold">For example, if the string <code>"Test text - 89001112233. Just example text - +7 900 999 4455."</code> is specified, then the action will return <code>["89001112233", "+7 900 999 4455"]</code>.</div>
	<div class="tr tooltip-paragraph-last-fold">The resulting list can be processed using actions from the "List" module.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
