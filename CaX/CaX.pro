QT       += core gui network webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialog/aboutdialog.cpp \
    dialog/addqobuzplaylist.cpp \
    dialog/addradiodialog.cpp \
    dialog/cdripinfo.cpp \
    dialog/cdripinfodialog.cpp \
    dialog/commondialog.cpp \
    dialog/edittagdelegate.cpp \
    dialog/edittagdialog.cpp \
    dialog/formdialog.cpp \
    dialog/groupplaydialog.cpp \
    dialog/inputfavoritedialog.cpp \
    dialog/inputnamedialog.cpp \
    dialog/inputtagdialog.cpp \
    dialog/limitcountdialog.cpp \
    dialog/logindialog.cpp \
    dialog/poweroffdialog.cpp \
    dialog/progressdialog.cpp \
    dialog/progressfmdialog.cpp \
    dialog/searchcoverartdialog.cpp \
    dialog/searchcoverartresultdialog.cpp \
    dialog/searchdialog.cpp \
    dialog/selectformatdialog.cpp \
    dialog/selectnetworkinterfacedialog.cpp \
    dialog/setcolumndialog.cpp \
    dialog/trackinfo.cpp \
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
    manager/queuelistmanager.cpp \
    manager/searchcoverartresultmanager.cpp \
    manager/searchmanager.cpp \
    manager/setupmanager.cpp \
    manager/sqlmanager.cpp \
    manager/ssdpmanager.cpp \
    network/observerclient.cpp \
    network/tcpclient.cpp \
    network/udpclient.cpp \
    util/CJsonNode.cpp \
    util/StringLib.cpp \
    util/TMapData.cpp \
    util/WidgetMarqueeLabel.cpp \
    util/loading.cpp \
    util/log.cpp \
    util/settingio.cpp \
    util/utilnovatron.cpp \
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
    widget/form/formsearchcoverart.cpp \
    widget/form/formsort.cpp \
    widget/form/formtitle.cpp \
    widget/formBottom/iconcoverart.cpp \
    widget/formBottom/iconcoverartdelegate.cpp \
    widget/formBottom/iconcoverarteditor.cpp \
    widget/formBottom/iconservice.cpp \
    widget/formBottom/iconservicedelegate.cpp \
    widget/formBottom/iconserviceeditor.cpp \
    widget/formBottom/icontracks.cpp \
    widget/formBottom/icontracksdelegate.cpp \
    widget/formBottom/icontrackseditor.cpp \
    widget/formBottom/listbrowser.cpp \
    widget/formBottom/listbrowserdelegate.cpp \
    widget/formBottom/listbrowsereditor.cpp \
    widget/formBottom/listdevice.cpp \
    widget/formBottom/listdevicedelegate.cpp \
    widget/formBottom/listdeviceeditor.cpp \
    widget/formBottom/listservice.cpp \
    widget/formBottom/listservicedelegate.cpp \
    widget/formBottom/listserviceeditor.cpp \
    widget/formBottom/listsetup.cpp \
    widget/formBottom/listsetupdelegate.cpp \
    widget/formBottom/listsetupeditor.cpp \
    widget/formBottom/listtracks.cpp \
    widget/formBottom/listtracksdelegate.cpp \
    widget/formBottom/listtrackseditor.cpp \
    widget/formQueue/queueartist.cpp \
    widget/formQueue/queuelyrics.cpp \
    widget/formQueue/queuetrack.cpp \
    widget/formQueue/queuetrackdelegate.cpp \
    widget/formQueue/queuetrackeditor.cpp \
    widget/formSearch/searchcategory.cpp \
    widget/formSearch/searchcategorydelegate.cpp \
    widget/formSearch/searchcategoryeditor.cpp \
    widget/formSearch/searchtrack.cpp \
    widget/formSearch/searchtrackdelegate.cpp \
    widget/formSearch/searchtrackeditor.cpp \
    widget/formTop/infobrowser.cpp \
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
    widget/queuelistwindow.cpp \
    widget/searchwindow.cpp \
    widget/setupwindow.cpp \
    widget/topwindow.cpp

