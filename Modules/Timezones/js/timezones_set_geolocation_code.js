_if((<%= latitude %>).toString().length > 0 && (<%= longitude %>).toString().length > 0, function(){
    geolocation(parseFloat(<%= latitude %>),parseFloat(<%= longitude %>))!
})!

geolocation_object(<%= geolocation %>)!
sleep(1000)!