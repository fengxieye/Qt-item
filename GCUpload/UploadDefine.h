#ifndef WCUPLOADDEFINE_H
#define WCUPLOADDEFINE_H
#include <QMap>

enum UploadType
{
    Qiniu,
    Ucloud,
    Amazon,
    Common_S3 = Amazon,  //不具体细分
    Aliyun,
};

typedef QMap<QByteArray,QString> ParamInfo;



#endif // WCUPLOADDEFINE_H
