const taskWaitTimeout = GetInputConstructorValue('taskWaitTimeout', loader);
const taskWaitDelay = GetInputConstructorValue('taskWaitDelay', loader);
const serviceName = GetInputConstructorValue('serviceName', loader);
const serviceKey = GetInputConstructorValue('serviceKey', loader);
const serviceUrl = GetInputConstructorValue('serviceUrl', loader);
const sendProxy = GetInputConstructorValue('sendProxy', loader);
const userAgent = GetInputConstructorValue('userAgent', loader);

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
  const code = Normalize(loader.GetAdditionalData() + _.template($('#hCaptchaBrowser_code').html())({
    taskWaitTimeout: taskWaitTimeout.updated,
    taskWaitDelay: taskWaitDelay.updated,
    serviceName: serviceName.updated,
    serviceKey: serviceKey.updated,
    serviceUrl: serviceUrl.updated,
    sendProxy: sendProxy.updated,
    userAgent: userAgent.updated,
    ...GetPath(loader)
  }), 0);
  BrowserAutomationStudio_Append('', BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
} catch (e) { console.log('Error while adding action:', e) }