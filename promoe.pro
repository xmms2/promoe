TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = lib src

include (config.pri)

target.path = $$BINDIR
target.files = promoe
INSTALLS += target
