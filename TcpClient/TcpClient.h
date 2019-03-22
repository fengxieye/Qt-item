#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QThread>
#include <QTime>


/*---------------------
 *
 * 设置端口和ip，然后connect
 *
 * -------------------*/

class TcpClient: public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);
    ~TcpClient();

    void SetIPAndPort(QString ip,int port);
    void StartReconnect(int ms, bool isOn = true); //是否需要定时重连
    void StartHeartCheck(int ms, bool isOn = true); //开始心跳
    QHostAddress GetClientAddress();
    quint16 GetClientPort();
    bool IsConnected();

    void Connect();
    void SendData(QString strData);

signals:
    void sigConnect(bool isConnected);
    void sigRecvData(QByteArray data);

protected slots:
    void OnReadData();
    void OnConnect();
    void OnDisConnect();
    void OnReconnect();
    void OnHeartSend(); //到时发送心跳数据

private:
    QTcpSocket* m_client = Q_NULLPTR;
    QString m_ip;
    bool m_bConnected;
    int m_port;
    bool m_bReconnect;
    QTimer m_timerReconnect;
    QTimer m_timerHeart; //定时发送心跳
    bool m_bChecked;
};

#endif // TCPCLIENT_H
