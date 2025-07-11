<div class="container-fluid">
	<%= _.template($('#variable_constructor').html())({
		id: "Save",
		description: tr("Variable to save the result"),
		default_variable: "SUPPORTED_ALGORITHMS_LIST",
		help: {
			description: tr("Variable in which, after successful execution of the action, the list of supported hashing algorithms will be written."),
			examples: [
				{code: "[\"blake3\",\"crc32\",\"keccak512\",\"sha512\"]"}
			]
		}
	}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get list of supported hashing algorithms.</div>
	<div class="tr tooltip-paragraph-fold">This action will return a full list of the names of the supported hashing algorithms.</div>
	<div class="tooltip-paragraph-fold"><span class="tr">Example</span>: <code>["blake3","crc32","keccak512","sha512"]</code></div>
	<div class="tr tooltip-paragraph-fold">The resulting list can be processed using actions from the "List" module.</div>
	<div class="tr tooltip-paragraph-last-fold">The resulting names of the algorithms can be used in the "String checksum" and "File checksum" actions.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
