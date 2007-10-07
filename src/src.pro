TEMPLATE = app

include (../config.pri)

COMPONENTS+=../lib/liblib.a
LIBS += $$COMPONENTS
PRE_TARGETDEPS = $$COMPONENTS

SOURCES += PixWidget.cpp \
		   Skin.cpp \
		   MainWindow.cpp \
		   Display.cpp \
		   MainDisplay.cpp \
		   TitleBar.cpp \
		   Button.cpp \
		   TextBar.cpp \
		   NumberDisplay.cpp \
		   TimeDisplay.cpp \
		   XMMSHandler.cpp \
		   SmallNumberDisplay.cpp \
		   StereoMono.cpp \
		   PosBar.cpp \
		   PlayStatus.cpp \
		   ShadedDisplay.cpp \
		   Playlist.cpp \
		   PlaylistView.cpp \
		   SkinChooser.cpp \
		   PlaylistShade.cpp \
		   qtmd5.cpp \
		   SettingsWindow.cpp \
		   PlaylistMenu.cpp \
		   VolumeSlider.cpp \
		   ClutterBar.cpp \
		   Equalizer.cpp \
		   FileDialog.cpp \
		   BrowseModel.cpp \
		   BrowseDialog.cpp


HEADERS += PixWidget.h \
		   Skin.h \
		   MainWindow.h \
		   Display.h \
		   MainDisplay.h \
		   TitleBar.h \
		   Button.h \
		   TextBar.h \
		   NumberDisplay.h \
		   TimeDisplay.h \
		   XMMSHandler.h \
		   SmallNumberDisplay.h \
		   StereoMono.h \
		   PosBar.h \
		   PlayStatus.h \
		   ShadedDisplay.h \
		   Playlist.h \
		   PlaylistView.h \
		   SkinChooser.h \
		   PlaylistShade.h \
		   qtmd5.h \
		   SettingsWindow.h \
		   PlaylistMenu.h \
		   VolumeSlider.h \
		   ClutterBar.h \
		   Equalizer.h \
		   FileDialog.h \
		   BrowseModel.h \
		   BrowseDialog.h

TARGET = promoe
DESTDIR = $$PWD/..

RESOURCES = promoe.qrc
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

