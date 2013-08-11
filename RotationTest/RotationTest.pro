TEMPLATE = app
TARGET = emssRotationTest
QT += core \
    gui
HEADERS += Graph.h \
    RotationTest.h
SOURCES += Graph.cpp \
    RotationTest.cpp \
    main.cpp
FORMS += 
RESOURCES += 
DESTDIR = ../bin
LIBPATH = ../bin
win32:RC_FILE = ../resources/Win32.rc
