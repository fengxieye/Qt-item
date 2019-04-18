#ifndef WCUPLOAD2CLOUD_H
#define WCUPLOAD2CLOUD_H

#include <QtCore>
#include <QThread>
//#include "WcClassInfo.h"
#include <QNetworkReply>
#include "UploadDefine.h"


//这个模块处理上传的实操部分, 处理不同云存储的差异, 以及其他细节问题.
//先按分课网规则拉token等, 然后用线程做特异化的云存储上传.
//token拿好,拉起线程,再拿token; 等线程完工取token, 再拿下一个token.


class CommonPostUpload;
class QiniuUpload;
class Upload2Cloud : public QThread  //私用线程.外部不可start
{
    Q_OBJECT
public:
    Upload2Cloud();
    ~Upload2Cloud();
    QString setUploadUrl(QString url);
    bool isDoingUpload();
    bool start(UploadType ut, QString fpath, ParamInfo param);
    //bool start(UploadType ut, QString fpath, ParamInfo param);
    void stop();

    int doAsynUpload();
protected:
    virtual void run();

signals:
    void sigUploadFinished(QString path, QString key = "");
    void sigUploadError(int er, const QString &path = "");
private slots:
    void onThreadFinish();
    void onModuleFinish(); //模块完工
    void onModuleError(int err);
private:
    ParamInfo param_;
    QString path_;
    QString key_ = "";
    UploadType uploadType_;
    QString uploadUrl_;
    int err_;  //upload error code.
    bool isUploading_;
    CommonPostUpload *postUploader_;
    QiniuUpload *qiniuUploader_;
};

#endif // WCUPLOAD2CLOUD_H
