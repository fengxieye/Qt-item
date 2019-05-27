#ifndef WCDETECTOUTBOUND_H
#define WCDETECTOUTBOUND_H
#include <QHostAddress>
#include <QNetworkInterface>
#include <QTcpSocket>
#include <QTimer>
//通过出埠流量探测工作网卡, 将此网卡MAC作为硬件ID

class GCDetectOutbound : public QObject
{
    Q_OBJECT
public:
    GCDetectOutbound(QObject *parent);
    ~GCDetectOutbound();
    void init(QString firstTryAddr = "");
    //尝试获取网卡,如果不成功,则只能通过下面的start函数连外网探测.
    //如果成功,遵循我们的探测策略.单卡直接用,否则false
    bool tryObtainFace(QNetworkInterface &face2);

    void start();
    void stop();

    static QString faceID(const QNetworkInterface &face);
signals:
    //自动重试,不会发失败的信号.
    //void sgDetectFailed(); //失败,或者网络不通.
    void sgLocalDetected(QHostAddress addr);
    void sgLocalInterface(QNetworkInterface face);
protected:
    bool chooseInterface(const QHostAddress &addr);
    void pushPubProbeHost();
protected slots:
    void onRetry();
private:
    QTcpSocket probe_;
    QStringList detectable_;
    bool stopFlag_;
    QTimer retry_;
    QString probeAddr_;

};

#endif // WCDETECTOUTBOUND_H
