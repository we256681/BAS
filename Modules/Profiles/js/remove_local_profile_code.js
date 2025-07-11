ProfilePath = (<%= profile %>);
if(ProfilePath == "")
	ProfilePath = _get_profile()

_if(ProfilePath == _get_profile(), function(){
	var Params = {};
	Params["ProfilePath"] = "<Incognito>"
	_settings(Params)!
})!

_do(function(){
	if(_iterator() > 30)
		fail(tr("Timeout during deleting profile ") + ProfilePath);
	
	native("filesystem", "removefile", ProfilePath + "/lockfile");

	if(!JSON.parse(native("filesystem", "fileinfo", ProfilePath + "/lockfile"))["exists"])
		_break();

	sleep(1000)!
})!

native("filesystem", "removefile", ProfilePath)	
