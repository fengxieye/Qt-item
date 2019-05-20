#ifndef CTLPROCESSCOMMUNICATION_H
#define CTLPROCESSCOMMUNICATION_H

#include <QObject>
#include "ctlprocesscommunicationdef.h"
#include "ctlprocesscommunicationinterface.h"
using namespace CLTProcessCommunicationSpace;
class CTLProcessCommunication : public QObject
{
    Q_OBJECT
public:
    explicit CTLProcessCommunication(CommunicationType type, QString discribe, QObject *parent = 0);
    /**
     * @brief bind：绑定到服务节点，客户端与服务端指定统一的节点就能进行通信
     * @param processName：节点名称
     * @return bool
     */
    bool bind(const QString &name);
    //解绑
    void unbind();
    /**
     * @brief isBind：是否已绑定
     * @return
     */
    bool isBind();
    /**
     * @brief sendMessage: 发送数据
     * @param msg：数据，以结构体Message类型发送
     * @param discribe：发往哪个进程。
     *              服务器：1、discribe为空，发往所有客户端
     *                     2、discribe为某个客户端（进程）描述，发往该客户端
     *              客户端：不需要
     * @return bool
     */
    bool sendMessage(const LocalMessage &msg, QString discribe = QString());

    bool sendMessage(const QString &message, QString discribe = QString());
    bool sendMessage(int command, QString discribe = QString());
    /**
     * @brief type：返回当前进程角色-->服务器 | 服务器
     * @return
     */
    int type();
    /**
     * @brief discribe:返回当前进程程序描述，服务器可以通过该discribe建立通
     * @return int
     */
    QString discribe() const;
signals:
    /**
     * @brief sigMessageComing：信号，收到数据
     */
    void sigMessageComing(LocalMessage);
private slots:
    void onMessage(LocalMessage &msg);
private:
    CTLProcessCommunicationInterface *m_communicationManger;
};

#endif // CTLPROCESSCOMMUNICATION_H
