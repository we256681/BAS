<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"string", description:tr("String"), default_selector: "string", disable_int:true, help: {description: tr("Depending on mode data parameter is encoded string which needs to be decoded, or ordinary string containing the Url component which needs to be encoded."), examples:[{code:"?x=test",description:tr("Arbitrary string. Works with encode mode")},{code:"%3Fx%3Dtest",description:tr("Encoded string. Works with decode mode")}]}}) %>
<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable to save the result"), default_variable: "URL_COMPONENT_ENCODING_RESULT", help: {description: tr("Variable name with result. It will be encoded string in encode mode and decode result as string in decode mode.")}}) %>
<div class="col-xs-12">
	<form class="form-horizontal">
		<div class="form-group">
			<div class="col-xs-2" style="width: auto;">
				<div class="input-group">
					<span data-preserve="true" data-preserve-type="select" data-preserve-id="Select">
						<select class="form-control input-sm" id="Select" placeholder="Is Encode">
							<option value="encode" selected="selected">encode</option>
							<option value="decode">decode</option>
						</select>
					</span>
				</div>
			</div>
			<label class="control-label text-right tr" style="padding-top:5px !important;">Encode or decode</label>
		</div>
	</form>
</div>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Encodes or decodes string containing the Url component.</div>
	<div class="tr tooltip-paragraph-fold">This action can work with in two modes: encode and decode. Modes can be switched with "Encode or decode" parameter. Depending on mode data parameter is encoded string which needs to be decoded, or ordinary string containing the Url component which needs to be encoded.</div>
	<div class="tr tooltip-paragraph-fold">For example, if the string <code>"?x=test"</code> is specified and encode mode is activated, the action will return <code>"%3Fx%3Dtest"</code>. And if the string <code>"%D1%82%D0%B5%D1%81%D1%82"</code> is specified and decode mode is activated, then the action will return <code>"тест"</code>.</div>
	<div class="tr tooltip-paragraph-fold">You can get a list of links from a string using the "Extract all links" action from the "String" module.</div>
	<div class="tr tooltip-paragraph-last-fold">You can parse a link to items using the "Parse URL" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
