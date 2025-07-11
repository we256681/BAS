var uid = GetInputConstructorValue("uid", loader);
if(uid["original"].length == 0){
    Invalid(tr("The parameter \"") + tr("Message id") + tr("\" is not specified"));
    return;
};
var getFrom = $("#getFrom").is(':checked');
var saveFrom = this.$el.find("#saveFrom").val().toUpperCase();
if(getFrom && saveFrom.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Recipient of letter") + tr("\" is not specified"));
    return;
};
var getTo = $("#getTo").is(':checked');
var saveTo = this.$el.find("#saveTo").val().toUpperCase();
if(getTo && saveTo.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Recipient of letter") + tr("\" is not specified"));
    return;
};
var getSubject = $("#getSubject").is(':checked');
var saveSubject = this.$el.find("#saveSubject").val().toUpperCase();
if(getSubject && saveSubject.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Letter subject") + tr("\" is not specified"));
    return;
};
var getTextHtml = $("#getTextHtml").is(':checked');
var saveTextHtml = this.$el.find("#saveTextHtml").val().toUpperCase();
if(getTextHtml && saveTextHtml.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Body of letter") + " (text/html)" + tr("\" is not specified"));
    return;
};
var getLinksTextHtml = $("#getLinksTextHtml").is(':checked');
var saveLinksTextHtml = this.$el.find("#saveLinksTextHtml").val().toUpperCase();
if(getTextHtml && getLinksTextHtml && saveLinksTextHtml.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Links from body of letter") + " (text/html)" + tr("\" is not specified"));
	return;
};
var getTextPlain = $("#getTextPlain").is(':checked');
var saveTextPlain = this.$el.find("#saveTextPlain").val().toUpperCase();
if(getTextPlain && saveTextPlain.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Body of letter") + " (text/plain)" + tr("\" is not specified"));
    return;
};
var getLinksTextPlain = $("#getLinksTextPlain").is(':checked');
var saveLinksTextPlain = this.$el.find("#saveLinksTextPlain").val().toUpperCase();
if(getTextPlain && getLinksTextPlain && saveLinksTextPlain.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Links from body of letter") + " (text/plain)" + tr("\" is not specified"));
	return;
};
var getTextRaw = $("#getTextRaw").is(':checked');
var saveTextRaw = this.$el.find("#saveTextRaw").val().toUpperCase();
if(getTextRaw && saveTextRaw.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Body of letter") + " (raw)" + tr("\" is not specified"));
    return;
};
var getLinksTextRaw = $("#getLinksTextRaw").is(':checked');
var saveLinksTextRaw = this.$el.find("#saveLinksTextRaw").val().toUpperCase();
if(getTextRaw && getLinksTextRaw && saveLinksTextRaw.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Links from body of letter") + " (raw)" + tr("\" is not specified"));
	return;
};
var getSize = $("#getSize").is(':checked');
var saveSize = this.$el.find("#saveSize").val().toUpperCase();
if(getSize && saveSize.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Letter size") + tr("\" is not specified"));
    return;
};
var getFlags = $("#getFlags").is(':checked');
var saveFlags = this.$el.find("#saveFlags").val().toUpperCase();
if(getFlags && saveFlags.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Flags of letter") + tr("\" is not specified"));
    return;
};
var getDate = $("#getDate").is(':checked');
var saveDate = this.$el.find("#saveDate").val().toUpperCase();
if(getDate && saveDate.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Receiving date") + tr("\" is not specified"));
    return;
};
var getAttachNames = $("#getAttachNames").is(':checked');
var saveAttachnames = this.$el.find("#saveAttachnames").val().toUpperCase();
if(getAttachNames && saveAttachnames.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Attachments names") + tr("\" is not specified"));
    return;
};
var getAttachments = $("#getAttachments").is(':checked');
var attachmentsMask = GetInputConstructorValue("attachmentsMask", loader);
if(getAttachments && attachmentsMask["original"].length == 0){
    Invalid(tr("The parameter \"") + tr("File name mask") + tr("\" is not specified"));
    return;
};
var saveAttachments = this.$el.find("#saveAttachments").val().toUpperCase();
if(getAttachments && saveAttachments.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Attachments") + tr("\" is not specified"));
    return;
};
var getRawHeader = $("#getRawHeader").is(':checked');
var saveRawHeader = this.$el.find("#saveRawHeader").val().toUpperCase();
if(getRawHeader && saveRawHeader.length == 0){
	Invalid(tr("The parameter \"") + tr("Variable") + " -> " + tr("Technical headers of letter") + tr("\" is not specified"));
    return;
};
var delAfter = $("#delAfter").is(':checked');
var setFlagsAfter = $("#setFlagsAfter").is(':checked');
var setFlags = GetInputConstructorValue("setFlags", loader);
if(setFlagsAfter && setFlags["original"].length == 0){
	Invalid(tr("The parameter \"") + tr("Flags") + tr("\" is not specified"));
    return;
};
var box = GetInputConstructorValue("box", loader);
try{
    var code = loader.GetAdditionalData() + _.template($("#InMail_GetMessage_code").html())({
        "uid": uid["updated"],
		"getFrom": getFrom,
        "saveFrom": "VAR_" + saveFrom,
		"getTo": getTo,
        "saveTo": "VAR_" + saveTo,
		"getSubject": getSubject,
        "saveSubject": "VAR_" + saveSubject,
		"getTextHtml": getTextHtml,
        "saveTextHtml": "VAR_" + saveTextHtml,
		"getLinksTextHtml": getLinksTextHtml,
        "saveLinksTextHtml": "VAR_" + saveLinksTextHtml,
		"getTextPlain": getTextPlain,
        "saveTextPlain": "VAR_" + saveTextPlain,
		"getLinksTextPlain": getLinksTextPlain,
        "saveLinksTextPlain": "VAR_" + saveLinksTextPlain,
		"getLinksTextRaw": getLinksTextRaw,
        "saveLinksTextRaw": "VAR_" + saveLinksTextRaw,
		"getTextRaw": getTextRaw,
        "saveTextRaw": "VAR_" + saveTextRaw,
		"getSize": getSize,
        "saveSize": "VAR_" + saveSize,
		"getFlags": getFlags,
        "saveFlags": "VAR_" + saveFlags,
        "getDate": getDate,
        "saveDate": "VAR_" + saveDate,
        "getAttachNames": getAttachNames,
        "saveAttachnames": "VAR_" + saveAttachnames,
        "getAttachments": getAttachments,
        "attachmentsMask": attachmentsMask["updated"],
        "saveAttachments": "VAR_" + saveAttachments,
        "getRawHeader": getRawHeader,
        "saveRawHeader": "VAR_" + saveRawHeader,
        "delAfter": delAfter,
		"setFlagsAfter": setFlagsAfter,
		"setFlags": setFlags["updated"],
        "box": box["updated"]
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}
