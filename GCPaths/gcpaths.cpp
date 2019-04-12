#include "gcpaths.h"
#include <QStandardPaths>
#include <QDir>
#include <QApplication>
Q_GLOBAL_STATIC(GCPaths, gc_paths)//全局静态类
#include <qcoreapplication.h>
GCPaths::GCPaths()
{
    init();//优先初始化
}

GCPaths::~GCPaths()
{
}

QString GCPaths::path(int type)
{
    return gc_paths()->m_paths.value(type);
}

void GCPaths::init()
{
    QString fileName = qApp->applicationName();

    // Config
#if defined(Q_OS_WIN) || defined(Q_OS_OS2)
    // Use %appdata%/ cloudboard as Config path on Windows
    m_paths.insert(Config, QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).append("/"));
#elif defined(Q_OS_MAC)
    m_paths.insert(Config, QDir::homePath() + QLatin1String("/Library/Application Support/cloudboard"));
#else // Unix
    m_paths.insert(Config, QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation)
                   + QLatin1String("/cloudboard"));
#endif

    m_paths.insert(HomePath, QDir::homePath()+ QLatin1String("/") + fileName);

    m_paths.insert(Profiles, m_paths.value(Config) + QLatin1String("profiles"));

    m_paths.insert(Cache, m_paths.value(Config) + QLatin1String("cache"));

    m_paths.insert(Temp, QDir::tempPath() + QLatin1String("/hytemp"));
    m_paths.insert(Documents, QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                   + QLatin1String("/") + fileName);

    foreach (QString path, m_paths.values()) {
        QDir dir;
        dir.mkpath(path);
    }

#ifdef Q_OS_ANDROID
    m_paths.insert(Apppath, QLatin1String("assets:");
#else
    m_paths.insert(Apppath, QApplication::applicationDirPath());
#endif

    m_paths.insert(Desktop, QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
}
