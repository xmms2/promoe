TEMPLATE = app

include (../config.pri)
PKGCONFIG += xmms2-client-cpp
load (../features/pkgconfig.prf)

!xmms2-client-cpp:error (Please install the xmms2 c++ client library)

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
RESOURCES += ../data/Almond-blue.qrc
macx:RC_FILE = ../promoe.icns

QT += network
;macx:QTPLUGIN += qjpeg
QMAKE_LFLAGS += -L$$[QT_INSTALL_PLUGINS]/imageformats


QMAKE_CXXFLAGS += -g
;CONFIG += debug warn_on
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter

;CONFIG += avahi

