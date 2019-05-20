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
    void sigClientLeave(QString name);
    void sigClientComing(QString name);
private:
    struct ClietnInfo{
        QLocalSocket *socket;
        QString discribe;
    };
    QLocalServer *m_server;
    QString m_targetDiscribe;
    QString m_name;
    QList<ClietnInfo *> m_clients;
private:
    void removeClient(QLocalSocket *socket);
    ClietnInfo *getInfo(QLocalSocket *socket);
    QList<ClietnInfo *> getInfos(const QString &discribe);
    bool isSocketReadyRead(QLocalSocket *socket);
    int getDataSize(QLocalSocket *socket);
    void sendMessageToAll(const LocalMessage &message);
    void sendMessageToOne(const LocalMessage &message, QLocalSocket *socket);
    void sendMessageToDiscribe(const LocalMessage &message, const QString &discribe);
public slots:
    void onNewConnection();
    void onSocketDisconnect();
    void onReadyRead();
};

#endif // CTLPROCESSCOMMUNICATIONSERVER_H
