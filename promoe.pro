TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = lib src

include (config.pri)

isEmpty(PREFIX): PREFIX = /usr/local
isEmpty(BINDIR): BINDIR = $$PREFIX/bin

message ( "Install path set to:" $$PREFIX )

binarys.path = $$BINDIR
binarys.files = promoe
INSTALLS += binarys
