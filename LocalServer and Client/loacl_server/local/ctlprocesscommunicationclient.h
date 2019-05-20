#ifndef CTLPROCESSCOMMUNICATIONCLIENT_H
#define CTLPROCESSCOMMUNICATIONCLIENT_H
#include "ctlprocesscommunicationinterface.h"
using namespace CLTProcessCommunicationSpace;
#include <QLocalSocket>
class CTLProcessCommunicationClient : public CTLProcessCommunicationInterface
{
    Q_OBJECT
public:
    explicit CTLProcessCommunicationClient(QObject *parent = 0);
    ~CTLProcessCommunicationClient();
    QString serverName(){return m_serverName;}
    virtual int type(){return type_client;}
    virtual bool bind(const QString &servername);
    virtual bool isBind();
    virtual void unbind();
    virtual void abort();
    bool sendMessage(const LocalMessage &msg);
signals:

private:
    QLocalSocket *m_socket;
    QString m_serverName;
    bool m_bConnect;
private:
    bool sendMessageToSever(const LocalMessage &msg);
    int getSocketDataSize();
    bool isSocketReadyRead();
public slots:
    void onDisConnect();
    void onReadyRead();
};

#endif // CTLPROCESSCOMMUNICATIONCLIENT_H
