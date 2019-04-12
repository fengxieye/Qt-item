#include "gcsettings.h"

GCSettings::GCSettings(const QString &fileName, QObject *parent)
{
    m_setting = new QSettings(fileName, QSettings::IniFormat, parent);
}

GCSettings::~GCSettings()
{
    delete m_setting;
    m_setting = 0;
}

void GCSettings::setCodec(const char *codec)
{
    m_setting->setIniCodec(QTextCodec::codecForName(codec));
}

QString GCSettings::path() const
{
    return m_setting->fileName();
}

QVariant GCSettings::value(const QString &key, const QVariant &defaultValue)
{
    QVariant value = m_setting->value(key, defaultValue);
    return value;
}

void GCSettings::setValue(const QString &key, const QVariant &value)
{
    m_setting->setValue(key, value);
    m_setting->sync();
}

bool GCSettings::containts(const QString &key)
{
    bool bcontain = m_setting->contains(key);
    return bcontain;
}

void GCSettings::remove(const QString &key)
{
    m_setting->remove(key);
}

void GCSettings::setValueWithGroup(const QString &group, const QString &key, const QVariant &value)
{
    m_setting->beginGroup(group);
    m_setting->setValue(key, value);
    m_setting->endGroup();
    m_setting->sync();
}

QVariant GCSettings::valueWithGroup(const QString &group, const QString &key, const QVariant &defaultValue)
{
    m_setting->beginGroup(group);
    QVariant value = m_setting->value(key, defaultValue);
    m_setting->endGroup();
    return value;
}

bool GCSettings::containtsWithGroup(const QString &group, const QString &key)
{
    m_setting->beginGroup(group);
    bool bcontain = m_setting->contains(key);
    m_setting->endGroup();
    return bcontain;
}

void GCSettings::removeWithGroup(const QString &group, const QString &key)
{
    m_setting->beginGroup(group);
    m_setting->remove(key);
    m_setting->endGroup();
}

//目前作用没有体现
void GCSettings::sync()
{
    m_setting->sync();
}
