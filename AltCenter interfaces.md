# Добавление модулей AltCenter

Для корректной работы AltCenter необходимы пакеты `alterator-manager` и `alterator-module-executor`.

## Интерфейс добавления модулей

Чтобы быть отобажённым в AltCenter модуль должен быть зарегистрирован на D-Bus, как один из подобъектов объекта `ru.basealt.alterator`, путём добавления .backend-файла в директорию `/usr/share/alterator/backends`, и реализовывать интерфейс `ru.basealt.alterator.application` с методами `info` и `category`, которые должны возвращать в формате .desktop-файла информацию о модуле и его категории, соответственно. Ниже можно увидеть пример такого .backend-файла:

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

## Интерфейс добавления интерфейсов модулей

Для того чтобы приложение реализующее интерфейс модуля альтератора было обнаружено AltCenter\`ом его .desktop-файл расположенный в `/usr/share/applications`, должен содержать секцию `[X-Alterator]` и имя реализуемого данным приложением интерфейса вычитывается из свойства `interface`.

```
[X-Alterator]
Interface=ru.basealt.alterator.diag1
```

Также `[Desktop Entry]` .desktop-файла должна содержать запись `Exec`.
