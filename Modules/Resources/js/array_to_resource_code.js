RClear(<%= key %>)

RSync(<%= key %>)

for(var i = 0;i<(<%= variable %>).length;i++)
{
	RInsert(<%= key %>,(<%= variable %>)[i],false)
}

RSync(<%= key %>)
