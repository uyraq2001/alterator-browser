#ifndef OBJECTINFOGETTER_H
#define OBJECTINFOGETTER_H

#include <future>
#include <mutex>
#include <unordered_set>
#include <QDBusInterface>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <qdbusreply.h>
#include <queue>

namespace ab
{
namespace model
{
class ObjectInfoGetter
{
public:
    ObjectInfoGetter(QString serviceName, QStringList objectsPaths, QString findInterface, QString methodName);
    ~ObjectInfoGetter();

public:
    QStringList getResults();

private:
    QString m_serviceName;
    QStringList m_objectsPaths;
    QString m_findInterface;
    QString m_methodName;
    QDBusConnection m_connection;

    std::mutex m_resultsMtx;
    QStringList m_results;

    unsigned int m_NumThreads;

    std::queue<std::pair<std::future<QString>, int>> m_q;
    std::mutex m_qMtx;
    std::condition_variable m_qCv;

    std::unordered_set<int> m_completedTasksIds;
    std::condition_variable m_completedTasksIdsCv;
    std::mutex m_completedTasksIdsMtx;

    std::vector<std::thread> m_threads;

    std::atomic<bool> m_quite{false};

    std::atomic<unsigned int> m_lastIdx;

private:
    void run();

    void waitForIbjectsInfo();

    unsigned int addTask(QString objectPath);
};

} // namespace model

} // namespace ab

#endif // OBJECTINFOGETTER_H
