#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <memory>
#include <QThread>
#include <QMap>
#include <QMutex>
#include <QTimer>

/*-------------
 *
 * SetListenPort,然后 listen
 *
 *
 * -----------------*/


class TcpServer : public QObject
{
    Q_OBJECT

public:
    TcpServer(int port,QObject *parent = nullptr);
    TcpServer(QObject *parent = nullptr);
    ~TcpServer();
    void SetListenPort(int port);
    bool Listen();
    void SendData(QString data , int port = 0);
    void SendData(QByteArray data, int port = 0);
    bool TestPort(int port);
    void CheckHeart();
    void setHeartCheck(bool bCheck);
    int GetClientCount();

signals:
    void sigConnect(QString ip,int port);
    void sigDisconnect(QString ip,int port);
    void sigRecvData(QByteArray array,int port = 0);

protected slots:
    void OnReadData();
    void OnConnect();
    void OnDisconnect();

private:
    QTcpServer* m_server;
    //监听的端口
    int m_port;
    //对连接的socket的管理
    QMap<int,QTcpSocket*> m_clientMap;
    //记录多久没有收到消息
    QMap<QTcpSocket*,int> m_heartCountMap;
    //定时检测是否收到信息，一定时间没有收到就判断已掉线
    QTimer* m_timer;
    //是否心跳检测
    bool m_bHeartChecked;
    //没有异步的东西，暂时不需要
    QMutex m_mutex;

};

#endif // TCPSERVER_H
