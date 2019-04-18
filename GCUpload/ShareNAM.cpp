#include "ShareNAM.h"
#include <QNetworkAccessManager>

#define STATIC_PRESET (0x19880904)
uint32_t ShareNAM::ScrambleCheck = STATIC_PRESET;
QNetworkAccessManager *ShareNAM::NAM = 0;
QNetworkAccessManager *ShareNAM::get(QObject *parent)
{
    if(ScrambleCheck!=STATIC_PRESET)
    {   //检查如果静态变量没有赋值，表示执行时序有问题，例如在构造函数中做了太多事情
        Q_ASSERT(false);
        return NULL;
    }
    if(NAM==0){
        NAM = new QNetworkAccessManager(parent);
    }
    return NAM;
}

void ShareNAM::free()
{
    if(NAM)
    {
        delete NAM;
    }
    NAM = 0;
}

ShareNAM::ShareNAM()
{
    Q_ASSERT(false);
}
