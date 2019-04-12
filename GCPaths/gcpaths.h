#ifndef GCPATHS_H
#define GCPATHS_H

#include <QObject>
#include <QMap>
/**
 * @brief The GCPaths class
 * 对Qt常用的系统路径进行单例封装
 *
 *
 * 调用的时候才会初始化
 *
 * 使用方式：
 * 加入头文件，调用path函数，例如：
 * qInfo()<<GCPaths::path(GCPaths::Apppath);
 */

class GCPaths
{
public:
    explicit GCPaths();
    ~GCPaths();
    enum PathType{
        Config = 0,//%appdata%/cloudboard配置的基础路径
        Profiles,//%appdata%/cloudboard/profiles 存放通用配置
        Cache,//%appdata%/cloudboard/cache存放解析文件缓存、下载文件缓存等
        Temp,//%temp% +/cbtemp临时文件存放
        HomePath,//留着，放着一些对外的配置，一般用不着
        Apppath,//app所在目录
        Desktop,//桌面
        Documents,//用于保存文件的默认路径等
    };
    static QString path(int type);
signals:

public slots:
private:
    void init();
    QMap<int, QString> m_paths;
};

#endif // GCPATHS_H
