TEMPLATE = app
TARGET = emssSensorTest
QT += core \
    gui
CONFIG += console
HEADERS += SensorTest.h
SOURCES += SensorTest.cpp \
    main.cpp
FORMS += 
RESOURCES += 
LIBS += -lemssCore
win32:DEFINES          += _TTY_WIN_
unix:DEFINES           += _TTY_POSIX_
DESTDIR = ../bin
LIBPATH = ../bin
win32:RC_FILE = ../resources/Win32.rc
