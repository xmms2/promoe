DEFINES += PROMOE_VERSION="\"\\\"0.1-rc1\\\"\""

isEmpty(PREFIX): PREFIX = /usr/local
isEmpty(BINDIR): BINDIR = $$PREFIX/bin
isEmpty(DATADIR): DATADIR = $$PREFIX/share/promoe
isEmpty(MANDIR): MANDIR = $$PREFIX/share/man

DEFINES += DATADIR="\"\\\"$$DATADIR\\\"\""

DEPENDPATH += $PWD/src $PWD/lib
INCLUDEPATH += $$PWD/src $$PWD/lib
