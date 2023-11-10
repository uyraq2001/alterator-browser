#ifndef AB_OBJECT_H
#define AB_OBJECT_H

#include "localapplication.h"

namespace ab
{
namespace model
{
class Object
{
public:
    void setLocale(QString locale);

    QString m_id{};
    QString m_displayName{};
    QString m_displayCategory{};
    QString m_type{};
    QString m_categoryId{};
    bool m_terminal = false;
    QString m_icon{};
    QString m_x_Alterator_URI{};
    QString m_x_Alterator_Weight{};
    QString m_x_Alterator_Help{};
    QString m_x_Alterator_UI{};
    QString m_x_Alterator_Internal_Name{};

    std::vector<QString> m_interfaces{};
    std::vector<LocalApplication *> m_applications{};

    QMap<QString, QString> m_nameLocaleStorage{};

    bool m_isLegacy = false;
};
} // namespace model
} // namespace ab

#endif // AB_OBJECT_H
