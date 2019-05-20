#include "ctlprocesscommunicationserver.h"
#include <QDebug>
CTLProcessCommunicationServer::CTLProcessCommunicationServer(QObject *parent) :
    CTLProcessCommunicationInterface(parent)
{
    m_server = new QLocalServer(this);
    connect(m_server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

CTLProcessCommunicationServer::~CTLProcessCommunicationServer()
{
    if(m_server){
        unbind();
        delete m_server;
        m_server = 0;
    }
}

bool CTLProcessCommunicationServer::bind(const QString &name)
{
    Q_ASSERT(!m_server->isListening());
    return m_server->listen(name);
}

bool CTLProcessCommunicationServer::isBind(){return m_server->isListening();}

void CTLProcessCommunicationServer::unbind(){
    m_server->close();
}

void CTLProcessCommunicationServer::abort(){
    m_server->close();
}

bool CTLProcessCommunicationServer::hasClient(const QString &discribe)
{
    return !getInfos(discribe).isEmpty();
}

void CTLProcessCommunicationServer::setTarget(const QString &discribe)
{
    m_targetDiscribe = discribe;

}

bool CTLProcessCommunicationServer::sendMessage(const LocalMessage &message)
{
    if(m_targetDiscribe.isEmpty()){
        sendMessageToAll(message);
    }else{
        if(getInfos(m_targetDiscribe).isEmpty()){
            return false;
        }
#if 1
        //send to all clients with have same @discride of !m_targetDiscribe
        qDebug()<<"CTLProcessCommunicationServer::sendMessage"<<getInfos(m_targetDiscribe).size()
               <<message.data;
        sendMessageToDiscribe(message, m_targetDiscribe);
#else
        sendMessageToOne(message, getInfos(m_targetDiscribe).first()->socket);//只发送给第一个
#endif
    }
    return true;
}

void CTLProcessCommunicationServer::removeClient(QLocalSocket *socket)
{
    ClietnInfo *info = getInfo(socket);
    if(info){
        info->socket->disconnect();
        info->socket->disconnectFromServer();
        info->socket->deleteLater();
        m_clients.removeAll(info);
    }
}

CTLProcessCommunicationServer::ClietnInfo *CTLProcessCommunicationServer::getInfo(QLocalSocket *socket)
{
    foreach (ClietnInfo *info, m_clients) {
        if(info->socket == socket){
            return info;
        }
    }
    return Q_NULLPTR;
}

QList<CTLProcessCommunicationServer::ClietnInfo *> CTLProcessCommunicationServer::getInfos(const QString &discribe)
{
    QList<ClietnInfo *> infos;
    foreach (ClietnInfo *info, m_clients) {
        if(info->discribe == discribe){
            infos.append(info);
        }
    }
    return infos;
}

bool CTLProcessCommunicationServer::isSocketReadyRead(QLocalSocket *socket)
{
    if(socket->bytesAvailable() < CTLProcessHeadSize){
        return false ;
    }
    TcpMessage peekmsg;
    if(CTLProcessHeadSize != socket->peek((char*)&peekmsg, CTLProcessHeadSize)){
        return false;
    }
    if(socket->bytesAvailable() < peekmsg.head.size){
        return false; //数据不够.
    }
    return true;
}

//偷窥下头部的一个数据大小看下这包数据多大
int CTLProcessCommunicationServer::getDataSize(QLocalSocket *socket)
{
    if(!isSocketReadyRead(socket)){
        return 0;
    }
    TcpMessage peekmsg;
    socket->peek((char*)&peekmsg, CTLProcessHeadSize);
    return peekmsg.head.size;
}

void CTLProcessCommunicationServer::sendMessageToAll(const LocalMessage &message)
{
    foreach (ClietnInfo *info, m_clients) {
        sendMessageToOne(message, info->socket);
    }
}

void CTLProcessCommunicationServer::sendMessageToOne(const LocalMessage &message, QLocalSocket *socket)
{
    Q_ASSERT(socket);
    if(!socket){
        return;
    }
    TcpMessage cmsg;
    CTLProcessCommunicationInterface::translateToTcpMessage(cmsg, message);
    socket->write((const char*)(&cmsg), cmsg.head.size);
    socket->waitForBytesWritten(1000);
}

void CTLProcessCommunicationServer::sendMessageToDiscribe(const LocalMessage &message, const QString &discribe)
{
    TcpMessage cmsg;
    CTLProcessCommunicationInterface::translateToTcpMessage(cmsg, message);
    foreach (ClietnInfo *info, getInfos(discribe)) {
        sendMessageToOne(message, info->socket);//
    }
}

void CTLProcessCommunicationServer::onNewConnection()
{
    ClietnInfo *info = new ClietnInfo;
    info->socket = m_server->nextPendingConnection();
    connect(info->socket, SIGNAL(readyRead()),
            this, SLOT(onReadyRead()));
    connect(info->socket, SIGNAL(disconnected()),
            this, SLOT(onSocketDisconnect()));
    m_clients.prepend(info);
}

void CTLProcessCommunicationServer::onSocketDisconnect()
{
    QLocalSocket *socket = qobject_cast<QLocalSocket *>(sender());
    qDebug()<<"CTLProcessCommunicationServer::onSocketDisconnect()";
    if(getInfo(socket)){
        emit sigClientLeave(getInfo(socket)->discribe);
        removeClient(socket);
    }
}

void CTLProcessCommunicationServer::onReadyRead()
{
    QLocalSocket *socket = qobject_cast<QLocalSocket *>(sender());
    Q_ASSERT(getInfo(socket));
    while(true)
    {
        if(!isSocketReadyRead(socket)){
            break;//数据不满足，不读取
        }
        LocalMessage msg;
        TcpMessage readmsg;
        memset(&readmsg, 0, sizeof(TcpMessage));
        int rsize_ = getDataSize(socket);
        socket->read((char*)&readmsg, rsize_);
        switch (readmsg.head.type) {
        case type_discribe:{
            qDebug()<<"CTLProcessCommunicationServer: new income"<<QString(readmsg.str);
            getInfo(socket)->discribe = QString(readmsg.str);
            emit sigClientComing(QString(readmsg.str));
        }
        default:
            break;
        }
        CTLProcessCommunicationInterface::translateToMessage(msg, readmsg);
        qDebug()<<"CTLProcessCommunicationServer: msg"<<msg.data;
        emit sigMessage(msg);
    }
}



