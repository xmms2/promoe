include($$PWD/widgets/widgets.pri)
include($$PWD/mainwindow/mainwindow.pri)
include($$PWD/playlist/playlist.pri)
include($$PWD/equalizer/equalizer.pri)
include($$PWD/dialogs/dialogs.pri)

HEADERS += Skin.h \
           XMMSHandler.h \
           SkinChooser.h \
           settingsdialog.h \
           basewindow.h \
           FileDialog.h \
           BrowseModel.h \
           BrowseDialog.h

SOURCES += main.cpp \
           Skin.cpp \
           XMMSHandler.cpp \
           SkinChooser.cpp \
           settingsdialog.cpp \
           basewindow.cpp \
           FileDialog.cpp \
           BrowseModel.cpp \
           BrowseDialog.cpp

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
