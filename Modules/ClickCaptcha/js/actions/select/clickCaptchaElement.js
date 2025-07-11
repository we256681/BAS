const textInstructions = GetInputConstructorValue('textInstructions', loader);
const taskWaitTimeout = GetInputConstructorValue('taskWaitTimeout', loader);
const taskWaitDelay = GetInputConstructorValue('taskWaitDelay', loader);
const serviceName = GetInputConstructorValue('serviceName', loader);
const serviceKey = GetInputConstructorValue('serviceKey', loader);
const serviceUrl = GetInputConstructorValue('serviceUrl', loader);
const emulateMouse = $("#emulateMouse").is(':checked');

if (!taskWaitTimeout.original.length) {
  return Invalid(tr('The "Task solution check interval" parameter is empty'));
}
if (!taskWaitDelay.original.length) {
  return Invalid(tr('The "Task solution check delay" parameter is empty'));
}
if (!serviceName.original.length) {
  return Invalid(tr('The "Service name" parameter is empty'));
}
if (!serviceKey.original.length) {
  return Invalid(tr('The "Service key" parameter is empty'));
}

try {
  const code = Normalize(loader.GetAdditionalData() + _.template($('#clickCaptchaElement_code').html())({
    textInstructions: textInstructions.updated,
    taskWaitTimeout: taskWaitTimeout.updated,
    taskWaitDelay: taskWaitDelay.updated,
    serviceName: serviceName.updated,
    serviceKey: serviceKey.updated,
    serviceUrl: serviceUrl.updated,
    emulateMouse,
    ...GetPath(loader)
  }), 0);
  BrowserAutomationStudio_Append('', BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
} catch (e) { console.log('Error while adding action:', e) }