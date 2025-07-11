var parse_res = new _url(<%= url %>, {normalize: <%= normalize %>, base_url: <%= base_url %>, rfail: <%= rfail %>});
<%= protocol %> = _avoid_nil(parse_res["protocol"]);
<%= username %> = _avoid_nil(parse_res["username"]);
<%= password %> = _avoid_nil(parse_res["password"]);
<%= host %> = _avoid_nil(parse_res["host"]);
<%= port %> = _avoid_nil(parse_res["port"]);
<%= path %> = _avoid_nil(parse_res["pathname"]);
<%= query %> = _avoid_nil(parse_res["query"]);
<%= hash %> = _avoid_nil(parse_res["hash"]);
