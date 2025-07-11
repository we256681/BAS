<div class="container-fluid"></div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Close FTP/SSH connection.</div>
	<div class="tr tooltip-paragraph-fold">If connection is already closed or not open yet, this action will do nothing.</div>
	<div class="tr tooltip-paragraph-fold">This action is optional because connection is closed automatically if it is not used for a long time.</div>
	<div class="tr tooltip-paragraph-last-fold">Exact autoclose timeout is specified in "Configure FTP/SSH" action.</div>

	
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
