function Checksum_String(){
	_embedded("Checksum_String", "Node", "18.10.0", "CHECKSUM_NODE_PARAMETERS", Checksum_PrepareArguments(_function_arguments()))!
	
	_function_return(VAR_CHECKSUM_NODE_PARAMETERS);
};
function Checksum_File(){
	_embedded("Checksum_File", "Node", "18.10.0", "CHECKSUM_NODE_PARAMETERS", Checksum_PrepareArguments(_function_arguments()))!
	
	_function_return(VAR_CHECKSUM_NODE_PARAMETERS);
};
function Checksum_GetAlgorithmsList(){
	_embedded("Checksum_GetAlgorithmsList", "Node", "18.10.0", "CHECKSUM_NODE_PARAMETERS", 60000)!
	
	_function_return(VAR_CHECKSUM_NODE_PARAMETERS);
};
function Checksum_PrepareArguments(args){
	var timeout = _avoid_nilb(args.timeout, 60000);
	if(args.hasOwnProperty('timeout')){
		delete args.timeout;
	};
	for(var key in args){
		var arg = args[key];
		if(key !== 'input'){
			arg = key === 'outputLength' ? Number(_avoid_nil(arg, 64)) : _avoid_nil(arg).toString().trim();
			if(['algorithm','inputEncoding','outputEncoding'].indexOf(key) > -1 && _is_nilb(arg)){
				arg = key=='algorithm' ? 'sha512' : (key=='inputEncoding' ? 'utf-8' : 'hex');
			}else if(key === 'algorithm'){
				var labels = {
					"crc-16":"crc16",
					"crc-32":"crc32",
					"sha-224":"sha224",
					"sha-256":"sha256",
					"sha-384":"sha384",
					"sha-512":"sha512",
					"shake-128":"shake128",
					"shake-256":"shake256",
					"keccak-224":"keccak224",
					"keccak-256":"keccak256",
					"keccak-384":"keccak384",
					"keccak-512":"keccak512",
					"blake2b-512":"blake2b512",
					"blake2s-256":"blake2s256",
				};
				if(labels.hasOwnProperty(arg)){
					arg = labels[arg];
				};
			};
			if(key === 'outputEncoding' && _ends_with(arg, ' encoded')){
				arg = arg=='hex encoded' ? 'hex' : 'base64';
			};
			args[key] = arg;
		};
	};
	args.lang = _K;
	VAR_CHECKSUM_NODE_PARAMETERS = args;
	return timeout;
};