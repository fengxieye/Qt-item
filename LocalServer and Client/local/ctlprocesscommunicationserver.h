#ifndef CTLPROCESSCOMMUNICATIONSERVER_H
#define CTLPROCESSCOMMUNICATIONSERVER_H
#include "ctlprocesscommunicationinterface.h"
#include <QLocalServer>
#include <QLocalSocket>
#include <QObject>

using namespace CLTProcessCommunicationSpace;
class CTLProcessCommunicationServer : public CTLProcessCommunicationInterface
{
    Q_OBJECT
public:
    explicit CTLProcessCommunicationServer(QObject *parent = 0);
    ~CTLProcessCommunicationServer();
    virtual bool bind(const QString &name);
    virtual bool isBind();
    virtual void unbind();
    virtual void abort();
    bool hasClient(const QString &discribe);
    void setTarget(const QString &discribe);
    bool sendMessage(const LocalMessage &message);
    virtual int type(){return type_server;}

signals:
    //发出信号给外面的接收者
    void sigClientLeave(QString name);
    void sigClientComing(QString name);
private:
    struct ClietnInfo{
        QLocalSocket *socket;
        QString discribe;
    };
    QLocalServer *m_server;
    //设立的发送目标，多个目标可以描述相同，相当于给一组socket发送消息
    QString m_targetDiscribe;
    QString m_name;
    QList<ClietnInfo *> m_clients;
private:
    void removeClient(QLocalSocket *socket);
    ClietnInfo *getInfo(QLocalSocket *socket);
    QList<ClietnInfo *> getInfos(const QString &discribe);
    //查看下一包数据是否已经收完
    bool isSocketReadyRead(QLocalSocket *socket);
    //看下一包的数据大小
    int getDataSize(QLocalSocket *socket);
    //发送消息
    void sendMessageToAll(const LocalMessage &message);
    void sendMessageToOne(const LocalMessage &message, QLocalSocket *socket);
    void sendMessageToDiscribe(const LocalMessage &message, const QString &discribe);
public slots:
    //接收socket的信号
    void onNewConnection();
    void onSocketDisconnect();
    void onReadyRead();
};

#endif // CTLPROCESSCOMMUNICATIONSERVER_H
