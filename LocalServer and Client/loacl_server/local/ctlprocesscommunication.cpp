#include "ctlprocesscommunication.h"
#include "ctlprocesscommunicationclient.h"
#include "ctlprocesscommunicationserver.h"
CTLProcessCommunication::CTLProcessCommunication(CommunicationType type, QString discribe, QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<LocalMessage>("LocalMessage");
    qRegisterMetaType<UnicodeMessage>("UnicodeMessage");

    m_communicationManger = 0;
    switch (type) {
    case type_server:
        m_communicationManger = new CTLProcessCommunicationServer(this);
        break;
    case type_client:
        m_communicationManger = new CTLProcessCommunicationClient(this);
        break;
    default:
        Q_ASSERT_X(m_communicationManger, "CTLProcessCommunication",
                   "m_communicationManger is not uninitlize");
        throw("var m_communicationManger is not initlize");
        return ;
    }
    connect(m_communicationManger, SIGNAL(sigMessage(LocalMessage&)),
            this, SLOT(onMessage(LocalMessage&)));
    m_communicationManger->setDiscribe(discribe);
}

bool CTLProcessCommunication::bind(const QString &name)
{
    return m_communicationManger->bind(name);
}

void CTLProcessCommunication::unbind()
{
    m_communicationManger->unbind();
}

bool CTLProcessCommunication::isBind()
{
    return m_communicationManger->isBind();
}


bool CTLProcessCommunication::sendMessage(const LocalMessage &msg, QString discribe)
{
    if(type() == type_server){
        dynamic_cast<CTLProcessCommunicationServer *>(m_communicationManger)->setTarget(discribe);
    }
    return m_communicationManger->sendMessage(msg);
}

bool CTLProcessCommunication::sendMessage(const QString &message, QString discribe)
{
    LocalMessage msg;
    msg.data = message.toLocal8Bit();
    msg.head.type = type_message;
    msg.head.size = CTLProcessHeadSize + msg.data.size();

    return sendMessage(msg, discribe);
}

bool CTLProcessCommunication::sendMessage(int command, QString discribe)
{
    LocalMessage msg;
    msg.data = QString::number(command).toLocal8Bit();
    msg.head.type = type_command;
    msg.head.size = CTLProcessHeadSize + QString::number(command).size();

    return sendMessage(msg, discribe);
}

int CTLProcessCommunication::type()
{
    return m_communicationManger->type();
}

QString CTLProcessCommunication::discribe() const
{
    return m_communicationManger->discribe();
}

void CTLProcessCommunication::onMessage(LocalMessage &msg)
{
    //TODO what u want while message is comming! smile
    emit sigMessageComing(msg);
}
