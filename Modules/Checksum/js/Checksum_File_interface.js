<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({
		id: "filePath",
		description: tr("File path"),
		default_selector: "string",
		disable_int: true,
		value_string: "",
		help: {
			description: tr("The path to the file for which needs to calculate the checksum."),
			examples: [
				{code:"{{file}}"},
				{code:"C:/test.txt"},
				{code:"D:/sqlite/your.db"},
				{code:"C:/Program Files/data.zip"}
			]
		}
	}) %>
	<%= _.template($('#input_constructor').html())({
		id: "algorithm",
		description: tr("Algorithm"),
		default_selector: "string",
		variants: [
			{value:"md5", description:"Message Digest 5 - 128-" + tr("bit hashing algorithm")},
			{value:"sha1", description:"Secure Hash Algorithm 1 - 160-" + tr("bit hashing algorithm")},
			{value:"sha-224", description:"Secure Hash Algorithm 2 - 224-" + tr("bit hashing algorithm")},
			{value:"sha-256", description:"Secure Hash Algorithm 2 - 256-" + tr("bit hashing algorithm")},
			{value:"sha-384", description:"Secure Hash Algorithm 2 - 384-" + tr("bit hashing algorithm")},
			{value:"sha-512", description:"Secure Hash Algorithm 2 - 512-" + tr("bit hashing algorithm")},
			{value:"sha3-224", description:"Secure Hash Algorithm 3 - 224-" + tr("bit hashing algorithm")},
			{value:"sha3-256", description:"Secure Hash Algorithm 3 - 256-" + tr("bit hashing algorithm")},
			{value:"sha3-384", description:"Secure Hash Algorithm 3 - 384-" + tr("bit hashing algorithm")},
			{value:"sha3-512", description:"Secure Hash Algorithm 3 - 512-" + tr("bit hashing algorithm")},
			{value:"shake-128", description:"Secure Hash Algorithm 3 - 128-" + tr("bit hashing algorithm") + " " + tr("with extendable result")},
			{value:"shake-256", description:"Secure Hash Algorithm 3 - 256-" + tr("bit hashing algorithm") + " " + tr("with extendable result")},
			{value:"keccak-224", description:"Keccak - 224-" + tr("bit hashing algorithm")},
			{value:"keccak-256", description:"Keccak - 256-" + tr("bit hashing algorithm")},
			{value:"keccak-384", description:"Keccak - 384-" + tr("bit hashing algorithm")},
			{value:"keccak-512", description:"Keccak - 512-" + tr("bit hashing algorithm")},
			{value:"blake2b-512", description:"Blake 2b - 512-" + tr("bit hashing algorithm")},
			{value:"blake2s-256", description:"Blake 2s - 256-" + tr("bit hashing algorithm")},
			{value:"blake3", description:"Blake 3 - 256-" + tr("bit hashing algorithm")},
			{value:"crc-16", description:"Cyclic Redundancy Check - 16-" + tr("bit hashing algorithm")},
			{value:"crc-32", description:"Cyclic Redundancy Check - 32-" + tr("bit hashing algorithm")},
		],
		disable_int: true,
		value_string: "sha-512",
		help: {
			description: tr("The name of the hashing algorithm. An algorithm is a set of rules used when hashing data."),
			examples: [
				{code:"md5", description:"Message Digest 5 - 128-" + tr("bit hashing algorithm")},
				{code:"sha1", description:"Secure Hash Algorithm 1 - 160-" + tr("bit hashing algorithm")},
				{code:"sha-512", description:"Secure Hash Algorithm 2 - 512-" + tr("bit hashing algorithm")},
				{code:"keccak-512", description:"Keccak - 512-" + tr("bit hashing algorithm")},
				{code:"blake3", description:"Blake 3 - 256-" + tr("bit hashing algorithm")}
			]
		}
	}) %>
	<%= _.template($('#block_start').html())({id:"Additional", name: tr("Additional settings"), description: ""}) %>
		<%= _.template($('#input_constructor').html())({
			id: "outputLength",
			description: tr("Shake result length"),
			default_selector: "int",
			variants: [
				{value:2, description:"16-" + tr("bit"), type:"int"},
				{value:4, description:"32-" + tr("bit"), type:"int"},
				{value:8, description:"64-" + tr("bit"), type:"int"},
				{value:16, description:"128-" + tr("bit"), type:"int"},
				{value:28, description:"224-" + tr("bit"), type:"int"},
				{value:32, description:"256-" + tr("bit"), type:"int"},
				{value:48, description:"384-" + tr("bit"), type:"int"},
				{value:64, description:"512-" + tr("bit"), type:"int"}
			],
			disable_string: true,
			value_number: "",
			min_number: 0,
			max_number: 999999,
			help: {
				description: tr("Optional parameter.") + " " + tr("The length of the raw checksum, used only for <code style=\"font-size:85%\">shake</code> algorithms. The length can be any integer that is greater than zero."),
				examples: [
					{code:2, description:"16-" + tr("bit")},
					{code:4, description:"32-" + tr("bit")},
					{code:8, description:"64-" + tr("bit")},
					{code:16, description:"128-" + tr("bit")},
					{code:28, description:"224-" + tr("bit")},
					{code:32, description:"256-" + tr("bit")},
					{code:48, description:"384-" + tr("bit")},
					{code:64, description:"512-" + tr("bit")},
					{code:tr("Empty string"), description:tr("Default length for the selected algorithm")}
				]
			}
		}) %>
	<%= _.template($('#block_end').html())() %>
	<%= _.template($('#input_constructor').html())({
		id: "outputEncoding",
		description: tr("Result format"),
		default_selector: "string",
		variants: [
			{value:"hex encoded", description:"5d41402abc4b2a76b9719d911017c592"},
			{value:"base64 encoded", description:"XUFAKrxLKna5cZ2REBfFkg=="},
		],
		disable_int: true,
		value_string: "hex encoded",
		help: {
			description: tr("The format in which the final checksum will be written to the specified variable."),
			examples: [
				{code: "hex encoded", description: tr("Each byte is encoded as 16-bit number converted to 2-byte string from <code style=\"font-size:85%\">00</code> to <code style=\"font-size:85%\">ff</code>. Example: <code style=\"font-size:85%\">5d41402abc4b2a76b9719d911017c592</code>")},
				{code: "</br>base64 encoded", description: tr("Raw hash data encoded in base64. Example: <code style=\"font-size:85%\">XUFAKrxLKna5cZ2REBfFkg==</code>")}
			]
		}
	}) %>
	<%= _.template($('#variable_constructor').html())({
		id: "Save",
		description: tr("Variable to save the result"),
		default_variable: "FILE_CHECKSUM",
		help: {
			description: tr("Variable in which, after successful execution of the action, the final checksum will be written."),
			examples: [
				{code: "3610a686", description: "<code style=\"font-size:85%\">crc-32</code> - <code style=\"font-size:85%\">hex encoded</code>"},
				{code: "hmQ3y3p5S84rcnrMA2LuJw==", description: "<code style=\"font-size:85%\">md4</code> - <code style=\"font-size:85%\">base64 encoded</code>"},
				{code: "5d41402abc4b2a76b9719d911017c592", description: "<code style=\"font-size:85%\">md5</code> - <code style=\"font-size:85%\">hex encoded</code>"},
				{code: "aaf4c61ddcc5e8a2dabede0f3b482cd9aea9434d", description: "<code style=\"font-size:85%\">sha1</code> - <code style=\"font-size:85%\">hex encoded</code>"},
				{code: "RVJOxFS8x9S490NQxKTmKAn8tJvCnfYuYbafpA==", description: "<code style=\"font-size:85%\">keccak-224</code> - <code style=\"font-size:85%\">base64 encoded</code>"}
			]
		}
	}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Calculate the checksum of the specified file.</div>
	<div class="tr tooltip-paragraph-fold">This action will return string containing the final checksum in the specified format.</div>
	<div class="tr tooltip-paragraph-fold">Checksum is the value obtained as a result of data processing by a hash function, this function works according to a certain set of rules named an algorithm.</div>
	<div class="tooltip-paragraph-fold"><span class="tr">Checksum for two identical</span> <span class="tr">files</span> <span class="tr">will always be the same, and if the data is the slightest difference, the checksum will be different.</span></div>
	<div class="tr tooltip-paragraph-fold">In most cases, the checksum is used to check the integrity of data during transmission and storage, as well as to quickly compare a set of data. More information about checksums can be found in the <a href="#" onclick="BrowserAutomationStudio_OpenUrl('https://en.wikipedia.org/wiki/Checksum');return false">Wiki</a>.</div>
	<div class="tr tooltip-paragraph-fold">Result formats:</div>
	<div class="tooltip-paragraph-fold">hex encoded - <span class="tr">Each byte is encoded as 16-bit number converted to 2-byte string from <code style="font-size:85%">00</code> to <code style="font-size:85%">ff</code>. Example: <code style="font-size:85%">5d41402abc4b2a76b9719d911017c592</code></span>.</div>
	<div class="tooltip-paragraph-fold">base64 encoded - <span class="tr">Raw hash data encoded in base64. Example: <code style="font-size:85%">XUFAKrxLKna5cZ2REBfFkg==</code></span>.</div>
	<div class="tr tooltip-paragraph-fold">For <code>shake</code> algorithms, you can specify the length of the raw checksum by filling in the "Shake result length" parameter located in the additional settings.</div>
	<div class="tr tooltip-paragraph-fold">You can get full list of supported hashing algorithms using the "Get list of algorithms" action.</div>
	<div class="tr tooltip-paragraph-fold">Algorithm <code>blake2b-512</code> is compatible with the NaCl and Sodium libraries.</div>
	<div class="tr tooltip-paragraph-fold">You can calculate the checksum for the finished string using the "String checksum" action.</div>
	<div class="tr tooltip-paragraph-fold">If a resource is specified in the "File path" parameter, resource location will be used.</div>
	<div class="tr tooltip-paragraph-last-fold">If an error occurred while execute action, the thread will stop with fail message. If you want to continue thread, use "Ignore errors" action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", use_timeout:true, visible:true}) %>
