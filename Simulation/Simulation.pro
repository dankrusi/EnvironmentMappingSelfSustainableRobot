TEMPLATE = app
TARGET = emssSimulation
QT += core \
    gui \
    opengl
HEADERS += src/sim/PersonController.h \
    src/sim/Controller.h \
    src/sim/Simulation.h \
    src/object/PersonObject.h \
    src/core/Core.h \
    src/object/HeatMapObject.h \
    src/map/HeatMap.h \
    src/object/RobotObject.h \
    src/map/GridMap.h \
    src/object/Object.h \
    src/gui/Viewport.h \
    src/gui/MainWindow.h \
    src/map/Map.h \
    src/map/PhysicalMap.h
SOURCES += src/map/Map.cpp \
    src/object/Object.cpp \
    src/sim/PersonController.cpp \
    src/sim/Simulation.cpp \
    src/object/PersonObject.cpp \
    src/object/HeatMapObject.cpp \
    src/map/HeatMap.cpp \
    src/object/RobotObject.cpp \
    src/map/GridMap.cpp \
    src/map/PhysicalMap.cpp \
    src/gui/Viewport.cpp \
    src/gui/MainWindow.cpp \
    main.cpp
FORMS += src/gui/MainWindow.ui
RESOURCES += images.qrc
LIBS += 
DESTDIR = ../bin
LIBPATH = ../bin
win32:RC_FILE = ../resources/Win32.rc
