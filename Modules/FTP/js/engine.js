_FTP_PROTOCOL = "";
_FTP_CONFIG = {host:"",port:"",username:"",password:""};
_FTP_CONNECTION_ID = rand(10);
_FTP_CONNECTION_TIMEOUT = 5*60*1000;

function FTP_Config(protocol, host, port, username, password, timeout){
    _FTP_PROTOCOL = protocol;
    _FTP_CONFIG["host"] = host;
    _FTP_CONFIG["port"] = port=="auto" ? (protocol=="FTP" ? "21" : "22") : port;
    _FTP_CONFIG["username"] = username;
	_FTP_CONFIG["user"] = username;
    _FTP_CONFIG["password"] = password;
	_FTP_CONFIG["pass"] = password;
	_FTP_CONNECTION_TIMEOUT = timeout*1000;
};
function FTP_ReadFile(){
	var file_path = FTP_FormatPath(_function_argument("FilePath"));
	var base64 = _function_argument("base64");
	var timeout = _function_argument("Timeout");
	
	FTP_CheckProtocol();
	VAR_FTP_NODE_PARAMETERS = [file_path, base64, _FTP_CONNECTION_ID, _FTP_CONFIG, _FTP_CONNECTION_TIMEOUT];
	
	_if(_FTP_PROTOCOL=="SFTP" || _FTP_PROTOCOL=="SSH",function(){
		_embedded("ReadFileSFTP", "Node", "18.10.0", "FTP_NODE_PARAMETERS", timeout)!
	})!
	
	_if(_FTP_PROTOCOL=="FTP",function(){
		_embedded("ReadFileFTP", "Node", "18.10.0", "FTP_NODE_PARAMETERS", timeout)!
	})!
	
	_function_return(VAR_FTP_NODE_PARAMETERS);
};
function FTP_WriteFile(){
	var file_path = FTP_FormatPath(_function_argument("FilePath"));
	var value = _function_argument("Value");
	var base64 = _function_argument("base64");
	var ending_symbol = _function_argument("EndingSymbol");
	var append = _function_argument("Append");
	var timeout = _function_argument("Timeout");
	
	FTP_CheckProtocol();
	VAR_FTP_NODE_PARAMETERS = [file_path, value, ending_symbol, base64, append, _FTP_CONNECTION_ID, _FTP_CONFIG, _FTP_CONNECTION_TIMEOUT];
	
	_if(_FTP_PROTOCOL=="SFTP" || _FTP_PROTOCOL=="SSH",function(){
		_embedded("WriteFileSFTP", "Node", "18.10.0", "FTP_NODE_PARAMETERS", timeout)!
	})!
	
	_if(_FTP_PROTOCOL=="FTP",function(){
		_embedded("WriteFileFTP", "Node", "18.10.0", "FTP_NODE_PARAMETERS", timeout)!
	})!
};
function FTP_Download(){
	var file_path = FTP_FormatPath(_function_argument("FilePath"));
	var destination_path = FTP_FormatPath(_function_argument("DestinationPath"));
	var verify_checksums = _function_argument("VerifyChecksums");
	var timeout = _function_argument("Timeout");
	
	FTP_CheckProtocol();
    VAR_FTP_NODE_PARAMETERS = [file_path, destination_path, verify_checksums, _FTP_CONNECTION_ID, _FTP_CONFIG, _FTP_CONNECTION_TIMEOUT, _FTP_PROTOCOL];
	
	_if(_FTP_PROTOCOL=="SFTP" || _FTP_PROTOCOL=="SSH", function(){
        _embedded("DownloadSFTP", "Node", "18.10.0", "FTP_NODE_PARAMETERS", timeout)!
    })!

    _if(_FTP_PROTOCOL=="FTP", function(){
        _embedded("DownloadFTP", "Node", "18.10.0", "FTP_NODE_PARAMETERS", timeout)!
    })!
};
function FTP_Upload(){
    var file_path = FTP_FormatPath(_function_argument("FilePath"));
	var destination_path = FTP_FormatPath(_function_argument("DestinationPath"));
	var verify_checksums = _function_argument("VerifyChecksums");
	var timeout = _function_argument("Timeout");
	
	FTP_CheckProtocol();
    VAR_FTP_NODE_PARAMETERS = [file_path, destination_path, verify_checksums, _FTP_CONNECTION_ID, _FTP_CONFIG, _FTP_CONNECTION_TIMEOUT, _FTP_PROTOCOL];

    _if(_FTP_PROTOCOL=="SFTP" || _FTP_PROTOCOL=="SSH", function(){
        _embedded("UploadSFTP", "Node", "18.10.0", "FTP_NODE_PARAMETERS", timeout)!
    })!

    _if(_FTP_PROTOCOL=="FTP", function(){
        _embedded("UploadFTP", "Node", "18.10.0", "FTP_NODE_PARAMETERS", timeout)!
    })!
};
function FTP_CheckExist(){
	var file_path = FTP_FormatPath(_function_argument("FilePath"));
	var timeout = _function_argument("Timeout");
	
	FTP_CheckProtocol();
	VAR_FTP_NODE_PARAMETERS = [file_path, _FTP_CONNECTION_ID, _FTP_CONFIG, _FTP_CONNECTION_TIMEOUT];
	
	_if(_FTP_PROTOCOL=="SFTP" || _FTP_PROTOCOL=="SSH",function(){
		_embedded("CheckExistSFTP", "Node", "18.10.0", "FTP_NODE_PARAMETERS", timeout)!
	})!
	
	_if(_FTP_PROTOCOL=="FTP",function(){
		_embedded("CheckExistFTP", "Node", "18.10.0", "FTP_NODE_PARAMETERS", timeout)!
	})!
	
	_function_return(VAR_FTP_NODE_PARAMETERS);
};
function FTP_GetInfo() {
    var file_path = FTP_FormatPath(_function_argument("FilePath"));
	var timeout = _function_argument("Timeout");
	
	FTP_CheckProtocol();
    VAR_FTP_NODE_PARAMETERS = [file_path, _FTP_CONNECTION_ID, _FTP_CONFIG, _FTP_CONNECTION_TIMEOUT];

    _if(_FTP_PROTOCOL=="SFTP" || _FTP_PROTOCOL=="SSH", function(){
        _embedded("GetInfoSFTP", "Node", "18.10.0", "FTP_NODE_PARAMETERS", timeout)!
    })!

    _if(_FTP_PROTOCOL=="FTP", function(){
        _embedded("GetInfoFTP", "Node", "18.10.0", "FTP_NODE_PARAMETERS", timeout)!
    })!

    _function_return(VAR_FTP_NODE_PARAMETERS);
};
function FTP_Create(){
	var file_path = FTP_FormatPath(_function_argument("FilePath"));
	var file_type = _function_argument("isFile") ? "f" : "d";
	var timeout = _function_argument("Timeout");
	
	FTP_CheckProtocol();
	VAR_FTP_NODE_PARAMETERS = [file_path, file_type, _FTP_CONNECTION_ID, _FTP_CONFIG, _FTP_CONNECTION_TIMEOUT];
	
	_if(_FTP_PROTOCOL=="SFTP" || _FTP_PROTOCOL=="SSH",function(){
		_embedded("CreateSFTP", "Node", "18.10.0", "FTP_NODE_PARAMETERS", timeout)!
	})!
	
	_if(_FTP_PROTOCOL=="FTP",function(){
		_embedded("CreateFTP", "Node", "18.10.0", "FTP_NODE_PARAMETERS", timeout)!
	})!
};
function FTP_Delete(){
	var file_path = FTP_FormatPath(_function_argument("FilePath"));
	var timeout = _function_argument("Timeout");
	
	FTP_CheckProtocol();
	VAR_FTP_NODE_PARAMETERS = [file_path, _FTP_CONNECTION_ID, _FTP_CONFIG, _FTP_CONNECTION_TIMEOUT];
	
	_if(_FTP_PROTOCOL=="SFTP" || _FTP_PROTOCOL=="SSH",function(){
		_embedded("DeleteSFTP", "Node", "18.10.0", "FTP_NODE_PARAMETERS", timeout)!
	})!
	
	_if(_FTP_PROTOCOL=="FTP",function(){
		_embedded("DeleteFTP", "Node", "18.10.0", "FTP_NODE_PARAMETERS", timeout)!
	})!
};
function FTP_Move(){
	var file_path = FTP_FormatPath(_function_argument("FilePath"));
	var new_file_path = FTP_FormatPath(_function_argument("NewFilePath"));
	var timeout = _function_argument("Timeout");
	
	FTP_CheckProtocol();
	VAR_FTP_NODE_PARAMETERS = [file_path, new_file_path, _FTP_CONNECTION_ID, _FTP_CONFIG, _FTP_CONNECTION_TIMEOUT];
	
	_if(_FTP_PROTOCOL=="SFTP" || _FTP_PROTOCOL=="SSH",function(){
		_embedded("MoveSFTP", "Node", "18.10.0", "FTP_NODE_PARAMETERS", timeout)!
	})!
	
	_if(_FTP_PROTOCOL=="FTP",function(){
		_embedded("MoveFTP", "Node", "18.10.0", "FTP_NODE_PARAMETERS", timeout)!
	})!
};
function FTP_Copy(){
	var file_path = FTP_FormatPath(_function_argument("FilePath"));
	var copy_path = FTP_FormatPath(_function_argument("CopyPath"));
	var timeout = _function_argument("Timeout");
	
	FTP_CheckProtocol();
	VAR_FTP_NODE_PARAMETERS = [file_path, copy_path, _FTP_CONNECTION_ID, _FTP_CONFIG, _FTP_CONNECTION_TIMEOUT];
	
	_if(_FTP_PROTOCOL=="SFTP" || _FTP_PROTOCOL=="SSH",function(){
		_embedded("CopySFTP", "Node", "18.10.0", "FTP_NODE_PARAMETERS", timeout)!
	})!
	
	_if(_FTP_PROTOCOL=="FTP",function(){
		_embedded("CopyFTP", "Node", "18.10.0", "FTP_NODE_PARAMETERS", timeout)!
	})!
};
function FTP_Search(){
	var folder_path = FTP_FormatPath(_function_argument("FolderPath"));
	var mask = _function_argument("Mask");
	var recursive = _function_argument("Recursive");
	var search_files = _function_argument("SearchFiles");
	var search_folders = _function_argument("SearchFolders");
	var timeout = _function_argument("Timeout");
	
	FTP_CheckProtocol();
	VAR_FTP_NODE_PARAMETERS = [folder_path, mask, recursive, search_files, search_folders, _FTP_CONNECTION_ID, _FTP_CONFIG, _FTP_CONNECTION_TIMEOUT];
	
	_if(_FTP_PROTOCOL=="SFTP" || _FTP_PROTOCOL=="SSH",function(){
		_embedded("SearchSFTP", "Node", "18.10.0", "FTP_NODE_PARAMETERS", timeout)!
	})!
	
	_if(_FTP_PROTOCOL=="FTP",function(){
		_embedded("SearchFTP", "Node", "18.10.0", "FTP_NODE_PARAMETERS", timeout)!
	})!
	
	_function_return(VAR_FTP_NODE_PARAMETERS);
};
function FTP_FileToList(){
	var file_path = FTP_FormatPath(_function_argument("FilePath"));
	var timeout = _function_argument("Timeout");
	
	_call_function(FTP_ReadFile,{"FilePath":file_path,"base64":false,"Timeout":timeout})!
	VAR_FTP_NODE_PARAMETERS = _result_function();
	
	_function_return(VAR_FTP_NODE_PARAMETERS.split(/\r?\n/));
};
function FTP_ListToFile(){
	var file_path = FTP_FormatPath(_function_argument("FilePath"));
    var value = _function_argument("Value").join("\r\n");
    var ending_symbol = _function_argument("EndingSymbol");
    var append = _function_argument("Append");
	var timeout = _function_argument("Timeout");
	
	_call_function(FTP_WriteFile,{"FilePath":file_path,"Value":value,"EndingSymbol":ending_symbol,"base64":false,"Append":append,"Timeout":timeout})!
	_result_function();
};
function FTP_RunCommand(){
	var сommand = _function_argument("Command");
	var working_folder = _function_argument("WorkingFolder");
	var wait_finish = _function_argument("WaitFinish");
	var pty = true;
	var timeout = _function_argument("Timeout");
	
	сommand = (working_folder.length>0 ? ("cd " + working_folder + "\n") : "") + сommand;
	
	FTP_CheckProtocol();
	
	if(_FTP_PROTOCOL!="SSH"){
		fail(_K=="ru" ? ("Выполнить команду можно только через SSH протокол") : ("The command can only be executed via SSH protocol"));
	};
	
	VAR_FTP_NODE_PARAMETERS = [сommand, wait_finish, pty, _FTP_CONNECTION_ID, _FTP_CONFIG, _FTP_CONNECTION_TIMEOUT];
	
	_embedded("RunCommandSSH", "Node", "18.10.0", "FTP_NODE_PARAMETERS", timeout)!
	
	_function_return(VAR_FTP_NODE_PARAMETERS);
}
function FTP_CalculateChecksum(){
	var file_path = _function_argument("FilePath");
	var timeout = _function_argument("Timeout");
	
	FTP_CheckProtocol();
	
	if(_FTP_PROTOCOL!="SSH"){
		fail(_K=="ru" ? ("Рассчитать контрольную сумму можно только через SSH протокол") : ("The checksum can be calculated only via the SSH protocol"));
	};
	
	VAR_FTP_NODE_PARAMETERS = [file_path, _FTP_CONNECTION_ID, _FTP_CONFIG, _FTP_CONNECTION_TIMEOUT];
	
	_embedded("CalculateChecksumSSH", "Node", "18.10.0", "FTP_NODE_PARAMETERS", timeout)!
	
	_function_return(VAR_FTP_NODE_PARAMETERS);
};
function FTP_Close(){
	VAR_FTP_NODE_PARAMETERS = _FTP_CONNECTION_ID;
	_embedded("CloseFTP", "Node", "18.10.0", "FTP_NODE_PARAMETERS", 60000)!
};
function FTP_CheckProtocol(){
	if(_FTP_PROTOCOL!="SFTP" && _FTP_PROTOCOL!="SSH" && _FTP_PROTOCOL!="FTP"){
		fail(_K=="ru" ? ("Настройка FTP/SSH не выполнена или выполнена неправильно") : ("FTP/SSH configuration failed or incorrect"));
	};
};
function FTP_FormatPath(path){
	return path.split("\\").join("/");
};
