#ifndef MAINWINDOWSETTINGS_H
#define MAINWINDOWSETTINGS_H

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QObject>

class MainWindow;
class MainWindowSettingsPrivate;

class MainWindowSettings : public QObject
{
    Q_OBJECT
public:
    MainWindowSettings(MainWindow *mWindow, Ui::MainWindow *ui);
    ~MainWindowSettings();

    void restoreSettings();

public slots:
    void saveSettings();

private:
    MainWindowSettings(const MainWindowSettings &) = delete;
    MainWindowSettings(MainWindowSettings &&)      = delete;
    MainWindowSettings &operator=(const MainWindowSettings &) = delete;
    MainWindowSettings &operator=(MainWindowSettings &&) = delete;

private:
    MainWindowSettingsPrivate *d;
};

#endif // MAINWINDOWSETTINGS_H
