_L = 
{
    "Failed to parse proxy": {"ru": "Не удалось распарсить прокси"},
	"Failed to load page ": {"ru": "Не удалось загрузить страницу "},
	"Failed to wait for changing url ": {"ru": "Не удалось дождаться смены url "},
	"Failed to wait for loading ": {"ru": "Не удалось дождаться загрузки "},
	"Failed to wait for content ": {"ru": "Не удалось дождаться содержимого "},
	"Failed to wait for css ": {"ru": "Не удалось дождаться элемента "},
    "Failed to wait for element ": {"ru": "Не удалось дождаться элемента "},
    "Failed to wait of state complete": {"ru": "Не удалось дождаться полной загрузки страницы"},
    "Image tag not found. Is this element captcha?": {"ru": "Тег img не найден. Этот элемент точно является капчей?"},
    "Url ": {"ru": "Страницы "},
    "Action 'Start Loop' has not yet been called.": {"ru": "Действие 'Начать цикл' еще не было вызвано."},
    " is not present in cache. Please enable cache before page load with Cache Mask Allow action": {"ru": " нет в кэше. Включите кэш перед загрузкой действием Разрешить кэш"},
    "Waiting for element ": {"ru": "Ожидаю элемент "},
    "Proxy should be set before any other action that uses browser. This rule applies only if you are using Multilogin.": {"ru": "Прокси должен быть установлен перед любым другим действием, использующим браузер. Это правило применяется только в том случае, если вы используете Multilogin."}
}


function tr(key)
{
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
