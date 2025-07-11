var Tags = GetInputConstructorValue("Tags", loader);
var Key = GetInputConstructorValue("Key", loader);
var Save = this.$el.find("#Save").val().toUpperCase();

var AddedDate = GetInputConstructorValue("AddedDate", loader);
var BrowserVersion = GetInputConstructorValue("BrowserVersion", loader);
var MaxBrowserVersion = GetInputConstructorValue("MaxBrowserVersion", loader);
var MinimumWidth = GetInputConstructorValue("MinimumWidth", loader);
var MinimumHeight = GetInputConstructorValue("MinimumHeight", loader);
var MaximumWidth = GetInputConstructorValue("MaximumWidth", loader);
var MaximumHeight = GetInputConstructorValue("MaximumHeight", loader);
var PerfectCanvasRequest = GetInputConstructorValue("PerfectCanvasRequest", loader);
var PerfectCanvasDebug = GetInputConstructorValue("PerfectCanvasDebug", loader);

var EnableCustomServers = GetInputConstructorValue("EnableCustomServers", loader);
var DynamicPerfectCanvas = GetInputConstructorValue("DynamicPerfectCanvas", loader);
var EnablePrecomputedFingerprints = GetInputConstructorValue("EnablePrecomputedFingerprints", loader);

if(Tags["original"].length == 0)
{
  Invalid("Tags are empty");
  return;
}

if(AddedDate["original"].length == 0)
{
  Invalid("AddedDate are empty");
  return;
}
if(BrowserVersion["original"].length == 0)
{
  Invalid("BrowserVersion are empty");
  return;
}
if(MaxBrowserVersion["original"].length == 0)
{
  Invalid("MaxBrowserVersion are empty");
  return;
}
if(MinimumWidth["original"].length == 0)
{
  Invalid("MinimumWidth are empty");
  return;
}
if(MinimumHeight["original"].length == 0)
{
  Invalid("MinimumHeight are empty");
  return;
}
if(MaximumWidth["original"].length == 0)
{
  Invalid("MaximumWidth are empty");
  return;
}
if(MaximumHeight["original"].length == 0)
{
  Invalid("MaximumHeight are empty");
  return;
}
if(EnableCustomServers["original"].length == 0)
{
  Invalid("EnableCustomServers is empty");
  return;
}
if(DynamicPerfectCanvas["original"].length == 0)
{
  Invalid("DynamicPerfectCanvas is empty");
  return;
}
if(EnablePrecomputedFingerprints["original"].length == 0)
{
  Invalid("EnablePrecomputedFingerprints is empty");
  return;
}



try{
  var code = loader.GetAdditionalData() + _.template($("#get_fingerprint_code").html())(
    {
      tags: Tags["updated"], 
      variable:"VAR_" + Save, 
      key: Key["updated"],
      min_browser_version: BrowserVersion["updated"],
      max_browser_version: MaxBrowserVersion["updated"],
      min_width: MinimumWidth["updated"],
      min_height: MinimumHeight["updated"],
      max_width: MaximumWidth["updated"],
      max_height: MaximumHeight["updated"],
      time_limit: AddedDate["updated"],
      perfectcanvas_request: PerfectCanvasRequest["updated"],
	    perfectcanvas_logs: PerfectCanvasDebug["updated"],
      enable_custom_server: EnableCustomServers["updated"],
      dynamic_perfect_canvas: DynamicPerfectCanvas["updated"],
      enable_precomputed_fingerprints: EnablePrecomputedFingerprints["updated"]
    })

  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}