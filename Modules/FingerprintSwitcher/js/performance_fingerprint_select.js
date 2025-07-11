var TimeDeformationFrom = GetInputConstructorValue("TimeDeformationFrom", loader);
var TimeDeformationTo = GetInputConstructorValue("TimeDeformationTo", loader);
var MaxMeasurementTime = GetInputConstructorValue("MaxMeasurementTime", loader);
var DisablePerformancePrecision = GetInputConstructorValue("DisablePerformancePrecision", loader);

if(TimeDeformationFrom["original"].length == 0)
{
  Invalid("TimeDeformationFrom are empty");
  return;
}
if(TimeDeformationTo["original"].length == 0)
{
  Invalid("TimeDeformationTo are empty");
  return;
}
if(MaxMeasurementTime["original"].length == 0)
{
  Invalid("MaxMeasurementTime are empty");
  return;
}
if(DisablePerformancePrecision["original"].length == 0)
{
  Invalid("DisablePerformancePrecision are empty");
  return;
}


try{
  var code = loader.GetAdditionalData() + _.template($("#performance_fingerprint_code").html())(
    {
      time_deformation_from: TimeDeformationFrom["updated"], 
      time_deformation_to: TimeDeformationTo["updated"],
      max_measurement_time: MaxMeasurementTime["updated"],
      disable_performance_precision: DisablePerformancePrecision["updated"],
    })

  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}