# Environment Mapping Self-Sustainable Robot #

![Robots](http://imageshack.us/a/img832/184/nbr4.png)

## Summary ##

EMSS is a C++ framework designed to support iRobot Create developers with a comprehensive set of Controllers, Tasks, Schedulers, Movement Trackers, et cetera. It allows interfacing with hardware or in an emulated environment and compiles on Linux, OS X, and Windows.

## Overview ##

The emss iRobot Create Framework is a collection of interacting modules written in C++ designed to support iRobot Create developers with a comprehensive set of Controllers, Tasks, Schedulers, Movement Trackers, et cetera. The framework provides not only an interface with the iRobot Create hardware, but also a completely emulated interface which mimics the hardware in every sense. Along with core components for controlling the robot, the framework also features a set of GUI widgets for drawing maps and navigational paths as well as joysticks, control panels and more. The emss framework compiles on Linux, OS X, and Windows.

The emss iRobot Create Framework is a great platform for anyone wishing to program the iRobot Create hardware. Written in C++ and utilizing the Qt framework, emss compiles on Linux, OS X, and Windows. It is designed for 'high-power' robots which feature some sort of x86 computer as a controller, such as a top-mounted laptop. With almost a year of development at the University of Applied Sciences in Rapperswil, the framework is now ready to be shared with the community. One of the most attractive features of the framework is the complete emulation of the iRobot Create hardware - a feature very valuable when writing and testing different algorithms. The framework is split into different modules which allow different implementations to be loaded. This design proves to be flexible for different project needs and remains as a useful source base.
Open Source

The emss project is licensed under GPLv3, which in short features the following:

* the freedom to use the software for any purpose,
* the freedom to change the software to suit your needs,
* the freedom to share the software with your friends and neighbors, and
* the freedom to share the changes you make. 

The entire source is available on our SourceForge project page. Anyone is welcome to make contributions in forms of code, suggestions, and bug reports, et cetera.

## Featured Videos ##

* [Roaming around the Hallway via Remote Interface ](http://www.youtube.com/watch?feature=player_embedded&v=5JqIuSvLvtQ)
* [Two Robots Interacting](http://www.youtube.com/watch?feature=player_embedded&v=M9OxHh6v-nY)
* [Driving Along Navigation Points](http://www.youtube.com/watch?feature=player_embedded&v=9n0CWv-kvOo)

## Screenshots ##

![Screenshot1](http://emssframework.sourceforge.net/images/thumb/f/fd/EmssInterfaceFullScreen2_v1.2.png/620px-EmssInterfaceFullScreen2_v1.2.png)

## Projects Using EMSS Framework ##

### EMSS Robot - Environment Mapping Self-Sustainable Robot ###

* What: Environment mapping self-sustainable robot with advanced navigation and remote control
* Where: University of Applied Sciences Rapperswil, Switzerland
* Access: emssrobot.dyndns.org
* [Project Page](http://emssframework.sourceforge.net/emssframework_Projects_-_emss)

### WPS Robot - Wireless Positioning System Robot ###

* What: Indoor positioning system using 802.11x supported by emss framework
* Where: University of Applied Sciences Rapperswil, Switzerland
* Access: wpsrobot.dyndns.org
* [Project Page](http://emssframework.sourceforge.net/emssframework_Projects_-_wps)

## Download ##

### Version 1.3 ###

* [emssComplete-1.3b-osx-x86.zip](http://sourceforge.net/projects/emssframework/files/emss%20Complete/1.3/emssComplete-1.3b-osx-x86.zip/download)
* [emssComplete-1.3-win-x86.zip](http://sourceforge.net/projects/emssframework/files/emss%20Complete/1.3/emssComplete-1.3-win-x86.zip/download)
* [emssComplete-1.3-linux-x86.tar.gz](http://sourceforge.net/projects/emssframework/files/emss%20Complete/1.3/emssComplete-1.3-linux-x86.tar.gz/download)
* [emssComplete-1.3-source.tar.gz](http://sourceforge.net/projects/emssframework/files/emss%20Complete/1.3/emssComplete-1.3-source.tar.gz/download)
* [emssComplete-1.3-source.zip](http://sourceforge.net/projects/emssframework/files/emss%20Complete/1.3/emssComplete-1.3-source.zip/download)

### Documention PDFs ###

* [Version 1](https://github.com/dankrusi/EnvironmentMappingSelfSustainableRobot/raw/master/Documentation/EnvironmentMappingSelfSustainable_RobotFramework_Documentation_v1.pdf)
* [Version 2](https://github.com/dankrusi/EnvironmentMappingSelfSustainableRobot/raw/master/Documentation/EnvironmentMappingSelfSustainable_RobotFramework_Documentation_v1.pdf)

## Contact ##

* Dan Krusi <dan.krusi@nerves.ch>

## Documentation ##

The software related problems of emss can be categorized into three components: communicating with hardware, solving the problems of objectives, and presenting the state of the software to the user. While these categories can be logically stacked on-top of each other, starting with hardware communication, the implementation process of our software fashioned a mix of all three, gradually improving on each category.

In software engineering, the traditional school of thought practices a one-way, three-tier, model consisting of a data tier, application tier, and presentation tier. While we adhere to these software principals, we have deliberately chosen not to religiously follow them. In the field of robotics it is much more profitable to focus on the componentization of different modules which naturally serve different tasks. This was aware to us from the start of the project, and in turn we largely profited from creating a framework of “hot-swappable” components.

Furthermore, it is worthwhile noting that the current software framework described here has undergone an evolutionary process. Before the start of the implementation process, the design phase provided us with an original structure based on our requirements. As requirements changed due to the research nature of the project, and certain parts of the structure proved no longer suitable, we scratched those design choices and made new ones. Changing our original software structure has cost us many long nights, but ultimately improved the overall design. All said and done, we would like to acknowledge that despite our enthusiasm to create a sound software design and structure, there are without doubt areas for improvement. The heavy mix of theory and implementation has made us jump through some high hoops. 

### Software Environment ###

Because of the natural distancing progression of software languages and operating systems, the choice and understanding of the environment of software has become increasingly important. Working with microcontrollers in the field of robotics, we chose the programming language of C++, which is well proven in the industry as a robust middle level language 20. Because C++ by itself does not offer any support in creating cross-platform graphical user interfaces, we use the Qt framework (version 4.4) from Trolltech to assist us. We are very proud to be able to offer a software framework in which every component, including components such as serial communications, cross-compile on Windows, Linux, and OS X. All software components are written in C++ and are fully compatible with any of the GNU 3.0 compilers. Additionally, we have made use of binary libraries. The core components of the emss software are packaged into a library, which in turn is linked to by any of the user interfaces. On Windows, the Core library is dynamically linked, while on Unix it is statically linked. 

### Software Model ###

The software which essentially runs our emss robot is divided into different components which ultimately make up the emss Core, described in detail in Section 4.3. Interfacing applications, which create an instance of the Core and provide the necessary connections for user interaction, have been omitted from our model as they are irrelevant to the functionality of the framework. 

![SoftwareModel](http://emssframework.sourceforge.net/images/thumb/c/cf/EmssSoftwareModelDomain_v1.0.png/620px-EmssSoftwareModelDomain_v1.0.png)

Figure 4-1 represents the final emss Core class structure. The Core class contains all the components, namely Navigation, Map, Movement Tracker, Controller, Task Manager, and COIL, and is responsible for instantiating them correctly in memory as well as de-allocating them. There are two running threads in the Core: Controller and Task Manager. The Task Manager is responsible for scheduling and executing Tasks, while the Controller is responsible for consequently communicating with the emss hardware. The design of separating Task execution and Controller execution produces a more robust controller, allowing certain logics such as cliff monitoring and emergency stops to be executed in a guaranteed fashion, even if a Task locks up and is no longer responding.

Our design has high transparency, allowing every component to “see” every other component, with one exception of COIL, which must only be accessed by the Controller. There are several lines of communication in the form of slots and signals21, the most important between the Controller and Movement Tracker, and between a Task and the Controller. In our design, all forms of action other than communication with hardware are realized through Tasks. While the Task Manager allows more than one instance of Task, only a single Task can be executed at once – part of the design to prevent multiple influences on the hardware communication interface. The Map and Movement Tracker components make up our data tier, assisting tasks in their job. Neatly packaging a set of functionally, the Navigation class also assists Tasks by providing navigational data structures such as splines and navigation points (way points).

The model in Figure 4-2 shows the basic lines of communication in the emss Core. When a Task is created, and eventually executed, it uses the different Maps and the Movement Tracker as its data source. The Movement Tracker holds the current position and orientation of the robot, while the Maps contain information about the environment, such as obstacles or previous collisions. The Task Manager is responsible for executing the Tasks one by one. The Controller receives the control commands from the Tasks and forwards them to COIL. COIL produces a data packet and sends the command over a serial port to the iRobot Create controller. The robot interprets the commands and executes them directly on the hardware. Per request of the Controller, COIL reads sensor data from the iRobot Create controller, translates them, and forwards them back to the Controller. The Controller in turn interprets the sensor data and passes them along to the Movement Tracker and appropriate Maps. For further understanding how the Core components work with each other, we present two scenarios in Section 4.2.1 and 4.2.2. 

![Model2](http://emssframework.sourceforge.net/images/thumb/3/35/EmssBasicLinesOfCommunication_v1.0.png/620px-EmssBasicLinesOfCommunication_v1.0.png)

The model in Figure 4-2 shows the basic lines of communication in the emss Core. When a Task is created, and eventually executed, it uses the different Maps and the Movement Tracker as its data source. The Movement Tracker holds the current position and orientation of the robot, while the Maps contain information about the environment, such as obstacles or previous collisions. The Task Manager is responsible for executing the Tasks one by one. The Controller receives the control commands from the Tasks and forwards them to COIL. COIL produces a data packet and sends the command over a serial port to the iRobot Create controller. The robot interprets the commands and executes them directly on the hardware. Per request of the Controller, COIL reads sensor data from the iRobot Create controller, translates them, and forwards them back to the Controller. The Controller in turn interprets the sensor data and passes them along to the Movement Tracker and appropriate Maps. For further understanding how the Core components work with each other, we present two scenarios in Section 4.2.1 and 4.2.2. 

#### Fluid Drive Controller Scenario ####

![Model3](http://emssframework.sourceforge.net/images/thumb/c/cc/EmssFluidDriveControllerScenario_v1.0.png/620px-EmssFluidDriveControllerScenario_v1.0.png)

The Fluid Drive Controller is a simple Core Controller which can control the robots movement by joystick. Its details can be found in Section 4.3.3.2. The emss Interface includes a joystick widget which sends the position of the x-yoke and the y-yoke upon change to the Fluid Drive Controller. In turn the Controller continuously calculates the left and right wheel speeds, based on the yoke positions, and forwards them to COIL, ultimately sending the movement commands to the iRobot Create controller. 

#### Emss Controller Scenario ####

![Model4](http://emssframework.sourceforge.net/images/thumb/8/88/EmssEmssControllerScenario_v1.0.png/620px-EmssEmssControllerScenario_v1.0.png)

The Emss Controller scenario, visualized in Figure 4-4, uses the Navigation component and is slightly more complex. First an actor must set navigation points where the robot should pass through. This could come from either the graphical user interface or another Task. When these points are added, the Navigation component stores them and automatically creates the correlating splines for each wheel. 

Furthermore, a Spline Navigation Task, which is executed by the Task Manager, will extract the needed wheel speeds from the Navigation splines and forward them to the Controller. Finally, the Emss Controller may “choose” to send the movement commands through COIL to the robot hardware, given that the action is deemed safe, et cetera. 

### Core ###

Any application which interfaces with emss must initialize the Core object. The allocation and initialization of the Core class just allocates the data tier, in turn creating an empty environment and unknown state of the world, disconnected from the hardware. To use the emss Core and communicate with hardware, the application must call connect(controller,tracker,port) with the desired parameters on the Core object, which in turn creates all the connections between software and hardware, as well as lines of communications between the appropriate components. When finished, the application is responsible for calling disconnect() and deleting the Core object. 

```
// Initialze Core and connect on COM4 using Block Drive Controller and Raw
// Movement Tracker
Core *core = new Core();
core->connect(“Block Drive”, “Raw”, “COM4”, false);
// Core is now connected and ready to be used...
core->disconnect();
delete core;
```

#### Library ####

The Library package is not considered a Core component, but however is an important part of the emss Core as it provides a substantial amount of functionality. It is a collection of classes which are used by many different components, such as math structures or debugging routines. Some trivial Library classes such as SleeperThread, Util, and TerrainPoint are not discussed in further detail. 

##### Vector2T #####

Vector2T is an abstraction of a two-dimensional vector. It realizes most of the mathematical operations operable on vectors, such as addition, scalar multiplication, dot products, et cetera. Vector2T is a template class supporting different internal data types such as doubles and floats, and has three defined typedefs of Vector2D (double), Vector2F (float), and Vector2L (long double). 

```
  // Basic vector operations and computations
  Vector2D v1(0.5,0.8);
  Vector2D v2(4.0,2.2);
  Vector2D v3 = v1 + (v2*2);
  double distance = Vector2D::dist(v1,v2);
  double angle = Vector2D::angle(v1,v2);
```

##### Trafo2T #####

Two-dimensional transformation matrices such as rotation matrices can be realized using the Trafo2T template class. Just as the Vector2T class, mathematical operations on transformation matrices have been implemented. There are three defined typedefs of Trafo2D (double), Trafo2F (float), and Trafo2L (long double). Multiple transformations can be appended by using the * operator.

```
  // Basic rotation and translation
  Trafo2D t1 = Trafo2D::trans(6.0,5.0) * Trafo2D::rot(Rad(45));
  Trafo2D t2 = Trafo2D::trans(1.0,1.0);
  Trafo2D t3 = t2 * t1;
```

The original Trafo2T class was implemented by our advisor Prof. Dr. Joachim Wirth.

##### Spline1T ####

This template class implements different one-dimensional spline approaches. As discussed in section 3.4, splines are smooth and efficient curve interpolations of a finite set of nodes. The different kernels we implemented include SplineKernalBruenner, SplineKernelNRNatural, and SplineKernelStoer. Nodes can be freely added and removed from the spline. When a call to getValue(node,t) is made, Spline1T automatically decides whether or not an internal recalculation of the spline coefficients is needed, maximizing the ease of use to the user. Each kernel has its own implementation of getValue(node,t), getFirstDerivative(node,t), and getSecondDerivative(node,t). Two dimensional curves can easily be realized by using two instances of Spline1T, as mentioned in Section 3.3. There are three defined typedefs of Spline1D (double), Spline1F (float), and Spline1L (long double).

```
  // Create two splines, one for each dimension and add some nodes
  Spline1D Sx = Spline1D(SplineKernelNRNatural);
  Spline1D Sy = Spline1D(SplineKernelNRNatural);
  Sx.addNode(0.0); Sy.addNode(0.0);
  Sx.addNode(1.4); Sy.addNode(1.1);
  Sx.addNode(3.1); Sy.addNode(2.0);
  Sx.addNode(6.7); Sy.addNode(5.4);
  Sx.addNode(8.0); Sy.addNode(1.1);
  // Draw each spline segment with fixed number of dots
  int DOTS_PER_SEGMENT = 20;
  for (int n = 0; n < Sx.getNodeCount() - n; n++) {
         for (int t = 1; t < DOTS_PER_SEGMENT; t++) {
                 double px = Sx.getValue(n, (double)t / (double)DOTS_PER_SEGMENT);
                 double py = Sy.getValue(n, (double)t / (double)DOTS_PER_SEGMENT);
                 graph.drawPoint(px, py);
         }
  }
```

##### Debug #####

The Debug class offers some debugging assistance, especially during runtime, by encapsulating common console commands. Printing color-formatted sets of values along with an automatic timestamp can be very easily achieved. Additionally, calling setOutput(widget) allows an application to re-route all debugging information to any given text widget.

```
  // Re-route debugging information to our widget
  Debug::setOutput(consoleWidget);
  // Simple debug messages
  Debug::print(“The shorthand value of PI is %1, while e is %2”, 3.14, 2.72);
  Debug::warning(“This is a warning”);
```

#### COIL ####

The Create Open Interface Library22, or COIL, was originally implemented by Jesse DeGuire and Nathan Sprague as a POSIX compliant C wrapper for the iRobot Open Interface. Together with Stefan Sander, Michal Policht, and Brandon Fosdick’s QextSerialPort23 we created a C++ version of COIL which compiles on Windows, Linux, and OS X. COIL opens a serial port and directly communicates with the iRobot Open Interface. All the functions of the interface have been ported in our version, with some additional useful functions. 

```
  // Allocate and start COIL
  coil = new COIL("COM4");
  coil->startOI();
  coil->enterFullMode();
  // Drive forward one meter, checking for collision at intervals
  coil->wheelDrive(100,100); // 100 mm/s
  coil->waitTime(10000); // 10 seconds
  for(int i = 0; i < 10; i++) {
        if(coil->getBumpsAndWheelDrops() != 0) break;
        coil->wheelDrive(100, 100); // 100 mm/s
        coil->waitTime(100); // 0.1 seconds
  }
  coil->wheelDrive(0,0); // stop moving
  // Turn around
  coil->turn(180);
  // Shutdown and cleanup
  coil->stopOI();
  delete COIL;
```

#### Controller ####

The abstract Controller class is the base class of fully functional Controllers. It provides the virtual methods run() and emergencyStop() which must be implemented by the child classes, and inherits from QThread, a cross-platform execution fork. The process routine of a Core Controller is the run() method and must only exit if the variable stopRequested is true. Currently, there are three different Controller implementations: Block Drive Controller, Fluid Drive Controller, and Emss Controller. Only the later is meant for real use, the others are for testing and diagnostic purposes only. In addition to sending movement commands to COIL, the Controller is also responsible for passing along sensor data to other components such as the Movement Tracker. This is realized in the form of Qt signals.

##### Block Drive Controller #####

The exact purpose of the Block Drive Controller is encoded in its name. It provides a function for moving forward, moving backward, turning right, and turning left with a predefined speed, angle, and distance. The run() method of the Controller thread executes the set action and sends the corresponding commands to COIL. The Block Drive Controller also offers an option for a slow start. Slow start can be set to linear or square. This means that the speed increases to its predefined value in linear or square fashion. This is attached for more exactness of driven distance. The disadvantage of this Controller is its blocking nature. This means that calling move(speed,distance) with speed set to 100 mm/s and distance set to 1000 mm will make the calling thread block for one second.

##### Fluid Drive Controller #####

The Fluid Drive Controller is a further development of the Block Drive Controller. It supports control of the robot with a two-dimensional GUI joystick. The Controller has two yokes, yokeX and yokeY which accept values between -1 and 0. The speed of the robot is determined by the yokeY value, while the direction of the robot’s movement is set by the yokeX value. Precisely put, the wheel speeds are given by

![FluidDrive](http://emssframework.sourceforge.net/images/b/bf/EmssFluidDriveControllerFormula_v1.0.png)

The disadvantage of this controller is that rotations around the center axis of the robot are not supported due to the nature of the joystick. For a visual representation of how the Fluid Drive Controller works, please see Figure 4-3.

##### Emss Controller #####

This Controller is the most important Controller in our project. Its main function is to receive the differential steering commands and to pass along sensor data to other Core components. The following code snippets originate from the run() method of the Emss Controller.

```
                 // Get movement...
                 double distanceDelta = coil_getDistance();
                 double angleDelta = coil_getAngle();
                 // Emit signals for movement tracker
                 emit signalMovedDistance(distanceDelta);
                 emit signalChangedAngle(angleDelta);
```

In this code the driven distance and the turned angle are prompted and the result sent as a signal. These signals are intercepted by the active Movement Tracker.

```
                 // Get other sensor data
                 int sharpIRSensor = coil_getAnalogSensorDistance();
                 int bumpsWheelDrop = coil_getBumpsAndWheelDrops();
                 // Collision?
                 if (((COIL::BUMPWHEELDROP_BUMP_LEFT & bumpsWheelDrop) ==
                         COIL::BUMPWHEELDROP_BUMP_LEFT) ||
                         ((COIL::BUMPWHEELDROP_BUMP_RIGHT & bumpsWheelDrop) ==
                                COIL::BUMPWHEELDROP_BUMP_RIGHT)) {
                         emit signalCollision();
                         if(core->boolSetting("EMSSCONTROLLER_EMERGENCY_STOP_ENABLED")
                                == true) emergencyStop();
                 }
                 // Object detected?
                 if (sharpIRSensor <
                 core->intSetting("EMSSCONTROLLER_SHARP_IR_SENSOR_CUTOFF_VALUE")) {
                         emit signalObjectDetected(sharpIRSensor, 0); // Angle is 0
                                                       because it is straight ahead always!
                         if(core->boolSetting("EMSSCONTROLLER_EMERGENCY_STOP_ENABLED")
                                == true && sharpIRSensor <
                 core->intSetting
                         ("EMSSCONTROLLER_SHARP_IR_SENSOR_EMERGENCYSTOP_BUFFER_MM"))
                         emergencyStop();
                 }
```

This is the code to check whether there has been a collision or if an obstacle is detected. The Maps, such as the Heat Map, receive a collision or object signal and register them in their data structures. Depending on the settings, the robot performs an emergency stop if a collision or obstacle is detected.

```
                 // Send wheel speeds to COIL
                if(mode != EmssController::EmergencyStop)
                          coil_directDrive(Lwheel, Rwheel);
                // Sleep our interval...
                this->msleep(interval);
         }
```

These commands pass the steering information of each wheel to COIL and subsequently wait for the next process execution. Currently, certain features from both the Block Drive Controller and Fluid Drive Controller are implemented within the Emss Controller, creating a set of Emss Controller modes such as Idle, Move, Turn, Joystick, WheelDrive, EmergencyStop. However, the only mode not used for diagnostic purposes (i.e. manually driving out of a corner) is the WheelDrive mode. For a visual representation of how the Emss Controller works, please see Figure 4-4 Figure 4-3.

##### Emulated Emss Controller #####

The Emulated Emss Controller has exactly the same functionality as the Emss Controller. The only difference is that COIL commands are not actually forwarded to COIL. These commands are emulated internally within the Controller. This Controller was developed to run the emss Core without a physical robot but still be able to perform navigational tests, algorithms, et cetera. To maximize code reuse and implementation efficiency, the Emulated Emss Controller extends from the Emss Controller, overwriting only the methods needed for emulation. This was achieved by wrapping the COIL commands directDrive(left,right), getDistance(), getAngle(), getAnalogSensorDistance(), and getBumpsAndWheelDrops() in the Emss Controller. Thus, when the Emss Controller wants to call getDistance() on COIL, instead it calls the local method coil_getDistance(). In the Emss Controller this function is defined simply as:

```
  double EmssController::coil_getDistance() {
         return (double) core->coil->getDistance();
  }
```

However, the Emulated Emss Controller overwrites this method, emulating the behavior of the iRobot Create Interface.

```
  double EmulatedEmssController::coil_getDistance() {
                // Get the elapsed time since last call
                double elapsedTimeSinceGetDistance =
                          (double)lastGetDistanceTimestamp.restart();
                // We need to return a value (mm) based on the wheel speed (mm/s) and
                    elapsed time (ms)...
                double wheelSpeed = ((double)emulatedLwheel +
                          (double)emulatedRwheel) / 2.0; // Average it out
                double distance = (wheelSpeed *
                          (elapsedTimeSinceGetDistance / 1000.0));
                return distance;
  }
```

This above code emulates the moved distance of the robot. First it measures the time between now and the last call of this method. The average between the left wheel speed and the right wheel speed is calculated and the result is the multiplication of the average and elapsed time. Emulating the changed angle is not as trivial, as the iRobot Create Interface does not disclose how this is done. We have implemented two different methods for emulated the changed angle. The first is based on the linear motion of each wheel, and becomes increasingly inaccurate as the elapsed time increases. The later is based on the theory discussed in Section 3.6.

```
  double EmulatedEmssController::coil_getAngle() {
         // Get the elapsed time since last call
         double elapsedTimeSinceGetAngle = (double)lastGetAngleTimestamp.restart();
         if(calculateAngleBasedOnRotationalSpeed == true) {
                  // Calculate the change of angle based on the rotational wheel speed
                  // ((distance / 2 PI r) * 360)
                  double angle = 0.0;
                  double radius = core->doubleSetting("ROBOT_WHEEL_OFFSET_MM");
                  double distanceL = emulatedLwheel *
                          (elapsedTimeSinceGetAngle / 1000.0);
                  double distanceR = emulatedRwheel *
                          (elapsedTimeSinceGetAngle / 1000.0);
                  double circumference = 2*PI*radius;
                  double angleL = (distanceL / circumference) * 360.0;
                  double angleR = (distanceR / circumference) * 360.0;
                  angle = angleR - angleL;
                  angle *= PI/2;
                  return angle;
         } else {
                  // Calculate change of angle using physics
                  if (emulatedLwheel != emulatedRwheel) {
                          double arclenL = emulatedLwheel *
                                  (elapsedTimeSinceGetAngle / 1000.0);
                          double d = core->doubleSetting("ROBOT_WHEEL_OFFSET_MM") * 2;
                          double radiusL = (emulatedLwheel*d) /
                                  (emulatedRwheel-emulatedLwheel);
                          double angleAlpha = Deg(arclenL / radiusL);
                          double angleBeta = 180.0 - 90.0 - angleAlpha;
                          double angleGamma = 90.0 - angleBeta;
                          return angleGamma;
                  }
                  return 0.0;
         }
  }
```

#### Movement Tracker ####

Responsible for tracking movements of the robot, and in turn performing the localization, a Movement Tracker accepts signals from the active Controller and translates them accordingly. Furthermore, Trackers also forward changes about the robot position to other components, especially the Maps, in forms of signals. The Movement Tracker is an abstract class which needs to be defined by subclasses. The only significant variable is transformation, which is a Trafo2D object. The input slots, i.e. signals received from the Controller, are registerMovedDistance(distance), registerChangedAngle(angle), registerCollision(), and registerObjectDetected(distance,angle). The current emss software offers only one implementation of Movement Tracker, namely the Raw Movement Tracker. However, in the future other, more advanced, Movement Trackers will be implemented in order to increase the localization precision.

##### Raw Movement Tracker #####

The Raw Movement Tracker tracks the robot’s movement by geometrically interpreting the sensor data sent back from the robot. This method remains accurate (at least as accurate as the sensor data) as long as the robot’s movement is linear, meaning that the wheels were not differentially driven. When a differential steering system is made use of, the accuracy of the tracker declines as the sensor interval increases. However, we have found that this implementation of the tracker is surprisingly sufficient, as shown in Figure 4-6.

```
  void RawMovementTracker::registerChangedAngle(double angle){
         if(angle != 0) {
                  transformation = transformation * Trafo2D::rot(Rad(angle));
                  emit moved(this->x(), this->y(), this->rotation());
         }
  }
```

![MovementTracker](http://emssframework.sourceforge.net/images/thumb/6/69/EmssMovementTrackerComposed_v1.0.png/620px-EmssMovementTrackerComposed_v1.0.png)

#### Navigation ####

The Navigation class holds the information of the navigation points, or way points, for the robot. Other data structures, such as splines, are also included in this class. Tasks who wish to navigate from point to point or via spline use the Navigation component as an abstraction to do so. The most important methods are calculateLeftRightWheelSpline() and getWheelSpeed(tick,interval) which have been discussed in detail in Section 3.5.1.

```
  void Navigation::calculateLeftRightWheelSpline() {
        double wheelNodeDistanceBetweenNode = targetSpeed *
                ((double) interval / 1000.0);
        // Init and drop nodes
         wheelLeftSplineX->clearNodes();
         wheelLeftSplineY->clearNodes();
         wheelRightSplineX->clearNodes();
         wheelRightSplineY->clearNodes();
         // Calculate new nodes for wheel splines
         for (int navNode = 0; navNode < navSplineX->getNodeCount() - 1; navNode++) {
                 // Get the distance between the two nav nodes
                 double distanceBetweenNodes =
                        getDistanceBetweenNavNodes(navNode, navNode+1);
                 // Create the wheel nodes at equal distance between the two nodes
                 for (int wheelNode = 0; wheelNode <
                  (distanceBetweenNodes / wheelNodeDistanceBetweenNode); wheelNode++) {
                        // Get the t value for the nav node, which is discretisized
                            based on the wheel node distance between nodes
                        double t = (double) wheelNode / (double)
                                (distanceBetweenNodes / wheelNodeDistanceBetweenNode);
                        // Get the node positions for both the left and right wheel
                        // splines
                        Vector2D c(navSplineX->getValue(navNode, t),
                                navSplineY->getValue(navNode, t));
                        Vector2D c1(navSplineX->getFirstDerivative(navNode, t),
                                navSplineY->getFirstDerivative(navNode, t));
                        Vector2D v = c1 / norm(c1);
                        Vector2D n = Vector2D(-v.y(), v.x());
                        Vector2D l = c + (wheelOffset * n);
                        Vector2D r = c - (wheelOffset * n);
                        wheelLeftSplineX->addNode(l.x());
                        wheelLeftSplineY->addNode(l.y());
                        wheelRightSplineX->addNode(r.x());
                        wheelRightSplineY->addNode(r.y());
                 }
         }
  }
```

The getWheelSpeed(tick,interval) method requires two parameters: tick and interval. The tick parameter is straightforward and is used to calculate which node in the wheel splines will be used to extract the current wheel speed. The interval parameter is used to insure that the requesting Task has the same interval as used for creating the wheel splines. However, if the setting NAVIGATION_DYNAMIC_INTERVAL_ENABLED is true, getWheelSpeed(tick,interval) checks to see what the actual interval was between the Tasks request for wheel speeds, and, if this interval has drifted too far from the original interval used to create the wheel splines, they are accordingly recalculated. The actual calculation of the wheel speeds are only the last four lines of the getWheelSpeed(tick,interval) method.

```
  Vector2D Navigation::getWheelSpeed(int tick, int interval) {
         bool rebuildWheelSplines = false;
  int actualInterval = interval;
  // Get the node based on the tick, and the current offset, so that you can
  // start navigation in the middle or so
  int node = tickOffset + tick;
  double t = 0.0;
  //Are we done?
  if (node >= wheelLeftSplineX->getNodeCount() - 1) {
         return SplineNavigationWheelSpeedFinished;
  }
  // Dynamic or fixed interval?
  if(dynamicIntervalEnabled == true) {
         // Dynamic interval - adjust spline for incomming interval but only
         // rebuild if exceeds flexibility
         actualInterval = lastGetWheelSpeedTimestamp.restart();
         // Rebuild spline?
         if(actualInterval > interval + intervalFlexibility ||
                actualInterval < interval - intervalFlexibility) {
                //Debug::print("[Navigation] Interval flexiblity exceeded by
                // %1", interval - actualInterval);
                rebuildWheelSplines = true;
         }
  } else {
         // Fixed interval, nothing to do except warning and return if
         // different
         if (this->interval != interval) {
                return SplineNavigationWheelSpeedFinished;
         }
  }
  // Rebuild wheel splines because of interval delay or node offset?
  if(rebuildWheelSplines == true) {
         interval = actualInterval;
         calculateLeftRightWheelSpline();
  }
  // Calculate speed in mm/s per wheel
  double vl = norm(Vector2D(wheelLeftSplineX->getFirstDerivative(node, t),
         wheelLeftSplineY->getFirstDerivative(node, t)));
  double vr = norm(Vector2D(wheelRightSplineX->getFirstDerivative(node, t),
         wheelRightSplineY->getFirstDerivative(node, t)));
  vl *= 1.0 / (interval / 1000.0);
  vr *= 1.0 / (interval / 1000.0);
  return Vector2D(vl, vr);
}
```

#### Map ####

Maps hold information about the environment. In the emss framework there exist several types of maps, each serving a different purpose. emss Maps must define the abstract class Map, however they enjoy a significant amount of freedom. The only virtual Map method is paint(view,scale), which is used by the Viewports for displaying the map to the user. In addition, any map must provide a width and a height. If a Map does not know its own dimensions, it can consult the emss Core for the basic world boundaries. A certain set of functionality shared by both the Heat Map and Terrain Map have been combined into the Color Map. Self-explanatory Maps, such as the Grid Map and Object Map, will not be discussed in further details.

As discussed in the introduction of Chapter 0, Maps is an example where the data tier and presentation tier have been merged into a single concept. We chose to do this as the presentation, i.e. the visual image of the Map, is typically identical to the data. Of course, the Map paint(view,scale) method could be refactored to a presentation “wrapper” class in a separate package, however we see no benefit of such a bloated design.

##### Heat Map #####

A Heat Map is built to visualize the discovered environment of the robot. When the robot moves through its environment, the Movement Tracker is responsible for translating the sensory data into physical movements through space. In turn, the Movement Tracker sends signals to the appropriate Maps, such as the Heat Map, registering various kinds of “heat”. The emss Heat Map support two kinds of heat: Open Area and Collision Area. Open Area is displayed as green, and portrays the robots path through the environment, while Collision Area is displayed as red, and represents any sort of collision or obstacle, forming the environments boundary. The Heat Map can be consulted by different emss components in order to make decisions, such as navigational choices. In addition, other influences, such as a user, can also “color” the Heat Map for diagnostic reasons such as staking out a territory.

```
   // Robot collided at x,y
   heatMap->registerHeat(ColorMap::CollisionAreaChannel, x, y, heatSpotOpacity,
         heatSpotSize);
                  Code 4-16: Example for Registering a Collision on the Heat Map
   // Look ahead if it is good to move using a sweep across a start and end angle...
   double heat = 0.0;
   int sweepAngle = 30;
   double lookAheadRange = 300; // 30cm
   for(int angle = -sweepAngle/2; angle <= sweepAngle/2; angle++) {
      Trafo2D checkpoint = core->movementTracker->transformation *
         Trafo2D::rot(-Rad(angle)) * Trafo2D::trans(0, lookAheadRange);
      heat += core->heatMap->getChannelValue(HeatMap::CollisionAreaChannel,
         checkpoint.trans().x(), checkpoint.trans().y());
   }
   if(heat == 0.0) {
      // Safe to move...
   }
```

##### Terrain Map #####

Currently, the Terrain Map is only used for displaying the results of the algorithm described in Section 3.7. However, the data structure is now ready to be used by future algorithms. The class provides three main functions: raise(x,y), lower(x,y), and getHeight(). The higher an area of the map is, the brighter white it is colored. The raise(x,y) and lower(x,y) methods are overloaded to allow entire sections of a map to be altered at once.

##### Physical Map #####

The emss Physical map represents the real world environment in its physical state. This Map is currently used for simulation purposes, but in the future can also be used for various Tasks which would like to combine a predefined perception of the environment. The Physical Map offers the method getAreaType(x,y) which returns either Open Area or Collision Area – just like the Heat Map. The Emulated Emss Controller uses this Map to emulate the IR Range Finder during simulations, something which has proven especially useful as the robots environment can now be almost entirely emulated. To define the areas of a physical map, all one has to do is draw a RGBA image, such as a 24-bit PNG, and let the Physical Map load the file. Any area in the image which has the alpha channel not at 255 (full) is regarded as Open Area. 

#### Tasks ####

Core Tasks bind all the components of the emss framework together, enabling the robot to perform actions in its environment. As already mentioned, the Task Manager is responsible for executing the Tasks one by one. Each emss Task must inherit from the class Task and implement the method process(). When a Task is created, its initial status is Waiting. When the Task Manager decides to schedule the Task for execution, its status is set to Running. The process() method of the Task is called by the Task Manager at its given interval (each Task can define its own interval) until the task sets its status as Finished.

##### Task Manager #####

The Task Manager is a separate thread which contains a Task List. The Task List is nothing more than a helper class for sequentially storing and retrieving Tasks, offering useful functions such as getNextSpecificTask(type). As soon as the Task Manager receives a Task, appends it to the Task List and starts to execute it. If the Task Manager receives more than one Task at a time, it appends it to the back of the Task List, where in time will eventually be executed. The Task Manager also allows Tasks to be interrupted. In this case the manager will set the current Task as Interrupted and go to the next Task in the list.

##### Test Move Task #####

Several test movements are provided by the Test Move Task. These test movements are used for diagnostic purposes, such as calibration, accuracy observation, and general research. The following self-explaining test moves are available: Square, Rotate 90, Rotate 360, Triangle, Straight, Circle, and Vector Circle.

##### Straight Path Move Task #####

This Task moves the robot in a linear fashion to the given destination. Multiple Straight Path Move Tasks can be appended to the Task Manager in order to create a series of chunky movements.

##### Spline Navigation Task #####

Spline navigation is very trivial. All it does is drive through all the navigational points contained in the Navigation component in the form of a spline by calling the Navigation methods getWheelSpeed(tick,interval). There is some additional functionality added, such as aligning the robot with the spline before navigation, and resetting the navigation points when finished.

```
  void SplineNavigationTask::process() {
          // Align the robot and then crawl the spline...
          if(tick == 0 &&
            core->boolSetting("SPLINENAVIGATIONTASK_ALIGN_ROBOT_WITH_SPLINE_") == true)
                 ((EmssController*) core->controller)->turn(
                          (int)(- core->movementTracker->rotation() +
                          core->navigation->getAngleForSplineAlignment()),
                          core->intSetting("
                                 SPLINENAVIGATIONTASK_ALIGN_ROBOT_WITH_SPLINE_SPEED"));
          // Send wheel speeds for current tick to controller
          Vector2D wheelSpeed = core->navigation->getWheelSpeed(tick, interval);
          ((EmssController*) core->controller)->wheelDrive((short)wheelSpeed.x(),
                 (short)wheelSpeed.y());
          // Finished?
          if(wheelSpeed == SplineNavigationWheelSpeedFinished) {
                 // Reset?
                 if(core->boolSetting("
                          SPLINENAVIGATIONTASK_RESET_NAV_POINTS_WHEN_DONE") == true)
                          core->navigation->clearNavPoints();
                 else
                          core->navigation->setTickOffset(tick);
                 status = Task::Finished;
          }
          tick++;
  }
```

##### Discovery Task #####

This is a simple implementation of the classic robot discovery algorithm. The robot moves straight until it reaches a collision. Upon reaching a collision it rotates until it is free to move. This algorithm works well for certain environments for finding the bounds (i.e. walls), but quickly finds itself stuck in a loop of a specific path.

##### Spline Discovery Task #####

The Spline Discovery Task is the implementation of the theory discussed in Section 3.7. For navigating in an unknown environment a height map is used as mentioned above. This is a rather complex procedure, and is presented along with the code. For further details or clarifications on the algorithm please see Section 3.7.

This first section is important. First we check if there are any nodes in the spline and if the environment is not already explored. If it is already explored the algorithm is not required and will be canceled. Otherwise new terrain cut points are added:

```
  void SplineDiscoveryTask::process() {
        // Are there nodes in the spline?
        if (node >= core->navigation->wheelLeftSplineX->getNodeCount() - 1) {
                 if (isExplored() == true) {
                          ((EmssController*) core->controller)->wheelDrive(0, 0);
                          Task::status = Task::Finished;
                          return;
                 } else {
                          core->navigation->clearNavPoints();
                          addNewTerrainCutPoints();
                          node = 0;
                          return;
                 }
        }
```

This next part of the code checks the heat map whether it is safe to move ahead. This is done by directly checking ahead of the robot from a given start and end angle. Figure 4-12 presents two areas of collision. If an obstacle in the dark area is detected the robot will immediately stop and turn ninety degrees. This is to avoid a head-on collision with the obstacle. If an obstacle in the light area is detected, new terrain cut points are calculated. This discrimination lets the calculation of new terrain cut points result in a natural collision avoidance per algorithm.

```
         // Look ahead if it is good to move there using a sweep across a start and
         // end angle...
         double heat = 0.0;
         for (int i = 0; i < core->longSetting("SPLINEDISCOVERYTASK_RANGE_MM");
                   i += 10) {
                  for (int angle = -(core->longSetting("
                          SPLINEDISCOVERYTASK_SWEEP_ANGLE") / 2);
                          angle <= (core->longSetting("
                          SPLINEDISCOVERYTASK_SWEEP_ANGLE") / 2);
                          angle++) {
                          Trafo2D checkpoint = core->movementTracker->transformation *
                                   Trafo2D::rot(-Rad(angle)) * Trafo2D::trans(0, i);
                          heat += core->heatMap->getChannelValue(
                                   HeatMap::CollisionAreaChannel,
                                   (long) ((checkpoint.trans().x()) / core->scale),
                                   (long) ((checkpoint.trans().y()) / core->scale));
                          if (i < core->longSetting("
                             SPLINENAVIGATIONTASK_CRITICAL_AREA_DISTANCE_MM") && heat > 0)
                          {
                                   collisionInCriticalArea = true;
                          }
                  }
         }
         // Safe to move?
         if (heat > 0.0 && lastProcessWasCollision == false) {
                  // is the collision in the critical area?
                  if (collisionInCriticalArea == false) {
                          core->navigation->clearNavPoints();
                          addNewTerrainCutPoints();
                          node = 0;
                          lastProcessWasCollision = true;
                  } else {
                          // turn away from the collision
                          ((EmssController*) core->controller)->turn(90,
                                   core->doubleSetting("NAVIGATION_TARGET_SPEED_MMPS"));
                          core->navigation->clearNavPoints();
                          addNewTerrainCutPoints();
                         node = 0;
                         lastProcessWasCollision = true;
                         collisionInCriticalArea = false;
                 }
         } else {
                 lastProcessWasCollision = false;
         }
                           Code 4-19: Spline Discovery Collision Avoidance
With the following code the steering information is sent to the controller:
         // Align the robot and then crawl the spline...
         Vector2D wheelSpeed = core->navigation->getWheelSpeed(node, interval);
         ((EmssController*) core->controller)->wheelDrive((short) wheelSpeed.x(),
                 (short) wheelSpeed.y());
         node++;
  }
```

The most interesting part in this class is how the terrain cut points are calculated, as discussed in the Chapter “Theory”. First the height array is filled with the value for an unknown height:

```
         int heights[mapWidth][mapHeight];
         int explorationMap[mapWidth][mapHeight];
         long robotX = core->movementTracker->x();
         long robotY = core->movementTracker->y();
         // Fill array with default values
         for (int x = 0; x < mapWidth; x++) {
                 for (int y = 0; y < mapHeight; y++) {
                         heights[x][y] = TERRAIN_UNKNOWN_HEIGHT;
                 }
         }
```

Then the exploration map is filled with the information from the Heat Map. That means that every point is assigned to either discovered, obstacle or unknown:

```
         // Fill the exploration array
         for (int x = 0; x < mapWidth; x++) {
                 for (int y = 0; y < mapHeight; y++) {
                         long heatMapX = robotX - RELATIVE_EXPLORATION_WIDTH / 2 +
                                  x * EXPLORATION_MAP_GRID_SIZE;
                         long heatMapY = robotY - RELATIVE_EXPLORATION_HEIGHT / 2 +
                                  y * EXPLORATION_MAP_GRID_SIZE;
                         if (core->heatMap->getChannelValue(HeatMap::OpenAreaChannel,
                              heatMapX / core->scale, heatMapY / core->scale) > 0.0) {
                                  explorationMap[x][y] = Discovered;
                         } else if (core->heatMap->getChannelValue(
                              HeatMap::CollisionAreaChannel, heatMapX / core->scale,
                              heatMapY / core->scale) > 0.0) {
                                explorationMap[x][y] = Obstacle;
                       } else {
                                explorationMap[x][y] = Unknown;
                       }
                }
        }
```

The algorithm begins with the robots position and calculates for each point the heights depending on the exploration map. As a result, points near an obstacle, or who are marked as discovered, get a low height, and unknown points get higher and higher in height, the further away they are from a known point.

```
        int h;
        QQueue<TerrainPointT<int> > queue;
        queue.enqueue(TerrainPointT<int> (mapWidth / 2, mapHeight / 2, 0));
        // Calculate the heights
        while (!queue.isEmpty()) {
                TerrainPointT<int> p = queue.dequeue();
                if (p.height + 1 > TERRAIN_MAP_MAX_HEIGHT) {
                       h = TERRAIN_MAP_MAX_HEIGHT;
                } else {
                       h = p.height + 1;
                }
                for (int k = -1; k <= 1; k++) {
                       for (int j = -1; j <= 1; j++) {
                                if (p.x + k < mapWidth && p.x + k >= 0 &&
                                        p.y + j < mapHeight && p.y + j >= 0 &&
                                        (heights[p.x + k][p.y + j] > h ||
                                        heights[p.x + k][p.y + j] ==
                                        TERRAIN_UNKNOWN_HEIGHT)) {
                                        switch (explorationMap[p.x + k][p.y + j]) {
                                        case Obstacle:
                                                heights[p.x + k][p.y + j] = 0;
                                                queue.enqueue(TerrainPointT<int> (p.x + k,
                                                        p.y + j, 0));
                                                break;
                                        case Discovered:
                                                heights[p.x + k][p.y + j] = 0;
                                                queue.enqueue(TerrainPointT<int> (p.x + k,
                                                        p.y + j, 0));
                                                break;
                                        case Unknown:
                                                heights[p.x + k][p.y + j] = h;
                                                queue.enqueue(TerrainPointT<int> (p.x + k,
                                                        p.y + j, h));
                                                break;
                                        }
                                }
                       }
                }
        }
```

All points which correlate to the given cut level point are found and added to a queue:

```
        std::vector<QPoint> terrainCutPoints;
        // Add the points with the terrain cut height to the spline
        while (terrainCutPoints.size() < 3) {
                for (int x = 0; x < mapWidth; x++) {
                        for (int y = 0; y < mapHeight; y++) {
                                 if (heights[x][y] == TERRAIN_CUT_LEVEL) {
                                         long navPointX = robotX -
                                         RELATIVE_EXPLORATION_WIDTH / 2 +
                                         x * EXPLORATION_MAP_GRID_SIZE;
                                         long navPointY = robotY -
                                         RELATIVE_EXPLORATION_HEIGHT / 2 +
                                         y * EXPLORATION_MAP_GRID_SIZE;
                                         terrainCutPoints.push_back(QPoint(navPointX,
                                                  navPointY));
                                 }
                        }
                }
                cutLevel--;
        }
```

The cut level points are in an unsorted list. To make use of these points it is necessary to sort the list and remove redundant points. For this reason a minimal distance between the cut points is defined. All points which do not have this distance to the last point will removed from the list:

```
        // Sort the terrain cut points
        for (int i = 0; i < terrainCutPoints.size(); i++) {
                int distance = INT_MAX;
                int nearestPoint = 0;
                for (int k = i + 1; k < terrainCutPoints.size(); k++) {
                        int temp = std::abs(terrainCutPoints[i].x() -
                                 terrainCutPoints[k].x()) +
                                 std::abs(terrainCutPoints[i].y() -
                                 terrainCutPoints[k].y());
                        if (temp < distance) {
                                 distance = temp;
                                 nearestPoint = k;
                        }
                }
                if (i != terrainCutPoints.size() - 1) {
                        QPoint temp = terrainCutPoints[i + 1];
                        terrainCutPoints[i + 1] = terrainCutPoints[nearestPoint];
                        terrainCutPoints[nearestPoint] = temp;
                }
         }
```

Finally, the calculated points are registered in the Navigation component.

```
         // Add the terrain cut point to the navigation spline
         QPoint lastPointAdded;
         for (int i = 0; i < terrainCutPoints.size(); i++) {
                 // Take only the cut points with the distance CUT_POINT_DISTANCE from
                 // the last point
                 if (i == 0) {
                         this->core->navigation->addNavPoint(terrainCutPoints[i].x(),
                                  terrainCutPoints[i].y());
                         lastPointAdded = terrainCutPoints[i];
                 }
                 if (i != 0 && i != terrainCutPoints.size() - 1 &&
                         std::sqrt(std::pow(lastPointAdded.x() -
                         terrainCutPoints[i].x(), 2.0) +
                         std::pow(lastPointAdded.y() - terrainCutPoints[i].y(),
                          2.0)) >core->intSetting("
                         SPLINEDISCOVERYTASK_TERRAIN_CUT_POINT_DISTANCE_MM")) {
                         this->core->navigation->addNavPoint(terrainCutPoints[i].x(),
                                  terrainCutPoints[i].y());
                         lastPointAdded = terrainCutPoints[i];
                 }
         }
```

#### GUI ####

The GUI packet consists of different widget classes which can be used in an emss application to control the robot, display information of the Core, set properties, et cetera. Most of these classes are quite generic and can be re-used in other projects.

##### Viewport #####

The Viewport class is a very important part of the emss user interface. A Viewport has the ability to display any emss Map as well as the Map Objects. In addition to displaying Maps, a Viewport can accept actions from the user and provides the necessary tools for navigating the Maps, such as scrolling. A Viewport allows multiple Maps to be layered on-top of each other, and automatically generates the proper GUI elements for hiding and showing specific Maps. Core Maps can have multiple viewports attached to it, allowing a user to create a set of specialized Viewports to his liking. Viewports also allow a set of actions to be added to its toolbar. When the user clicks an action, the Viewport forwards this information back to the connected application which can in turn act on the given action. Other features such as auto focus and auto refresh make the Viewport a very flexible widget. The drawing routines have been specifically designed to run in a separate thread space, interfering with the emss Core as little as possible.

```
  // Initialize viewport
  Viewport *viewport = new Viewport(100); // Refresh every 100ms
  viewport->setAntiAlias(false);
  // Register maps with viewport
  viewport->registerMap(core->gridMap);
  viewport->registerMap(core->physicalMap);
  viewport->registerMap(core->terrainMap);
  viewport->registerMap(core->heatMap);
  viewport->registerMap(core->objectMap);
  // Add toolbar actions
  viewport->addToolbarAction("Task","task");
  viewport->addToolbarAction("Nav Point","navpoint");
  viewport->addToolbarAction("Environment Info","environmentinfo");
  viewport->addToolbarAction("Find Robot","findrobot");
  // Show widget and focus on robot
  viewport->show();
  viewport->focusOnPoint(core->robotObject->x, core->robotObject->y);
  ...
  // Cleanup...
  delete viewport;
```

##### Joystick #####

The Joystick class can be used to control the various Controllers. It provides a widget which can be controlled by the mouse and provides all the appropriate slots and signals. The joystick has two yokes: x and y, which in turn can be used for control. Furthermore, it can be customized to force exclusive yokes – a mode where only one of the yokes can be moved at a time. In addition, the Joystick class also provides control over its acceleration and deceleration, a useful feature for gently controlling a robot.

##### Options Dialog #####

Often a user interface requires some sort of choice from the user. The Options Dialog class provides an easy to call, blocking static method choose(options,description,title) which creates a modal dialog of choices based on the options parameter. The choice in options must separated by the | character.

```
  QString controller = OptionsDialog::choose("Block Drive|Fluid|emss", "Please
  select a controller type:", "New Controller");
```

##### Settings Editor #####

Any emss application, including the Core itself, may have various settings. The Settings Editor class provides a simple text field where all settings are displayed and may be edited. The class accepts any standard QSettings data structure, making it plug-and-play with all Qt settings objects.

##### Task Editor #####

The Task Manager holds a list with all created Tasks. The Task Editor class allows an emss application to easily manage the Cores current Tasks. In addition to displaying Task Information, Running Tasks may be stopped. 

### Interface ###

The Interface application emssInterface is the predominant graphical user interface to control the emss robot and other Core components. The application is divided into three groups: Connection, Controller, and Data. The Connection group allows one to connect and disconnect with the emss hardware. It supports the selection of all the implemented Core Controllers and Movement Trackers. The Controller panel features various tabs each designed specifically for the individual Controllers. The important diagnostic panel History is located inside the Data group box. The history widget provides all necessary diagnostic and debugging information which streams from the various Core components. Along with the History panel, other data sources are provided such as Serial Port and Sensors. The emss Interface allows multiple instances of Core Viewports, as well as modal windows such as the Task Editor and Settings Editor of both the Core configuration settings and the Interface settings. 

### Test Applications ###

#### Simulation ####

Before the start of the project, and even the realization of the hardware, a simulation (emssSimulation) was created in order to get a head start on the algorithms, et cetera. Although never completed, many of the simulation components were ported to the emss Core when needed, including a large portion of the drawing code.

#### Vector Test ####

The Vector Test application (emssVectorTest) is a very simple program to visualize the rotation and translations of two vectors.

#### Rotation Test ####

The Rotation Test application (emssRotationTest) was built to help us to understand the concept of a rotation matrix. It is a very simple program with a modest GUI. This application holds two transformation matrices A1 and A2, and outputs the difference of angle and distance between them.

#### Spline Test ####

To test the different splines we implemented the emssSplineTest application. Various navigation points can be set, which in turn calculates the curve of the spline. Additional information such as the first derivative can be output as well.

#### Terrain Map ####

This application was built for testing the height map algorithm. It visualizes the exploration map together with a heat map. The red space is unexplored, the brighter the square the higher is the height map value. The two known obstacle and free space can be painted with the mouse. The yellow point represents the highest point in the map. It is also possible to show the cut points on different levels adjusted with a slider. 

### Build Instructions ###

In order to successfully build the emss framework, the following requirements must be met:

* GNU C++ Compiler, version 3.0 or newer
* Qt Open Source Edition for C++, version 4.4 or newer 

The emss software requires only the standard Qt build. No additional modules are required as everything is packaged within the emss source. Additionally, the Qt debug build is not required as long as emss software is linked against Qt with release binaries. All emss binaries are compiled to either /emss/Binaries/Unix and /emss/Binaries/Windows, depending on which environment the build is executed in.

To build the emss Core library, which is required for emss applications, the following commands can be used:

```
  cd <path>/emms/Core
  qmake
  make
```

To build any emss application, such as the emss Interface, the following commands can be used:

```
  cd <path>/emms/<application directory>
  qmake
  make
```

The following application directories are available for building:

* Interface
* COILTest
* RotationTest
* SensorTest
* Simulation
* SplineTest
* TerrainMap
* VectorTest 

In order to execute any emss application, the following dependencies must be resolved (i.e. libraries in the path):

* emssCore
* QtCore4
* QtGui4 

When first starting an emss application which requires a configuration file, the path to the wanted configuration file is requested. Pre-defined configuration files can be found at /emss/Resources/emss<configuration>.config, such as /emss/Resources/emssCore.config. 

## Links ##

Wiki: http://emssframework.sourceforge.net/
