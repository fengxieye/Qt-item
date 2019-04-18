#ifndef __ShareNAM_H__
#define __ShareNAM_H__

#include <stdint.h>

#pragma warning(disable:4819)

/*
 * 功能： 提供应用唯一的QNetworkAccessManager
 * 注意： 一定要在主程序主对象初始化时率先使用ShareNAM::get带上QObject做父对象，这样可以免去主动析构
 *       可以总是传入parent=0，这样需最后的时候调用析构释放。
 */
class QObject;
class QNetworkAccessManager;

class ShareNAM
{
public:
    static QNetworkAccessManager* get(QObject *parent=0);
    static void free();
protected:
    static QNetworkAccessManager* NAM;
    static uint32_t ScrambleCheck;
private:
    ShareNAM();
};

#endif // __ShareNAM_H__
