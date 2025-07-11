<div class="container-fluid filewrite">
    <%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Profile path"), default_variable: "CURRENT_PROFILE_ID", help: {description: tr("Absolute profile folder path.")}}) %>
    <%= _.template($('#variable_constructor').html())({id:"Save2", description:tr("Has proxy"), default_variable: "HAS_PROXY", help: {description: tr("Boolean value, if current profile has proxy. It can be applied automatically with 'Create or switch to regular profile' action.")}}) %>
    <%= _.template($('#variable_constructor').html())({id:"Save3", description:tr("Has fingerprint"), default_variable: "HAS_FINGERPRINT", help: {description: tr("Boolean value, if current profile has fingerprint. It can be applied automatically with 'Create or switch to regular profile' action.")}}) %>
</div>


<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Return information about current profile.</div>
	<div class="tr tooltip-paragraph-last-fold">This action doesn't create browser, returns immediately and can be used from any place of script.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
