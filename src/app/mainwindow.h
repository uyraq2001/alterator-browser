#ifndef AB_MAIN_WINDOW_H
#define AB_MAIN_WINDOW_H

#include "mainwindowsettings.h"
#include "model/modelinterface.h"
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

class MainWindowPrivate;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

public:
    MainWindow(const MainWindow &)            = delete;
    MainWindow(MainWindow &&)                 = delete;
    MainWindow &operator=(const MainWindow &) = delete;
    MainWindow &operator=(MainWindow &&)      = delete;

public:
    void closeEvent(QCloseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    void setController(Controller *c);

    void setModel(model::ModelInterface *m);
    void clearUi();

    //    void showModuleMenu(PushButton *button, std::unique_ptr<QMenu> menu);
    void onModuleClicked(PushButton *button);
    //    void onInterfaceClicked(ao_builder::LocalAppObject *app);

private:
    MainWindowPrivate *d;
};
} // namespace ab

#endif // AB_MAIN_WINDOW_H
