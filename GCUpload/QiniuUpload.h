#ifndef QINIUUPLOAD_H
#define QINIUUPLOAD_H

//#include <WcSingleton.h>
#include <QString>

struct _Qiniu_Client;
typedef struct _Qiniu_Client Qiniu_Client;
class QiniuUpload// : public WcSingleton<QiniuUpload>
{
public:
    //friend class WcSingleton<QiniuUpload>;
    QiniuUpload();
    ~QiniuUpload();
    int upload(QString localFilePath,QString token, QString& key);

private:
    Qiniu_Client *m_Client;
};
#endif // QINIUUPLOAD_H
