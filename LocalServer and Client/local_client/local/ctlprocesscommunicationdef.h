#ifndef CTLPROCESSCOMMUNICATIONDEF_H
#define CTLPROCESSCOMMUNICATIONDEF_H
#include <QtGlobal>
#include "ctlpersionaldef.h"
#pragma pack(push,1)

namespace CLTProcessCommunicationSpace {
#define MAXSIZE 1024*8//最大信息流字节数（8kb）
enum CommunicationType{
    type_server,
    type_client,
    type_unknow
};

struct MessageHead
{
    signed short size;  //本次数据大小.用于信息提取和校验.用于信息提取和校验.size = CTLProcessHeadSize + data.size();
    unsigned short type;  //消息类型,需要自己定义让收发双方知会.
};
struct UnicodeMessage
{
    MessageHead head;
    ushort *str;  //Unicode格式
};

struct TcpMessage{
    MessageHead head;
    char str[MAXSIZE];  //字节流格式，貌似传输数据大小太小了
};

struct LocalMessage
{
    MessageHead head;
    QByteArray data;  //防止char*指向内容被销毁
};

}
#pragma pack(pop)
#define CTLProcessHeadSize sizeof(MessageHead)
#endif // CTLPROCESSCOMMUNICATIONDEF_H
