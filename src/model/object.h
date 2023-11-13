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
    Object()          = default;
    virtual ~Object() = default;

public:
    QString m_id{};
    QString m_displayName{};
    QString m_type{};
    QString m_icon{};
    std::vector<QString> m_interfaces{};

    QMap<QString, QString> m_nameLocaleStorage{};
    void setLocale(QString locale);

    //QString m_displayCategory{};
    //    QString m_categoryId{};
    //    bool m_terminal = false;
    //    QString m_x_Alterator_URI{};
    //    QString m_x_Alterator_Weight{};
    //    QString m_x_Alterator_Help{};
    //    QString m_x_Alterator_UI{};
    //    QString m_x_Alterator_Internal_Name{};
    //    std::vector<LocalApplication *> m_applications{};
    //bool m_isLegacy = false;
};
} // namespace model
} // namespace ab

#endif // AB_OBJECT_H
