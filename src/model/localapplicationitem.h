#ifndef AB_LOCAL_APPLICATION_ITEM_H
#define AB_LOCAL_APPLICATION_ITEM_H

#include "localappobject.h"

#include <QStandardItemModel>

namespace ab
{
namespace model
{
class LocalApplicationItem : public QStandardItem
{
public:
    LocalAppObject *getLocalApplicationObject();

    void setLocale(QString locale);

public:
    std::unique_ptr<LocalAppObject> m_acLocalApplication{nullptr};
};
} // namespace model
} // namespace ab

#endif // AB_LOCAL_APPLICATION_ITEM_H
