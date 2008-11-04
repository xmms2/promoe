TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = lib src

include (config.pri)

message ( "Install path set to:" $$PREFIX )

binary.path = $$BINDIR
binary.files = promoe

INSTALLS += binary
