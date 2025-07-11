RANDOM_FILE = "temp_" + rand() + ".bat"
native("filesystem", "writefile", JSON.stringify({path: RANDOM_FILE,value: "chcp 65001\r\n" + ((<%= working_folder %>.length>0) ? ("cd " + <%= working_folder %> + "\r\n") : "" ) + ((<%= command %>).replace(/\r?\n/g, "\r\n")),base64:false,append:false}))
native_async("processmanager", "start", JSON.stringify({location: RANDOM_FILE, working_folder: "", waitfinish: <%= waitfinish %>, arguments: "", version: 2}))!

try
{
	var split = _result().split(",")
	<%= variable %> = base64_decode(split[0])
	<%= variable_fail %> = base64_decode(split[1])
}catch(e)
{
	<%= variable %> = _result()	
}


sleep(1000)!
native("filesystem", "removefile", RANDOM_FILE)
