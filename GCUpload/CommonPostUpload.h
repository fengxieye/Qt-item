#ifndef COMMONPOSTUPLOAD_H
#define COMMONPOSTUPLOAD_H

#include <QString>
#include <QFile>
#include <QNetworkReply>
//#include "WcClassInfo.h"

//通用POST上传功能.
//面向S3接口上传, 因为参数都由服务器搞定了, 直接做Forms

class CommonPostUpload : public QObject
{
    Q_OBJECT
public:
    CommonPostUpload();
    ~CommonPostUpload();
    void stop();
    int upload(QString localFilePath,QMap<QByteArray, QString> param,QString uploadUrl);
signals:
    void sigUploadFinished();
    void sigUploadError(int er);
protected slots:
    void onPostReplyFinish();
    void error(QNetworkReply::NetworkError code);
private:
    QNetworkReply* reply_;
    QFile file_;
};


#endif // COMMONPOSTUPLOAD_H
