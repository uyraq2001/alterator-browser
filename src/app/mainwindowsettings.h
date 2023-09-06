#ifndef AB_MAIN_WINDOW_SETTINGS_H
#define AB_MAIN_WINDOW_SETTINGS_H

#include "ui_mainwindow.h"

#include <QObject>

namespace ab
{
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
} // namespace ab

#endif // AB_MAIN_WINDOW_SETTINGS_H
