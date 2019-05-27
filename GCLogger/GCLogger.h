#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QMutex>
#include <QFile>
#include <QDebug>
#include <QThread>
#include <QObject>
#include <QDateTime>
#include <QDir>
#include <QFileInfoList>
#include <QThread>


/****************
 *
 * 开发调试用的信息使用qDebug()输出
 * 其它输出会写入日志文件
 * 使用时可以把此对象moveToThread 来在新的线程里执行写入操作
 *
 * **************/

//日志保留的个数
#define LOG_FILE_KEEP_NUM 7
//项目文件夹名字
#define PROGRAMNAME "GCTest"

static QtMessageHandler gDefaultHandler = NULL;
class GCLogger:public QObject
{
    Q_OBJECT
public:
    GCLogger(QObject* parent = NULL);
    ~GCLogger();
    bool setPath(QString logFullFilePath);
    void openFile();

signals:
    void sigLog(QString strText,QString time);

private:
    void writeLog(QString strText,QString time);
    void log(QString strText);
    //回调函数，需要static
    static void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    QMutex m_mutex;
    QFile m_file;
    QString m_path;
};

#endif // LOGGER_H
