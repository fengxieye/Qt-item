#include "QiniuUpload.h"
#include "qiniu/http.h"
#include "qiniu/io.h"
#include "qiniu/resumable_io.h"

static int QinuiUploadInstance = 0;

QiniuUpload::QiniuUpload()
{
    m_Client = new Qiniu_Client;
    if(m_Client)
    {
        if(QinuiUploadInstance++==0)
        {
            Qiniu_Global_Init(-1);                  /* 全局初始化函数，整个进程只需要调用一次 */
        }
        Qiniu_Client_InitNoAuth(m_Client, 1024); /* HTTP客户端初始化。HTTP客户端是线程不安全的，不要在多个线程间共用 */
    }
}
QiniuUpload::~QiniuUpload()
{
    if(m_Client)
    {
        Qiniu_Client_Cleanup(m_Client);          /* 每个HTTP客户端使用完后释放 */
        if(--QinuiUploadInstance == 0)
        {
            Qiniu_Global_Cleanup();                 /* 全局清理函数，只需要在进程退出时调用一次 */
        }
        delete m_Client;
    }
}
int QiniuUpload::upload(QString localFilePath,QString token,QString& key)
{
    Qiniu_Error err;

    Qiniu_Rio_PutExtra extra;
    Qiniu_Zero(extra);
    Qiniu_Rio_PutRet putRet;
    err = Qiniu_Rio_PutFile(m_Client,
                            &putRet,
                            qPrintable(token),
                            NULL,
                            qPrintable(localFilePath),
                            &extra);
//    Qiniu_Io_PutRet putRet;
//    err = Qiniu_Io_PutFile(m_Client,
//                           &putRet,
//                           qPrintable(token),
//                           NULL,
//                           qPrintable(localFilePath),
//                           NULL);
//    qInfo("hash and key %s %s",putRet.hash,putRet.key);
    if(err.code != 200)
    {
        qInfo("qiniu upload fail:[%d]%s",err.code,err.message);
        Q_ASSERT(err.code!=0);
        if(err.code==0) return -1;
        return err.code;
    }
    else
    {
        if(putRet.key != "")
        {
            key = putRet.key;
        }
    }
    return 0;
}
