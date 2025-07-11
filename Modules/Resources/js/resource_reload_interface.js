<div class="container-fluid">
    <%= _.template($('#resource_constructor').html())({id:"ResourceName", description:tr("Resource name"), help: 
{
	description: tr("Resource name, which will be reloaded")
}}) %>
</div>

<div class="tooltipinternal">
      <div class="tr tooltip-paragraph-first-fold">Reload lines from file or rows from database.</div>
      <div class="tr tooltip-paragraph-fold">Resource data will be completely recreated. All data about resource items state(successes and failures) will be lost. If some resource items was deleted, then this information will be lost too.</div>
      <div class="tr tooltip-paragraph-last-fold">This action loads heavily cpu and hard drive, so it is not recomended to use it multithreaded mode.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true, name: tr("Delete Current Element")}) %>
