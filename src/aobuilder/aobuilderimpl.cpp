#include "constants.h"

#include "aobuilderimpl.h"

#include "builders/localapplicationobjectbuilder.h"
#include "builders/objectbulderinterface.h"
#include <builders/objectbuilderfactory.h>

#include "datasource/datasourcedbusimpl.h"
#include "datasource/datasourceinterface.h"

#include <parsers/baseobjectparser.h>

namespace ao_builder
{
class AOBuilderImplPrivate
{
public:
    AOBuilderImplPrivate()
        : m_localAppBuilder(new LocalApplicationObjectBuilder())
        , m_dataSource(new DataSourceDBusImpl(DBUS_SERVICE_NAME))
        , m_parser(new BaseObjectParser())
    {}

public:
    std::unique_ptr<ObjectBuilderInterface> m_localAppBuilder;
    std::unique_ptr<DataSourceInterface> m_dataSource;
    std::unique_ptr<ObjectParserInterface> m_parser;
};

AOBuilderImpl::AOBuilderImpl()
    : d(new AOBuilderImplPrivate())
{}

AOBuilderImpl::~AOBuilderImpl()
{
    delete d;
}

std::vector<std::unique_ptr<Object>> AOBuilderImpl::buildLocalApps()
{
    QStringList appsList = d->m_dataSource->getLocalAppPaths();

    std::vector<std::unique_ptr<Object>> result{};

    for (QString currentApp : appsList)
    {
        QString currentAppInfo = d->m_dataSource->getLocalAppInfo(currentApp);

        std::unique_ptr<Object> currentObject = buildObject(currentAppInfo);

        if (currentObject)
        {
            result.push_back(std::move(currentObject));
        }
    }

    return result;
}

std::vector<std::unique_ptr<Object>> AOBuilderImpl::buildCategories()
{
    QStringList catList = d->m_dataSource->getCategoriesList();

    std::vector<std::unique_ptr<Object>> result;

    for (QString currentCategory : catList)
    {
        QString currentCatInfo = d->m_dataSource->getCategoryInfo(currentCategory);

        std::unique_ptr<Object> currentObject = buildObject(currentCatInfo);

        if (currentObject)
        {
            result.push_back(std::move(currentObject));
        }
    }

    return result;
}

std::vector<std::unique_ptr<Object>> AOBuilderImpl::buildLegacyObject()
{
    QStringList legacyObjectsList = d->m_dataSource->getLegacyObjectsPaths();

    std::vector<std::unique_ptr<Object>> result;

    for (QString currentLegacy : legacyObjectsList)
    {
        QString currentLegacyInfo = d->m_dataSource->getLegacyObjectInfo(currentLegacy);

        std::unique_ptr<Object> currentObject = buildObject(currentLegacyInfo);

        if (currentObject)
        {
            result.push_back(std::move(currentObject));
        }
    }

    return result;
}

std::vector<std::unique_ptr<Object>> AOBuilderImpl::buildObjects()
{
    QStringList objectsList = d->m_dataSource->getObjectsPath();

    std::vector<std::unique_ptr<Object>> result;

    for (QString currentObjectPath : objectsList)
    {
        QString currentObjectInfo = d->m_dataSource->getObjectInfo(currentObjectPath);

        std::unique_ptr<Object> currentObject = buildObject(currentObjectInfo);

        if (currentObject)
        {
            result.push_back(std::move(currentObject));
        }
    }

    return result;
}

std::unique_ptr<Object> AOBuilderImpl::buildObject(QString info)
{
    if (info.isEmpty() || !d->m_parser->parse(info))
    {
        return nullptr;
    }

    ObjectBuilderFactory factory;

    std::unique_ptr<ObjectBuilderInterface> objectBuilder = factory.getBuilder(d->m_parser.get());

    if (!objectBuilder)
    {
        return nullptr;
    }

    return objectBuilder->buildObject(d->m_parser.get());
}

} // namespace ao_builder
