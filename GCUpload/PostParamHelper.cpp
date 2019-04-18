#include "PostParamHelper.h"

PostParamHelper::PostParamHelper()
    : mFilePost(NULL)
    , mIsMultiPartFilePost(false)
{
}

void PostParamHelper::addKeyValue(const char *keyName, const char *value)
{
    qDebug()<<"addKeyValue(C)"<<keyName<<value;
    addKeyValue(QByteArray(keyName),QByteArray(value));
}

void PostParamHelper::addKeyValue(QByteArray keyName, QString value)
{
    qDebug()<<"addKeyValue(S)"<<keyName<<value;
    if(mFilePost)
    {
        qWarning()<<"WcPostParamHelper::addKeyValue post file exist,name="<<mFilePost->fileName();
    }
    ParamPair fm;
    fm.keyName = keyName;
    fm.value = value.toUtf8().toPercentEncoding();
    mParamList.push_back(fm);
    mIsMultiPartFilePost = false;
}

void PostParamHelper::addKeyValue(QByteArray keyName, QByteArray value)
{
#ifdef QDebug
    QByteArray debugVal = value;
    if(debugVal.length()>100){
        debugVal = value.left(100);
    }
    qInfo()<<"addKeyValue(B)"<<keyName<<debugVal;
#endif

    ParamPair fm;
    fm.keyName = keyName;
    fm.value = value;
    mParamList.push_back(fm);
    mIsMultiPartFilePost = false;
}
// ？？？？ 可能有问题
void PostParamHelper::addKeyGroupValue(QByteArray keyName, QByteArray value)
{
    addKeyValue(keyName+"[]",value);
}

void PostParamHelper::addPostFile(QFile* file)
{
    if(mParamList.size()!=0)
    {
        qWarning()<<"WcPostParamHelper::addPostFile key-value exist.count="<<mParamList.size();
    }
    mFilePost = file;
    mIsMultiPartFilePost = true;
}

QNetworkReply *PostParamHelper::doPost(QNetworkAccessManager *manager, const QNetworkRequest &request)
{
    qDebug()<<"WcPostParamHelper::doPost target="<<request.url().toString();

    if(mIsMultiPartFilePost)
    {
        QHttpMultiPart* MultiPart = new QHttpMultiPart();
        MultiPart->setContentType(QHttpMultiPart::FormDataType);

        foreach(ParamPair param, mParamList)
        {
            QHttpPart dataPart;
            dataPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                               QString("form-data; name=\"%1\"").arg(QString(param.keyName)));
            dataPart.setBody(param.value);
            MultiPart->append(dataPart);
        }

        //增加文件主体数据
        QHttpPart filePart;
        QFileInfo fn(mFilePost->fileName());
        filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                           QString("form-data; name=\"file\"; filename=\"%1\"").arg(fn.fileName()));
        filePart.setBodyDevice(mFilePost);
        MultiPart->append(filePart);

        QNetworkReply* reply = manager->post(request,MultiPart);
        MultiPart->setParent(reply);  //绑定对象用于销毁.
        return reply;

    }
    else
    {
        QByteArray PostData;
        QNetworkRequest request2 = request;
        request2.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
        foreach(ParamPair param, mParamList)
        {
            qDebug()<<"param :"<<param.keyName<<"="<<param.value;
            PostData.append(param.keyName).append("=").append(param.value).append("&");
        }
        qDebug()<<"当前的PostData = "<<PostData.length();
        if(PostData.length()>0)
        {
            PostData.truncate(PostData.size()-1);
        }
        return manager->post(request2,PostData);
    }
}

QNetworkReply *PostParamHelper::doPost(QNetworkAccessManager *manager, QString targetUrl)
{
    QNetworkRequest request;
    request.setUrl(QUrl(targetUrl));
    return this->doPost(manager,request);
}

QNetworkReply *PostParamHelper::doGet(QNetworkAccessManager *manager, QString targetUrl)
{
    QNetworkRequest request;
    request.setUrl(QUrl(targetUrl));
    qInfo()<<"doget "<<targetUrl;
    return manager->get(request);
}

QString PostParamHelper::dumpParams()
{
    QString postParams;
    foreach(ParamPair param, mParamList)
    {
        postParams.append(param.keyName).append("=").append(param.value).append("&");
    }
    if(postParams.length()>0)
    {
        postParams.truncate(postParams.size()-1);
    }
    return postParams;
}



