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
    resource/Component 2-1.png \
    resource/Component 2-1.png \
    resource/Component 2-1@2x.png \
    resource/Component 2-1@2x.png \
    resource/Component 2-1@3x.png \
    resource/Component 2-1@3x.png \
    resource/Component 2-3.png \
    resource/Component 2-3.png \
    resource/Component 2-3@2x.png \
    resource/Component 2-3@2x.png \
    resource/Component 2-3@3x.png \
    resource/Component 2-3@3x.png \
    resource/Group 1464.png \
    resource/Group 1464.png \
    resource/Group 1464@2x.png \
    resource/Group 1464@2x.png \
    resource/Group 1464@3x.png \
    resource/Group 1464@3x.png \
    resource/Group 1497.png \
    resource/Group 1497.png \
    resource/Group 1497@2x.png \
    resource/Group 1497@2x.png \
    resource/Group 1497@3x.png \
    resource/Group 1497@3x.png \
    resource/Group 1903.png \
    resource/Group 1903.png \
    resource/Group 1903@2x.png \
    resource/Group 1903@2x.png \
    resource/Group 1903@3x.png \
    resource/Group 1903@3x.png \
    resource/Group 1973.png \
    resource/Group 1973.png \
    resource/Group 1973@2x.png \
    resource/Group 1973@2x.png \
    resource/Group 1973@3x.png \
    resource/Group 1973@3x.png \
    resource/Group 1978.png \
    resource/Group 1978.png \
    resource/Group 1978@2x.png \
    resource/Group 1978@2x.png \
    resource/Group 1978@3x.png \
    resource/Group 1978@3x.png \
    resource/Group 2003.png \
    resource/Group 2003.png \
    resource/Group 2003@2x.png \
    resource/Group 2003@2x.png \
    resource/Group 2003@3x.png \
    resource/Group 2003@3x.png \
    resource/Group 2031.png \
    resource/Group 2031.png \
    resource/Group 2031@2x.png \
    resource/Group 2031@2x.png \
    resource/Group 2031@3x.png \
    resource/Group 2031@3x.png \
    resource/Group 2033.png \
    resource/Group 2033.png \
    resource/Group 2033@2x.png \
    resource/Group 2033@2x.png \
    resource/Group 2033@3x.png \
    resource/Group 2033@3x.png \
    resource/Group 2034.png \
    resource/Group 2034.png \
    resource/Group 2034@2x.png \
    resource/Group 2034@2x.png \
    resource/Group 2034@3x.png \
    resource/Group 2034@3x.png \
    resource/Group 2094.png \
    resource/Group 2094.png \
    resource/Group 2094@2x.png \
    resource/Group 2094@2x.png \
    resource/Group 2094@3x.png \
    resource/Group 2094@3x.png \
    resource/Group 2095.png \
    resource/Group 2095.png \
    resource/Group 2095@2x.png \
    resource/Group 2095@2x.png \
    resource/Group 2095@3x.png \
    resource/Group 2095@3x.png \
    resource/Path 289.png \
    resource/Path 289.png \
    resource/Path 289@2x.png \
    resource/Path 289@2x.png \
    resource/Path 289@3x.png \
    resource/Path 289@3x.png \
    resource/Rectangle 380.png \
    resource/Rectangle 380.png \
    resource/Rectangle 380@2x.png \
    resource/Rectangle 380@2x.png \
    resource/Rectangle 380@3x.png \
    resource/Rectangle 380@3x.png \
    resource/Rectangle 383.png \
    resource/Rectangle 383.png \
    resource/Rectangle 383@2x.png \
    resource/Rectangle 383@2x.png \
    resource/Rectangle 383@3x.png \
    resource/Rectangle 383@3x.png \
    resource/audiocd-popup-btn110-ok-h.png \
    resource/audiocd-popup-btn110-ok-h@2x.png \
    resource/audiocd-popup-btn110-ok-h@3x.png \
    resource/audiocd-popup-btn110-ok-n.png \
    resource/audiocd-popup-btn110-ok-n@2x.png \
    resource/audiocd-popup-btn110-ok-n@3x.png \
    resource/audiocd-popup-icon16-cdrip.png \
    resource/audiocd-popup-icon16-cdrip@2x.png \
    resource/audiocd-popup-icon16-cdrip@3x.png \
    resource/audiocd-popup-icon16-eject.png \
    resource/audiocd-popup-icon16-eject@2x.png \
    resource/audiocd-popup-icon16-eject@3x.png \
    resource/audiocd-popup-icon16-play.png \
    resource/audiocd-popup-icon16-play@2x.png \
    resource/audiocd-popup-icon16-play@3x.png \
    resource/audiocd-popup-icon16-trackinfo.png \
    resource/audiocd-popup-icon16-trackinfo@2x.png \
    resource/audiocd-popup-icon16-trackinfo@3x.png \
    resource/blackscreen-btn26-close.png \
    resource/blackscreen-btn26-close@2x.png \
    resource/blackscreen-btn26-close@3x.png \
    resource/blackscreen-btn26-lyrics-n.png \
    resource/blackscreen-btn26-lyrics-n@2x.png \
    resource/blackscreen-btn26-lyrics-n@3x.png \
    resource/blackscreen-btn26-play-n.png \
    resource/blackscreen-btn26-play-n@2x.png \
    resource/blackscreen-btn26-play-n@3x.png \
    resource/blackscreen-btn26-play-u.png \
    resource/blackscreen-btn26-play-u@2x.png \
    resource/blackscreen-btn26-play-u@3x.png \
    resource/blackscreen-btn26-review-n.png \
    resource/blackscreen-btn26-review-n@2x.png \
    resource/blackscreen-btn26-review-n@3x.png \
    resource/browser-icon16-cleanreplaygain.png \
    resource/browser-icon16-cleanreplaygain@2x.png \
    resource/browser-icon16-cleanreplaygain@3x.png \
    resource/browser-icon16-convertaudio.png \
    resource/browser-icon16-convertaudio@2x.png \
    resource/browser-icon16-convertaudio@3x.png \
    resource/browser-icon16-copy.png \
    resource/browser-icon16-copy@2x.png \
    resource/browser-icon16-copy@3x.png \
    resource/browser-icon16-delete.png \
    resource/browser-icon16-delete@2x.png \
    resource/browser-icon16-delete@3x.png \
    resource/browser-icon16-infoalbum.png \
    resource/browser-icon16-infoalbum@2x.png \
    resource/browser-icon16-infoalbum@3x.png \
    resource/browser-icon16-move.png \
    resource/browser-icon16-move@2x.png \
    resource/browser-icon16-move@3x.png \
    resource/browser-icon16-nameedit.png \
    resource/browser-icon16-nameedit@2x.png \
    resource/browser-icon16-nameedit@3x.png \
    resource/browser-icon16-newfolder.png \
    resource/browser-icon16-newfolder@2x.png \
    resource/browser-icon16-newfolder@3x.png \
    resource/browser-icon16-reload.png \
    resource/browser-icon16-reload@2x.png \
    resource/browser-icon16-reload@3x.png \
    resource/browser-icon16-replaygain.png \
    resource/browser-icon16-replaygain@2x.png \
    resource/browser-icon16-replaygain@3x.png \
    resource/browser-icon16-select.png \
    resource/browser-icon16-select@2x.png \
    resource/browser-icon16-select@3x.png \
    resource/browser-icon16-selectall.png \
    resource/browser-icon16-selectall@2x.png \
    resource/browser-icon16-selectall@3x.png \
    resource/browser-icon16-settag.png \
    resource/browser-icon16-settag@2x.png \
    resource/browser-icon16-settag@3x.png \
    resource/browser-icon16-songinfo.png \
    resource/browser-icon16-songinfo@2x.png \
    resource/browser-icon16-songinfo@3x.png \
    resource/browser-img160-brank-h.png \
    resource/browser-img160-brank-h@2x.png \
    resource/browser-img160-brank-h@3x.png \
    resource/browser-img160-brank-n.png \
    resource/browser-img160-brank-n@2x.png \
    resource/browser-img160-brank-n@3x.png \
    resource/browser-img160-hdd-h.png \
    resource/browser-img160-hdd-h@2x.png \
    resource/browser-img160-hdd-h@3x.png \
    resource/browser-img160-hdd-n.png \
    resource/browser-img160-hdd-n@2x.png \
    resource/browser-img160-hdd-n@3x.png \
    resource/browser-img160-net-h.png \
    resource/browser-img160-net-h@2x.png \
    resource/browser-img160-net-h@3x.png \
    resource/browser-img160-net-n.png \
    resource/browser-img160-net-n@2x.png \
    resource/browser-img160-net-n@3x.png \
    resource/browser-img160-newfolder-h.png \
    resource/browser-img160-newfolder-h@2x.png \
    resource/browser-img160-newfolder-h@3x.png \
    resource/browser-img160-upnp-h.png \
    resource/browser-img160-upnp-h@2x.png \
    resource/browser-img160-upnp-h@3x.png \
    resource/browser-img160-upnp-n.png \
    resource/browser-img160-upnp-n@2x.png \
    resource/browser-img160-upnp-n@3x.png \
    resource/btm-btn20-info-h.png \
    resource/btm-btn20-info-h@2x.png \
    resource/btm-btn20-info-h@3x.png \
    resource/btm-btn20-info-n.png \
    resource/btm-btn20-info-n@2x.png \
    resource/btm-btn20-info-n@3x.png \
    resource/btm-btn20-random-h.png \
    resource/btm-btn20-random-h@2x.png \
    resource/btm-btn20-random-h@3x.png \
    resource/btm-btn20-random-n.png \
    resource/btm-btn20-random-n@2x.png \
    resource/btm-btn20-random-n@3x.png \
    resource/btm-btn26-ff-h.png \
    resource/btm-btn26-ff-h@2x.png \
    resource/btm-btn26-ff-h@3x.png \
    resource/btm-btn26-ff-n.png \
    resource/btm-btn26-ff-n@2x.png \
    resource/btm-btn26-ff-n@3x.png \
    resource/btm-btn26-rew-h.png \
    resource/btm-btn26-rew-h@2x.png \
    resource/btm-btn26-rew-h@3x.png \
    resource/btm-btn26-rew-n.png \
    resource/btm-btn26-rew-n@2x.png \
    resource/btm-btn26-rew-n@3x.png \
    resource/btm-btn30-volume-h.png \
    resource/btm-btn30-volume-h@2x.png \
    resource/btm-btn30-volume-h@3x.png \
    resource/btm-btn30-volume-n.png \
    resource/btm-btn30-volume-n@2x.png \
    resource/btm-btn30-volume-n@3x.png \
    resource/btm-btn40-pause-h.png \
    resource/btm-btn40-pause-h@2x.png \
    resource/btm-btn40-pause-h@3x.png \
    resource/btm-btn40-pause-n.png \
    resource/btm-btn40-pause-n@2x.png \
    resource/btm-btn40-pause-n@3x.png \
    resource/btm-btn40-play-h.png \
    resource/btm-btn40-play-h@2x.png \
    resource/btm-btn40-play-h@3x.png \
    resource/btm-btn40-play-n.png \
    resource/btm-btn40-play-n@2x.png \
    resource/btm-btn40-play-n@3x.png \
    resource/btm-btn50-device-h.png \
    resource/btm-btn50-device-h@2x.png \
    resource/btm-btn50-device-h@3x.png \
    resource/btm-btn50-device-n.png \
    resource/btm-btn50-device-n@2x.png \
    resource/btm-btn50-device-n@3x.png \
    resource/groupp-btn20-togle-off.png \
    resource/groupp-btn20-togle-off@2x.png \
    resource/groupp-btn20-togle-off@3x.png \
    resource/groupp-btn20-togle-on.png \
    resource/groupp-btn20-togle-on@2x.png \
    resource/groupp-btn20-togle-on@3x.png \
    resource/groupp-icon30-volume.png \
    resource/groupp-icon30-volume@2x.png \
    resource/groupp-icon30-volume@3x.png \
    resource/groupp-img160-n.png \
    resource/groupp-img160-n@2x.png \
    resource/groupp-img160-n@3x.png \
    resource/heart.png \
    resource/heart@2x.png \
    resource/heart@3x.png \
    resource/input-img160-aesebu-h.png \
    resource/input-img160-aesebu-h@2x.png \
    resource/input-img160-aesebu-h@3x.png \
    resource/input-img160-aesebu-n.png \
    resource/input-img160-aesebu-n@2x.png \
    resource/input-img160-aesebu-n@3x.png \
    resource/input-img160-analogin-h.png \
    resource/input-img160-analogin-h@2x.png \
    resource/input-img160-analogin-h@3x.png \
    resource/input-img160-analogin-n.png \
    resource/input-img160-analogin-n@2x.png \
    resource/input-img160-analogin-n@3x.png \
    resource/input-img160-auxin-h.png \
    resource/input-img160-auxin-h@2x.png \
    resource/input-img160-auxin-h@3x.png \
    resource/input-img160-auxin-n.png \
    resource/input-img160-auxin-n@2x.png \
    resource/input-img160-auxin-n@3x.png \
    resource/input-img160-coaxial-h.png \
    resource/input-img160-coaxial-h@2x.png \
    resource/input-img160-coaxial-h@3x.png \
    resource/input-img160-coaxial-n.png \
    resource/input-img160-coaxial-n@2x.png \
    resource/input-img160-coaxial-n@3x.png \
    resource/input-img160-ponoin-h.png \
    resource/input-img160-ponoin-h@2x.png \
    resource/input-img160-ponoin-h@3x.png \
    resource/input-img160-ponoin-n.png \
    resource/input-img160-ponoin-n@2x.png \
    resource/input-img160-ponoin-n@3x.png \
    resource/input-img160-toslink-h.png \
    resource/input-img160-toslink-h@2x.png \
    resource/input-img160-toslink-h@3x.png \
    resource/input-img160-toslink-n.png \
    resource/input-img160-toslink-n@2x.png \
    resource/input-img160-toslink-n@3x.png \
    resource/internets-img160-airable-h.png \
    resource/internets-img160-airable-h@2x.png \
    resource/internets-img160-airable-h@3x.png \
    resource/internets-img160-airable-n.png \
    resource/internets-img160-airable-n@2x.png \
    resource/internets-img160-airable-n@3x.png \
    resource/internets-img160-deezer-h.png \
    resource/internets-img160-deezer-h@2x.png \
    resource/internets-img160-deezer-h@3x.png \
    resource/internets-img160-deezer-n.png \
    resource/internets-img160-deezer-n@2x.png \
    resource/internets-img160-deezer-n@3x.png \
    resource/internets-img160-napster-h.png \
    resource/internets-img160-napster-h@2x.png \
    resource/internets-img160-napster-h@3x.png \
    resource/internets-img160-napster-n.png \
    resource/internets-img160-napster-n@2x.png \
    resource/internets-img160-napster-n@3x.png \
    resource/internets-img160-qobuz-h.png \
    resource/internets-img160-qobuz-h@2x.png \
    resource/internets-img160-qobuz-h@3x.png \
    resource/internets-img160-qobuz-n.png \
    resource/internets-img160-qobuz-n@2x.png \
    resource/internets-img160-qobuz-n@3x.png \
    resource/internets-img160-roon-h.png \
    resource/internets-img160-roon-h@2x.png \
    resource/internets-img160-roon-h@3x.png \
    resource/internets-img160-roon-n.png \
    resource/internets-img160-roon-n@2x.png \
    resource/internets-img160-roon-n@3x.png \
    resource/internets-img160-spotify-h.png \
    resource/internets-img160-spotify-h@2x.png \
    resource/internets-img160-spotify-h@3x.png \
    resource/internets-img160-spotify-n.png \
    resource/internets-img160-spotify-n@2x.png \
    resource/internets-img160-spotify-n@3x.png \
    resource/internets-img160-tidal-h.png \
    resource/internets-img160-tidal-h@2x.png \
    resource/internets-img160-tidal-h@3x.png \
    resource/internets-img160-tidal-n.png \
    resource/internets-img160-tidal-n@2x.png \
    resource/internets-img160-tidal-n@3x.png \
    resource/mid-icon16-likeoff.png \
    resource/mid-icon16-likeoff@2x.png \
    resource/mid-icon16-likeoff@3x.png \
    resource/mid-icon16-likeon.png \
    resource/mid-icon16-likeon@2x.png \
    resource/mid-icon16-likeon@3x.png \
    resource/mid-icon16-rankoff.png \
    resource/mid-icon16-rankoff@2x.png \
    resource/mid-icon16-rankoff@3x.png \
    resource/mid-icon16-rankon-h.png \
    resource/mid-icon16-rankon-h1.png \
    resource/mid-icon16-rankon-h1@2x.png \
    resource/mid-icon16-rankon-h1@3x.png \
    resource/mid-icon16-rankon-h2.png \
    resource/mid-icon16-rankon-h2@2x.png \
    resource/mid-icon16-rankon-h2@3x.png \
    resource/mid-icon16-rankon-h3.png \
    resource/mid-icon16-rankon-h3@2x.png \
    resource/mid-icon16-rankon-h3@3x.png \
    resource/mid-icon16-rankon-h4.png \
    resource/mid-icon16-rankon-h4@2x.png \
    resource/mid-icon16-rankon-h4@3x.png \
    resource/mid-icon16-rankon-h5.png \
    resource/mid-icon16-rankon-h5@2x.png \
    resource/mid-icon16-rankon-h5@3x.png \
    resource/mid-icon16-rankon-h@2x.png \
    resource/mid-icon16-rankon-h@3x.png \
    resource/mid-icon16-rankon.png \
    resource/mid-icon16-rankon@2x.png \
    resource/mid-icon16-rankon@3x.png \
    resource/mid-icon30-play-n.png \
    resource/mid-icon30-play-n@2x.png \
    resource/mid-icon30-play-n@3x.png \
    resource/play-btn12-like-h.png \
    resource/play-btn12-like-h@2x.png \
    resource/play-btn12-like-h@3x.png \
    resource/play-btn12-like-n.png \
    resource/play-btn12-like-n@2x.png \
    resource/play-btn12-like-n@3x.png \
    resource/play-btn12-like-u.png \
    resource/play-btn12-like-u@2x.png \
    resource/play-btn12-like-u@3x.png \
    resource/play-btn16-item-menu-h.png \
    resource/play-btn16-item-menu-h@2x.png \
    resource/play-btn16-item-menu-h@3x.png \
    resource/play-btn16-item-menu-n.png \
    resource/play-btn16-item-menu-n@2x.png \
    resource/play-btn16-item-menu-n@3x.png \
    resource/play-btn20-pause-icon.png \
    resource/play-btn20-pause-icon@2x.png \
    resource/play-btn20-pause-icon@3x.png \
    resource/play-btn20-play-icon.png \
    resource/play-btn20-play-icon@2x.png \
    resource/play-btn20-play-icon@3x.png \
    resource/play-btn28-like-h.png \
    resource/play-btn28-like-h@2x.png \
    resource/play-btn28-like-h@3x.png \
    resource/play-btn28-like-n.png \
    resource/play-btn28-like-n@2x.png \
    resource/play-btn28-like-n@3x.png \
    resource/play-btn28-menu-h.png \
    resource/play-btn28-menu-h@2x.png \
    resource/play-btn28-menu-h@3x.png \
    resource/play-btn28-menu-n.png \
    resource/play-btn28-menu-n@2x.png \
    resource/play-btn28-menu-n@3x.png \
    resource/play-btn28-popupmenu-h.png \
    resource/play-btn28-popupmenu-h@2x.png \
    resource/play-btn28-popupmenu-h@3x.png \
    resource/play-btn28-popupmenu-n.png \
    resource/play-btn28-popupmenu-n@2x.png \
    resource/play-btn28-popupmenu-n@3x.png \
    resource/play-btn28-rank-h.png \
    resource/play-btn28-rank-h@2x.png \
    resource/play-btn28-rank-h@3x.png \
    resource/play-btn28-rank-n.png \
    resource/play-btn28-rank-n@2x.png \
    resource/play-btn28-rank-n@3x.png \
    resource/play-popup-icon16-addplaylist.png \
    resource/play-popup-icon16-addplaylist@2x.png \
    resource/play-popup-icon16-addplaylist@3x.png \
    resource/play-popup-icon16-bar261-n.png \
    resource/play-popup-icon16-bar261-n@2x.png \
    resource/play-popup-icon16-bar261-n@3x.png \
    resource/play-popup-icon16-cleanreplaygain.png \
    resource/play-popup-icon16-cleanreplaygain@2x.png \
    resource/play-popup-icon16-cleanreplaygain@3x.png \
    resource/play-popup-icon16-delete.png \
    resource/play-popup-icon16-delete@2x.png \
    resource/play-popup-icon16-delete@3x.png \
    resource/play-popup-icon16-exportsongs.png \
    resource/play-popup-icon16-exportsongs@2x.png \
    resource/play-popup-icon16-exportsongs@3x.png \
    resource/play-popup-icon16-load1000.png \
    resource/play-popup-icon16-load1000@2x.png \
    resource/play-popup-icon16-load1000@3x.png \
    resource/play-popup-icon16-loadalloff.png \
    resource/play-popup-icon16-loadalloff@2x.png \
    resource/play-popup-icon16-loadalloff@3x.png \
    resource/play-popup-icon16-nameedit.png \
    resource/play-popup-icon16-nameedit@2x.png \
    resource/play-popup-icon16-nameedit@3x.png \
    resource/play-popup-icon16-reload.png \
    resource/play-popup-icon16-reload@2x.png \
    resource/play-popup-icon16-reload@3x.png \
    resource/play-popup-icon16-replaygain.png \
    resource/play-popup-icon16-replaygain@2x.png \
    resource/play-popup-icon16-replaygain@3x.png \
    resource/play-popup-icon16-searchcoverart.png \
    resource/play-popup-icon16-searchcoverart@2x.png \
    resource/play-popup-icon16-searchcoverart@3x.png \
    resource/play-popup-icon16-select-h.png \
    resource/play-popup-icon16-select-h@2x.png \
    resource/play-popup-icon16-select-h@3x.png \
    resource/play-popup-icon16-select-n.png \
    resource/play-popup-icon16-select-n@2x.png \
    resource/play-popup-icon16-select-n@3x.png \
    resource/play-popup-icon16-selectall.png \
    resource/play-popup-icon16-selectall@2x.png \
    resource/play-popup-icon16-selectall@3x.png \
    resource/play-popup-icon16-songinfo.png \
    resource/play-popup-icon16-songinfo@2x.png \
    resource/play-popup-icon16-songinfo@3x.png \
    resource/playlist-btn100-addalbum.png \
    resource/playlist-btn100-addalbum@2x.png \
    resource/playlist-btn100-addalbum@3x.png \
    resource/playlist-btn100-addsong.png \
    resource/playlist-btn100-addsong@2x.png \
    resource/playlist-btn100-addsong@3x.png \
    resource/playlist-btn30-selecton-h.png \
    resource/playlist-btn30-selecton-h@2x.png \
    resource/playlist-btn30-selecton-h@3x.png \
    resource/playlist-btn30-selecton-n.png \
    resource/playlist-btn30-selecton-n@2x.png \
    resource/playlist-btn30-selecton-n@3x.png \
    resource/playlist-icon16-clearall.png \
    resource/playlist-icon16-clearall@2x.png \
    resource/playlist-icon16-clearall@3x.png \
    resource/playlist-icon16-delete.png \
    resource/playlist-icon16-delete@2x.png \
    resource/playlist-icon16-delete@3x.png \
    resource/playlist-icon16-exportsong.png \
    resource/playlist-icon16-exportsong@2x.png \
    resource/playlist-icon16-exportsong@3x.png \
    resource/playlist-icon16-nameedit.png \
    resource/playlist-icon16-nameedit@2x.png \
    resource/playlist-icon16-nameedit@3x.png \
    resource/playlist-icon16-newplaylist.png \
    resource/playlist-icon16-newplaylist@2x.png \
    resource/playlist-icon16-newplaylist@3x.png \
    resource/playlist-icon16-play.png \
    resource/playlist-icon16-play@2x.png \
    resource/playlist-icon16-play@3x.png \
    resource/playlist-icon16-songinfo.png \
    resource/playlist-icon16-songinfo@2x.png \
    resource/playlist-icon16-songinfo@3x.png \
    resource/playlist-icon30-select.png \
    resource/playlist-icon30-select@2x.png \
    resource/playlist-icon30-select@3x.png \
    resource/playlist-img160-albumart-h.png \
    resource/playlist-img160-albumart-h@2x.png \
    resource/playlist-img160-albumart-h@3x.png \
    resource/popup-bar200-h.png \
    resource/popup-bar200-h@2x.png \
    resource/popup-bar200-h@3x.png \
    resource/popup-bar200-n.png \
    resource/popup-bar200-n@2x.png \
    resource/popup-bar200-n@3x.png \
    resource/radio-icon-bbc.png \
    resource/radio-icon-bbc@2x.png \
    resource/radio-icon-bbc@3x.png \
    resource/radio-icon-k.png \
    resource/radio-icon-k@2x.png \
    resource/radio-icon-k@3x.png \
    resource/radio-icon-ytn.png \
    resource/radio-icon-ytn@2x.png \
    resource/radio-icon-ytn@3x.png \
    resource/radio-img160-blank-h.png \
    resource/radio-img160-blank-h@2x.png \
    resource/radio-img160-blank-h@3x.png \
    resource/radio-img160-blank-n.png \
    resource/radio-img160-blank-n@2x.png \
    resource/radio-img160-blank-n@3x.png \
    resource/radio-img160-channelicon-h.png \
    resource/radio-img160-channelicon-h@2x.png \
    resource/radio-img160-channelicon-h@3x.png \
    resource/radio-img160-channelicon-n.png \
    resource/radio-img160-channelicon-n@2x.png \
    resource/radio-img160-channelicon-n@3x.png \
    resource/rectangle_280.png \
    resource/search-icon24-on.png \
    resource/search-icon24-on@2x.png \
    resource/search-icon24-on@3x.png \
    resource/submenu-icon40-audiocd.png \
    resource/submenu-icon40-audiocd@2x.png \
    resource/submenu-icon40-audiocd@3x.png \
    resource/submenu-icon40-background.png \
    resource/submenu-icon40-background@2x.png \
    resource/submenu-icon40-background@3x.png \
    resource/submenu-icon40-browser.png \
    resource/submenu-icon40-browser@2x.png \
    resource/submenu-icon40-browser@3x.png \
    resource/submenu-icon40-davradio.png \
    resource/submenu-icon40-davradio@2x.png \
    resource/submenu-icon40-davradio@3x.png \
    resource/submenu-icon40-fmradio.png \
    resource/submenu-icon40-fmradio@2x.png \
    resource/submenu-icon40-fmradio@3x.png \
    resource/submenu-icon40-groupplay.png \
    resource/submenu-icon40-groupplay@2x.png \
    resource/submenu-icon40-groupplay@3x.png \
    resource/submenu-icon40-input.png \
    resource/submenu-icon40-input@2x.png \
    resource/submenu-icon40-input@3x.png \
    resource/submenu-icon40-internetservice.png \
    resource/submenu-icon40-internetservice@2x.png \
    resource/submenu-icon40-internetservice@3x.png \
    resource/submenu-icon40-musicdb.png \
    resource/submenu-icon40-musicdb@2x.png \
    resource/submenu-icon40-musicdb@3x.png \
    resource/submenu-icon40-playlist.png \
    resource/submenu-icon40-playlist@2x.png \
    resource/submenu-icon40-playlist@3x.png \
    resource/submenu-icon40-poweroff.png \
    resource/submenu-icon40-poweroff@2x.png \
    resource/submenu-icon40-poweroff@3x.png \
    resource/submenu-icon40-selectdevice.png \
    resource/submenu-icon40-selectdevice@2x.png \
    resource/submenu-icon40-selectdevice@3x.png \
    resource/submenu-icon40-setup.png \
    resource/submenu-icon40-setup@2x.png \
    resource/submenu-icon40-setup@3x.png \
    resource/top-bar22-iconsize-drag.png \
    resource/top-bar22-iconsize-drag@2x.png \
    resource/top-bar22-iconsize-drag@3x.png \
    resource/top-bar22-iconsize-n.png \
    resource/top-bar22-iconsize-n@2x.png \
    resource/top-bar22-iconsize-n@3x.png \
    resource/top-bar22-iconsize-on.png \
    resource/top-bar22-iconsize-on@2x.png \
    resource/top-bar22-iconsize-on@3x.png \
    resource/top-btn130-sort-h.png \
    resource/top-btn130-sort-h@2x.png \
    resource/top-btn130-sort-h@3x.png \
    resource/top-btn130-sort-n.png \
    resource/top-btn130-sort-n@2x.png \
    resource/top-btn130-sort-n@3x.png \
    resource/top-btn22-classify-like-h.png \
    resource/top-btn22-classify-like-h@2x.png \
    resource/top-btn22-classify-like-h@3x.png \
    resource/top-btn22-classify-like-n.png \
    resource/top-btn22-classify-like-n@2x.png \
    resource/top-btn22-classify-like-n@3x.png \
    resource/top-btn22-classify-rank-h.png \
    resource/top-btn22-classify-rank-h@2x.png \
    resource/top-btn22-classify-rank-h@3x.png \
    resource/top-btn22-classify-rank-n.png \
    resource/top-btn22-classify-rank-n@2x.png \
    resource/top-btn22-classify-rank-n@3x.png \
    resource/top-btn22-sortu-h.png \
    resource/top-btn22-sortu-h@2x.png \
    resource/top-btn22-sortu-h@3x.png \
    resource/top-btn22-sortu-n.png \
    resource/top-btn22-sortu-n@2x.png \
    resource/top-btn22-sortu-n@3x.png \
    resource/top-btn40-ff-h.png \
    resource/top-btn40-ff-h@2x.png \
    resource/top-btn40-ff-h@3x.png \
    resource/top-btn40-ff-n.png \
    resource/top-btn40-ff-n@2x.png \
    resource/top-btn40-ff-n@3x.png \
    resource/top-btn40-home-h.png \
    resource/top-btn40-home-h@2x.png \
    resource/top-btn40-home-h@3x.png \
    resource/top-btn40-home-n.png \
    resource/top-btn40-home-n@2x.png \
    resource/top-btn40-home-n@3x.png \
    resource/top-btn40-menu-h.png \
    resource/top-btn40-menu-h@2x.png \
    resource/top-btn40-menu-h@3x.png \
    resource/top-btn40-menu-n.png \
    resource/top-btn40-menu-n@3x.png \
    resource/top-btn40-rew-h.png \
    resource/top-btn40-rew-h@2x.png \
    resource/top-btn40-rew-h@3x.png \
    resource/top-btn40-rew-n.png \
    resource/top-btn40-rew-n@2x.png \
    resource/top-btn40-rew-n@3x.png \
    resource/top-btn52-menu-h.png \
    resource/top-btn52-menu-h@2x.png \
    resource/top-btn52-menu-h@3x.png \
    resource/top-btn52-menu-n.png \
    resource/top-btn52-menu-n@2x.png \
    resource/top-btn52-menu-n@3x.png \
    resource/top-btn52-play-h.png \
    resource/top-btn52-play-h@2x.png \
    resource/top-btn52-play-h@3x.png \
    resource/top-btn52-play-n.png \
    resource/top-btn52-play-n@2x.png \
    resource/top-btn52-play-n@3x.png \
    resource/top-btn52-random-h.png \
    resource/top-btn52-random-h@2x.png \
    resource/top-btn52-random-h@3x.png \
    resource/top-btn52-random-n.png \
    resource/top-btn52-random-n@2x.png \
    resource/top-btn52-random-n@3x.png \
    resource/top-btn52-search-h.png \
    resource/top-btn52-search-h@2x.png \
    resource/top-btn52-search-h@3x.png \
    resource/top-btn82-classify-add-h.png \
    resource/top-btn82-classify-add-h@2x.png \
    resource/top-btn82-classify-add-h@3x.png \
    resource/top-btn82-classify-add-n.png \
    resource/top-btn82-classify-add-n@2x.png \
    resource/top-btn82-classify-add-n@3x.png \
    resource/top-icon20-musicx-logo.png \
    resource/top-icon20-musicx-logo@2x.png \
    resource/top-icon20-musicx-logo@3x.png \
    resource/top-icon68-album.png \
    resource/top-icon68-album@2x.png \
    resource/top-icon68-album@3x.png \
    resource/top-icon68-artist.png \
    resource/top-icon68-artist@2x.png \
    resource/top-icon68-artist@3x.png \
    resource/top-icon68-genre.png \
    resource/top-icon68-genre@2x.png \
    resource/top-icon68-genre@3x.png \
    resource/top-icon68-track.png \
    resource/top-icon68-track@2x.png \
    resource/top-icon68-track@3x.png \
    resource/top-search-icon16-mini.png \
    resource/top-search-icon16-mini@2x.png \
    resource/top-search-icon16-mini@3x.png \
    resource/top-search-icon24-off.png \
    resource/top-search-icon24-off@2x.png \
    resource/top-search-icon24-off@3x.png \
    resource/top-searchbar52-off.png \
    resource/top-searchbar52-off@2x.png \
    resource/top-searchbar52-off@3x.png \
    resource/volume-btn16-high-h.png \
    resource/volume-btn16-high-h@2x.png \
    resource/volume-btn16-high-h@3x.png \
    resource/volume-btn16-high-n.png \
    resource/volume-btn16-high-n@2x.png \
    resource/volume-btn16-high-n@3x.png \
    resource/volume-btn16-low-h.png \
    resource/volume-btn16-low-h@2x.png \
    resource/volume-btn16-low-h@3x.png \
    resource/volume-btn16-low-n.png \
    resource/volume-btn16-low-n@2x.png \
    resource/volume-btn16-low-n@3x.png \
    resource/volume-btn160.png \
    resource/volume-btn160@2x.png \
    resource/volume-btn160@3x.png



