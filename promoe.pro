SOURCES += XmmsQT4.cpp PixWidget.cpp Skin.cpp MainWindow.cpp Display.cpp MainDisplay.cpp TitleBar.cpp Button.cpp TextBar.cpp
HEADERS += XmmsQT4.h PixWidget.h Skin.h MainWindow.h Display.h MainDisplay.h TitleBar.h Button.h TextBar.h
CONFIG += link_pkgconfig
CONFIG += debug
QMAKE_CFLAGS_WARN_OFF += -Wno-unused-parameter
PKGCONFIG += xmms2-client
