# Добавление модулей AltCenter

Для корректной работы AltCenter необходимы пакеты `alterator-manager` и `alterator-module-executor`.

## Интерфейс добавления модулей

Чтобы быть отобажённым в AltCenter модуль должен быть зарегистрирован на D-Bus, как один из подобъектов объекта `ru.basealt.alterator`, путём добавления .backend-файла в директорию `/usr/share/alterator/backends`, и реализовывать интерфейс `ru.basealt.alterator.application` с методами `info` и `category`. Ниже можно увидеть пример такого .backend-файла:

```
[Manager]
module_name = executor
node_name = adtapp

interface_name = application

[info]
execute = cat /usr/share/alterator/module-info/adt-app.desktop
stdout_strings = enabled

[category]
execute = bash /usr/share/alterator/backends/script.sh /usr/share/alterator/module-info/adt-app.desktop
stdout_strings = enabled
```

Более подробно с форматом .backend-файлов можно ознакомиться в [репозитории проекта alterator-manager](https://gitlab.basealt.space/alt/alterator-manager/-/blob/master/docs/README-ru.md).

Метод `info` должен возвращать информацию о модуле в формате .desktop-файла: 

```
[Desktop Entry]
Type=Application
Categories=/usr/share/alterator/desktop-directories/system.directory
Icon=net-wifi
Terminal=false
Name=Alt Diagnostic Tool
Name[ru]=Инструмент диагностики

Name[ru_RU]=adt-app.desktop

[X-Alterator]
Interface=ru.basealt.alterator.diag1;
```

А метод `category` возвращает данные о категории к которой относится модуль в формате .directory-файла:

```
[Desktop Entry]
Type=Directory
Name=System
Comment=Operating system information and setup
Icon=groups/system
X-Alterator-Category=X-Alterator-System
Name[ru]=Система
Name[uk]=Система
Name[pt_BR]=Sistema
Name[es]=Sistema
Comment[ru]=Информация об установленной системе и её настройка
Comment[uk]=Налаштування операційної системи та інформація щодо неї
Comment[pt_BR]=Informações e Configurações do Sistema operacional
Comment[es]=Información del sistema operativo y la configuración
```

Для хранения этих файлов предусмотренны директории `/usr/share/alterator/module-info/` и `/usr/share/alterator/desktop-directories/`.

## Интерфейс добавления интерфейсов модулей

Для того чтобы приложение реализующее интерфейс модуля альтератора было обнаружено AltCenter\`ом его .desktop-файл расположенный в `/usr/share/applications`, должен содержать секцию `[X-Alterator]` и имя реализуемого данным приложением интерфейса вычитывается из свойства `interface`.

```
[X-Alterator]
Interface=ru.basealt.alterator.diag1
```

Также `[Desktop Entry]` .desktop-файла должна содержать запись `Exec`.
