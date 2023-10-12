#include "objectinfogetter.h"
#include <thread>

namespace ab
{
namespace model
{
ObjectInfoGetter::ObjectInfoGetter(QString serviceName,
                                   QStringList objectsPaths,
                                   QString findInterface,
                                   QString methodName)
    : m_serviceName(serviceName)
    , m_objectsPaths(objectsPaths)
    , m_findInterface(findInterface)
    , m_methodName(methodName)
    , m_connection(QDBusConnection::systemBus())
    , m_resultsMtx()
    , m_results()
    , m_NumThreads(std::thread::hardware_concurrency())
    , m_q()
    , m_qMtx()
    , m_qCv()
    , m_completedTasksIds()
    , m_completedTasksIdsCv()
    , m_completedTasksIdsMtx()
    , m_threads()
    , m_quite(false)
    , m_lastIdx(0)
{
    m_threads.reserve(m_NumThreads);
    for (unsigned int i = 0; i < m_NumThreads; ++i)
    {
        m_threads.emplace_back(&ObjectInfoGetter::run, this);
    }

    for (QString &currentPath : m_objectsPaths)
    {
        addTask(currentPath);
    }
}

ObjectInfoGetter::~ObjectInfoGetter()
{
    waitForIbjectsInfo();
    m_quite = true;
    for (unsigned int i = 0; i < m_threads.size(); ++i)
    {
        m_qCv.notify_all();
        m_threads[i].join();
    }
}

QStringList ObjectInfoGetter::getResults()
{
    auto l = m_lastIdx.load();
    waitForIbjectsInfo();
    return m_results;
}

void ObjectInfoGetter::run()
{
    while (!m_quite)
    {
        std::unique_lock<std::mutex> lock(m_qMtx);

        m_qCv.wait(lock, [this]() -> bool { return !m_q.empty() || m_quite; });

        if (!m_q.empty())
        {
            auto elem = std::move(m_q.front());
            m_q.pop();
            lock.unlock();

            std::lock_guard<std::mutex> resultLock(m_resultsMtx);
            m_results.append(elem.first.get());

            std::unique_lock<std::mutex> clock(m_completedTasksIdsMtx);
            m_completedTasksIds.insert(elem.second);

            m_completedTasksIdsCv.notify_all();
        }
    }
}

void ObjectInfoGetter::waitForIbjectsInfo()
{
    std::unique_lock<std::mutex> lock(m_qMtx);

    m_completedTasksIdsCv.wait(lock, [this]() -> bool {
        std::lock_guard<std::mutex> task_lock(m_completedTasksIdsMtx);
        return m_q.empty() && m_lastIdx == m_completedTasksIds.size();
    });
}

unsigned int ObjectInfoGetter::addTask(QString objectPath)
{
    unsigned int newTaskIdx = m_lastIdx++;

    std::lock_guard<std::mutex> q_lock(m_qMtx);

    m_q.emplace(std::async(
                    std::launch::deferred,
                    [this](const QString path) -> QString {
                        QDBusInterface iface(m_serviceName, path, m_findInterface, m_connection);

                        if (!iface.isValid())
                        {
                            qWarning() << "Warning: object: " + path + " doesn't provide interface " + m_findInterface;

                            return QString();
                        }

                        QDBusReply<QByteArray> reply = iface.call(m_methodName);

                        if (!reply.isValid())
                        {
                            qWarning() << "Warning: object: " + path + " info reply is not valid " + m_findInterface;

                            return QString();
                        }
                        if (reply.value().isEmpty())
                        {
                            qWarning() << "Warning: Can't get info of object: " + path
                                              + " in interface: " + m_findInterface;
                        }

                        return QString(reply.value());
                    },
                    objectPath),
                newTaskIdx);

    m_qCv.notify_one();

    return newTaskIdx;
}

} // namespace model
} // namespace ab
