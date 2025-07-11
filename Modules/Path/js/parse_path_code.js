var parse_res = _path.parse(<%= path %>);
<%= root %> = parse_res["root"];
<%= dir %> = parse_res["dir"];
<%= base %> = parse_res["base"];
<%= ext %> = parse_res["ext"];
<%= name %> = parse_res["name"];
<%= items %> = parse_res["items"];
