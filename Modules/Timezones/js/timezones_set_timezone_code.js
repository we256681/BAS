var UpdatedSettings = {}
if((<%= value %>).toString().length > 0)
{
    UpdatedSettings["Timezone"] = (-parseInt(<%= value %>)).toString()
}

if((<%= timezonename %>).toString().length > 0)
{
    UpdatedSettings["TimezoneName"] = <%= timezonename %>
}

_settings(UpdatedSettings)!
sleep(1000)!