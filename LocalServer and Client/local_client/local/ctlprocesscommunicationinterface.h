#ifndef CTLPROCESSCOMMUNICATIONINTERFACE_H
#define CTLPROCESSCOMMUNICATIONINTERFACE_H

#include <QObject>
#include "ctlprocesscommunicationdef.h"
#include <QLocalSocket>
using namespace CLTProcessCommunicationSpace;
class CTLProcessCommunicationInterface : public QObject
{
    Q_OBJECT
public:
    explicit CTLProcessCommunicationInterface(QObject *parent = 0);
    void setDiscribe(const QString &discribe);
    inline QString discribe()const{return m_discribe;}
    virtual int type(){return type_unknow;}
    virtual bool bind(const QString &name) = 0;
    virtual bool isBind() = 0;
    virtual void unbind() = 0;
    virtual bool sendMessage(const LocalMessage &message) = 0;
    bool sendQStringMessage(const QString &message);
    bool sendStrMessage(const char *message);
    virtual void abort() = 0;
//protected:
    static bool isServerRuning(const QString &servername)
     {
         // 用一个localsocket去连一下,如果能连上就说明已经有服务了
         QLocalSocket ls;
         ls.connectToServer(servername);
         if (ls.waitForConnected(1000)){
             // 说明已经在运行了
             ls.disconnectFromServer();
             ls.close();
             return true;
         }
         return false;
     }
    //TCP传输型数据转使用型数据，给应用层传输用
    static void translateToMessage(LocalMessage &msg, const TcpMessage &cmsg){
        msg.head.size = cmsg.head.size;
        msg.head.type = cmsg.head.type;
        QByteArray data;
        data.resize(msg.head.size - 4);
        memcpy(data.data(), cmsg.str, data.size());
        msg.data = data;
    }
    //使用型数据转传输型数据，给TCP传输用
    static void translateToTcpMessage(TcpMessage &cmsg, const LocalMessage &msg){
        cmsg.head.size = msg.head.size;
        cmsg.head.type = msg.head.type;
        memset(cmsg.str, 0, sizeof(cmsg.str));
        memcpy(cmsg.str, msg.data.data(), msg.data.size());
    }
signals:
    void sigMessage(LocalMessage &msg);
private:
    QString m_discribe;
public slots:

};

#endif // CTLPROCESSCOMMUNICATIONINTERFACE_H
