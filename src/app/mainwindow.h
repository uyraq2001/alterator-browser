#ifndef AB_MAIN_WINDOW_H
#define AB_MAIN_WINDOW_H

#include "model/model.h"
#include "pushbutton.h"

#include <utility>

#include <QMainWindow>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

namespace ab
{
class CategoryWidget;
class Controller;
class MainWindowSettings;
class PushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent *event);

    void setController(Controller *c);

    bool eventFilter(QObject *watched, QEvent *event);

    void setModel(model::Model *m);
    void clearUi();

    void showModuleMenu(model::ObjectItem *item);
    void onModuleClicked(PushButton *button);
    void onInterfaceClicked(model::LocalApplication *app);

private:
    MainWindow(const MainWindow &) = delete;
    MainWindow(MainWindow &&)      = delete;
    MainWindow &operator=(const MainWindow &) = delete;
    MainWindow &operator=(MainWindow &&) = delete;

    Ui::MainWindow *ui;
    QStandardItemModel *model;
    Controller *controller;
    std::unique_ptr<MainWindowSettings> settings;

signals:

    void showMenu(model::ObjectItem *item);
};
} // namespace ab

#endif // AB_MAIN_WINDOW_H
