TEMPLATE = lib
CONFIG += static
include (../config.pri)

PKGCONFIG += xmms2-client-cpp
load (../features/pkgconfig.prf)

!xmms2-client-cpp:error (Please install the xmms2 c++ client library)

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
		   sourcepref.h \
           compat.h \
		   debug.h

QT += network
QMAKE_LFLAGS += -L$$[QT_INSTALL_PLUGINS]/imageformats

QMAKE_CXXFLAGS += -g
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
