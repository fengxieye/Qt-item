#include "TcpClient.h"

TcpClient::TcpClient(QObject *parent):
    QObject(parent),
    m_client(NULL),
    m_bConnected(false),
    m_bReconnect(false),
    m_bChecked(false)
{
    m_client = new QTcpSocket();
    if(m_client==NULL)
    {
        qInfo()<<"create tcpsocket client wrong";
        return;
    }
    connect(m_client,&QTcpSocket::readyRead,this,&TcpClient::OnReadData);
    connect(m_client,&QTcpSocket::connected,this,&TcpClient::OnConnect);
    connect(m_client,&QTcpSocket::disconnected,this,&TcpClient::OnDisConnect);
    connect(&m_timerReconnect,&QTimer::timeout,this,&TcpClient::OnReconnect);
    connect(&m_timerHeart,&QTimer::timeout,this,&TcpClient::OnHeartSend);
}

TcpClient::~TcpClient()
{
    if(m_client)
    {
        delete m_client;
        m_client = NULL;
    }
}

void TcpClient::SetIPAndPort(QString ip,int port)
{
    m_ip =  ip;
    m_port = port;
}

void TcpClient::StartHeartCheck(int ms, bool isOn)
{
    m_bChecked = isOn;
    if(m_bChecked)
    {
        m_timerHeart.start(ms);
    }
    else
    {
        m_timerHeart.stop();
    }
}

void TcpClient::StartReconnect(int ms, bool isOn)
{
    m_bReconnect = isOn;
    if(!isOn)
    {
        m_timerReconnect.stop();
    }
    else
    {
        m_timerReconnect.start(2000);
    }
}

void TcpClient::Connect()
{
    if(m_client==NULL)
    {
        qInfo()<<"create tcpsocket client again";
        m_client = new QTcpSocket();
        connect(m_client,&QTcpSocket::readyRead,this,&TcpClient::OnReadData);
        connect(m_client,&QTcpSocket::connected,this,&TcpClient::OnConnect);
        connect(m_client,&QTcpSocket::disconnected,this,&TcpClient::OnDisConnect);
    }
    m_client->connectToHost(m_ip,m_port);
}

void TcpClient::OnReconnect()
{
    if(!m_bConnected)
    {
        Connect();
        qInfo()<<"try to reconnect"<<m_ip<<m_port;
    }
}

void TcpClient::OnReadData()
{
    QByteArray data =  m_client->readAll();
//    qDebug()<<"tcp recv data "<<data;

    emit sigRecvData(data);
}

void TcpClient::SendData(QString strData)
{
    if(!m_client->isWritable())
    {
        qInfo()<<"not ready to write";
        return;
    }
    qInfo()<<"tcp as client send "<<strData;
    int result = m_client->write(strData.toUtf8());
    if(result == -1)
    {
        qInfo()<<"出错";
    }

    if(m_bChecked)
    {
        m_timerReconnect.start();
    }
}

void TcpClient::OnConnect()
{
    qDebug()<<"connect sucess";
    m_bConnected = true;
    emit sigConnect(true);
}

void TcpClient::OnDisConnect()
{
    qDebug()<<"disconnect";
    m_bConnected = false;
    emit sigConnect(false);
}

QHostAddress TcpClient::GetClientAddress()
{
    return m_client->peerAddress();
}

quint16 TcpClient::GetClientPort()
{
    return m_client->peerPort();
}

bool TcpClient::IsConnected()
{
    return m_bConnected;
}

void TcpClient::OnHeartSend()
{

    if(!m_client->isWritable())
    {
        qInfo()<<"not ready to write";
        return;
    }
    int result = m_client->write("$start$$end$");
    if(result == -1)
    {
        qInfo()<<"OnHeartSend出错";
    }
}

