<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({id:"Template", description:tr("Template"), default_selector: "expression", disable_int:true, disable_editor:true, disable_string:true, use_textarea:true, size: 8, disable_type_chooser:true,textarea_height:80, help: {description: tr("Variable value"), examples:[{code:"any string",description:tr("Any string will work")},{code:"[[LOGIN]]:[[PASSWORD]]",description:tr("Concat 2 variables")},{code:"{Hi|Hello}, John!",description:tr("Spintax")},{code:"&lt;EMaleName&gt;",description:tr("Will be replaced with male name")},{code:"{a{1|2|3}|}",description:tr("a1, a2, a3 or nothing")}]}}) %>
	<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable To Save Result"), default_variable: "TEMPLATE_RESULT", help: {description: tr("The name of the variable to be created or modified.")}}) %>
	<div>
		<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check2">
			<input type="checkbox" id="Check2" checked="checked"/> <label for="Check2" class="tr">Expand spintax.</label>
		</span>
	</div>
	<div>
		<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check3">
			<input type="checkbox" id="Check3" checked="checked"/> <label for="Check3" class="tr">Expand variables and resources.</label>
		</span>
	</div>
	<div> 
		<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check">
			<input type="checkbox" id="Check"/> <label for="Check" class="tr">Additional processing. Use this option if you want to load template from file or variable.</label>
		</span>
	</div>
</div>
<div class="tooltipinternal">
  <div class="tr tooltip-paragraph-first-fold">Create new variable or update existing with multiline text. Can expand resources, variables, spintax, may be used to load template from file or dynamically generate it. This action suites best if you want to generate post text.</div>
  <div class="tr tooltip-paragraph-fold">This action gets text from "Template" multiline textbox and saves it to variable defined in "Variable To Save Result". Thats it, unless text doesn't contain special values. Here are list of them:</div>
  <div class="tr tooltip-paragraph-fold">Variables [[VARIABLE]] will be expanded to variable value.</div>
  <div class="tr tooltip-paragraph-fold">Resources {{resource}} also will be expanded to resource value.</div>
  <div class="tr tooltip-paragraph-fold">Spintax {value1|value2} will be expanded to value1 or value2.</div>
  <div class="tr tooltip-paragraph-fold">&lt;AnyLetter&gt; - will be converted to any letter.</div>
  <div class="tr tooltip-paragraph-fold">&lt;AnyDigit&gt; - will be converted to any digit.</div>
  <div class="tr tooltip-paragraph-fold">&lt;ELowVow&gt; - same as {a|e|i|o|u}</div>
  <div class="tr tooltip-paragraph-fold">&lt;EUpVow&gt; - same as {a|e|i|o|u}</div>
  <div class="tr tooltip-paragraph-fold">&lt;ELowCons&gt; - same as {b|c|d|f|g|h|j|k|l|m|n|p|q|r|s|t|v|w|x|y|z}</div>
  <div class="tr tooltip-paragraph-fold">&lt;EUpCons&gt; - same as {B|C|D|F|G|H|J|K|L|M|N|P|Q|R|S|T|V|W|X|Y|Z}</div>
  <div class="tr tooltip-paragraph-fold">&lt;EUp&gt; - same as {&lt;EUpVow&gt;|&lt;EUpCons&gt;}</div>
  <div class="tr tooltip-paragraph-fold">&lt;ELow&gt; - same as {&lt;ELowVow&gt;|&lt;ELowCons&gt;}</div>
  <div class="tr tooltip-paragraph-fold">&lt;EFemName&gt; - english female names starting from capital letter.</div>
  <div class="tr tooltip-paragraph-fold">&lt;EFemNameLow&gt; - english female names.</div>
  <div class="tr tooltip-paragraph-fold">&lt;EMaleName&gt; - english male names starting from capital letter.</div>
  <div class="tr tooltip-paragraph-fold">&lt;EMaleNameLow&gt; - english male names.</div>
  <div class="tr tooltip-paragraph-fold">&lt;ESurname&gt; - english surnames starting from capital letter.</div>
  <div class="tr tooltip-paragraph-fold">&lt;ESurnameLow&gt; - english surnames.</div>
  <div class="tr tooltip-paragraph-last-fold">If "Additional processing" is checked, than this action will process dynamic templates, i.e template will be taken from variable. Consider following example: you have file with content "{Hello|Hi}, &lt;EMaleName&gt;{!|}" and want to expand it with BAS. At first you have to load file to variable [[FILE_CONTENT]]. If you put [[FILE_CONTENT]] to template action and don't set "Additional processing" flag, that result will be the same as original file content. But if you set this flag, BAS will expand [[FILE_CONTENT]] first and then expend result again, finally you will get something like this "Hello, John!".</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>