HEADERS += \
    dialog/aboutdialog.h \
    dialog/addqobuzplaylist.h \
    dialog/addradiodialog.h \
    dialog/cdripinfo.h \
    dialog/cdripinfodialog.h \
    dialog/commondialog.h \
    dialog/edittagdelegate.h \
    dialog/edittagdialog.h \
    dialog/formdialog.h \
    dialog/groupplaydialog.h \
    dialog/inputfavoritedialog.h \
    dialog/inputnamedialog.h \
    dialog/inputtagdialog.h \
    dialog/limitcountdialog.h \
    dialog/logindialog.h \
    dialog/poweroffdialog.h \
    dialog/progressdialog.h \
    dialog/progressfmdialog.h \
    dialog/requestinterceptor.h \
    dialog/searchcoverartdialog.h \
    dialog/searchcoverartresultdialog.h \
    dialog/searchdialog.h \
    dialog/selectformatdialog.h \
    dialog/selectnetworkinterfacedialog.h \
    dialog/setcolumndialog.h \
    dialog/trackinfo.h \
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
    manager/queuelistmanager.h \
    manager/searchcoverartresultmanager.h \
    manager/searchmanager.h \
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
    util/WidgetMarqueeLabel.h \
    util/caxconstants.h \
    util/caxkeyvalue.h \
    util/caxtranslate.h \
    util/loading.h \
    util/log.h \
    util/settingio.h \
    util/sqlconstatns.h \
    util/utilnovatron.h \
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
    widget/form/formsearchcoverart.h \
    widget/form/formsort.h \
    widget/form/formtitle.h \
    widget/formBottom/iconcoverart.h \
    widget/formBottom/iconcoverartdelegate.h \
    widget/formBottom/iconcoverarteditor.h \
    widget/formBottom/iconservice.h \
    widget/formBottom/iconservicedelegate.h \
    widget/formBottom/iconserviceeditor.h \
    widget/formBottom/icontracks.h \
    widget/formBottom/icontracksdelegate.h \
    widget/formBottom/icontrackseditor.h \
    widget/formBottom/listbrowser.h \
    widget/formBottom/listbrowserdelegate.h \
    widget/formBottom/listbrowsereditor.h \
    widget/formBottom/listdevice.h \
    widget/formBottom/listdevicedelegate.h \
    widget/formBottom/listdeviceeditor.h \
    widget/formBottom/listservice.h \
    widget/formBottom/listservicedelegate.h \
    widget/formBottom/listserviceeditor.h \
    widget/formBottom/listsetup.h \
    widget/formBottom/listsetupdelegate.h \
    widget/formBottom/listsetupeditor.h \
    widget/formBottom/listtracks.h \
    widget/formBottom/listtracksdelegate.h \
    widget/formBottom/listtrackseditor.h \
    widget/formQueue/queueartist.h \
    widget/formQueue/queuelyrics.h \
    widget/formQueue/queuetrack.h \
    widget/formQueue/queuetrackdelegate.h \
    widget/formQueue/queuetrackeditor.h \
    widget/formSearch/searchcategory.h \
    widget/formSearch/searchcategorydelegate.h \
    widget/formSearch/searchcategoryeditor.h \
    widget/formSearch/searchtrack.h \
    widget/formSearch/searchtrackdelegate.h \
    widget/formSearch/searchtrackeditor.h \
    widget/formTop/infobrowser.h \
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
    widget/queuelistwindow.h \
    widget/searchwindow.h \
    widget/setup.h \
    widget/setupwindow.h \
    widget/topwindow.h

