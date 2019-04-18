INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

QT += network

LIBS += -L$$PWD/third/lib
win32:{
    CONFIG(debug,debug|release){
         LIBS += -llibcurld_imp
    }
    else {
        LIBS += -llibcurl_imp
    }
    LIBS += -lssleay32 -llibeay32
}

SOURCES += \
    $$PWD/qiniu/auth_mac.c \
    $$PWD/b64/b64.c \
    $$PWD/qiniu/base.c \
    $$PWD/qiniu/base_io.c \
    $$PWD/cJSON/cJSON.c \
    $$PWD/qiniu/conf.c \
    $$PWD/qiniu/emu_posix.c \
    $$PWD/qiniu/http.c \
    $$PWD/qiniu/io.c \
    $$PWD/qiniu/reader.c \
    $$PWD/qiniu/resumable_io.c \
    $$PWD/qiniu/rs.c \
    $$PWD/b64/urlsafe_b64.c \
    $$PWD/CommonPostUpload.cpp \
    $$PWD/Upload2Cloud.cpp \
    $$PWD/GCQiniuUpload.cpp \
    $$PWD/ShareNAM.cpp \
    $$PWD/PostParamHelper.cpp

HEADERS += \
    $$PWD/QiniuUpload.h \
    $$PWD/b64/b64.h \
    $$PWD/qiniu/base.h \
    $$PWD/cJSON/cJSON.h \
    $$PWD/qiniu/conf.h \
    $$PWD/qiniu/emu_posix.h \
    $$PWD/qiniu/http.h \
    $$PWD/qiniu/io.h \
    $$PWD/qiniu/macro.h \
    $$PWD/qiniu/reader.h \
    $$PWD/qiniu/resumable_io.h \
    $$PWD/qiniu/rs.h \
    $$PWD/b64/urlsafe_b64.h \
    $$PWD/CommonPostUpload.h \
    $$PWD/ShareNAM.h \
    $$PWD/Upload2Cloud.h \
    $$PWD/UploadDefine.h \
    $$PWD/PostParamHelper.h
