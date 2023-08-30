#ifndef AB_LOCAL_APPLICATION_MODEL_H
#define AB_LOCAL_APPLICATION_MODEL_H

#include "localapplication.h"

#include <QStandardItemModel>

class LocalApplicationModel : public QStandardItemModel
{
public:
    LocalApplicationModel();
    ~LocalApplicationModel();

    std::vector<LocalApplication *> getAppsByInterface(QString interface);

    void setLocale();
};

#endif // AB_LOCAL_APPLICATION_MODEL_H
