#ifndef ACINTERFACE_H
#define ACINTERFACE_H

#include "abstractacentity.h"
#include "definiciones.h"

class ACInterface : public AbstractACEntity
{
public:
    ACInterface();
    ~ACInterface();

    void setLocale(QString locale) override;

    ACObject *toObject() override;
    ACCategory *toCategory() override;
    ACInterface *toInterface() override;
    ACApplication *toAplication() override;

    QString m_name;
};

#endif // ACINTERFACE_H
