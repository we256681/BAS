<div class="container-fluid">
<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check">
	<div class="col-xs-12">
		<form class="form-horizontal">
			<div class="form-group">
				<div class="col-xs-2" style="width: auto;">
					<div class="input-group">
						<span data-preserve="true" data-preserve-type="select" data-preserve-id="Select">
							<select class="form-control input-sm" id="Select">
								<option value="CarriageReturn" selected="selected" class="tr">Carriage return and line break (\r\n)</option>
								<option value="EOL" class="tr">Only line break (\n)</option>
							</select>
						</span>
					</div>
				</div>
			</div>
		</form>
	</div>
</span>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "LINE_BREAK", help: {description: tr("Variable in which, after successful execution of the action, a string containing a line break will be written."), examples:[{code:"\\r\\n",description:tr("Carriage return and line break")},{code:"\\n",description:tr("Line break")}]} }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Create a string containing a line break symbol.</div>
	<div class="tr tooltip-paragraph-fold">This action will return a string containing a line break. You can read more about line breaks in <a href="#" onclick="BrowserAutomationStudio_OpenUrl('https://en.wikipedia.org/wiki/Newline');return false">Wiki</a>.</div>
	<div class="tr tooltip-paragraph-fold">If the parameter "Carriage return" is activated, then a carriage return will be added before the line break.</div>
	<div class="tr tooltip-paragraph-last-fold">The result of this action can be used to split the text into multiple lines.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
