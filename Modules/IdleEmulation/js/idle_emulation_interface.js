<div class="container-fluid">
  <%= _.template($('#input_constructor').html())({id:"IddleTime", description:tr("Time spent on page in seconds"), default_selector: "int", disable_string:true, value_number: 10, help: 
{
	description: tr("This parameter sets idle time duration in seconds. Time setting is not precise, actual action duration can be a little longer than specified."), examples:
	[
		{
			code:"60",description:tr("Idle for one minute")
		},
		{
			code:"300",description:tr("Idle for 5 minutes")
		}
	]
}}) %>
  <%= _.template($('#input_constructor').html())({id:"Distribution", description:tr("Idle type and filling"), default_selector: "expression", disable_string:true,disable_int:true, value_string: "[1,1,2,3,3,3,4,4]"
		, help: 
		{
			description: tr("Whole idle duration is splitted into intervals. Every interval has a type. Here are list of intervals: 1 - scroll page with mouse wheel, 2 - random mouse movements for a long distance, 3 - random mouse movements for a short distance, 4 - sleep. Besides that, frequency of occurrence of interval with some types, may be bigger then others. This parameter must contain list of types, the more often different types will appear in the list, the more often they will meet during idle."), examples:
			[
				{
					code:"[1]",description:tr("Execute only page scrolling.")
				},
				{
					code:"[1,4]",description:tr("Execute page scrolling mixed with sleep. Sleep and page scroll types will appear with same frequency")
				},
				{
					code:"[1,4,4]",description:tr("Execute page scrolling mixed with sleep. Sleep will be executed 2 times more often then scroll.")
				}
				,
				{
					code:"[1,3,4,4]",description:tr("Execute page scrolling mixed with sleep and short mouse moves. Sleep will be executed 2 times more often then scroll. Scroll and short mouse moves has same frequencies.")
				}
			]
		}
	}) %>
</div>
<div class="tooltipinternal">
      <div class="tr tooltip-paragraph-first-fold">Imitate user behaviour on webpage.</div>
      <div class="tr tooltip-paragraph-fold">This is more convenient sleep action replacement.</div>
      <div class="tr tooltip-paragraph-fold">During idle user can scroll and move mouse, but no clicks are performed.</div>
      <div class="tr tooltip-paragraph-fold">Whole idle duration is splitted into intervals. Every interval has a type. Here are list of intervals:</div>
      <div class="tr tooltip-paragraph-fold">1 - scroll page with mouse wheel.</div>
      <div class="tr tooltip-paragraph-fold">2 - random mouse movements for a long distance.</div>
      <div class="tr tooltip-paragraph-fold">3 - random mouse movements for a short distance.</div>
      <div class="tr tooltip-paragraph-fold">4 - sleep.</div>
      <div class="tr tooltip-paragraph-fold">Each interval appearance is randomised. So you'll get completely different behaviour each time the action is called.</div>
      <div class="tr tooltip-paragraph-fold">Besides that, frequency of occurrence of interval with some types, may be bigger then others.</div>
      <div class="tr tooltip-paragraph-last-fold">'Idle type and filling' parameter must contain list of types, the more often different types will appear in the list, the more often they will meet during idle. Check help for 'Idle type and filling' parameter for examples.</div>
      
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
