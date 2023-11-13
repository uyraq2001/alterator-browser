#ifndef LEGACYOBJECT_H
#define LEGACYOBJECT_H

#include "basicobject.h"

namespace ab
{
namespace model
{
class LegacyObject final : public BasicObject
{
public:
    LegacyObject();
    ~LegacyObject();

    void setLocale(QString locale) override;

public:
    bool m_terminal = false;
    QString m_x_Alterator_URI{};
    QString m_x_Alterator_Weight{};
    QString m_x_Alterator_Help{};
    QString m_x_Alterator_UI{};
    QString m_x_Alterator_Internal_Name{};
};

} // namespace model

} // namespace ab

#endif // LEGACYOBJECT_H
