<div class="container-fluid">
<%= _.template($('#input_constructor').html())({id:"Value", description:tr("Ip address"), default_selector: "string", disable_int:true, help: 
{
	description: tr("A string which contains ip address. Ipv6 string should be enclosed with square brackets."), examples:
	[
		{
			code:"140.97.75.15",description:tr("Ipv4 example")
		},
		{
			code:"<s>140.97.75.15:8080</s>",description:tr("Not correct, ip string can't contain port.")
		},
		{
			code:"[2a03:2880:f11c:8083:face:b00c:0:25de]",description:tr("Ipv6 example")
		}
	]
}}) %>
<%= _.template($('#variable_constructor').html())({id:"SaveValid", description:tr("Is Valid"), default_variable: "IPINFO_VALID", help: 
{
	description: tr("If ip was parsed correctly. This values must always be checked before others."), examples:
	[
		{
			code:"true",description:tr("Ip was parsed correctly")
		},
		{
			code:"false",description:tr("failed to parse ip")
		}
	]
}
}) %>
<%= _.template($('#variable_constructor').html())({id:"SaveCountry", description:tr("Country"), default_variable: "IPINFO_COUNTRY", help: 
{
	description: tr("Country abbreviation"), examples:
	[
		{
			code:"US",description:tr("United States")
		},
		{
			code:"RU",description:tr("Russian Federation")
		}
	]
}}) %>
<%= _.template($('#variable_constructor').html())({id:"SaveCity", description:tr("City"), default_variable: "IPINFO_CITY", help: 
{
	description: tr("Ip city"), examples:
	[
		{
			code:"Boston"
		},
		{
			code:"Moscow"
		}
	]
}}) %>
<%= _.template($('#variable_constructor').html())({id:"SaveLatitude", description:tr("Latitude"), default_variable: "IPINFO_LATITUDE", help: 
		{
			description: tr("Exact coordinate, which identifies ip position."), examples:
			[
				{
					code:"10.433"
				}
			]
		}}) %>
<%= _.template($('#variable_constructor').html())({id:"SaveLongitude", description:tr("Longitude"), default_variable: "IPINFO_LONGITUDE", help: 
		{
			description: tr("Exact coordinate, which identifies ip position."), examples:
			[
				{
					code:"10.433"
				}
			]
		}}) %>
<%= _.template($('#variable_constructor').html())({id:"SaveTimezone", description:tr("Timezone"), default_variable: "IPINFO_TIMEZONE", help: 
		{
			description: tr("Timezone name."), examples:
			[
				{
					code:"Europe/Lisbon"
				},
				{
					code:"Asia/Dubai"
				}
			]
		}}) %>
<%= _.template($('#variable_constructor').html())({id:"SaveOffset", description:tr("Offset"), default_variable: "IPINFO_OFFSET", help: 
{
	description: tr("The UTC offset is the difference in minutes from Coordinated Universal Time (UTC) to the ip local time. Note that this means that the offset is positive if the local timezone is behind UTC and negative if it is ahead. For example, time zone UTC+10:00 (Australian Eastern Standard Time, Vladivostok Time, Chamorro Standard Time) corresponds to -600 offset, because 600 minutes equals to 10 hours."), examples:
	[
		{
			code:"-60",description:tr("UTC+01:00 - Spain, France, Germany")
		},
		{
			code:"-120",description:tr("UTC+02:00 - Finland, Romania, Ukraine, Greece")
		}
	]
}}) %>
<%= _.template($('#variable_constructor').html())({id:"SaveDstOffset", description:tr("Dst Offset"), default_variable: "IPINFO_DST_OFFSET", help: 
{
	description: tr("An offset for daylight saving time.")
}}) %>
</div>






<div class="container-fluid resize">
	<%= _.template($('#ip_information').html())() %>
</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Get information for selected ip, this includes timezone UTC offset and other.</div>
    <div class="tr tooltip-paragraph-last-fold">To obtain ip information internal database or external service is used, check "IP information" section for more details.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>