TEMPLATE = app
TARGET = emssRemoteInterfaceTest 
QT        += core gui network 

HEADERS   += RemoteInterfaceTest.h
SOURCES   += main.cpp \
    RemoteInterfaceTest.cpp
FORMS     += RemoteInterfaceTest.ui    
RESOURCES +=
LIBS += -lemssCore
win32:DEFINES          += _TTY_WIN_
unix:DEFINES           += _TTY_POSIX_
DESTDIR = ../bin
LIBPATH = ../bin
win32:RC_FILE = ../resources/Win32.rc
unix:LIBS += -lasound