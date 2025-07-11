_L = 
{
	"Control browser directly":{"ru": "Прямое управление браузером"},
	"Search":{"ru": "Поиск"},
	"Use drag to find by image":{"ru": "Выделите область для поиска по изображению"},
	"Use click to find by selector":{"ru": "Используйте клик для поиска по селектору"},
	"Nothing found":{"ru": "Ничего не найдено"},

	"Move and click on coordinates":{"ru": "Двигать мышь и кликнуть"},
	"Click on coordinates":{"ru": "Кликнуть"},
	"Move on coordinates":{"ru": "Двигать мышь"},
	"Move and click on element":{"ru": "Двигать мышь и кликнуть по элементу"},
	"Click on element":{"ru": "Кликнуть по элементу"},
	"Move on element":{"ru": "Двигать мышь над элементом"},
	"Type text":{"ru": "Ввод текста"},
	"Clear edit":{"ru": "Очистить поле"},
	"Inspect element in devtools":{"ru": "Изучить элемент на странице"},
	"Add new tab":{"ru": "Добавить вкладку"},
	"Get information about tabs": {"ru": "Получить информацию о вкладках"},
	"Select tab":{"ru": "Переключиться"},
	"Remove tab":{"ru": "Закрыть"},
	"Tabs":{"ru": "Вкладки"},
	"Is element exists":{"ru": "Проверить существование элемента"},
	"Get element content(html)":{"ru": "Получить код элемента"},
	"Get element text":{"ru": "Получить текст"},
	"Execute javascript on element":{"ru": "Выполнить javascript на элементе"},
	"Screenshot":{"ru": "Скриншот"},
	"Solve captcha":{"ru": "Решить капчу"},
	"Wait while element exists":{"ru": "Ждать появления элемента"},
	"Scroll to element":{"ru": "Прокрутка к элементу"},
	"Get element coordinates":{"ru": "Получить координаты элемента"},
	"Get elements count":{"ru": "Получить количество элементов"},
	"Get element style":{"ru": "Получить стиль элемента"},
	"Drag from coordinates":{"ru": "Начать перетягивание"},
	"Drop on coordinates":{"ru": "Закончить перетягивание"},
	"Drag from element":{"ru": "Начать перетягивание на элементе"},
	"Drop on element":{"ru": "Закончить перетягивание на элементе"},
	"Get element attribute":{"ru": "Получить атрибут элемента"},
	"Set element attribute":{"ru": "Установить атрибут элемента"},
	"Set combobox by value":{"ru": "Установить значение комбобокса"},
	"Set combobox by index":{"ru": "Установить индекс комбобокса"},
	"Set combobox to random":{"ru": "Выбрать случайный элемент из комбобокса"},
	"Start loop":{"ru": "Начать цикл"},
	"For each element":{"ru": "Для каждого элемента"},
	"Solve captcha with clicks": {"ru": "Решить капчу кликами"},
	"Solve Recaptcha 2.0":{"ru": "Решить Recaptcha 2.0"},
	"Solve FunCaptcha": {"ru": "Решить FunCaptcha"},
	"Solve HCaptcha": {"ru": "Решить HCaptcha"},
	"Get link URL" : {"ru": "Получить адрес ссылки"},

}


function tr(key)
{
	if(typeof(key) == "undefined")
	{

		var all = $(".tr")
		
		for(var i = 0;i< all.length;i++)
		{
			var el = $(all[i])
			if(typeof(el.attr("tr")) == "undefined" || el.attr("tr") != "true" )
			{
				el.html(tr(el.html()))
				el.attr("tr","true")
			}
		}

		all = $(".trtitle")
		
		for(var i = 0;i< all.length;i++)
		{
			var el = $(all[i])
			if(typeof(el.attr("tr")) == "undefined" || el.attr("tr") != "true" )
			{
				el.attr("title",tr(el.attr("title")))
				el.attr("tr","true")
			}
		}

		var all = $("*[placeholder]")
		
		for(var i = 0;i< all.length;i++)
		{
			var el = $(all[i])
			if(typeof(el.attr("tr")) == "undefined" || el.attr("tr") != "true" )
			{
				el.attr("placeholder",tr(el.attr("placeholder")))
				el.attr("tr","true")
			}
		}

		
		
		return;
	}

	
	if(typeof(_K) == "undefined" || _K == "en")
		return key;
	if(key in _L)
	{

		if(_K in _L[key])
		{
			return _L[key][_K]
		}
	}
	return key;
}