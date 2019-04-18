#ifndef POSTPARAMHELPER_H
#define POSTPARAMHELPER_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QObject>
#include <QFile>
#include <QHttpMultiPart>
#include <QFileInfo>
class PostParamHelper
{
public:
    PostParamHelper();

    /// 对应第一种请求形式，两种不能混用。
    void addKeyValue(const char* keyName, const char* value);
    void addKeyValue(QByteArray keyName, QString value);
    void addKeyValue(QByteArray keyName, QByteArray value);
    // 添加参数数组键值.和keyValue的区别是key名后会多[]
    void addKeyGroupValue(QByteArray keyName, QByteArray value);

    /// 对应第二种请求形式，两种不能混用。只处理一个文件
    void addPostFile(QFile* file);

    /// 执行POST
    /// 最后一次add key-value就用第一种，忽略file；最后一次addfile就会忽略key-value
    QNetworkReply* doPost(QNetworkAccessManager* manager, const QNetworkRequest &request);
    QNetworkReply* doPost(QNetworkAccessManager* manager,QString targetUrl);
    QNetworkReply* doGet(QNetworkAccessManager* manager,QString targetUrl);
    QString dumpParams();
private:
    struct ParamPair
    {
        QByteArray keyName;
        QByteArray value;
    };

    //使用list的原因是有时候对顺序有需求
    //至于重复key 不管
    QList<ParamPair> mParamList;
    bool mIsMultiPartFilePost;
    QFile* mFilePost;
};

#endif // POSTPARAMHELPER_H
