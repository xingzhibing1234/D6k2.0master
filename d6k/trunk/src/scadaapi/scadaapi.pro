# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = lib
TARGET = scadaapi
DESTDIR = ../../bin
QT += core widgets gui
CONFIG += debug
DEFINES += _WINDOWS WIN64 SCADAAPI_LIBRARY WIN64 SCADA_SDK_LIB QT_WIDGETS_LIB
INCLUDEPATH += . \
    ./GeneratedFiles \
    ./GeneratedFiles/Debug \
    ./../../include \
    ./../../thirdparty/ACE_wrappers \
    ./GeneratedFiles/debug \
    $(QTDIR)/mkspecs/win32-msvc2013 \
    $(QTDIR)/mkspecs/win32-msvc2013 \
    ./../scdsvc \
    ./../fespub
LIBS += -L"./../../lib" \
    -L"./../../thirdparty/ACE_wrappers/lib" \
    -l../../lib/log \
    -l../../lib/mail \
    -l../../lib/ACEd
DEPENDPATH += .
MOC_DIR += GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
TRANSLATIONS += scadaapi-zh-CN.ts
win32:RC_FILE = scadaapi.rc
HEADERS += ../../include/scadaapi/scdapp_def.h \
    ../fespub/tag_att.h \
    ../scdpub/variant_data.h \
    ../scdsvc/data_def.h \
    ./base_mem_db.h \
    ./base_module.h \
    ./client_db.h \
    ./db_svc.h \
    ./fes_db.h \
    ./mem_db.h \
    ./scadaapi.h \
    ./scadaapi_global.h \
    ../../include/scadaapi/scdsvcapi.h \
    ./scada_db.h \
    ./server_db.h \
    ./sharemem.h \
    ./tagname_pool.h
SOURCES += ../fespub/tag_att.cpp \
    ../scdpub/variant_data.cpp \
    ./base_mem_db.cpp \
    ./base_module.cpp \
    ./client_db.cpp \
    ./db_svc.cpp \
    ./fes_db.cpp \
    ./mem_db.cpp \
    ./scadaapi.cpp \
    ./scadasvc_api.cpp \
    ./scada_db.cpp \
    ./server_db.cpp \
    ./sharemem.cpp \
    ./tagname_pool.cpp
TRANSLATIONS += ./scadaapi-zh-CN.ts
RESOURCES += scadaapi.qrc
