pro�ļ������ӹ���
include(GCUpload/GCUpload.pri)

����Ŀ¼��������dll�ļ��µ�dll

��ʼ��
WcUpload2Cloud *m_pUploadWorker = Q_NULLPTR;
m_pUploadWorker = new WcUpload2Cloud;
connect(m_pUploadWorker, SIGNAL(sigUploadFinished(QString)), this, SLOT(onUploadFinished(QString)));
connect(m_pUploadWorker, SIGNAL(sigUploadError(int, QString)), this, SLOT(onUploadError(int, QString)));

ɾ��
if(m_pUploadWorker)
{
    m_pUploadWorker->stop();
    m_pUploadWorker->disconnect();
    delete m_pUploadWorker;
    m_pUploadWorker = Q_NULLPTR;
}

��ʱֹͣ
if(m_pUploadWorker)
{
    m_pUploadWorker->stop();
}

��ʼ����
//m_uploadType��������ͱ�����m_tokenPart��QMap���ͣ���Ӧtoken��("token", XXXXX)
if(m_uploadType == Common_S3)
{
    m_pUploadWorker->setUploadUrl(m_uploadUrl);
}
m_pUploadWorker->start(m_uploadType, m_uploadPath, m_tokenPart);

�������ţ�ص�����������
Qiniu_Rio_PutFile ��� Qiniu_Rio_PutRet������key�ǿյ�
�����ֱ�ӻص����ͻ�����
Qiniu_Rio_PutFile ��� Qiniu_Rio_PutRet������key���ϴ���ַ�ı�ʶ
Ȼ���ļ��ĵ�ַ����� http://qiniu.��������ַ/key
����
http://qiniu.xxxxx.cn/Fu_Xzx8MgtiLQwwuyvlI3

���� incorrect region, please use up-z2.qiniup.com��
�ϴ���ַ�����ˣ�����������ɶ�Ӧ�ĵ�ַ�����ܻ���Ҫ����sdk
conf.c
//Ĭ�ϻ�������
const char *QINIU_UP_HOST = "http://upload-z2.qiniu.com";
const char *QINIU_IOVIP_HOST = "http://iovip-z2.qbox.me";

