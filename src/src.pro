TEMPLATE = app

include (../config.pri)

COMPONENTS+=../lib/liblib.a
LIBS += $$COMPONENTS
PRE_TARGETDEPS = $$COMPONENTS

MOC_DIR     = .moc
OBJECTS_DIR = .obj
UI_DIR      = .ui

include(src.pri)

TARGET = promoe
DESTDIR = $$PWD/..

RESOURCES += ../data/promoe.qrc
macx:RC_FILE = promoe.icns
macx:INCLUDEPATH = /sw/include

QT += network
;macx:QTPLUGIN += qjpeg
QMAKE_LFLAGS += -L$$[QT_INSTALL_PLUGINS]/imageformats

CONFIG += link_pkgconfig

QMAKE_CXXFLAGS += -g
;CONFIG += debug warn_on
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
PKGCONFIG += xmms2-client xmms2-client-cpp

;CONFIG += avahi

