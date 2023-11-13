#ifndef AB_LOCAL_APPLICATION_MODEL_H
#define AB_LOCAL_APPLICATION_MODEL_H

#include "localappobject.h"

#include <QStandardItemModel>

namespace ab
{
namespace model
{
class LocalApplicationModel : public QStandardItemModel
{
public:
    std::vector<LocalAppObject *> getAppsByInterface(QString interface);

    void setLocale();
};
} // namespace model
} // namespace ab

#endif // AB_LOCAL_APPLICATION_MODEL_H