<div class="container-fluid">
  <%= _.template($('#input_constructor').html())({id:"Value", description:tr("Timezone UTC offset. Can be blank"), default_selector: "expression", disable_string:true, variants: [

  	"-840<br/><span style='color:gray;font-size:small'>UTC+14:00</span>",
  	"-780<br/><span style='color:gray;font-size:small'>UTC+13:00</span>",
  	"-765<br/><span style='color:gray;font-size:small'>UTC+12:45</span>",
  	"-720<br/><span style='color:gray;font-size:small'>UTC+12:00</span>",
  	"-660<br/><span style='color:gray;font-size:small'>UTC+11:00</span>",
  	"-630<br/><span style='color:gray;font-size:small'>UTC+10:30</span>",
  	"-600<br/><span style='color:gray;font-size:small'>UTC+10:00</span>",
  	"-570<br/><span style='color:gray;font-size:small'>UTC+09:30</span>",
  	"-540<br/><span style='color:gray;font-size:small'>UTC+09:00</span>",
  	"-525<br/><span style='color:gray;font-size:small'>UTC+08:45</span>",
  	"-510<br/><span style='color:gray;font-size:small'>UTC+08:30</span>",
  	"-480<br/><span style='color:gray;font-size:small'>UTC+08:00</span>",
  	"-420<br/><span style='color:gray;font-size:small'>UTC+07:00</span>",
  	"-390<br/><span style='color:gray;font-size:small'>UTC+06:30</span>",
  	"-360<br/><span style='color:gray;font-size:small'>UTC+06:00</span>",
  	"-345<br/><span style='color:gray;font-size:small'>UTC+05:40</span>",
  	"-330<br/><span style='color:gray;font-size:small'>UTC+05:30</span>",
  	"-300<br/><span style='color:gray;font-size:small'>UTC+05:00</span>",
  	"-270<br/><span style='color:gray;font-size:small'>UTC+04:30</span>",
  	"-240<br/><span style='color:gray;font-size:small'>UTC+04:00</span>",
  	"-210<br/><span style='color:gray;font-size:small'>UTC+03:30</span>",
  	"-180<br/><span style='color:gray;font-size:small'>UTC+03:00</span>",
  	"-120<br/><span style='color:gray;font-size:small'>UTC+02:00</span>",
  	"-60<br/><span style='color:gray;font-size:small'>UTC+01:00</span>",
  	"0<br/><span style='color:gray;font-size:small'>UTC+00:00</span>",
  	"60<br/><span style='color:gray;font-size:small'>UTC-01:00</span>",
  	"120<br/><span style='color:gray;font-size:small'>UTC-02:00</span>",
  	"180<br/><span style='color:gray;font-size:small'>UTC-03:00</span>",
  	"210<br/><span style='color:gray;font-size:small'>UTC-03:30</span>",
  	"240<br/><span style='color:gray;font-size:small'>UTC-04:00</span>",
  	"300<br/><span style='color:gray;font-size:small'>UTC-05:00</span>",
  	"360<br/><span style='color:gray;font-size:small'>UTC-06:00</span>",
  	"420<br/><span style='color:gray;font-size:small'>UTC-07:00</span>",
  	"480<br/><span style='color:gray;font-size:small'>UTC-08:00</span>",
  	"540<br/><span style='color:gray;font-size:small'>UTC-09:00</span>",
  	"570<br/><span style='color:gray;font-size:small'>UTC-09:30</span>",
  	"600<br/><span style='color:gray;font-size:small'>UTC-10:00</span>",
  	"660<br/><span style='color:gray;font-size:small'>UTC-11:00</span>",
  	"720<br/><span style='color:gray;font-size:small'>UTC-12:00</span>"], help: 
{
	description: tr("The UTC offset is the difference in minutes from Coordinated Universal Time (UTC) for a particular place and date. Note that this means that the offset is positive if the local timezone is behind UTC and negative if it is ahead. For example, time zone UTC+10:00 (Australian Eastern Standard Time, Vladivostok Time, Chamorro Standard Time) corresponds to -600 offset, because 600 minutes equals to 10 hours."), examples:
	[
		{
			code:tr("Empty string"),description:tr("Don't set timezone UTC offset")
		},
		{
			code:"-60",description:tr("UTC+01:00 - Spain, France, Germany")
		},
		{
			code:"-120",description:tr("UTC+02:00 - Finland, Romania, Ukraine, Greece")
		}
	]
}

 }) %>

<%= _.template($('#input_constructor').html())({id:"TimezoneName", description:tr("Timezone name. Can be blank"), default_selector: "string", disable_int:true, variants: ["Europe/Berlin","America/Santiago"], help: 
{
	description: tr("Timezone name."), examples:
[
  {
	  code:tr("Empty string"),description:tr("Don't set timezone name")
  },
  {
	  code:"Europe/Berlin"
  }
]
}

}) %>
</div>

<div class="tooltipinternal">
      <div class="tr tooltip-paragraph-first-fold">Set browser timezone. Timezone can be changed by "Proxy" action as well.</div>
      <div class="tooltip-paragraph-fold"><span class="tr">Site can obtain information about system timezone by using internal '</span><a href="#" onclick="BrowserAutomationStudio_OpenUrl('https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date/getTimezoneOffset');return false">Date</a><span class="tr">' object. This action allows to spoof local time values returned by browser.</span></div>
      <div class="tooltip-paragraph-fold"><span class="tr">Full list of offsets can be found</span> <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://en.wikipedia.org/wiki/List_of_UTC_time_offsets');return false">here</a>, <span class="tr">list of timezone names can be found</span> <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://en.wikipedia.org/wiki/List_of_tz_database_time_zones');return false">here</a> .</div>
	  <div class="tr tooltip-paragraph-fold">Leave any field blank if you don't want to change it.</div>

      <div class="tr tooltip-paragraph-last-fold">It is recommended to use 'Proxy' action instead of this one, because it will set timezone automatically according to current proxy.</div>
</div>

<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>