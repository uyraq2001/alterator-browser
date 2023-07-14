#ifndef ABSTRACTMODULEMANAGER_H
#define ABSTRACTMODULEMANAGER_H

#include <QObject>

class AbstractModuleManager : public QObject
{
    Q_OBJECT
public:
    explicit AbstractModuleManager(QObject *parent = nullptr);

signals:

};

#endif // ABSTRACTMODULEMANAGER_H
