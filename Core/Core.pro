TEMPLATE = lib
TARGET = emssCore
QT += core \
    gui \
    network \
    opengl
HEADERS += GUI/HeapLoggerView.h \
    GUI/RemoteInterfaceView.h \
    CoreThread.h \
    CoreObject.h \
    Tracker/SingleTracker.h \
    Tracker/AveragedTracker.h \
    Tracker/Tracker.h \
    Tracker/ExpectedMovementTracker.h \
    Tracker/MovementTracker.h \
    Tracker/RawMovementTracker.h \
    Library/HeapLogger.h \
    GUI/RemoteInterfaceView.h \
    Library/Math/gps.h \
    Task/DiscoveryTask2.h \
    CoreFactory.h \
    Map/FadingCollisionMap.h \
    Map/StructureMap.h \
    Task/RoamingTask.h \
    Navigation/Weight/RoamingWeight.h \
    Watchdog/ThreadMonitorWatchdogAction.h \
    Watchdog/NetworkMonitorWatchdogAction.h \
    Library/Audio.h \
    Watchdog/BeepWatchdogAction.h \
    Watchdog/WatchdogAction.h \
    GUI/TextToSpeechView.h \
    Library/Joystick2D.h \
    Task/JoystickNavigationTask.h \
    Navigation/Weight/JoystickWeight.h \
    Watchdog/Watchdog.h \
    Task/MoveTask.h \
    GUI/CameraView.h \
    Map/MapMeta.h \
    GUI/MapOverview.h \
    GUI/DrawCanvasGL.h \
    GUI/DrawCanvas.h \
    GUI/WeightEditor.h \
    Navigation/Weight/WallFollowerWeight.h \
    Navigation/Weight/AccelerationFilterWeight.h \
    GUI/RobotSpeedGraph.h \
    Navigation/Weight/CollisionAvoidanceWeight.h \
    Navigation/Weight/ControllerSpeedWeight.h \
    Navigation/Weight/OrientationWeight.h \
    Task/NavigationTask.h \
    Navigation/Weight/FullSpeedWeight.h \
    Navigation/Weight/Weight.h \
    Navigation/SystemOfWeightsNavigation.h \
    Navigation/SplineNavigation.h \
    GUI/RobotControlPanel.h \
    Object/MarkerObject.h \
    COIL/EmulatedCOIL.h \
    Task/FingerprintNavigationTask.h \
    RemoteInterface/RemoteInterface.h \
    RemoteInterface/RemoteInterfaceMessage.h \
    RemoteInterface/RemoteInterfaceSender.h \
    RemoteInterface/RemoteInterfaceListener.h \
    Library/Math/spline1.h \
    Task/SplineNavigationTask.h \
    Map/TerrainMap.h \
    Map/ColorMap.h \
    Library/TerrainPoint.h \
    GUI/SettingsEditor.h \
    Object/NavigationObject.h \
    Navigation/Navigation.h \
    Library/Debug.h \
    Library/Util.h \
    Core.h \
    Library/Math.h \
    Task/TestMoveTask.h \
    GUI/OptionsDialog.h \
    Library/Math/math-ext.h \
    Library/Math/trafo2.h \
    Library/Math/vector2.h \
    Task/DiscoveryTask.h \
    Task/StraightPathMoveTask.h \
    Task/TaskManager.h \
    Library/SleeperThread.h \
    GUI/TaskEditor.h \
    Task/TaskList.h \
    Object/NavPathObject.h \
    Task/Task.h \
    Task/WallFollowerTask.h \
    GUI/Joystick.h \
    GUI/Viewport.h \
    Controller/EmssController.h \
    COIL/COIL.h \
    Controller/Controller.h \
    Object/Object.h \
    Object/RobotObject.h \
    Map/GridMap.h \
    Map/HeatMap.h \
    Map/Map.h \
    Map/ObjectMap.h \
    Map/PhysicalMap.h
SOURCES += GUI/HeapLoggerView.cpp \
    Library/HeapLogger.cpp \
    GUI/RemoteInterfaceView.cpp \
    Library/Math/gps.cpp \
    Task/DiscoveryTask2.cpp \
    CoreFactory.cpp \
    Tracker/SingleTracker.cpp \
    Tracker/AveragedTracker.cpp \
    Tracker/Tracker.cpp \
    Tracker/ExpectedMovementTracker.cpp \
    Tracker/MovementTracker.cpp \
    Tracker/RawMovementTracker.cpp \
    Map/FadingCollisionMap.cpp \
    Map/StructureMap.cpp \
    Task/RoamingTask.cpp \
    Library/Audio.cpp \
    GUI/TextToSpeechView.cpp \
    Watchdog/Watchdog.cpp \
    Task/JoystickNavigationTask.cpp \
    Task/MoveTask.cpp \
    GUI/CameraView.cpp \
    GUI/MapOverview.cpp \
    GUI/WeightEditor.cpp \
    GUI/RobotSpeedGraph.cpp \
    Task/NavigationTask.cpp \
    Navigation/SystemOfWeightsNavigation.cpp \
    Navigation/SplineNavigation.cpp \
    GUI/RobotControlPanel.cpp \
    Object/MarkerObject.cpp \
    COIL/EmulatedCOIL.cpp \
    Task/FingerprintNavigationTask.cpp \
    RemoteInterface/RemoteInterface.cpp \
    RemoteInterface/RemoteInterfaceMessage.cpp \
    RemoteInterface/RemoteInterfaceSender.cpp \
    RemoteInterface/RemoteInterfaceListener.cpp \
    Task/SplineNavigationTask.cpp \
    Map/TerrainMap.cpp \
    Map/ColorMap.cpp \
    GUI/SettingsEditor.cpp \
    Object/NavigationObject.cpp \
    Navigation/Navigation.cpp \
    Library/Debug.cpp \
    Core.cpp \
    Task/TestMoveTask.cpp \
    Task/WallFollowerTask.cpp \
    GUI/OptionsDialog.cpp \
    Task/DiscoveryTask.cpp \
    Task/StraightPathMoveTask.cpp \
    Task/TaskManager.cpp \
    GUI/TaskEditor.cpp \
    Task/TaskList.cpp \
    Task/Task.cpp \
    Object/NavPathObject.cpp \
    GUI/Joystick.cpp \
    GUI/Viewport.cpp \
    Controller/Controller.cpp \
    Controller/EmssController.cpp \
    COIL/COIL.cpp \
    Object/Object.cpp \
    Object/RobotObject.cpp \
    Map/GridMap.cpp \
    Map/HeatMap.cpp \
    Map/Map.cpp \
    Map/ObjectMap.cpp \
    Map/PhysicalMap.cpp
HEADERS += COIL/qextserialport/qextserialbase.h \
    COIL/qextserialport/qextserialport.h
SOURCES += COIL/qextserialport/qextserialbase.cpp \
    COIL/qextserialport/qextserialport.cpp
unix:VERSION = 1.0.0
unix:HEADERS += COIL/qextserialport/posix_qextserialport.h
unix:SOURCES += COIL/qextserialport/posix_qextserialport.cpp
unix:DEFINES += _TTY_POSIX_
win32:HEADERS += COIL/qextserialport/win_qextserialport.h
win32:SOURCES += COIL/qextserialport/win_qextserialport.cpp
win32:DEFINES += _TTY_WIN_
FORMS += 
RESOURCES += 
DESTDIR = ../bin
LIBPATH = ../bin
unix:CONFIG += staticlib
win32:CONFIG += staticlib
DEFINES += ENABLE_HEAP_LOGGING
