TEMPLATE = lib
CONFIG += static
include (../config.pri)
load (../features/pkgconfig.prf)

MOC_DIR     = .moc
OBJECTS_DIR = .obj

SOURCES += xclient.cpp \
		   xclientcache.cpp \
		   xconfig.cpp \
		   xplayback.cpp \
		   xcollection.cpp \
		   playlistmodel.cpp \
		   xmmsqt4.cpp


HEADERS += xclient.h \
		   xclientcache.h \
		   xconfig.h \
		   xplayback.h \
		   xcollection.h \
		   xcollection_p.h \
		   playlistmodel.h \
		   xmmsqt4.h \
		   debug.h



;RESOURCES = promoe.qrc
;macx:RC_FILE = promoe.icns
;macx:INCLUDEPATH = /sw/include

QT += network
;macx:QTPLUGIN += qjpeg
QMAKE_LFLAGS += -L$$[QT_INSTALL_PLUGINS]/imageformats


QMAKE_CXXFLAGS += -g
;CONFIG += debug warn_on
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter

XMMSCLIENTPKG += xmms2-client-cpp
pkgconfig_exists ($$XMMSCLIENTPKG) {
	QMAKE_CXXFLAGS  += $$pkgconfig_cflags($$XMMSCLIENTPKG)
	LIBS += $$pkgconfig_libs($$XMMSCLIENTPKG)
} else {
	error (Please install the xmms2 c++ client library)
}
;CONFIG += avahi

