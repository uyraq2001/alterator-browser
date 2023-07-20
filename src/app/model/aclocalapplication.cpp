#include "aclocalapplication.h"

ACLocalApplication::ACLocalApplication()
    : m_id()
    , m_name()
    , m_genericName()
    , m_displayName()
    , m_displayComment()
    , m_try_Exec()
    , m_exec()
    , m_icon()
    , m_type()
    , m_displayKeywords()
    , m_implementedInterface()
    , m_categories()
    , m_mimeTypes()
    , m_nameLocaleStorage()
    , m_genericNameLocaleStorage()
    , m_keywordsLocaleStorage()
    , m_commentLocaleStorage()

{}

ACLocalApplication::~ACLocalApplication() {}

void ACLocalApplication::setLocale() {}
