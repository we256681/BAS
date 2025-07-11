<div class="container-fluid">
  <%= _.template($('#input_constructor').html())({id:"TimeDeformationFrom", description:tr("Time deformation from"), default_selector: "expression", disable_int:true, value_string: "1.0", help: 
		{
			description: tr("Specifies how much the timing measurement results will be stretched. Big values means that site will think, that code execution will take longer than it actually would. Values which are close to 0.0 creates a sense that code executes faster. For example, if resulting time deformation is 1.1 and execution of particular code will take 10 msec, site will think, that code was executed in 11 msec instead. Values below zero are irrelevant. This parameter works only with \"Time deformation to\" parameter."), examples:
			[
				{
					code:"1.0 - 1.2",description:tr("Time deformation coefficient will be selected randomly between 1.0 and 1.2. Time measurements will show higher values than it actually is.")
				},
				{
					code:"0.8 - 1.0",description:tr("Time deformation coefficient will be selected randomly between 0.8 and 1.0. Time measurements will show lower values than it actually is.")
				},
				{
					code:"1.15 - 1.15",description:tr("Set deformation coefficient to exact value.")
				},
				{
					code:"1.0 - 1.0",description:tr("No time deformation will be applied.")
				}
			]
		} }) %>
	<%= _.template($('#input_constructor').html())({id:"TimeDeformationTo", description:tr("Time deformation to"), default_selector: "expression", disable_int:true, value_string: "1.2", help: 
		{
			description: tr("Specifies how much the timing measurement results will be stretched. Big values means that site will think, that code execution will take longer than it actually would. Values which are close to 0.0 creates a sense that code executes faster. For example, if resulting time deformation is 1.1 and execution of particular code will take 10 msec, site will think, that code was executed in 11 msec instead. Values below zero are irrelevant. This parameter works only with \"Time deformation from\" parameter."), examples:
			[
				{
					code:"1.0 - 1.2",description:tr("Time deformation coefficient will be selected randomly between 1.0 and 1.2. Time measurements will show higher values than it actually is.")
				},
				{
					code:"0.8 - 1.0",description:tr("Time deformation coefficient will be selected randomly between 0.8 and 1.0. Time measurements will show lower values than it actually is.")
				},
				{
					code:"1.15 - 1.15",description:tr("Set deformation coefficient to exact value.")
				},
				{
					code:"1.0 - 1.0",description:tr("No time deformation will be applied.")
				}
			]
		} }) %>

	<%= _.template($('#input_constructor').html())({id:"MaxMeasurementTime", description:tr("Max measurement time"), default_selector: "int", disable_string:true, value_number: 100, variants: ["30<br/><span style='color:gray;font-size:small'>" + tr("Do replacement only if interval is less than 30 milliseconds") + "</span>", "100<br/><span style='color:gray;font-size:small'>" + tr("Do replacement only if interval is less than 100 milliseconds") + "</span>", "100000<br/><span style='color:gray;font-size:small'>" + tr("Always do replacement.") + "</span>"], help: 
	{
		description: tr("This action will try to modify time measurements only when it is executed between small time intervals. In case if interval between measurements is big, it can't be used for browser fingerprinting, therefore replacement is not required. This parameter designates maximum time between measurements to perform replacement."), examples:
		[
			{
				code:"100",description:tr("Default value, do replacement only if interval is less than 100 milliseconds.")
			},
			{
				code:"100000",description:tr("Always do replacement.")
			}
		]
	} }) %>

	<%= _.template($('#input_constructor').html())({id:"DisablePerformancePrecision", description:tr("Disable performance precision"), default_selector: "string", disable_int:true, value_string: "false", variants: ["true", "false"],
     help: 
     {
         description: tr("By default time measurement will return value that has high precision, it can be more than 1 milliseconds. For example, value 1200.695 means 1 second and 200.695 milliseconds. This can be used to generate browser fingerprint, but you can limit presicion up to 1 millisecond by using this parameter. This means that returned value will be 1200.0 instead of 1200.695. It will neglect fingerprinting but may look suspicious, so it is better to use time deformation instead."), examples:
         [
             {
                code:"false",description:tr("Default, allow high precision.")
             },
             {
                code:"true",description:tr("Disable high precision.")
             }
         ]
     } }) %>

</div>


<div class="tooltipinternal">
      <div class="tr tooltip-paragraph-first-fold">Modify performance fingerprint.</div>
	<div class="tr tooltip-paragraph-fold">This action allows to bypass device fingerprinting techniques based on measuring the execution time of a certain javascript functionality. For example, site can measure execution time of some cryptographic methods which depends on installed hardware. Results will be the same across different runs and therefore may be used for fingerprinting.</div>
      <div class="tr tooltip-paragraph-fold">Following action will slightly change results of time measurements in order to reduce these techniques. The magnitude of these time changes can be set with "Time deformation from" and "Time deformation to" parameters.</div>
      <div class="tr tooltip-paragraph-fold">Default values must work fine and can be used without changes.</div>
	  <div class="tr tooltip-paragraph-fold">In order to reset default values, set "Time deformation from" and "Time deformation to" parameters with 1.0 value.</div>
	  <div class="tr tooltip-paragraph-fold">This technology is still highly experimental and may lead to unpredictable results.</div>
	  <div class="tr tooltip-paragraph-fold">All values will be saved to current profile and will be automatically restored on next profile usage.</div>
	  <div class="tr tooltip-paragraph-last-fold">By using "Disable performance precision" parameter precision of time measurement will be reduced. It will neglect fingerprinting, but may look suspicious. See parameter description for more details.</div>

     
</div>


<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
