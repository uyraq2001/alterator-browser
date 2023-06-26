#ifndef MODULESWIDGET_H
#define MODULESWIDGET_H

#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QWidget>

#include "interfaces/toolswidgetinterface.h"

namespace Ui
{
class ModulesWidget;
}

class ModulesWidget : public QWidget, public ToolsWidgetInterface
{
    Q_OBJECT
public:
    ModulesWidget(QWidget *parent = nullptr);
    ~ModulesWidget();

public:
    void setController(MainWindowControllerInterface *controller) override;

    void setModel(QAbstractItemModel *model) override;

    void enableButtons() override;
    void disableButtons() override;

    void enableExitButton() override;
    void disableExitButton() override;

    void setDescription(QString description) override;

    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void on_runAllPushButton_clicked();

    void on_browseCheckPushButton_clicked();

    void on_exitPushButton_clicked();

    void onSelectionChanged(const QItemSelection &newSelection, const QItemSelection &previousSelection);

private:
    Ui::ModulesWidget *ui;

    MainWindowControllerInterface *m_controller;

private:
    ModulesWidget(const ModulesWidget &) = delete;
    ModulesWidget(ModulesWidget &&)      = delete;
    ModulesWidget &operator=(const ModulesWidget &) = delete;
    ModulesWidget &operator=(ModulesWidget &&) = delete;
};

#endif // MODULESWIDGET_H
