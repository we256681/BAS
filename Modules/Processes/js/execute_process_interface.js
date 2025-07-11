<div class="container-fluid">
  <%= _.template($('#input_constructor').html())({id:"Command", description:tr("Batch script or executable path"), default_selector: "string", disable_int:true, disable_editor:true, disable_string:true, use_textarea:true, size: 8, disable_type_chooser:true,textarea_height:80, help: 
{
	description: tr("This field must contain either path to executable that you want to start or batch script. Batch content may contain any windows utilities like cd, dir, echo, etc and can have multiple lines"), examples:
	[
		{
			code:"c:\\path\\to\\executable.exe",description:tr("Run executable")
		},
		{
			code:"c:\\path\\to\\executable 1.exe<br/>c:\\path\\to\\executable 2.exe<br>",description:tr("Run several executables one by one")
		},
		{
			code:"c:\\path\\to\\executable.exe --argument1 --argument2",description:tr("Run executable with command line parameters")
		},
		{
			code:"where cmd",description:tr("Find cmd.exe location")
		},
		{
			code:"echo \"Message\"",description:tr("Display message, it will be available in PROCESS_STANDART_OUTPUT variable.")
		},
		{
			code:"cd c:\\download<br/>dir<br/>",description:tr("Show content of download folder")
		},
		{
			code:"FOR %%G IN (a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z) DO (echo %%G)",description:tr("Output all letters to log")
		},
		
	]
}}) %>
  
  <%= _.template($('#input_constructor').html())({id:"WorkingFolder", description:tr("Working folder"), default_selector: "string", disable_int:true, help: 
{
	description: tr("Working folder in which batch will be executed. Leave blank to execute in folder, where BrowserAutomationStudio.exe file is located."), examples:
	[
		{
			code:"c:/dev"
		},
		{
			code:"c:\\dev"
		},
		{
			code:tr("Empty string"),description:tr("Run in BAS folder")
		}
	]
}}) %>

  <span data-preserve="true" data-preserve-type="check" data-preserve-id="Check">
    <input type="checkbox" id="Check" checked="checked"/> <label for="Check" class="tr">Wait while process will finish</label>
  </span>
  <%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Process standart output"), default_variable: "PROCESS_STANDART_OUTPUT", help: 
{
	description: tr("This variable will contain process standart output")
}}) %>
  <%= _.template($('#variable_constructor').html())({id:"SaveError", description:tr("Process error output"), default_variable: "PROCESS_ERROR_OUTPUT", help: 
{
	description: tr("This variable will contain process error output")
}}) %>
</div>

<div class="tooltipinternal">
      <div class="tr tooltip-paragraph-first-fold">Run executable or batch script.</div>
      <div class="tr tooltip-paragraph-fold">'Batch script or executable path' parameter may contain path to executable that you want to run, or batch script.</div>
      <div class="tooltip-paragraph-fold"><span class="tr">Batch script can have multiple lines, may use</span> <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('http://www.robvanderwoude.com/batchcommands.php');return false">windows utilities</a> <span class="tr">and start other executables.</span> </div>
      <div class="tr tooltip-paragraph-fold">Working folder by default is path where BAS installed, you may want to change it with 'Working folder' parameter or by running 'cd' command inside batch script.</div>
      <div class="tr tooltip-paragraph-last-fold">Check PROCESS_STANDART_OUTPUT and PROCESS_ERROR_OUTPUT variables to get execution results, you may want to use regular expressions to extract values from them.</div>
      
</div>

<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>