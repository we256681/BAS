var hash = proxy_parse(<%= ProxyText %>)
if(<%= ProxyType %> != "auto" ){
	hash["IsHttp"] = <%= ProxyType %> == "http"
}
var login = <%= ProxyLogin %>
var password = <%= ProxyPassword %>
if(login.length > 0 && password.length > 0 )
{
	hash["name"] = login
	hash["password"] = password
}

smtp_client_set_proxy(hash["server"], hash["Port"], hash["IsHttp"], hash["name"], hash["password"])