FORMS += \
    dialog/aboutdialog.ui \
    dialog/addqobuzplaylist.ui \
    dialog/addradiodialog.ui \
    dialog/cdripinfo.ui \
    dialog/cdripinfodialog.ui \
    dialog/commondialog.ui \
    dialog/edittagdialog.ui \
    dialog/formdialog.ui \
    dialog/groupplaydialog.ui \
    dialog/inputfavoritedialog.ui \
    dialog/inputnamedialog.ui \
    dialog/inputtagdialog.ui \
    dialog/limitcountdialog.ui \
    dialog/logindialog.ui \
    dialog/poweroffdialog.ui \
    dialog/progressdialog.ui \
    dialog/progressfmdialog.ui \
    dialog/searchcoverartdialog.ui \
    dialog/searchcoverartresultdialog.ui \
    dialog/searchdialog.ui \
    dialog/selectformatdialog.ui \
    dialog/selectnetworkinterfacedialog.ui \
    dialog/setcolumndialog.ui \
    dialog/trackinfo.ui \
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
    widget/form/formsearchcoverart.ui \
    widget/form/formsort.ui \
    widget/form/formtitle.ui \
    widget/formBottom/iconcoverart.ui \
    widget/formBottom/iconcoverarteditor.ui \
    widget/formBottom/iconservice.ui \
    widget/formBottom/iconserviceeditor.ui \
    widget/formBottom/icontracks.ui \
    widget/formBottom/icontrackseditor.ui \
    widget/formBottom/listbrowser.ui \
    widget/formBottom/listbrowsereditor.ui \
    widget/formBottom/listdevice.ui \
    widget/formBottom/listdeviceeditor.ui \
    widget/formBottom/listservice.ui \
    widget/formBottom/listserviceeditor.ui \
    widget/formBottom/listsetup.ui \
    widget/formBottom/listsetupeditor.ui \
    widget/formBottom/listtracks.ui \
    widget/formBottom/listtrackseditor.ui \
    widget/formQueue/queueartist.ui \
    widget/formQueue/queuelyrics.ui \
    widget/formQueue/queuetrack.ui \
    widget/formQueue/queuetrackeditor.ui \
    widget/formSearch/searchcategory.ui \
    widget/formSearch/searchcategoryeditor.ui \
    widget/formSearch/searchtrack.ui \
    widget/formSearch/searchtrackeditor.ui \
    widget/formTop/infobrowser.ui \
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
    widget/queuelistwindow.ui \
    widget/searchwindow.ui \
    widget/setupwindow.ui \
    widget/topwindow.ui

RC_ICONS = logo-icon-musicxneo-256.ico

TRANSLATIONS += \
    CaX_en_US.ts \
    CaX_de_DE.ts \
    CaX_it_IT.ts \
    CaX_ja_JP.ts \
    CaX_ko_KR.ts \
    CaX_nl_NL.ts \
    CaX_ru_RU.ts \
    CaX_zh_CN.ts 
    
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# about jansson lib
win32:contains(QMAKE_HOST.arch, x86_64) {
  LIBS += -L$$PWD/../jansson-2.13.1/lib/x64 -ljansson
} else {
  LIBS += -L$$PWD/../jansson-2.13.1/lib/x86 -ljansson
}
unix: LIBS += -L$$PWD/../jansson-2.13.1/lib/osx -ljansson

INCLUDEPATH += $$PWD/../jansson-2.13.1/include
DEPENDPATH += $$PWD/../jansson-2.13.1/include

#win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../jansson-2.13.1/lib/jansson.lib
#else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../jansson-2.13.1/lib/osx/libjansson.a

RESOURCES += \
    resource.qrc

DISTFILES += \
    ../installer/build-64.bat \
    ../installer/build-86.bat \
    ../installer/installer-64.iss \
    ../installer/installer-86.iss \
    CaX_de_DE.ts \
    CaX_en_US.ts \
    CaX_it_IT.ts \
    CaX_ja_JP.ts \
    CaX_nl_NL.ts \
    CaX_ru_RU.ts \
    CaX_zh_CN.ts \
    app.ico \
    logo-icon-musicxneo-256.ico \

