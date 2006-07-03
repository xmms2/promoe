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
			 TimeDisplay.cpp \
		   XMMSHandler.cpp \
		   SmallNumberDisplay.cpp \
		   StereoMono.cpp \
		   Slider.cpp \
		   PlayStatus.cpp \
		   ShadedDisplay.cpp \
		   Playlist.cpp \
		   PlaylistList.cpp \
		   SkinChooser.cpp \
		   PlaylistShade.cpp \
		   qtmd5.cpp \
		   SettingsWindow.cpp \
		   PlaylistMenu.cpp \
		   VolumeSlider.cpp

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
		   TimeDisplay.h \
		   XMMSHandler.h \
		   SmallNumberDisplay.h \
		   StereoMono.h \
		   Slider.h \
		   PlayStatus.h \
		   ShadedDisplay.h \
		   Playlist.h \
		   PlaylistList.h \
		   SkinChooser.h \
		   PlaylistShade.h \
		   qtmd5.h \
		   SettingsWindow.h \
		   PlaylistMenu.h \
		   VolumeSlider.h \



RESOURCES = promoe.qrc
macx:RC_FILE = promoe.icns

QT += network
macx:QTPLUGIN += qjpeg
QMAKE_LFLAGS += -L$$[QT_INSTALL_PLUGINS]/imageformats

CONFIG += link_pkgconfig

;QMAKE_CXXFLAGS += -g
;CONFIG += debug warn_on
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
PKGCONFIG += xmms2-client xmms2-client-cpp

;CONFIG += avahi

