QT       += core gui network webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialog/cdripinfodialog.cpp \
    dialog/logindialog.cpp \
    dialog/searchdialog.cpp \
    dialog/submenudelegate.cpp \
    dialog/submenudialog.cpp \
    dialog/trackinfodialog.cpp \
    dialog/webengineviewdialog.cpp \
    main.cpp \
    manager/airablemanager.cpp \
    manager/appmanager.cpp \
    manager/audiocdmanager.cpp \
    manager/basemanager.cpp \
    manager/browsermanager.cpp \
    manager/dabradiomanager.cpp \
    manager/devicemanager.cpp \
    manager/fmradiomanager.cpp \
    manager/inputmanager.cpp \
    manager/musicdbmanager.cpp \
    manager/observermanager.cpp \
    manager/playmanager.cpp \
    manager/qobuzmanager.cpp \
    manager/sqlmanager.cpp \
    manager/ssdpmanager.cpp \
    network/observerclient.cpp \
    network/tcpclient.cpp \
    network/udpclient.cpp \
    util/CJsonNode.cpp \
    util/StringLib.cpp \
    util/TMapData.cpp \
    util/loading.cpp \
    util/log.cpp \
    util/settingio.cpp \
    util/waitingspinnerwidget.cpp \
    widget/audiocdwindow.cpp \
    widget/base/iconservice.cpp \
    widget/base/iconservicedelegate.cpp \
    widget/base/iconserviceeditor.cpp \
    widget/base/icontracks.cpp \
    widget/base/icontracksdelegate.cpp \
    widget/base/icontrackseditor.cpp \
    widget/base/infohome.cpp \
    widget/base/infoservice.cpp \
    widget/base/infotracks.cpp \
    widget/base/listservice.cpp \
    widget/base/listservicedelegate.cpp \
    widget/base/listserviceeditor.cpp \
    widget/base/listtracks.cpp \
    widget/base/listtracksdelegate.cpp \
    widget/base/listtrackseditor.cpp \
    widget/browserwindow.cpp \
    widget/dabradiowindow.cpp \
    widget/fmradiowindow.cpp \
    widget/groupplaywindow.cpp \
    widget/inputwindow.cpp \
    widget/iservicewindow.cpp \
    widget/mainwindow.cpp \
    widget/musicdbwindow.cpp \
    widget/playlistwindow.cpp \
    widget/playwindow.cpp \
    widget/setupwindow.cpp \
    widget/sidemenu.cpp \
    widget/sidemenudelegate.cpp \
    widget/topwindow.cpp

HEADERS += \
    dialog/cdripinfodialog.h \
    dialog/logindialog.h \
    dialog/requestinterceptor.h \
    dialog/searchdialog.h \
    dialog/submenudelegate.h \
    dialog/submenudialog.h \
    dialog/trackinfodialog.h \
    dialog/webengineviewdialog.h \
    manager/airablemanager.h \
    manager/appmanager.h \
    manager/audiocdmanager.h \
    manager/basemanager.h \
    manager/browsermanager.h \
    manager/dabradiomanager.h \
    manager/devicemanager.h \
    manager/fmradiomanager.h \
    manager/imanager.h \
    manager/inputmanager.h \
    manager/musicdbmanager.h \
    manager/observermanager.h \
    manager/playmanager.h \
    manager/qobuzmanager.h \
    manager/sqlmanager.h \
    manager/ssdpmanager.h \
    network/observerclient.h \
    network/tcpclient.h \
    network/udpclient.h \
    util/CJsonNode.h \
    util/CommonTypeDef.h \
    util/StringLib.h \
    util/TMapData.h \
    util/TMapDataDef.h \
    util/caxconstants.h \
    util/caxkeyvalue.h \
    util/loading.h \
    util/log.h \
    util/settingio.h \
    util/sqlconstatns.h \
    util/waitingspinnerwidget.h \
    widget/airable.h \
    widget/audiocdwindow.h \
    widget/base/iconservice.h \
    widget/base/iconservicedelegate.h \
    widget/base/iconserviceeditor.h \
    widget/base/icontracks.h \
    widget/base/icontracksdelegate.h \
    widget/base/icontrackseditor.h \
    widget/base/infohome.h \
    widget/base/infoservice.h \
    widget/base/infotracks.h \
    widget/base/listservice.h \
    widget/base/listservicedelegate.h \
    widget/base/listserviceeditor.h \
    widget/base/listtracks.h \
    widget/base/listtracksdelegate.h \
    widget/base/listtrackseditor.h \
    widget/browser.h \
    widget/browserwindow.h \
    widget/dabradiowindow.h \
    widget/fmradiowindow.h \
    widget/groupplaywindow.h \
    widget/inputwindow.h \
    widget/iservicewindow.h \
    widget/mainwindow.h \
    widget/musicdbwindow.h \
    widget/playlistwindow.h \
    widget/playwindow.h \
    widget/qobuz.h \
    widget/setupwindow.h \
    widget/sidemenu.h \
    widget/sidemenudelegate.h \
    widget/topwindow.h

FORMS += \
    dialog/cdripinfodialog.ui \
    dialog/logindialog.ui \
    dialog/searchdialog.ui \
    dialog/submenudialog.ui \
    dialog/trackinfodialog.ui \
    dialog/webengineviewdialog.ui \
    widget/audiocdwindow.ui \
    widget/base/iconservice.ui \
    widget/base/iconserviceeditor.ui \
    widget/base/icontracks.ui \
    widget/base/icontrackseditor.ui \
    widget/base/infohome.ui \
    widget/base/infoservice.ui \
    widget/base/infotracks.ui \
    widget/base/listservice.ui \
    widget/base/listserviceeditor.ui \
    widget/base/listtracks.ui \
    widget/base/listtrackseditor.ui \
    widget/browserwindow.ui \
    widget/dabradiowindow.ui \
    widget/fmradiowindow.ui \
    widget/groupplaywindow.ui \
    widget/inputwindow.ui \
    widget/iservicewindow.ui \
    widget/mainwindow.ui \
    widget/musicdbwindow.ui \
    widget/playlistwindow.ui \
    widget/playwindow.ui \
    widget/setupwindow.ui \
    widget/sidemenu.ui \
    widget/topwindow.ui

TRANSLATIONS += \
    CaX_03_ko_KR.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# about jansson lib
unix|win32: LIBS += -L$$PWD/../jansson-2.13.1/lib/ -ljansson

INCLUDEPATH += $$PWD/../jansson-2.13.1/include
DEPENDPATH += $$PWD/../jansson-2.13.1/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../jansson-2.13.1/lib/jansson.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../jansson-2.13.1/lib/libjansson.a

RESOURCES += \
    resource.qrc



