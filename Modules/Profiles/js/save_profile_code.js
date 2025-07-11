ProfilePath = _get_profile()
native("filesystem", "copyfile", JSON.stringify({path: ProfilePath,dest: <%= profile %>}))