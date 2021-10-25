QT       += core gui network webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialog/cdripinfodialog.cpp \
    dialog/limitcountdialog.cpp \
    dialog/logindialog.cpp \
    dialog/poweroffdialog.cpp \
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
    manager/groupplaymanager.cpp \
    manager/inputmanager.cpp \
    manager/musicdbmanager.cpp \
    manager/observermanager.cpp \
    manager/playlistmanager.cpp \
    manager/playmanager.cpp \
    manager/qobuzmanager.cpp \
    manager/setupmanager.cpp \
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
    widget/browserwindow.cpp \
    widget/dabradiowindow.cpp \
    widget/devicelistwindow.cpp \
    widget/fmradiowindow.cpp \
    widget/form/formclassify.cpp \
    widget/form/formcoverart.cpp \
    widget/form/formlabel.cpp \
    widget/form/formplay.cpp \
    widget/form/formsort.cpp \
    widget/form/formtitle.cpp \
    widget/formBottom/iconservice.cpp \
    widget/formBottom/iconservicedelegate.cpp \
    widget/formBottom/iconserviceeditor.cpp \
    widget/formBottom/icontracks.cpp \
    widget/formBottom/icontracksdelegate.cpp \
    widget/formBottom/icontrackseditor.cpp \
    widget/formBottom/listdevice.cpp \
    widget/formBottom/listdevicedelegate.cpp \
    widget/formBottom/listdeviceeditor.cpp \
    widget/formBottom/listservice.cpp \
    widget/formBottom/listservicedelegate.cpp \
    widget/formBottom/listserviceeditor.cpp \
    widget/formBottom/listtracks.cpp \
    widget/formBottom/listtracksdelegate.cpp \
    widget/formBottom/listtrackseditor.cpp \
    widget/formTop/infohome.cpp \
    widget/formTop/infoservice.cpp \
    widget/formTop/infotracks.cpp \
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
    dialog/limitcountdialog.h \
    dialog/logindialog.h \
    dialog/poweroffdialog.h \
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
    manager/groupplaymanager.h \
    manager/imanager.h \
    manager/inputmanager.h \
    manager/musicdbmanager.h \
    manager/observermanager.h \
    manager/playlistmanager.h \
    manager/playmanager.h \
    manager/qobuzmanager.h \
    manager/setupmanager.h \
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
    widget/browser.h \
    widget/browserwindow.h \
    widget/dabradiowindow.h \
    widget/devicelistwindow.h \
    widget/fmradiowindow.h \
    widget/form/formclassify.h \
    widget/form/formcoverart.h \
    widget/form/formlabel.h \
    widget/form/formplay.h \
    widget/form/formsort.h \
    widget/form/formtitle.h \
    widget/formBottom/iconservice.h \
    widget/formBottom/iconservicedelegate.h \
    widget/formBottom/iconserviceeditor.h \
    widget/formBottom/icontracks.h \
    widget/formBottom/icontracksdelegate.h \
    widget/formBottom/icontrackseditor.h \
    widget/formBottom/listdevice.h \
    widget/formBottom/listdevicedelegate.h \
    widget/formBottom/listdeviceeditor.h \
    widget/formBottom/listservice.h \
    widget/formBottom/listservicedelegate.h \
    widget/formBottom/listserviceeditor.h \
    widget/formBottom/listtracks.h \
    widget/formBottom/listtracksdelegate.h \
    widget/formBottom/listtrackseditor.h \
    widget/formTop/infohome.h \
    widget/formTop/infoservice.h \
    widget/formTop/infotracks.h \
    widget/groupplaywindow.h \
    widget/inputwindow.h \
    widget/iservicewindow.h \
    widget/mainwindow.h \
    widget/musicdbwindow.h \
    widget/playlistwindow.h \
    widget/playwindow.h \
    widget/qobuz.h \
    widget/setup.h \
    widget/setupwindow.h \
    widget/sidemenu.h \
    widget/sidemenudelegate.h \
    widget/topwindow.h

FORMS += \
    dialog/cdripinfodialog.ui \
    dialog/limitcountdialog.ui \
    dialog/logindialog.ui \
    dialog/poweroffdialog.ui \
    dialog/searchdialog.ui \
    dialog/submenudialog.ui \
    dialog/trackinfodialog.ui \
    dialog/webengineviewdialog.ui \
    widget/audiocdwindow.ui \
    widget/browserwindow.ui \
    widget/dabradiowindow.ui \
    widget/devicelistwindow.ui \
    widget/fmradiowindow.ui \
    widget/form/formclassify.ui \
    widget/form/formcoverart.ui \
    widget/form/formlabel.ui \
    widget/form/formplay.ui \
    widget/form/formsort.ui \
    widget/form/formtitle.ui \
    widget/formBottom/iconservice.ui \
    widget/formBottom/iconserviceeditor.ui \
    widget/formBottom/icontracks.ui \
    widget/formBottom/icontrackseditor.ui \
    widget/formBottom/listdevice.ui \
    widget/formBottom/listdeviceeditor.ui \
    widget/formBottom/listservice.ui \
    widget/formBottom/listserviceeditor.ui \
    widget/formBottom/listtracks.ui \
    widget/formBottom/listtrackseditor.ui \
    widget/formTop/infohome.ui \
    widget/formTop/infoservice.ui \
    widget/formTop/infotracks.ui \
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

DISTFILES += \
    resource/Component 2-1.png \
    resource/Component 2-1@2x.png \
    resource/Component 2-1@3x.png \
    resource/Component 2-3.png \
    resource/Component 2-3@2x.png \
    resource/Component 2-3@3x.png \
    resource/Group 1464.png \
    resource/Group 1464@2x.png \
    resource/Group 1464@3x.png \
    resource/Group 1497.png \
    resource/Group 1497@2x.png \
    resource/Group 1497@3x.png \
    resource/Group 1903.png \
    resource/Group 1903@2x.png \
    resource/Group 1903@3x.png \
    resource/Group 1973.png \
    resource/Group 1973@2x.png \
    resource/Group 1973@3x.png \
    resource/Group 1978.png \
    resource/Group 1978@2x.png \
    resource/Group 1978@3x.png \
    resource/Group 2003.png \
    resource/Group 2003@2x.png \
    resource/Group 2003@3x.png \
    resource/Group 2031.png \
    resource/Group 2031@2x.png \
    resource/Group 2031@3x.png \
    resource/Group 2033.png \
    resource/Group 2033@2x.png \
    resource/Group 2033@3x.png \
    resource/Group 2034.png \
    resource/Group 2034@2x.png \
    resource/Group 2034@3x.png \
    resource/Group 2094.png \
    resource/Group 2094@2x.png \
    resource/Group 2094@3x.png \
    resource/Group 2095.png \
    resource/Group 2095@2x.png \
    resource/Group 2095@3x.png \
    resource/Path 289.png \
    resource/Path 289@2x.png \
    resource/Path 289@3x.png \
    resource/Rectangle 380.png \
    resource/Rectangle 380@2x.png \
    resource/Rectangle 380@3x.png \
    resource/Rectangle 383.png \
    resource/Rectangle 383@2x.png \
    resource/Rectangle 383@3x.png
