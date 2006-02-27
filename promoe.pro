SOURCES += XmmsQT4.cpp \
		   PixWidget.cpp \
		   Skin.cpp \
		   MainWindow.cpp \
		   Display.cpp \
		   MainDisplay.cpp \
		   TitleBar.cpp \
		   Button.cpp \
		   TextBar.cpp \
		   NumberDisplay.cpp \
		   XMMSHandler.cpp \
		   SmallNumberDisplay.cpp \
		   StereoMono.cpp \
		   Slider.cpp \
		   PlayStatus.cpp \
		   ShadedDisplay.cpp \
		   Playlist.cpp \
		   PlaylistList.cpp \
		   SkinChooser.cpp

HEADERS += XmmsQT4.h \
		   PixWidget.h \
		   Skin.h \
		   MainWindow.h \
		   Display.h \
		   MainDisplay.h \
		   TitleBar.h \
		   Button.h \
		   TextBar.h \
		   NumberDisplay.h \
		   XMMSHandler.h \
		   SmallNumberDisplay.h \
		   StereoMono.h \
		   Slider.h \
		   PlayStatus.h \
		   ShadedDisplay.h \
		   Playlist.h \
		   PlaylistList.h \
		   SkinChooser.h


RESOURCES = promoe.qrc
CONFIG += link_pkgconfig
CONFIG += debug warn_on
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
PKGCONFIG += xmms2-client xmms2-client-cpp sigc++-2.0

