#ifndef ENUMS_H
#define ENUMS_H

#include <QObject>

enum UserRoles: int{
    DescriptionRole = Qt::UserRole,
    ActionRole
}; /*Q_ENUM(UserRoles)*/

enum AlteratorModes: int{
    StandardMode,
    DevMode
}; /*Q_ENUM(UserRoles)*/

#endif // ENUMS_H
