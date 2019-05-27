#include "DetectOutbound.h"
#include <QDateTime>
#include <QtGlobal>
#include <QTimer>
#include <QDebug>
#include <QUrl>
#include <QCryptographicHash>
//把选中的网卡写进临时目录给其他程序提示.
#include <QSettings>
#include <QDir>

GCDetectOutbound::GCDetectOutbound(QObject *parent)
    : QObject(parent)
    , stopFlag_(false)
{
    //随机数种子
    qsrand(QDateTime::currentMSecsSinceEpoch());
}

GCDetectOutbound::~GCDetectOutbound()
{
    stop();
}

//probeAddr想要测试的网址，如果 空则调用其它网址
void GCDetectOutbound::init(QString probeAddr)
{
    qInfo()<<"WcDetectOutbound::init"<<probeAddr;
    Q_ASSERT(detectable_.count()==0); //侦测重复初始化.
    if(probeAddr.length()>0){
        detectable_ << probeAddr;
    }
    else{
        pushPubProbeHost();
    }

    retry_.setSingleShot(true);
    connect(&retry_, &QTimer::timeout, [=](){
        probe_.abort();
        onRetry();
    });
    connect(&probe_, &QTcpSocket::connected, this, [=](){
        QHostAddress ha = probe_.localAddress();
        emit sgLocalDetected(ha);
        if( chooseInterface(ha) ){
            stop();
        }
        else{
            probe_.close();
            retry_.start(2000);
        }
    },Qt::QueuedConnection);

    connect(&probe_, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
            [=](QAbstractSocket::SocketError socketError){
        probe_.close();
        retry_.start(2000);
        if(detectable_.count()<=1){
            pushPubProbeHost();
        }
    });
}

//
bool GCDetectOutbound::tryObtainFace(QNetworkInterface &face2)
{
    //目前的策略是,如果只有一个物理网卡,直接用. 否则返回false.
    //物理网卡的判断策略:不判断虚拟机vpn之类的虚拟网卡
    //通常能够访问外网的,都可以通过探测方式拿到对的网卡.对不能访问外网的,通过访问服务器拿到.

    auto faces = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface face, faces)
    {
        if(face.isValid()==false) continue;
        QNetworkInterface::InterfaceFlags flags = face.flags();
        if((flags & (int)QNetworkInterface::IsRunning) == 0) continue;
        if(flags & (int)QNetworkInterface::IsLoopBack) continue;

        if(face2.isValid()) return false;//此时已经不止一个网卡了
        qInfo()<<"cadidate face:"<<face;
        face2 = face;
    }

    //只有一个网卡
    if(face2.isValid()){
        qInfo()<<"choose interface"<<face2;
        auto addrents = face2.addressEntries();
        foreach(QNetworkAddressEntry addrent, addrents)
        {
            qInfo()<<"address:"<<addrent.ip();
        }
        //把选中的网卡写进临时目录给其他程序提示.要加上格式才是对滴
        QString strPath = QDir::temp().filePath("fkNet.ini");
        QSettings set(strPath,QSettings::IniFormat);
        qDebug()<<strPath;
        set.setValue("name",face2.name());
        set.setValue("time",QTime::currentTime().toString());

        return true;
    }

    return false;
}

void GCDetectOutbound::start()
{
    if(detectable_.count()==0){
        init();
    }
    qInfo()<<__FUNCTION__<<"start outbound detection.";
    stopFlag_ = false;
    onRetry();
}

void GCDetectOutbound::stop()
{
    if(stopFlag_ == false){
        qInfo()<<"outbound detection stopped.";
    }
    stopFlag_ = true;
    probe_.abort();
    retry_.stop();
}

QString GCDetectOutbound::faceID(const QNetworkInterface &face)
{
    //  md5 加密
//    QByteArray ba = QCryptographicHash::hash(face.hardwareAddress().toLocal8Bit(),
//                                             QCryptographicHash::Md5);
//    return QString("bmm:")+ba.toBase64();  //bmm = base64(md5(mac))

    return face.hardwareAddress();
}

//检验选择的address是不是本机存在的
bool GCDetectOutbound::chooseInterface(const QHostAddress &addr)
{
    auto faces = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface face, faces)
    {
        if(face.isValid()==false) continue;
        if(face.flags() & QNetworkInterface::IsLoopBack) continue;
        auto addrents = face.addressEntries();
        foreach(QNetworkAddressEntry addrent, addrents)
        {
            if(addrent.ip() == addr){
                qInfo()<<"choose interface"<<face<<"with"<<addr;
                emit sgLocalInterface(face);
                return true;
            }
        }
    }

    qInfo()<<"error: Corresponding interface of"<<addr<<"not found!";
    return false;
}

void GCDetectOutbound::pushPubProbeHost()
{
    //插入候选探测服务器host. 对于自动跳https的不优先选择(也可以使用)
    detectable_ << "qq.com";
    detectable_ << "163.com";
    detectable_ << "sohu.com";
    detectable_ << "youku.com";
    qInfo()<<"detectable pool now:"<<detectable_;
}

void GCDetectOutbound::onRetry()
{
    if(stopFlag_){
        qInfo()<<"stopFlag was set!";
        return;
    }
    if(probe_.state() != QAbstractSocket::UnconnectedState){
        qInfo()<<"socket state error="<<probe_.state();
        return;
    }
    QString candi = detectable_.at(qrand()%(detectable_.count()));;
    QUrl u = QUrl::fromUserInput(candi);
    qInfo()<<"detect outbound using"<<u;
    probe_.connectToHost(u.host(),u.port(80));
}
