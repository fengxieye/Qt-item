#include "TcpServer.h"

//检测时间 ms
#define TIME_CHECK 10*1000
//检测次数，超了认为socket已断
#define COUNT_CHECK 5

TcpServer::TcpServer(int port,QObject *parent):
    QObject(parent),
    m_port(0),
    m_server(NULL),
    m_bHeartChecked(false)
{
    m_port = port;
    m_server = new QTcpServer();

    connect(m_server,&QTcpServer::newConnection,this,&TcpServer::OnConnect);
}

TcpServer::TcpServer(QObject *parent):
    QObject(parent),
    m_port(0),
    m_server(NULL),
    m_bHeartChecked(false)
{
    m_server = new QTcpServer();
    m_timer = new QTimer(this);
    connect(m_timer,&QTimer::timeout,this,&TcpServer::CheckHeart);
    connect(m_server,&QTcpServer::newConnection,this,&TcpServer::OnConnect);
}

TcpServer::~TcpServer()
{
    if(m_server)
    {
        delete m_server;
        m_server = NULL;
    }

    if(m_timer)
    {
        delete m_timer;
        m_timer = NULL;
    }
}

void TcpServer::SetListenPort(int port)
{
    m_port = port;
}

void TcpServer::setHeartCheck(bool bCheck)
{
    m_bHeartChecked = bCheck;
    if(m_bHeartChecked)
    {
        m_timer->start(TIME_CHECK);
    }
}

bool TcpServer::Listen()
{
    if(!m_server->listen(QHostAddress::Any,(quint16)m_port))
    {
        qInfo()<<m_server->errorString();
        return false;
    }
    else
    {
        qInfo()<<"start listen "<<m_port;
        return true;
    }
}

//检测端口是否占用，暂时先用吧，有问题再改
bool TcpServer::TestPort(int port)
{
    static QTcpSocket* test = new QTcpSocket(this);
    if(test->bind(port,QAbstractSocket::DontShareAddress))
    {
        qInfo()<<"test bind ok "<<port;
        test->close();
        return true;
    }
    else
    {
        qInfo()<<"test bind wrong"<<test->errorString();
        test->close();
        return false;
    }
}

//心跳检测，超过一定次数没有回复就认为断线
void TcpServer::CheckHeart()
{
    QMap<QTcpSocket*,int>::iterator it;
    for(it = m_heartCountMap.begin(); it != m_heartCountMap.end();)
    {
        if(it.value() == COUNT_CHECK)
        {
            //因为断开后会直接在槽函数里删除节点，所以it++要放到里面
            QTcpSocket* tmp = it.key();
            qInfo()<<"5 times not recv "<<tmp->peerPort();
            it++;
            tmp->disconnectFromHost();
        }
        else
        {
            it.value()++;
            it++;
        }
    }
}

//分包情况也是应该业务类来处理，和网络类无关联
void TcpServer::OnReadData()
{
    QTcpSocket* client = (QTcpSocket*)sender();
    int port = client->peerPort();

    QByteArray data =  client->readAll();
    qInfo()<<"as server recv "<<data;
    //收到消息了，清0
    m_heartCountMap[client]=0;
    emit sigRecvData(data,port);
}

void TcpServer::OnDisconnect()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    int port = socket->peerPort();
    QString strIP = socket->peerAddress().toString();
    qDebug()<<strIP<<" "<<port<<"disconnect";

    m_heartCountMap.remove(socket);
    m_clientMap.remove(socket->peerPort());
    socket->close();

    qDebug()<<"client count"<<m_clientMap.size();
    emit sigDisconnect(strIP,port);
}

void TcpServer::SendData(QString data,int port)
{
    QTcpSocket* socket = NULL;
    if(m_clientMap.count() == 0)
    {
        return;
    }

    if(port!=0)
    {
        socket  = m_clientMap.value(port);
    }
    else
    {
        socket = m_clientMap.begin().value();
    }

    if(socket == NULL)
    {
        return;
    }
    socket->write(data.toStdString().c_str());
}


void TcpServer::SendData(QByteArray data, int port)
{
    QTcpSocket* socket = NULL;
    if(m_clientMap.count() == 0)
    {
        return;
    }

    if(port!=0)
    {
        socket  = m_clientMap.value(port);
    }
    else
    {
        socket = m_clientMap.begin().value();
    }

    if(socket == NULL)
    {
        return;
    }
    socket->write(data.data());
}



void TcpServer::OnConnect()
{
    QTcpSocket* client = m_server->nextPendingConnection();
    //格式为 ::ffff:+ip
    QString address = client->peerAddress().toString();
    int port = client->peerPort();
    address.remove("::ffff:");
    qDebug()<<address<<port;

    m_clientMap.insert(port,client);
    m_heartCountMap.insert(client,0);

    connect(client,&QTcpSocket::readyRead,this,&TcpServer::OnReadData);
    connect(client,&QTcpSocket::disconnected,this,&TcpServer::OnDisconnect);

    emit sigConnect(address,port);
}

int TcpServer::GetClientCount()
{
    return m_clientMap.count();
}






