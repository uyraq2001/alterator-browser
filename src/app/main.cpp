#include "mainwindow.h"
#include "modelbuilder.h"

#include <QApplication>
#include <QStandardItemModel>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QLocale locale;
    QTranslator translator;
    QString language = locale.system().name().split("_").at(0);
    translator.load(language, ".");
    a.installTranslator(&translator);

    QStandardItemModel *model = new QStandardItemModel();
    ModelBuilder *builder = new ModelBuilder();
    builder->build(model);
    MainWindow w(model);
    w.show();
    return a.exec();
}
