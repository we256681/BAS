<%= variable %> = _get_profile()
<%= variable2 %> = _get_profile().length > 0 && JSON.parse(native("filesystem", "fileinfo", _get_profile() + "/proxy.txt"))["exists"]
<%= variable3 %> = _get_profile().length > 0 && JSON.parse(native("filesystem", "fileinfo", _get_profile() + "/fingerprint.json"))["exists"]
