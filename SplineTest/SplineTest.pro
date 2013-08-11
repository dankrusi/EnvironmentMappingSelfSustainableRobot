TEMPLATE = app
TARGET = emssSplineTest
QT += core
HEADERS += SplineTest.h \
	Graph.h \ 
	spline1.h \ 
	math-ext.h \ 
	vector2.h
SOURCES += SplineTest.cpp \
    main.cpp \
    Graph.cpp
FORMS += 
RESOURCES += 
DESTDIR = ../bin
win32:RC_FILE = ../resources/Win32.rc

