#ifndef MODELBUILDER_H
#define MODELBUILDER_H

#include <QObject>
#include <QStandardItemModel>
#include <QDBusMessage>

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

class ModelBuilder : public QObject
{
    Q_OBJECT
public:
    ModelBuilder(QObject *parent = nullptr);

    bool build(QStandardItemModel *model);

private:
    ModelBuilder(const ModelBuilder &) = delete;
    ModelBuilder(ModelBuilder &&)      = delete;
    ModelBuilder &operator=(const ModelBuilder &) = delete;
    ModelBuilder &operator=(ModelBuilder &&) = delete;

    QVariantMap findTranslations(QString field, QMap<QString, QString> dump);
    QMap<QString, QVariant> parseDesktopFile(QStringList data);
    QMap<QString, QVariant> getNextLevelOfPtree(boost::property_tree::ptree pt);
    QMap<QString, QString> readIniFromDBus(QDBusMessage mess);
};

#endif // MODELBUILDER_H
