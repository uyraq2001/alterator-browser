#include "mainwindow.h"
#include "modelbuilder.h"

#include <QApplication>
#include <QStandardItemModel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStandardItemModel *model = new QStandardItemModel();
    ModelBuilder *builder = new ModelBuilder(model);
    builder->build(model);
    MainWindow w(model);
    w.show();
    return a.exec();
}
