pro文件包含子工程
include(GCUpload/GCUpload.pri)

运行目录加入所需dll文件下的dll

初始化
WcUpload2Cloud *m_pUploadWorker = Q_NULLPTR;
m_pUploadWorker = new WcUpload2Cloud;
connect(m_pUploadWorker, SIGNAL(sigUploadFinished(QString)), this, SLOT(onUploadFinished(QString)));
connect(m_pUploadWorker, SIGNAL(sigUploadError(int, QString)), this, SLOT(onUploadError(int, QString)));

删除
if(m_pUploadWorker)
{
    m_pUploadWorker->stop();
    m_pUploadWorker->disconnect();
    delete m_pUploadWorker;
    m_pUploadWorker = Q_NULLPTR;
}

暂时停止
if(m_pUploadWorker)
{
    m_pUploadWorker->stop();
}

开始任务
//m_uploadType定义的类型变量，m_tokenPart是QMap类型，对应token：("token", XXXXX)
if(m_uploadType == Common_S3)
{
    m_pUploadWorker->setUploadUrl(m_uploadUrl);
}
m_pUploadWorker->start(m_uploadType, m_uploadPath, m_tokenPart);

如果是七牛回调到服务器则
Qiniu_Rio_PutFile 里的 Qiniu_Rio_PutRet参数的key是空的
如果是直接回到到客户端则
Qiniu_Rio_PutFile 里的 Qiniu_Rio_PutRet参数的key是上传地址的标识
然后文件的地址大概是 http://qiniu.服务器地址/key
例如
http://qiniu.xxxxx.cn/Fu_Xzx8MgtiLQwwuyvlI3

错误 incorrect region, please use up-z2.qiniup.com：
上传地址不对了，在下面更换成对应的地址，可能还需要更新sdk
conf.c
//默认华东机房
const char *QINIU_UP_HOST = "http://upload-z2.qiniu.com";
const char *QINIU_IOVIP_HOST = "http://iovip-z2.qbox.me";

