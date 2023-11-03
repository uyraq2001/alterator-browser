#ifndef AB_MODEL_CONSTANTS_H
#define AB_MODEL_CONSTANTS_H

#include <QString>

const char *const DBUS_SERVICE_NAME                    = "ru.basealt.alterator";
const char *const DBUS_PATH                            = "/ru/basealt/alterator";
const char *const DBUS_FIND_INTERFACE_NAME             = "ru.basealt.alterator.displayable";
const char *const DBUS_MANAGER_INTERFACE_NAME          = "ru.basealt.alterator.manager";
const char *const GET_OBJECTS_METHOD_NAME              = "GetObjects";
const char *const INFO_METHOD_NAME_FOR_ACOBJECT        = "Info";
const char *const CATEGORY_INTERFACE_NAME_FOR_ACOBJECT = "ru.basealt.alterator.categories";
const char *const CATEGORY_METHOD_NAME_FOR_ACOBJECT    = "Info";
const char *const CATEGORY_OBJECT_PATH                 = "/ru/basealt/alterator/categories";

const char *const DBUS_LOCAL_APP_PATH              = "/ru/basealt/alterator/applications";
const char *const DBUS_LOCAL_APP_INTERFACE_NAME    = "ru.basealt.alterator.applications";
const char *const DBUS_LOCAL_APP_GET_LIST_OF_FILES = "List";
const char *const DBUS_LOCAL_APP_GET_DESKTOP_FILE  = "Info";

namespace ab
{
namespace model
{
namespace xalterator_entry
{
const QString OBJECT_SECTION_PREXIX      = "X-Alterator Object";
const QString CATEGORY_SECTION_PREXIX    = "X-Alterator Category";
const QString APPLICATION_SECTION_PREXIX = "X-Alterator Application";
const QString OBJECTS_LIST_KEY           = "object";
const QString CATEGORIES_LIST_KEY        = "category";
const QString APPLICATIONS_LIST_KEY      = "application";
const QString XALTERATOR_SECTION         = "X-Alterator Entry";
const QString CATEGORY_KEY               = "categoryinternalname";
const QString ICON_KEY                   = "icon";
const QString NAME_KEY                   = "name";
} // namespace xalterator_entry
} // namespace model
} // namespace ab

#endif // AB_MODEL_CONSTANTS_H
