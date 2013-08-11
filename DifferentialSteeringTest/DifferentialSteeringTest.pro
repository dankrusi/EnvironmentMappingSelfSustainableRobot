TEMPLATE = app
TARGET = emssDifferentialSteeringTest
QT += core
HEADERS += diffsteer2.h \
    math-ext.h \
    vector2.h \
    DifferentialSteeringTest.h \
    Graph.h
SOURCES += DifferentialSteeringTest.cpp \
    main.cpp \
    Graph.cpp
FORMS += 
RESOURCES += 
DESTDIR = ../bin
win32:RC_FILE = ../resources/Win32.rc
