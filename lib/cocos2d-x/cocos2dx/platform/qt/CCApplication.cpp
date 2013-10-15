#include "CCApplication.h"
#include "CCEGLView.h"
#include "CCDirector.h"
#include <algorithm>
#include "platform/CCFileUtils.h"

#include <QTimer>
#include <QLocale>

/**
@brief    This function change the PVRFrame show/hide setting in register.
@param  bEnable If true show the PVRFrame window, otherwise hide.
*/
static void PVRFrameEnableControlWindow(bool bEnable);

NS_CC_BEGIN

// sharedApplication pointer
CCApplication * CCApplication::sm_pSharedApplication = 0;

CCApplication::CCApplication(int argc, char *argv[])
    : QApplication(argc, argv)
    , m_timer(NULL)
    , m_refCount(0)
{
    m_nAnimationInterval = 1.0f / 60.0f * 1000.0f;
    CC_ASSERT(! sm_pSharedApplication);
    sm_pSharedApplication = this;
}

CCApplication::~CCApplication()
{
    CC_ASSERT(this == sm_pSharedApplication);
    sm_pSharedApplication = NULL;
}

int CCApplication::run()
{
    // Initialize instance and cocos2d.
    if (! applicationDidFinishLaunching())
    {
        return 0;
    }

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    m_timer->start(m_nAnimationInterval);

    return CCApplication::sharedApplication()->exec();
}

void CCApplication::setAnimationInterval(double interval)
{
    m_nAnimationInterval = interval * 1000.0f;
    if (m_timer)
    {
        m_timer->start(m_nAnimationInterval);
    }
}

//////////////////////////////////////////////////////////////////////////
// static member function
//////////////////////////////////////////////////////////////////////////
CCApplication* CCApplication::sharedApplication()
{
    CC_ASSERT(sm_pSharedApplication);
    return sm_pSharedApplication;
}

cocos2d::ccLanguageType CCApplication::getCurrentLanguage()
{
    switch (QLocale::system().language())
    {
    case QLocale::Chinese: return cocos2d::kLanguageChinese;
    case QLocale::French:  return cocos2d::kLanguageFrench;
    case QLocale::Italian: return cocos2d::kLanguageItalian;
    case QLocale::German:  return cocos2d::kLanguageGerman;
    case QLocale::Spanish: return cocos2d::kLanguageSpanish;
    case QLocale::Russian: return cocos2d::kLanguageRussian;

    return cocos2d::kLanguageEnglish;
    }
}

cocos2d::TargetPlatform CCApplication::getTargetPlatform()
{
    return cocos2d::kTargetWindows;
}

void CCApplication::setStartupScriptFilename(const std::string& startupScriptFile)
{
    m_startupScriptFilename = startupScriptFile;
    int pos;
    while ((pos = m_startupScriptFilename.find_first_of("\\")) != std::string::npos)
    {
        m_startupScriptFilename.replace(pos, 1, "/");
    }
}

void CCApplication::timerUpdate()
{
    // m_refCount is here to prevent calling the mainloop from nested event loops
    if (!m_refCount)
    {
        cocos2d::CCDirector::sharedDirector()->mainLoop();
    }
}

NS_CC_END
