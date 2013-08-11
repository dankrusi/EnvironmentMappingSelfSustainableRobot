TEMPLATE = app
TARGET = emssTerrainMap
QT += core
HEADERS += TerrainPoint.h \
    Map.h \
    TerrainMap.h
SOURCES += Map.cpp \
    TerrainMap.cpp \
    main.cpp
FORMS += 
RESOURCES += 
DESTDIR = ../bin
win32:RC_FILE = ../resources/Win32.rc
