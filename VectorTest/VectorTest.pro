TEMPLATE = app
TARGET = emssVectorTest
QT += core \
    gui
HEADERS += Graph.h \
    VectorTest.h \
    vector2.h \
    trafo2.h \
    math-ext.h
SOURCES += Graph.cpp \
    VectorTest.cpp \
    main.cpp
FORMS += 
RESOURCES += 
DESTDIR = ../bin
LIBPATH = ../bin
win32:RC_FILE = ../resources/Win32.rc
