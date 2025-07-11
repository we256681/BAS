function new_smtp_client()
{
	_SMTP_SETTINGS = 
	{
		read_from_string: "NULL",
		options:
		{
			CURLOPT_SSL_VERIFYPEER: 0,
			CURLOPT_SSL_VERIFYHOST: 0,
		}
	};

	
};


function _ensure_smtp_client()
{
	if(typeof(_SMTP_SETTINGS)=='undefined')
		new_smtp_client();
};


function smtp_client_set_config(host, port, login, pass, encrypt)
{
	_ensure_smtp_client();
	if(encrypt == "none" ){
		_SMTP_SETTINGS.options["CURLOPT_USE_SSL"] = 0;
		host = "smtp://" + host
	}else{
		_SMTP_SETTINGS.options["CURLOPT_USE_SSL"] = 3;
		host = "smtps://" + host
	}
	_SMTP_SETTINGS.options["CURLOPT_URL"] = host + "/pc";
	_SMTP_SETTINGS.options["CURLOPT_PORT"] = port;
	
	if(login.length > 0 && pass.length >0)
	{
		_SMTP_SETTINGS.options["CURLOPT_USERNAME"] = login;
		_SMTP_SETTINGS.options["CURLOPT_PASSWORD"] = pass;
	};
	
};


function smtp_client_set_proxy(server, Port, IsHttp, name, password)
{
	_ensure_smtp_client();
	
	proxy = "";
	if(server.length > 0)
	{
		if(IsHttp)
	    {
			proxy += "http://";
	    }else
	    {
			proxy += "socks5://";
	    };
		proxy += server + ":" + Port;
	}
	 
	_SMTP_SETTINGS.options["CURLOPT_PROXY"] = proxy;
	if(name.length > 0 && password.length >0)
	{
		_SMTP_SETTINGS.options["CURLOPT_PROXYUSERNAME"] = name;
		_SMTP_SETTINGS.options["CURLOPT_PROXYPASSWORD"] = password;
	};
	
};


function smtp_client_send_mail(mailfrom, mailto, mailcc, mailsubject, mailbody, is_html, attachments)
{
	if(typeof(_SMTP_SETTINGS)=='undefined')
		fail("No smtp settings")

	var _SMTP_NEW_MAIL = JSON.parse(JSON.stringify(_SMTP_SETTINGS));

	if(typeof(_SMTP_DEBUG) != "undefined" && _SMTP_DEBUG)
	{
		_SMTP_NEW_MAIL["trace"] = true
	}

	_SMTP_NEW_MAIL.options["CURLOPT_MAIL_FROM"] = mailfrom;
	mailto = mailto.replace(/\s/g, "");
	mail_rcpt = mailto.split(",");
	attachments_list = []
	if(typeof(attachments) == "string")
	{
		if (attachments.length > 0)
		{
			attachments_list = attachments.split(",");
		}
	}else if(typeof(attachments) == "object")
	{
		attachments_list = attachments
	}

	
	if(mailcc.length > 0)
	{
		mailcc = mailcc.replace(/\s/g, "")
		mail_rcpt = mail_rcpt.concat(mailcc.split(",").slice());
	};
	
	_SMTP_NEW_MAIL.options["CURLOPT_MAIL_RCPT"] = mail_rcpt;
	
	mail_raw = "To: <" + mailto + ">\r\nFrom: <" + mailfrom + ">";
	if(mailcc.length > 0)
	{
		mail_raw += "\r\nCC: <" + mailcc + ">"
	};
	
	
	mail_raw += "\r\nSubject: " + mailsubject;
	
	mailbody = base64_encode(mailbody);
	var boundary = rand(22);
	if(is_html == true)
	{
		var msg_body_type = "text/html; charset=utf-8";
	} else
	{
		var msg_body_type = "text/plain; charset=utf-8";
	}
	mail_raw += "\r\nMIME-Version: 1.0\r\nContent-Type: multipart/mixed; boundary=" + boundary + "\r\n\r\nThis is a multipart message in MIME format.\r\n\r\n" + "--" + boundary;
	mail_raw += "\r\nContent-Type: " + msg_body_type + "\r\nContent-Transfer-Encoding: base64\r\n\r\n" + mailbody + "\r\n";
	if(attachments_list.length > 0)
	{
		mail_raw += "\r\n--" + boundary;
		for(var i = 0; i<attachments_list.length; i++)
		{
			var attach_filename = attachments_list[i]
			if (attachments_list[i].indexOf("/") >=0)
			{
				attach_filename = attachments_list[i].split("/")[attachments_list[i].split("/").length - 1]
			}else if (attachments_list[i].indexOf("\\") >=0)
			{
				attach_filename = attachments_list[i].split("\\")[attachments_list[i].split("\\").length - 1]
			};
			var extension = "." + attach_filename.split(".")[attach_filename.split(".").length - 1];
			var content_type = BAS_GET_MIME_CTYPE(extension);			
			var attach_base64 = native("filesystem", "readfile", JSON.stringify({value: attachments_list[i],base64:true,from:0,to:0}));
			mail_raw += "\r\nContent-Type: " + content_type + ";\r\nContent-Disposition: attachment; filename=\"" + attach_filename + "\"\r\nContent-Transfer-Encoding: base64\n\n" + attach_base64;
	
			if(i == attachments_list.length - 1)
			{
				mail_raw += "\r\n--" + boundary + "--";
			}else
			{
				mail_raw += "\r\n--" + boundary;
			};
		};
	}else
	{
		mail_raw += "\r\n--" + boundary + "--";
	};

	_SMTP_NEW_MAIL["read_from_string"] = mail_raw;
	return _SMTP_NEW_MAIL
}


function BAS_GET_MIME_CTYPE(extension)
{
	var mime_arr = {
		".7z" : "application/x-7z-compressed",
		".AAC" : "audio/aac",
		".avi" : "video/x-msvideo",
		".bmp" : "image/bmp",
		".csv" : "text/csv",
		".doc" : "application/msword",
		".docx" : "application/vnd.openxmlformats-officedocument.wordprocessingml.document",
		".flv" : "video/x-flv",
		".gif" : "image/gif",
		".html" : "text/html",
		".ico" : "image/x-icon",
		".jpeg" : "image/jpeg",
		".jpg" : "image/jpeg",
		".mp3" : "audio/mpeg",
		".mp4" : "video/mp4",
		".mp4v" : "video/mp4",
		".mpeg" : "video/mpeg",
		".png" : "image/png",
		".wav" : "audio/wav",
		".xls" : "application/vnd.ms-excel",
		".xlsx" : "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet",
		".xml" : "text/xml",
		".zip" : "application/x-zip-compressed",
		".gif" : "image/gif",
		"other" : "application/octet-stream"};
		
	if(mime_arr[extension])
	{
		return mime_arr[extension]
	}else
	{
		return mime_arr["other"]
	};
};