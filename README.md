# BAS (Browser Automation Studio) - Docker Implementation

Browser Automation Studio - приложение для автоматизации браузера с полнофункциональным веб-интерфейсом.

## 🚀 Quick Start

### Docker Compose (Рекомендуется)
```bash
# Клонировать репозиторий
git clone https://github.com/we256681/BAS.git
cd BAS

# Собрать и запустить контейнер
docker-compose up --build

# Или в фоновом режиме
docker-compose up --build -d
```

### Manual Docker Build
```bash
# Собрать образ
docker build -t bas-app .

# Запустить контейнер
docker run -p 10000-20000:10000-20000 -p 8080:8080 bas-app
```

## 🌐 Web Interface Access

После запуска контейнера веб-интерфейс доступен по адресам:

- **🏠 Главная страница**: http://localhost:8080
- **🔧 Toolbox (Monaco Editor)**: http://localhost:8080/toolbox
- **📝 Scenario Editor**: http://localhost:8080/scenario  
- **⚙️ Settings**: http://localhost:8080/central
- **📅 Scheduler**: http://localhost:8080/scheduler

## ✨ Возможности

### 📝 Создание и редактирование скриптов
- **Monaco Editor** - полнофункциональный редактор кода с подсветкой синтаксиса
- **IntelliSense** - автодополнение и проверка синтаксиса
- **Поддержка переменных BAS** через маркеры `[[VARIABLE_NAME]]`
- **Выполнение JavaScript** в контексте браузера

### 🎯 Визуальное создание сценариев
- **Drag & Drop** интерфейс для создания скриптов
- **Готовые блоки действий** для автоматизации браузера
- **Визуальное представление** логики скрипта

### 🔍 Тестирование и отладка
- **Chrome DevTools** интеграция для отладки
- **Пошаговое выполнение** скриптов
- **Система обработки ошибок** с переменными `[[WAS_ERROR]]` и `[[LAST_ERROR]]`
- **Валидация скриптов** перед выполнением

### 📦 Работа со сторонними скриптами
- **Загрузка проектов** из файлов
- **Импорт/экспорт** готовых скриптов
- **Автогенерация интерфейсов** для сторонних скриптов

## 🔌 WebSocket Connection

Приложение автоматически запускает WebSocket сервер на случайном порту в диапазоне 10000-20000.

### Проверка статуса WebSocket
```bash
# Просмотр логов контейнера для поиска порта WebSocket
docker logs <container_id>

# Или для docker-compose
docker-compose logs
```

### Подключение к WebSocket
```javascript
// Пример подключения к WebSocket серверу
const ws = new WebSocket('ws://localhost:PORT');
ws.onopen = () => console.log('Connected to BAS WebSocket');
```

## 🛠️ Development

### Локальная разработка
```bash
# Установка зависимостей Node.js
npm install

# Запуск только веб-сервера (для разработки)
npm start

# Веб-интерфейс будет доступен на http://localhost:8080
```

### Структура проекта
```
BAS/
├── ChromeWorker/html/          # HTML интерфейсы
│   ├── toolbox/               # Monaco Editor
│   ├── scenario/              # Визуальный редактор
│   ├── central/               # Настройки
│   └── scheduler/             # Планировщик
├── Engine/                    # Основная логика C++
├── docker-web-server.js       # Node.js веб-сервер
├── Dockerfile                 # Docker конфигурация
└── docker-compose.yml         # Docker Compose
```

## 🐛 Troubleshooting

### Контейнер не запускается
```bash
# Проверить логи
docker-compose logs

# Пересобрать образ
docker-compose build --no-cache
```

### Веб-интерфейс недоступен
```bash
# Проверить статус контейнера
docker ps

# Проверить порты
netstat -tulpn | grep 8080
```

### JavaScript ошибки в браузере
1. Открыть Developer Tools (F12)
2. Проверить Console на ошибки
3. Убедиться что все статические файлы загружаются

**Распространенные JavaScript ошибки:**
- `SyntaxError: Unexpected token 'else'` - исправлено через комплексную обработку Underscore.js шаблонов
- `Template file disabled in Docker mode` - нормальное поведение, шаблоны заменены заглушками
- Monaco Editor не загружается - проверить что все зависимости обработаны корректно

### WebSocket подключение не работает
1. Проверить логи контейнера для порта WebSocket
2. Убедиться что порты 10000-20000 открыты
3. Проверить firewall настройки

### Проблемы с обработкой шаблонов
Docker версия использует комплексную систему обработки Underscore.js шаблонов:
- Файлы с обширным template синтаксисом заменяются функциональными заглушками
- Сложные control structures (`if/else`, циклы) полностью удаляются
- Template expressions (`<%= %>`) заменяются пустыми строками
- Orphaned `else` statements автоматически очищаются

**Файлы с template обработкой:**
- `functionmanager.js` - заменен полной функциональной заглушкой
- `tooledViewPlugin.js` - обработка template expressions
- `documents.js` - создание DocumentsStore заглушки
- `modal.js` - обработка Inspector.Modal компонентов

## 📚 Usage Examples

### Создание простого скрипта
1. Открыть http://localhost:8080/toolbox
2. В Monaco Editor написать JavaScript код:
```javascript
// Пример автоматизации
document.getElementById("search").value = "BAS automation";
document.querySelector("button[type=submit]").click();
```

### Использование переменных BAS
```javascript
// Чтение переменной
let searchTerm = [[SEARCH_QUERY]];

// Запись в переменную
[[RESULT_TEXT]] = document.querySelector(".result").innerText;
```

### Обработка ошибок
```javascript
try {
    // Ваш код
    document.querySelector(".element").click();
} catch (error) {
    [[WAS_ERROR]] = true;
    [[LAST_ERROR]] = error.message;
}
```

### Работа с визуальным редактором сценариев
1. Открыть http://localhost:8080/scenario
2. Использовать Drag & Drop для создания блоков действий
3. Настроить параметры каждого блока
4. Тестировать сценарий пошагово

### Отладка и тестирование
1. Использовать Chrome DevTools для отладки JavaScript
2. Проверять переменные BAS через консоль браузера
3. Мониторить WebSocket соединение для связи с backend
4. Использовать встроенную валидацию скриптов

### Загрузка сторонних скриптов
1. Открыть http://localhost:8080/scheduler
2. Импортировать готовые проекты через интерфейс
3. Настроить параметры выполнения
4. Запустить автоматическое выполнение по расписанию

## 🤝 Contributing

1. Fork репозиторий
2. Создать feature branch
3. Внести изменения
4. Создать Pull Request

## 📄 License

Этот проект использует оригинальную лицензию BAS (Browser Automation Studio).

## 🔗 Links

- **Original BAS**: [Browser Automation Studio](https://bablosoft.com/shop/BrowserAutomationStudio)
- **Docker Hub**: (будет добавлено после публикации)
- **Documentation**: См. файлы в директории `ChromeWorker/html/`
