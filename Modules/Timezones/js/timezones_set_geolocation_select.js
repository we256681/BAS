var Latitude = GetInputConstructorValue("Latitude", loader);
var Longitude = GetInputConstructorValue("Longitude", loader);
var Geolocation = GetInputConstructorValue("GeolocationObject", loader);


 try{
  var code = loader.GetAdditionalData() + _.template($("#timezones_set_geolocation_code").html())({latitude: Latitude["updated"],longitude: Longitude["updated"],geolocation: Geolocation["updated"]})
  code = Normalize(code,0)
  BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e)
{}