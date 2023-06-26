#ifndef TESTSWIDGETINTERFACE_H
#define TESTSWIDGETINTERFACE_H

#include "../core/treemodel.h"
#include "mainwindowcontrollerinterface.h"

#include "mainwindow/statuscommonwidget.h"
#include <QIcon>

class TestWidgetInterface
{
public:
    virtual ~TestWidgetInterface();

    virtual void setToolItem(TreeItem *item) = 0;

    virtual void enableButtons()                                    = 0;
    virtual void disableButtons()                                   = 0;
    virtual void setEnabledRunButtonOfStatusWidgets(bool isEnabled) = 0;

    virtual void showDetails(QString detailsText) = 0;
    virtual void showAllTest()                    = 0;

    virtual void setWidgetStatus(ADTExecutable *task, StatusCommonWidget::WidgetStatus status, bool moveScroll = true)
        = 0;

    virtual void setController(MainWindowControllerInterface *controller) = 0;

    virtual std::vector<ADTExecutable *> getTasks() = 0;
};

#endif
