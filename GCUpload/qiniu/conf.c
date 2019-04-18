/*
 ============================================================================
 Name        : conf.c
 Author      : Qiniu.com
 Copyright   : 2012(c) Shanghai Qiniu Information Technologies Co., Ltd.
 Description :
 ============================================================================
 */
#include "conf.h"

const char *version = "6.2.6";

/*============================================================================*/

const char *QINIU_ACCESS_KEY = "<Please apply your access key>";
const char *QINIU_SECRET_KEY = "<Dont send your secret key to anyone>";

//公共服务http域名
const char *QINIU_RS_HOST = "http://rs.qiniu.com";
const char *QINIU_RSF_HOST = "http://rsf.qiniu.com";
const char *QINIU_API_HOST = "http://api.qiniu.com";
const char *QINIU_FUSION_HOST = "http://fusion.qiniuapi.com";
const char *QINIU_UC_HOST = "https://uc.qbox.me";

//默认华东机房
const char *QINIU_UP_HOST = "http://upload-z2.qiniu.com";
const char *QINIU_IOVIP_HOST = "http://iovip-z2.qbox.me";


//设置华东机房域名
void Qiniu_Use_Zone_Huadong(Qiniu_Bool useHttps) {
    if (useHttps == Qiniu_True) {
        QINIU_RS_HOST = "https://rs.qiniu.com";
        QINIU_RSF_HOST = "https://rsf.qiniu.com";
        QINIU_API_HOST = "https://api.qiniu.com";
        QINIU_UP_HOST = "https://upload.qbox.me";
        QINIU_IOVIP_HOST = "https://iovip.qbox.me";
    } else {
        QINIU_RS_HOST = "http://rs.qiniu.com";
        QINIU_RSF_HOST = "http://rsf.qiniu.com";
        QINIU_API_HOST = "http://api.qiniu.com";
        QINIU_UP_HOST = "http://upload.qiniu.com";
        QINIU_IOVIP_HOST = "http://iovip.qbox.me";
    }
}

//设置华北机房域名
void Qiniu_Use_Zone_Huabei(Qiniu_Bool useHttps) {
    if (useHttps == Qiniu_True) {
        QINIU_RS_HOST = "https://rs-z1.qiniu.com";
        QINIU_RSF_HOST = "https://rsf-z1.qiniu.com";
        QINIU_API_HOST = "https://api-z1.qiniu.com";
        QINIU_UP_HOST = "https://upload-z1.qbox.me";
        QINIU_IOVIP_HOST = "https://iovip-z1.qbox.me";
    } else {
        QINIU_RS_HOST = "http://rs-z1.qiniu.com";
        QINIU_RSF_HOST = "http://rsf-z1.qiniu.com";
        QINIU_API_HOST = "http://api-z1.qiniu.com";
        QINIU_UP_HOST = "http://upload-z1.qiniu.com";
        QINIU_IOVIP_HOST = "http://iovip-z1.qbox.me";
    }
}

//设置华南机房域名
void Qiniu_Use_Zone_Huanan(Qiniu_Bool useHttps) {
    if (useHttps == Qiniu_True) {
        QINIU_RS_HOST = "https://rs-z2.qiniu.com";
        QINIU_RSF_HOST = "https://rsf-z2.qiniu.com";
        QINIU_API_HOST = "https://api-z2.qiniu.com";
        QINIU_UP_HOST = "https://upload-z2.qbox.me";
        QINIU_IOVIP_HOST = "https://iovip-z2.qbox.me";
    } else {
        QINIU_RS_HOST = "http://rs-z2.qiniu.com";
        QINIU_RSF_HOST = "http://rsf-z2.qiniu.com";
        QINIU_API_HOST = "http://api-z2.qiniu.com";
        QINIU_UP_HOST = "http://upload-z2.qiniu.com";
        QINIU_IOVIP_HOST = "http://iovip-z2.qbox.me";
    }
}

//设置北美机房域名
QINIU_DLLAPI extern void Qiniu_Use_Zone_Beimei(Qiniu_Bool useHttps) {
    if (useHttps == Qiniu_True) {
        QINIU_RS_HOST = "https://rs-na0.qiniu.com";
        QINIU_RSF_HOST = "https://rsf-na0.qiniu.com";
        QINIU_API_HOST = "https://api-na0.qiniu.com";
        QINIU_UP_HOST = "https://upload-na0.qbox.me";
        QINIU_IOVIP_HOST = "https://iovip-na0.qbox.me";
    } else {
        QINIU_RS_HOST = "http://rs-na0.qiniu.com";
        QINIU_RSF_HOST = "http://rsf-na0.qiniu.com";
        QINIU_API_HOST = "http://api-na0.qiniu.com";
        QINIU_UP_HOST = "http://upload-na0.qiniu.com";
        QINIU_IOVIP_HOST = "http://iovip-na0.qbox.me";
    }
}
/*============================================================================*/

