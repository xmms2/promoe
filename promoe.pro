TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = lib src

include (config.pri)

message ( "Install prefix set to:" $$PREFIX )

binary.path = $$BINDIR
binary.files = promoe

INSTALLS += binary

unix: {
manpages1.path = $$MANDIR/man1
manpages1.files = promoe.1
INSTALLS += manpages1
desktop.path = $$PREFIX/share/applications
desktop.files = promoe.desktop
INSTALLS += desktop
}

