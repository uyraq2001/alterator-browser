#ifndef AB_MODEL_LOCAL_APPLICATION_ITEM_H
#define AB_MODEL_LOCAL_APPLICATION_ITEM_H

#include "localapplication.h"

#include <QStandardItemModel>

namespace ab
{
namespace model
{
class LocalApplicationItem : public QStandardItem
{
public:
    LocalApplication *getLocalApplicationObject();

    void setLocale(QString locale);

public:
    std::unique_ptr<LocalApplication> m_acLocalApplication{nullptr};
};
} // namespace model
} // namespace ab

#endif // AB_MODEL_LOCAL_APPLICATION_ITEM_H
