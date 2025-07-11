<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"string", description: tr("String"), default_selector: "string", disable_int:true, value_string: "", help: {description: tr("The string to be latinized."),examples:[{code:"ỆᶍǍᶆṔƚÉ áéíóúýčďěňřšťžů"},{code:"Привет мир!"},{code:"Просто пример текста"}]} }) %>
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check">
	<input type="checkbox" checked="checked" id="Check" style="margin-left:25px"/> <label for="Check" class="tr">Latinize Cyrillic</label>
</span>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "LATINIZED_STRING", help: {description: tr("Variable in which, after successful execution of the action, the converted string will be written."), examples:[{code:"ExAmPlE aeiouycdenrstzu"},{code:"Privet mir!"},{code:"Prosto primer teksta"}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Convert all letters to Latin.</div>
	<div class="tr tooltip-paragraph-fold">This action will return a string in which all letters will be latinized, and the rest of the characters will remain unchanged.</div>
	<div class="tr tooltip-paragraph-fold">For example, the string <code>"ỆᶍǍᶆṔƚÉ áéíóúýčďěňřšťžů"</code> will be converted to the string <code>"ExAmPlE aeiouycdenrstzu"</code>, and the string <code>"Привет мир!"</code> will be converted to the string <code>"Privet mir!"</code>.</div>
	<div class="tr tooltip-paragraph-last-fold">By default, the Cyrillic is also latinized, but this can be changed by disabling the "Latinize Cyrillic" parameter.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
