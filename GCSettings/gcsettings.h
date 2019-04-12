#ifndef C5SETTINGS_H
#define C5SETTINGS_H

#include <QObject>
#include <QVariant>
#include <QSettings>
#include <QTextCodec>
/**
 * @brief The GCSettings class
 * 通用型配置类。generic class
 * 对QSetting的再封装，让接口使用更方便
 * 参考：https://blog.csdn.net/fxy0325/article/details/83190218
 */
class GCSettings
{
public:
    GCSettings(const QString &fileName, QObject *parent = Q_NULLPTR);
    ~GCSettings();
    //需要在创建QSetting后立刻设置编码方式。
    void setCodec(const char *codec);
    QString path() const;

    void setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant());
    bool containts(const QString &key);
    void remove(const QString &key);

    void setValueWithGroup(const QString &group, const QString &key, const QVariant &value);
    QVariant valueWithGroup(const QString &group, const QString &key, const QVariant &defaultValue = QVariant());
    bool containtsWithGroup(const QString &group, const QString &key);
    void removeWithGroup(const QString &group, const QString &key);

    void sync();
private:
    QSettings *m_setting;
};

#endif // C5SETTINGS_H
