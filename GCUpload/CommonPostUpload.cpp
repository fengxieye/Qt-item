#include "CommonPostUpload.h"
#include "PostParamHelper.h"
#include "ShareNAM.h"

CommonPostUpload::CommonPostUpload()
    : reply_(NULL)
{

}

CommonPostUpload::~CommonPostUpload()
{
    qDebug()<<"~CommonPostUpload";
    stop();
}

void CommonPostUpload::stop()
{
    if(reply_)
    {   //主动close会触发onPostReplyFinish，这里断掉信号槽再close，避免错误当成功上传.
        disconnect(reply_, SIGNAL(finished()), this, SLOT(onPostReplyFinish()));
        disconnect(reply_, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
        reply_->close();
        delete reply_;
        reply_ = NULL;
    }
}

int CommonPostUpload::upload(QString localFilePath, QMap<QByteArray, QString> param, QString uploadUrl)
{
    if(reply_)
    {
        Q_ASSERT(reply_==NULL);
        qInfo()<<"is uploading, stop first.";
        return -2;
    }
    PostParamHelper helper;
    QMap<QByteArray, QString>::iterator it = param.begin();
    while(it!=param.end())
    {
        helper.addKeyValue(it.key(),it.value().toUtf8());
        ++it;
    }
    if(file_.isOpen())
    {
        qDebug()<<"close upload file obj";
        file_.close();
    }
    file_.setFileName(localFilePath);
    if(!file_.open(QFile::ReadOnly))
    {
        qCritical()<<"open file to upload failed."<<localFilePath;
        return -1;
    }
    helper.addPostFile(&file_);
    reply_ = helper.doPost(ShareNAM::get(),QNetworkRequest(QUrl(uploadUrl)));
    connect(reply_, SIGNAL(finished()), this, SLOT(onPostReplyFinish()));
    connect(reply_, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));

    return 0;
}

void CommonPostUpload::onPostReplyFinish()
{
    if(!reply_)
    {
        qDebug()<<"reply_ is NULL!";
        return;
    }
    file_.close();
    disconnect(reply_, SIGNAL(finished()), this, SLOT(onPostReplyFinish()));
    disconnect(reply_, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));

    QUrl relocate = reply_->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    reply_->deleteLater();
    reply_ = NULL;
    if(relocate.isValid())
    {  //自行实现上传后重定向.
        qInfo()<<"post redirect"<<relocate;
        reply_ = ShareNAM::get()->get(QNetworkRequest(relocate));
        connect(reply_, SIGNAL(finished()), this, SLOT(onPostReplyFinish()));
        connect(reply_, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
        return;
    }

    emit sigUploadFinished();
}

void CommonPostUpload::error(QNetworkReply::NetworkError code)
{
    qInfo()<<"CommonPostUpload::error code="<<code<<reply_->readAll();
    file_.close();
    disconnect(reply_, SIGNAL(finished()), this, SLOT(onPostReplyFinish()));
    disconnect(reply_, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
    reply_->deleteLater();
    reply_ = NULL;

    emit sigUploadError(code);
}
