include($$PWD/playlist/playlist.pri)
include($$PWD/equalizer/equalizer.pri)

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
           SkinChooser.h \
           SettingsWindow.h \
           VolumeSlider.h \
           ClutterBar.h \
           FileDialog.h \
           BrowseModel.h \
           BrowseDialog.h

SOURCES += main.cpp \
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
           PosBar.cpp \
           PlayStatus.cpp \
           ShadedDisplay.cpp \
           SkinChooser.cpp \
           SettingsWindow.cpp \
           VolumeSlider.cpp \
           ClutterBar.cpp \
           FileDialog.cpp \
           BrowseModel.cpp \
           BrowseDialog.cpp

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
