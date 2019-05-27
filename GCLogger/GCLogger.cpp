#include "Logger.h"

static GCLogger* pThis;

GCLogger::GCLogger(QObject* parent)
    : QObject(parent)
{
    pThis = this;
    connect(this,&GCLogger::sigLog,this,&GCLogger::writeLog);
    gDefaultHandler = qInstallMessageHandler(&GCLogger::myMessageOutput);

    QThread* thread;
    thread = new QThread(this);
    this->moveToThread(thread);
    thread->start();
}

GCLogger::~GCLogger()
{
    if(m_file.isOpen())
        m_file.close();
}

void GCLogger::writeLog(QString strText,QString time)
{
    bool ret = m_file.isOpen();
    if(ret==true){
        m_mutex.lock();
        QTextStream stream(&m_file);
        //window 换行
        stream<<QThread::currentThreadId()<<strText<<" "<<time<<"\r\n";
        m_file.flush();
        m_mutex.unlock();
    }
}
//not used
bool GCLogger::setPath(QString logFullFilePath)
{
    m_file.setFileName(logFullFilePath);
    bool ret = false;
    if(m_file.size()>1024*1024)
    {
        ret = m_file.open(QFile::ReadWrite|QFile::Truncate);
    }
    else
    {
        ret = m_file.open(QFile::ReadWrite|QFile::Append);
    }

    if(ret==false){
        qInfo()<<"open "<<logFullFilePath<<" fail";
    }

    return ret;
}

//根据日期建日志文件,PROGRAMNAME日志文件夹名
void GCLogger::openFile()
{
    QString strPath = QDir::homePath()+"/."+PROGRAMNAME"/log";

    QDir::home().mkpath(strPath); //创建文件夹

    // create log file by current date ==> eg:log20170418.txt
    QString currentDate = QDateTime::currentDateTime().toString("yyyyMMdd");
    QString logName = "log" + currentDate + ".txt";
    QString logFileName = strPath+"/" + logName;

    m_path = logFileName;
    m_file.setFileName(logFileName);
    bool ret = false;
    ret = m_file.open(QFile::ReadWrite|QFile::Append);
    if(ret==false){
        qInfo()<<"open "<<logFileName<<" fail";
    }
    else
    {
        //删除老的日志，根据文件的时间
        QDir dir(strPath);
        QFileInfoList fileList = dir.entryInfoList(QStringList() << "*.txt", QDir::NoFilter, QDir::Time);
         if (fileList.size() >= LOG_FILE_KEEP_NUM)
         {
             int i = 0;
             foreach(QFileInfo fileInfo , fileList)
             {
                 if (i >= LOG_FILE_KEEP_NUM)
                 {
                     QString fileName = fileInfo.absoluteFilePath();
                     QFile::remove(fileName);
                 }
                 i++;
             }
         }
    }
}

void GCLogger::log(QString strText)
{
    QDateTime time = QDateTime::currentDateTime();
    emit sigLog(strText,time.toString("MM-dd hh:mm:ss"));
}

void GCLogger::myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QDateTime time = QDateTime::currentDateTime();
    QString strTime = time.toString("MM-dd hh:mm:ss");
    QString strMessage = QString(" %1 File:%2  Line:%3  Function:%4\r\n  %5")
            .arg(strTime).arg(context.file).arg(context.line).arg(context.function).arg(msg);

    QString strMsg("");
    switch(type)
    {
    case QtDebugMsg:
        strMsg = QString("Debug");
        break;
    case QtInfoMsg:
        strMsg = QString("Info");
        break;
    case QtWarningMsg:
        strMsg = QString("Warning:");
        break;
    case QtCriticalMsg:
        strMsg = QString("Critical:");
        break;
    case QtFatalMsg:
        strMsg = QString("Fatal:");
        break;
    default:
        strMsg = QString("Err");
        break;
    }

    strMessage = strMsg+strMessage;
    pThis->log(strMessage);

    //用系统原来的函数完成原来的功能. 比如输出到调试窗
    if(gDefaultHandler)
    {
        gDefaultHandler(type,context,msg);
    }

}
