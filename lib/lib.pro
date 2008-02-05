TEMPLATE = lib
CONFIG += static
include (../config.pri)

MOC_DIR     = .moc
OBJECTS_DIR = .obj

SOURCES += xclient.cpp \
		   xclientcache.cpp \
		   xconfig.cpp \
		   playlistmodel.cpp \
		   xmmsqt4.cpp


HEADERS += xclient.h \
		   xclientcache.h \
		   xconfig.h \
		   playlistmodel.h \
		   xmmsqt4.h \
		   debug.h



;RESOURCES = promoe.qrc
;macx:RC_FILE = promoe.icns
;macx:INCLUDEPATH = /sw/include

QT += network
;macx:QTPLUGIN += qjpeg
QMAKE_LFLAGS += -L$$[QT_INSTALL_PLUGINS]/imageformats

CONFIG += link_pkgconfig

QMAKE_CXXFLAGS += -g
;CONFIG += debug warn_on
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
PKGCONFIG += xmms2-client xmms2-client-cpp

;CONFIG += avahi

