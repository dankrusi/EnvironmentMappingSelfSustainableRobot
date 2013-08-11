TEMPLATE = app
TARGET = emssMedialAxisTest
QT += core
HEADERS += Voronoi.h \
    BinaryPriorityQueue.h \
    MedialAxisTest.h \
    Graph.h
SOURCES += MedialAxisTest.cpp \
    main.cpp \
    Graph.cpp
FORMS += 
RESOURCES += 
DESTDIR = ../bin
LIBPATH = ../bin
win32:RC_FILE = ../resources/Win32.rc
