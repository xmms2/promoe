DEFINES += PROMOE_VERSION="\"\\\"0.1-dev\\\"\""

isEmpty(PREFIX): PREFIX = /usr/local
isEmpty(BINDIR): BINDIR = $$PREFIX/bin
isEmpty(DATADIR): DATADIR = $$PREFIX/share/promoe

DEFINES += DATADIR="\"\\\"$$DATADIR\\\"\""

DEPENDPATH += $PWD/src $PWD/lib
INCLUDEPATH += $$PWD/src $$PWD/lib
