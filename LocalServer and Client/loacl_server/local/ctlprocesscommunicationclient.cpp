#include "ctlprocesscommunicationclient.h"
#include <QDebug>
CTLProcessCommunicationClient::CTLProcessCommunicationClient(QObject *parent) :
    CTLProcessCommunicationInterface(parent),
    m_bConnect(false)
{
    m_socket = new QLocalSocket(this);
    connect(m_socket, SIGNAL(readyRead()),
            this, SLOT(onReadyRead()));
    connect(m_socket, SIGNAL(disconnected()),
            this, SLOT(onDisConnect()));
}

CTLProcessCommunicationClient::~CTLProcessCommunicationClient()
{
    if(m_socket){
        unbind();
    }
}

bool CTLProcessCommunicationClient::bind(const QString &servername)
{
    m_serverName = servername;
    m_socket->connectToServer(m_serverName);
    m_bConnect = m_socket->waitForConnected(1000);
    LocalMessage msg;
    msg.data = discribe().toLocal8Bit();
    msg.head.type = type_discribe;
    msg.head.size = CTLProcessHeadSize + discribe().size();
    sendMessage(msg);
    return m_bConnect;
}

bool CTLProcessCommunicationClient::isBind(){return m_bConnect;}

void CTLProcessCommunicationClient::unbind()
{
    if(m_bConnect){
        m_socket->disconnectFromServer();
    }
}

void CTLProcessCommunicationClient::abort()
{
    if(m_bConnect){
        m_socket->abort();
    }
}

bool CTLProcessCommunicationClient::sendMessage(const LocalMessage &msg)
{
    return sendMessageToSever(msg);
}

bool CTLProcessCommunicationClient::sendMessageToSever(const LocalMessage &msg)
{
    if(!m_bConnect){
        return false;
    }
    TcpMessage cmsg;
    CTLProcessCommunicationInterface::translateToTcpMessage(cmsg, msg);
    QByteArray data;
    data.resize(cmsg.head.size);
    memcpy(data.data(), &cmsg, cmsg.head.size);
    int size = m_socket->write(data, data.size());
    Q_ASSERT(size == cmsg.head.size);
    m_socket->waitForBytesWritten(1000);
    return true;
}

void CTLProcessCommunicationClient::onDisConnect()
{
    m_bConnect = false;
}

bool CTLProcessCommunicationClient::isSocketReadyRead()
{
    if(m_socket->bytesAvailable() < CTLProcessHeadSize){
        return false ;
    }
    TcpMessage peekmsg;
    if(CTLProcessHeadSize != m_socket->peek((char*)&peekmsg, CTLProcessHeadSize)){
        return false;
    }
    if(m_socket->bytesAvailable() < peekmsg.head.size){
        return false; //数据不够.
    }
    return true;
}

int CTLProcessCommunicationClient::getSocketDataSize()
{
    if(!isSocketReadyRead()){
        return 0;
    }
    TcpMessage peekmsg;
    m_socket->peek((char*)&peekmsg, CTLProcessHeadSize);
    return peekmsg.head.size;
}

void CTLProcessCommunicationClient::onReadyRead()
{
    QLocalSocket *socket = qobject_cast<QLocalSocket *>(sender());
    Q_ASSERT(m_socket == socket);
    while(true)
    {
        if(!isSocketReadyRead()){
            break;//数据不满足，不读取
        }
        LocalMessage msg;
        TcpMessage readmsg;
        memset(&readmsg, 0 , sizeof(TcpMessage));
        int rsize_ = getSocketDataSize();
        socket->read((char*)&readmsg, rsize_);
        switch (readmsg.head.type) {
        case type_discribe:{
            qDebug()<<"server Name:"<<QString(readmsg.str);
            m_serverName = QString(readmsg.str);
        }
        default:
            break;
        }
        CTLProcessCommunicationInterface::translateToMessage(msg, readmsg);
        emit sigMessage(msg);
    }
}
