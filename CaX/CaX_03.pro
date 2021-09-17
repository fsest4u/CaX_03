QT       += core gui network webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialog/logindialog.cpp \
    dialog/searchdialog.cpp \
    dialog/submenudelegate.cpp \
    dialog/submenudialog.cpp \
    dialog/webengineviewdialog.cpp \
    main.cpp \
    manager/airablemanager.cpp \
    manager/appmanager.cpp \
    manager/basemanager.cpp \
    manager/browsermanager.cpp \
    manager/dabradiomanager.cpp \
    manager/devicemanager.cpp \
    manager/fmradiomanager.cpp \
    manager/inputmanager.cpp \
    manager/mymusicmanager.cpp \
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
    widget/base/albuminfo.cpp \
    widget/base/categoryconts.cpp \
    widget/base/categorycontsdelegate.cpp \
    widget/base/categorycontseditor.cpp \
    widget/base/menuicon.cpp \
    widget/base/menuicondelegate.cpp \
    widget/base/menuiconeditor.cpp \
    widget/base/menuinfo.cpp \
    widget/base/menulist.cpp \
    widget/base/menulistdelegate.cpp \
    widget/base/menulisteditor.cpp \
    widget/base/musicinfo.cpp \
    widget/base/songconts.cpp \
    widget/base/songcontscover.cpp \
    widget/base/songcontsdelegate.cpp \
    widget/base/songcontseditor.cpp \
    widget/browserwindow.cpp \
    widget/dabradiowindow.cpp \
    widget/fmradiowindow.cpp \
    widget/groupplaywindow.cpp \
    widget/inputwindow.cpp \
    widget/iservicewindow.cpp \
    widget/mainwindow.cpp \
    widget/mymusicwindow.cpp \
    widget/playlistwindow.cpp \
    widget/playwindow.cpp \
    widget/setupwindow.cpp \
    widget/sidemenu.cpp \
    widget/sidemenudelegate.cpp \
    widget/topwindow.cpp

HEADERS += \
    dialog/logindialog.h \
    dialog/requestinterceptor.h \
    dialog/searchdialog.h \
    dialog/submenudelegate.h \
    dialog/submenudialog.h \
    dialog/webengineviewdialog.h \
    manager/airablemanager.h \
    manager/appmanager.h \
    manager/basemanager.h \
    manager/browsermanager.h \
    manager/dabradiomanager.h \
    manager/devicemanager.h \
    manager/fmradiomanager.h \
    manager/imanager.h \
    manager/inputmanager.h \
    manager/mymusicmanager.h \
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
    widget/base/albuminfo.h \
    widget/base/categoryconts.h \
    widget/base/categorycontsdelegate.h \
    widget/base/categorycontseditor.h \
    widget/base/menuicon.h \
    widget/base/menuicondelegate.h \
    widget/base/menuiconeditor.h \
    widget/base/menuinfo.h \
    widget/base/menulist.h \
    widget/base/menulistdelegate.h \
    widget/base/menulisteditor.h \
    widget/base/musicinfo.h \
    widget/base/songconts.h \
    widget/base/songcontscover.h \
    widget/base/songcontsdelegate.h \
    widget/base/songcontseditor.h \
    widget/browser.h \
    widget/browserwindow.h \
    widget/dabradiowindow.h \
    widget/fmradiowindow.h \
    widget/groupplaywindow.h \
    widget/inputwindow.h \
    widget/iservicewindow.h \
    widget/mainwindow.h \
    widget/mymusicwindow.h \
    widget/playlistwindow.h \
    widget/playwindow.h \
    widget/qobuz.h \
    widget/setupwindow.h \
    widget/sidemenu.h \
    widget/sidemenudelegate.h \
    widget/topwindow.h

FORMS += \
    dialog/logindialog.ui \
    dialog/searchdialog.ui \
    dialog/submenudialog.ui \
    dialog/webengineviewdialog.ui \
    widget/audiocdwindow.ui \
    widget/base/albuminfo.ui \
    widget/base/categoryconts.ui \
    widget/base/categorycontseditor.ui \
    widget/base/menuicon.ui \
    widget/base/menuiconeditor.ui \
    widget/base/menuinfo.ui \
    widget/base/menulist.ui \
    widget/base/menulisteditor.ui \
    widget/base/musicinfo.ui \
    widget/base/songconts.ui \
    widget/base/songcontscover.ui \
    widget/base/songcontseditor.ui \
    widget/browserwindow.ui \
    widget/dabradiowindow.ui \
    widget/fmradiowindow.ui \
    widget/groupplaywindow.ui \
    widget/inputwindow.ui \
    widget/iservicewindow.ui \
    widget/mainwindow.ui \
    widget/mymusicwindow.ui \
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
    resource.qrc \
    resource.qrc

DISTFILES += \
    resource/baseline_menu_black_24dp.png \
    resource/baseline_search_black_24dp.png \
    resource/outline_arrow_back_black_24dp.png \
    resource/outline_arrow_forward_black_24dp.png
