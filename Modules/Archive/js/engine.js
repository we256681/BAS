function Archive_Unpack(){
	var archivePath = _function_argument("ArchivePath");
	var destinationPath = _function_argument("DestinationPath");
	var archiveType = Archive_Сlean(_function_argument("ArchiveType"));
	var filesList = _to_arr(_avoid_nilb(_function_argument("ListOfFiles"), []));
	var password = _function_argument("password");
	var timeout = _avoid_nilb(_function_argument("Timeout"), 60000);
	
	VAR_ARCHIVE_NODE_PARAMETERS = {archivePath:archivePath, destinationPath:destinationPath, archiveType:archiveType, filesList:filesList, password:password, lang:_K};
	
	_embedded("Archive_Unpack", "Node", "18.10.0", "ARCHIVE_NODE_PARAMETERS", timeout)!
};
function Archive_ArchiveFolder(){
	var folderPath = _function_argument("FolderPath");
	var destinationPath = _function_argument("DestinationPath");
	var archiveType = Archive_Сlean(_function_argument("ArchiveType"));
	var timeout = _avoid_nilb(_function_argument("Timeout"), 60000);
	
	VAR_ARCHIVE_NODE_PARAMETERS = {folderPath:folderPath, destinationPath:destinationPath, archiveType:archiveType, lang:_K};
	
	_embedded("Archive_ArchiveFolder", "Node", "18.10.0", "ARCHIVE_NODE_PARAMETERS", timeout)!
};
function Archive_ArchiveFiles(){
	var destinationPath = _function_argument("DestinationPath");
	var archiveType = Archive_Сlean(_function_argument("ArchiveType"));
	var file1 = _function_argument("File1");
	var file2 = _function_argument("File2");
	var file3 = _function_argument("File3");
	var filesList = _function_argument("ListOfFiles");
	var timeout = _avoid_nilb(_function_argument("Timeout"), 60000);
	
	filesList = [_avoid_nil(file1), _avoid_nil(file2), _avoid_nil(file3)].concat(_to_arr(_avoid_nilb(filesList, []))).filter(function(file){return !_is_nilb(file)});
	
	if(filesList.length < 1){
		fail(_K=="ru" ? ("Файлы требующие архивации не указаны") : ("Files requiring archiving are not specified"));
	};
	
	VAR_ARCHIVE_NODE_PARAMETERS = {filesList:filesList, destinationPath:destinationPath, archiveType:archiveType, lang:_K};
	
	_embedded("Archive_ArchiveFiles", "Node", "18.10.0", "ARCHIVE_NODE_PARAMETERS", timeout)!
};
function Archive_GetFileList(){
	var archivePath = _function_argument("ArchivePath");
	var archiveType = Archive_Сlean(_function_argument("ArchiveType"));
	var timeout = _avoid_nilb(_function_argument("Timeout"), 60000);
	
	VAR_ARCHIVE_NODE_PARAMETERS = {archivePath:archivePath, archiveType:archiveType, lang:_K};
	
	_embedded("Archive_GetFileList", "Node", "18.10.0", "ARCHIVE_NODE_PARAMETERS", timeout)!
	
	_function_return(VAR_ARCHIVE_NODE_PARAMETERS)
};
function Archive_Сlean(data){
	return _avoid_nil(data).toString().trim();
};