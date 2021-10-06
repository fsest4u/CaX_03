QT       += core gui network webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialog/cdripinfodialog.cpp \
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
    widget/base/iconservice.cpp \
    widget/base/iconservicedelegate.cpp \
    widget/base/iconserviceeditor.cpp \
    widget/base/icontracks.cpp \
    widget/base/icontracksdelegate.cpp \
    widget/base/icontrackseditor.cpp \
    widget/base/infohome.cpp \
    widget/base/infoservice.cpp \
    widget/base/infotracks.cpp \
    widget/base/listdevice.cpp \
    widget/base/listdevicedelegate.cpp \
    widget/base/listdeviceeditor.cpp \
    widget/base/listservice.cpp \
    widget/base/listservicedelegate.cpp \
    widget/base/listserviceeditor.cpp \
    widget/base/listtracks.cpp \
    widget/base/listtracksdelegate.cpp \
    widget/base/listtrackseditor.cpp \
    widget/browserwindow.cpp \
    widget/dabradiowindow.cpp \
    widget/devicelistwindow.cpp \
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
    widget/base/iconservice.h \
    widget/base/iconservicedelegate.h \
    widget/base/iconserviceeditor.h \
    widget/base/icontracks.h \
    widget/base/icontracksdelegate.h \
    widget/base/icontrackseditor.h \
    widget/base/infohome.h \
    widget/base/infoservice.h \
    widget/base/infotracks.h \
    widget/base/listdevice.h \
    widget/base/listdevicedelegate.h \
    widget/base/listdeviceeditor.h \
    widget/base/listservice.h \
    widget/base/listservicedelegate.h \
    widget/base/listserviceeditor.h \
    widget/base/listtracks.h \
    widget/base/listtracksdelegate.h \
    widget/base/listtrackseditor.h \
    widget/browser.h \
    widget/browserwindow.h \
    widget/dabradiowindow.h \
    widget/devicelistwindow.h \
    widget/fmradiowindow.h \
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
    dialog/logindialog.ui \
    dialog/poweroffdialog.ui \
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
    widget/base/listdevice.ui \
    widget/base/listdeviceeditor.ui \
    widget/base/listservice.ui \
    widget/base/listserviceeditor.ui \
    widget/base/listtracks.ui \
    widget/base/listtrackseditor.ui \
    widget/browserwindow.ui \
    widget/dabradiowindow.ui \
    widget/devicelistwindow.ui \
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

DISTFILES += \
    resource/Icon-12-iconsizeb.png \
    resource/Icon-130-sort-o.png \
    resource/Icon-130-sort.png \
    resource/Icon-22-favorite-o.png \
    resource/Icon-22-favorite.png \
    resource/Icon-22-iconsize-o.png \
    resource/Icon-22-iconsize.png \
    resource/Icon-22-rating-o.png \
    resource/Icon-22-rating.png \
    resource/Icon-22-sortd-o.png \
    resource/Icon-22-sortd-u.png \
    resource/Icon-22-sortd.png \
    resource/Icon-22-sortu-o.png \
    resource/Icon-22-sortu.png \
    resource/Icon-360-iconsize-u.png \
    resource/Icon-40-clo-o.png \
    resource/Icon-40-clo.png \
    resource/Icon-40-max.png \
    resource/Icon-52-play-o.png \
    resource/Icon-52-play-u.png \
    resource/Icon-52-play.png \
    resource/Icon-52-random-o.png \
    resource/Icon-52-random.png \
    resource/Icon-52-smenu-o.png \
    resource/Icon-52-smenu.png \
    resource/Icon-68-album.png \
    resource/Icon-68-artist.png \
    resource/Icon-68-genre.png \
    resource/Icon-68-track.png \
    resource/Icon-82-classify-o.png \
    resource/Icon-82-classify.png \
    resource/Icon-favorite-16-o.png \
    resource/Icon-favorite-16-u.png \
    resource/Icon-favorite-16.png \
    resource/Icon-hom-40-o.png \
    resource/Icon-hom-40.png \
    resource/Icon-lef-40-o.png \
    resource/Icon-lef-40.png \
    resource/Icon-max-o.png \
    resource/Icon-menu-o.png \
    resource/Icon-menu.png \
    resource/Icon-min-o.png \
    resource/Icon-min.png \
    resource/Icon-play-30-o.png \
    resource/Icon-play-30.png \
    resource/Icon-playbar-selectd-160-b.png \
    resource/Icon-playbar-selectd-160.png \
    resource/Icon-playbar-volume-160.png \
    resource/Icon-playdevices-52-o.png \
    resource/Icon-playdevices-52.png \
    resource/Icon-playff-26-o.png \
    resource/Icon-playff-26.png \
    resource/Icon-playinfo-20-o.png \
    resource/Icon-playinfo-20.png \
    resource/Icon-playpause-40-o.png \
    resource/Icon-playpause-40.png \
    resource/Icon-playplay-40-o.png \
    resource/Icon-playplay-40.png \
    resource/Icon-playrandom-20-o.png \
    resource/Icon-playrandom-20-u.png \
    resource/Icon-playrandom-20.png \
    resource/Icon-playreplay1-20-o.png \
    resource/Icon-playreplay1-20-u.png \
    resource/Icon-playreplay1-20.png \
    resource/Icon-playrew-26-o.png \
    resource/Icon-playrew-26.png \
    resource/Icon-playtext-20-o.png \
    resource/Icon-playtext-20.png \
    resource/Icon-playvolume-52-o.png \
    resource/Icon-playvolume-52.png \
    resource/Icon-rating-16-o.png \
    resource/Icon-rating-16-u.png \
    resource/Icon-rating-16.png \
    resource/Icon-rig-o.png \
    resource/Icon-rig.png \
    resource/Icon-search-16.png \
    resource/Icon-search-o.png \
    resource/Icon-search-u.png \
    resource/Icon-search.png \
    resource/Icon-sidebar-250-10background-o.png \
    resource/Icon-sidebar-250-10background.png \
    resource/Icon-sidebar-250-11setup-o.png \
    resource/Icon-sidebar-250-11setup.png \
    resource/Icon-sidebar-250-12devices-o.png \
    resource/Icon-sidebar-250-12devices.png \
    resource/Icon-sidebar-250-13power-o.png \
    resource/Icon-sidebar-250-13power.png \
    resource/Icon-sidebar-250-1musicdb-o.png \
    resource/Icon-sidebar-250-1musicdb.png \
    resource/Icon-sidebar-250-2audiocd-o.png \
    resource/Icon-sidebar-250-2audiocd.png \
    resource/Icon-sidebar-250-3playlist-o.png \
    resource/Icon-sidebar-250-3playlist.png \
    resource/Icon-sidebar-250-4browser-o.png \
    resource/Icon-sidebar-250-4browser.png \
    resource/Icon-sidebar-250-5inters-o.png \
    resource/Icon-sidebar-250-5inters.png \
    resource/Icon-sidebar-250-6input-o.png \
    resource/Icon-sidebar-250-6input.png \
    resource/Icon-sidebar-250-7fmradio-o.png \
    resource/Icon-sidebar-250-7fmradio.png \
    resource/Icon-sidebar-250-8dab-o.png \
    resource/Icon-sidebar-250-8dab.png \
    resource/Icon-sidebar-250-9groupplay-o.png \
    resource/Icon-sidebar-250-9groupplay.png \
    resource/Icon-tit.png \
    resource/rectangle_280.png



