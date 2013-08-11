TEMPLATE = app
TARGET = emssInterface
QT += core \
    gui \
    network \
    opengl
HEADERS += Interface.h
SOURCES += main.cpp \
    Interface.cpp
FORMS += 
RESOURCES += 
LIBS += -lemssCore
win32:DEFINES          += _TTY_WIN_
unix:DEFINES           += _TTY_POSIX_
DESTDIR = ../bin
LIBPATH = ../bin
win32:RC_FILE = ../resources/Win32.rc
unix:LIBS += -lasound
