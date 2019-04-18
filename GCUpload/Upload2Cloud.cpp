#include "Upload2Cloud.h"
#include "QiniuUpload.h"
#include "CommonPostUpload.h"
#include "ShareNAM.h"

Upload2Cloud::Upload2Cloud()
    : uploadType_(Qiniu)
    , err_(0)
    , isUploading_(false)
    , postUploader_(NULL)
    , qiniuUploader_(NULL)
{
    this->setObjectName("WcUpload2Cloud");
    connect(this,SIGNAL(finished()),this,SLOT(onThreadFinish()));
}

Upload2Cloud::~Upload2Cloud()
{
    if(qiniuUploader_)
    {
        delete qiniuUploader_;
    }
    if(postUploader_)
    {
        delete postUploader_;
        postUploader_ = NULL;
    }
}

QString Upload2Cloud::setUploadUrl(QString url)
{
    QString oldUploadUrl = uploadUrl_;
    uploadUrl_ = url;
    return oldUploadUrl;
}

bool Upload2Cloud::isDoingUpload()
{
    return isUploading_;
    //return this->isRunning();
}

bool Upload2Cloud::start(UploadType ut, QString fpath, ParamInfo param)
{
    qDebug()<<"uploadtype:"<<ut<<"fpath="<<fpath<<"param count="<<param.size();
    if(isDoingUpload()) return false;
    param_ = param;
    path_ = fpath;
    err_ = 0;
    uploadType_ = ut;

    isUploading_ = true;
    if(uploadType_ == Common_S3)
    {   //非阻塞实现.
        if(0!=doAsynUpload())
        {
            return false;
        }
    }
    else
    {   //阻塞实现.
        QThread::start();
    }
    return true;
}

void Upload2Cloud::stop()
{
    if(!isUploading_) return;

    if(isRunning()) this->terminate();

    if(postUploader_) postUploader_->stop();
}

int Upload2Cloud::doAsynUpload()
{
    switch(uploadType_)
    {
    case Common_S3:
    {
        Q_ASSERT(!uploadUrl_.isEmpty()); //记得设置url
        if(uploadUrl_.isEmpty())
        {
            qCritical("error: Common_S3 upload no upload url.");
            err_ = -101;
            return err_;
        }
        if(postUploader_ == NULL)
        {
            postUploader_ = new CommonPostUpload;
            connect(postUploader_,SIGNAL(sigUploadFinished()),
                    this,SLOT(onModuleFinish()));
            connect(postUploader_,SIGNAL(sigUploadError(int)),
                    this,SLOT(onModuleError(int)));
        }
        err_ = postUploader_->upload(path_,param_,uploadUrl_);
        //err_ = postUploader_->upload(path_,param_,"http://kify.s3.amazonaws.com");
        return err_;
    }
        break;
    default:
        qCritical("not implement this uploadType=%d",uploadType_);
        Q_ASSERT("not implement this cloudstorage type"==NULL);
        break;
    }
    return 0;
}

void Upload2Cloud::run()
{
    switch(uploadType_)
    {
    case Qiniu:
        if(qiniuUploader_==NULL)
        {
            qiniuUploader_ = new QiniuUpload();
        }
        err_ = qiniuUploader_->upload(path_,param_["token"],key_);
        if(err_ != 0)
        {
            delete qiniuUploader_;
            qiniuUploader_ = NULL;
        }
        break;
    default:
        qCritical("not implement this uploadType=%d",uploadType_);
        Q_ASSERT("not implement this cloudstorage type"==NULL);
        break;
    }
}

void Upload2Cloud::onThreadFinish()
{
    isUploading_ = false;
    QString path = path_;
    QString key = key_;
    int err = err_;

    path_.clear();
    key_.clear();
    err_ = 0;
    if(err == 0) emit sigUploadFinished(path, key);
    else emit sigUploadError(err, path);
}

void Upload2Cloud::onModuleFinish()
{
    isUploading_ = false;
    emit sigUploadFinished(path_);
}

void Upload2Cloud::onModuleError(int err)
{
    isUploading_ = false;
    emit sigUploadError(err);
}
