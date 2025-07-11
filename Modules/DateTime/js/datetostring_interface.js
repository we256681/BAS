<div class="container-fluid">
    <%= _.template($('#input_constructor').html())({id:"String", description:tr("DateTime or string"), default_selector: "string", disable_int:true}) %>
    <%= _.template($('#input_constructor').html())({id:"Format", description:tr("DateTime format"), default_selector: "string", disable_int:true, value_string: "yyyy-MM-dd", variants: ["yyyy-MM-dd","hh:mm:ss","yyyy-MM-ddThh:mm:ss"]}) %>
	<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable To Save"), default_variable: "FORMATTED_DATE"}) %>
    <a onclick="BrowserAutomationStudio_OpenUrl('http://doc.qt.io/qt-5/qdatetime.html#fromString')" class="tr" style="margin-left: 20px;cursor: pointer;">DateTime formatting</a>
</div>
<div class="tooltipinternal">
    <div class="tr tooltip-paragraph-first-fold">Convert date and time to string using a specified format.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>