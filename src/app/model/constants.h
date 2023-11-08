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
const char *const MANAGER_IFACES_METHOD                = "GetInterfaces";

const char *const DBUS_LOCAL_APP_PATH              = "/ru/basealt/alterator/applications";
const char *const DBUS_LOCAL_APP_INTERFACE_NAME    = "ru.basealt.alterator.applications";
const char *const DBUS_LOCAL_APP_GET_LIST_OF_FILES = "List";
const char *const DBUS_LOCAL_APP_GET_DESKTOP_FILE  = "Info";

const QString LOCAL_APP_DESKTOP_ENTRY_SECTION_NAME           = "Desktop Entry";
const QString LOCAL_APP_NAME_KEY_NAME                        = "name";
const QString LOCAL_APP_GENERIC_NAME_KEY_NAME                = "genericname";
const QString LOCAL_APP_COMMENT_KEY_NAME                     = "comment";
const QString LOCAL_APP_TRY_EXEC_KEY_NAME                    = "tryexec";
const QString LOCAL_APP_ICON_KEY_NAME                        = "icon";
const QString LOCAL_APP_TYPE_KEY_NAME                        = "type";
const QString LOCAL_APP_CATEGORIES_KEY_NAME                  = "categories";
const QString LOCAL_APP_KEYWORDS_KEY_NAME                    = "keywords";
const QString LOCAL_APP_INTERFACE_KEY_NAME                   = "implements";
const QString LOCAL_APP_EXEC_KEY_NAME                        = "exec";
const QString LOCAL_APP_MIMETYPE_KEY_NAME                    = "mimetype";
const QString LOCAL_APP_X_ALTERATOR_ENTRY_SECTION_NAME       = "X-Alterator Entry";
const QString LOCAL_APP_X_ALTERATOR_ENTRY_SECTION_PREFIX     = "X-Alterator Application";
const QString LOCAL_APP_X_ALTERATOR_ENTRY_SECTION_KEY_NAME   = "exec";
const QString LOCAL_APP_X_ALTERATOR_ENTRY_INTERFACE_LIST_KEY = "interfaces";
const QString LOCAL_APP_X_ALTERATOR_ENTRY_APPS_LIST_KEY      = "aplications";

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
const QString APPLICATIONS_LIST_KEY      = "applications";
const QString XALTERATOR_SECTION         = "X-Alterator Entry";
const QString CATEGORY_KEY               = "categoryinternalname";
const QString ICON_KEY                   = "icon";
const QString NAME_KEY                   = "name";
} // namespace xalterator_entry
} // namespace model
} // namespace ab

#endif // AB_MODEL_CONSTANTS_H
