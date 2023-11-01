#ifndef AB_MODEL_LOCAL_APPLICATION_MODEL_H
#define AB_MODEL_LOCAL_APPLICATION_MODEL_H

#include "localapplication.h"

#include <QStandardItemModel>

namespace ab
{
namespace model
{
class LocalApplicationModel : public QStandardItemModel
{
public:
    std::vector<LocalApplication *> getAppsByInterface(QString interface);

    void setLocale();
};
} // namespace model
} // namespace ab

#endif // AB_MODEL_LOCAL_APPLICATION_MODEL_H
