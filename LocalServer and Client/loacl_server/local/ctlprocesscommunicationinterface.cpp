#include "ctlprocesscommunicationinterface.h"

CTLProcessCommunicationInterface::CTLProcessCommunicationInterface(QObject *parent) :
    QObject(parent)
{
}

void CTLProcessCommunicationInterface::setDiscribe(const QString &discribe)
{
    if(discribe.size() > sizeof(TcpMessage) - CTLProcessHeadSize){
        m_discribe = discribe.left(sizeof(TcpMessage) - CTLProcessHeadSize -1);
    }else{
        m_discribe = discribe;
    }
}

bool CTLProcessCommunicationInterface::sendQStringMessage(const QString &message)
{
    LocalMessage msg;
    msg.data = message.toLocal8Bit();
    msg.head.type = type_message;
    msg.head.size = CTLProcessHeadSize + message.size();
    return sendMessage(msg);
}

bool CTLProcessCommunicationInterface::sendStrMessage(const char *message)
{
    return sendQStringMessage(QString(message));
}
