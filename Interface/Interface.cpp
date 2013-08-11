/*
 *  Interface.cpp
 *
 *  ===========================================================================
 *
 *  Copyright 2008-2009 Daniel Kruesi (Dan Krusi) and David Grob
 *
 *  This file is part of the emms framework.
 *
 *  The emms framework  is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The emms framework is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "Interface.h"

#include <QLayout>

#include "../Core/Library/Debug.h"
#include "../Core/Library/Util.h"
#include "../Core/Library/SleeperThread.h"
#include "../Core/Library/Math.h"
#include "../Core/Library/Joystick2D.h"

#include "../Core/Core.h"

#include "../Core/Map/GridMap.h"
#include "../Core/Map/HeatMap.h"
#include "../Core/Map/TerrainMap.h"
#include "../Core/Map/ObjectMap.h"
#include "../Core/Map/PhysicalMap.h"
#include "../Core/Map/StructureMap.h"
#include "../Core/Map/FadingCollisionMap.h"

#include "../Core/Object/RobotObject.h"
#include "../Core/Object/NavPathObject.h"

#include "../Core/COIL/COIL.h"

#include "../Core/Tracker/MovementTracker.h"
#include "../Core/Tracker/Tracker.h"

#include "../Core/Controller/Controller.h"

#include "../Core/Task/Task.h"
#include "../Core/Task/TaskManager.h"
#include "../Core/Task/DiscoveryTask.h"
#include "../Core/Task/DiscoveryTask2.h"
#include "../Core/Task/NavigationTask.h"
#include "../Core/Task/SplineNavigationTask.h"
#include "../Core/Task/StraightPathMoveTask.h"
#include "../Core/Task/TestMoveTask.h"
#include "../Core/Task/WallFollowerTask.h"
#include "../Core/Task/RoamingTask.h"
#include "../Core/Task/FingerprintNavigationTask.h"

#include "../Core/Navigation/Navigation.h"

#include "../Core/GUI/OptionsDialog.h"
#include "../Core/GUI/SettingsEditor.h"
#include "../Core/GUI/CameraView.h"
#include "../Core/GUI/TextToSpeechView.h"
#include "../Core/GUI/MapOverview.h"
#include "../Core/GUI/RemoteInterfaceView.h"
#include "../Core/GUI/HeapLoggerView.h"


Interface::Interface(QWidget *parent) :
	QWidget(parent) {

	// Init
	core = NULL;
	settings = NULL;
	ignoreViewportClosedSignal = false;

	// GUI setup...

	// Main window
	this->setLayout(new QVBoxLayout());
	this->layout()->setContentsMargins(0,0,0,0);
	this->setWindowTitle("emss Interface");
	//this->setAttribute(Qt::WA_DeleteOnClose, true);

	// Menubar
	menu = new QMenuBar();
	this->layout()->addWidget(menu);
	menuInterface = new QMenu("emss Interface");
	menu->addMenu(menuInterface);
	actionInterfaceReset = new QAction("Reset", menuInterface);
	menuInterface->addAction(actionInterfaceReset);
	actionInterfaceSettingsCore = new QAction("Core Settings...", menuInterface);
	actionInterfaceSettingsCore->setEnabled(false);
	menuInterface->addAction(actionInterfaceSettingsCore);
	actionInterfaceSettingsController = new QAction("Interface Settings...", menuInterface);
	menuInterface->addAction(actionInterfaceSettingsController);
	actionInterfaceExit = new QAction("Exit", menuInterface);
	menuInterface->addAction(actionInterfaceExit);
	menuView = new QMenu("Views");
	menu->addMenu(menuView);
	actionViewNewViewport = new QAction("Viewport", menuView);
	menuView->addAction(actionViewNewViewport);
	actionViewNewMapOverview = new QAction("Map Overview", menuView);
	menuView->addAction(actionViewNewMapOverview);
	actionViewTaskEditor = new QAction("Task Editor", menuView);
	menuView->addAction(actionViewTaskEditor);
	actionViewWeightEditor = new QAction("Weight Editor", menuView);
	menuView->addAction(actionViewWeightEditor);
	actionViewRemoteInterface = new QAction("Remote Interface", menuView);
	menuView->addAction(actionViewRemoteInterface);
	actionViewCameraView = new QAction("Camera", menuView);
	menuView->addAction(actionViewCameraView);
	actionViewTextToSpeechView = new QAction("Text to Speech", menuView);
	menuView->addAction(actionViewTextToSpeechView);
	actionViewHeapLogger = new QAction("Heap Logger", menuView);
	menuView->addAction(actionViewHeapLogger);
	menuView->setEnabled(false);

	// Top frame, including connection, nav, parameters, statistics...
	frameTop = new QFrame();
	frameTop->setLayout(new QHBoxLayout());
	this->layout()->addWidget(frameTop);

	// Connection, navigation, parameters
	frameTopLeft = new QFrame();
	frameTopLeft->setLayout(new QVBoxLayout());
	frameTopLeft->layout()->setMargin(0);
	frameTop->layout()->addWidget(frameTopLeft);

	// Controller box
	frameTopLeftBottom = new QGroupBox("Controller");
	tabNavigation = new QTabWidget();
	frameTopLeft->layout()->addWidget(tabNavigation);

	// Connection box
	tabConnection = new QWidget();
	tabConnection->setLayout(new QVBoxLayout());
	cbController = new QComboBox();
	cbController->addItem("emss Controller");
	tabConnection->layout()->addWidget(cbController);
	cbTracker = new QComboBox();
	cbTracker->addItem("Averaged Tracker");
	cbTracker->addItem("Single Tracker");
	tabConnection->layout()->addWidget(cbTracker);
	cbNavigation = new QComboBox();
	cbNavigation->addItem("Spline Navigation");
	cbNavigation->addItem("System of Weights Navigation");
	tabConnection->layout()->addWidget(cbNavigation);
	cbPort = new QComboBox();
	cbPort->addItem("COM1");
	cbPort->addItem("COM2");
	cbPort->addItem("COM3");
	cbPort->addItem("COM4");
	cbPort->addItem("COM5");
	cbPort->addItem("/dev/ttyUSB0");
	cbPort->addItem("/dev/ttyUSB1");
	cbPort->addItem("/dev/ttyUSB2");
	cbPort->addItem("/dev/ttyUSB3");
	cbPort->addItem("/dev/ttyUSB4");
	cbPort->addItem("/dev/ttyUSB5");
	tabConnection->layout()->addWidget(cbPort);
	checkEmulation = new QCheckBox("Emulate Hardware Interface");
	tabConnection->layout()->addWidget(checkEmulation);
	checkSafeMode = new QCheckBox("Safe Mode");
	tabConnection->layout()->addWidget(checkSafeMode);
	btnConnect = new QPushButton("Connect");
	tabConnection->layout()->addWidget(btnConnect);
	btnAbort = new QPushButton("Abort");
	btnAbort->setEnabled(false);
	tabConnection->layout()->addWidget(btnAbort);
	tabNavigation->addTab(tabConnection, "Connection");

	// emss Controller
	tabEmssController = new QWidget();
	tabEmssController->setEnabled(false);
	tabEmssController->setLayout(new QVBoxLayout());
	tabNavigation->addTab(tabEmssController, "Controller");
	joystickEmssController = new Joystick(Joystick::JOYSTICK_MODE_FREE);
	joystickEmssController->acceleration = Joystick::JOYSTICK_ACCELERATION_INSTANT;
	tabEmssController->layout()->addWidget(joystickEmssController);
	lcdSpeed = new QLCDNumber();
	lcdSpeed->setFrameStyle(QFrame::NoFrame);
	lcdSpeed->setSegmentStyle(QLCDNumber::Flat);
	sliderSpeed = new QSlider(Qt::Horizontal);
	tabEmssController->layout()->addWidget(sliderSpeed);
	tabEmssController->layout()->addWidget(lcdSpeed);

	taskEditor = new TaskEditor(NULL);
	tabNavigation->addTab(taskEditor, "Tasks");

	weightEditor = new WeightEditor(NULL);
	tabNavigation->addTab(weightEditor, "Weights");

	// Loggers, including history and serial port...
	QFrame *frameBottomSpacer = new QFrame();
	frameBottomSpacer->setLayout(new QHBoxLayout());
	this->layout()->addWidget(frameBottomSpacer);
	tabLoggers = new QTabWidget();
	frameBottomSpacer->layout()->addWidget(tabLoggers);

	txtHistory = new QTextEdit();
	Util::setTextEditAsLogger(txtHistory);
	tabLoggers->addTab(txtHistory, "History");
	txtSerialPort = new QTextEdit();
	Util::setTextEditAsLogger(txtSerialPort);
	tabLoggers->addTab(txtSerialPort, "Serial Port");
	frameSensorData = new QFrame();
	frameSensorData->setLayout(new QVBoxLayout());
	btnRefreshSensors = new QPushButton("Refresh Data");
	btnRefreshSensors->setEnabled(false);
	frameSensorData->layout()->addWidget(btnRefreshSensors);
	txtSensorData = new QTextEdit();
	Util::setTextEditAsLogger(txtSensorData);
	frameSensorData->layout()->addWidget(txtSensorData);
	tabLoggers->addTab(frameSensorData, "Sensor Data");
	robotControlPanel = new RobotControlPanel(NULL);
	tabLoggers->addTab(robotControlPanel, "Control Panel");
	robotSpeedGraph = new RobotSpeedGraph(NULL);
	tabLoggers->addTab(robotSpeedGraph, "Speed Graph");

	// Make GUI connections
	connect(btnConnect, SIGNAL(clicked()), this, SLOT(connectDisconnect()));
	connect(sliderSpeed, SIGNAL(valueChanged(int)), lcdSpeed, SLOT(display(int)));
	connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(menuAction(QAction*)));
	connect(btnRefreshSensors, SIGNAL(clicked()), this, SLOT(refreshSensors()));

	// Load settings and default values if needed
	settings = new QSettings(QString("%1emssInterface.config").arg(Util::getResourcesFilePath()), QSettings::IniFormat);

	// Set default values
	sliderSpeed->setRange(0, 500);
	sliderSpeed->setValue(settings->value("Controller_TargetSpeed", 0).toInt());
	cbPort->setCurrentIndex(settings->value("Connection_Port", 0).toInt());
	cbController->setCurrentIndex(settings->value("Connection_Controller", 0).toInt());
	cbTracker->setCurrentIndex(settings->value("Connection_Tracker", 0).toInt());
	cbNavigation->setCurrentIndex(settings->value("Connection_Navigation", 0).toInt());
	checkEmulation->setChecked(settings->value("Connection_EmulateHardware", false).toBool());
	checkSafeMode->setChecked(settings->value("Connection_SafeMode", false).toBool());
	if(settings->value("Window_X", -1) != -1 && settings->value("Window_Y", -1) != -1 && settings->value("Window_Width", -1) != -1 && settings->value("Window_Height", -1) != -1) this->setGeometry(settings->value("Window_X").toInt(), settings->value("Window_Y").toInt(), settings->value("Window_Width").toInt(), settings->value("Window_Height").toInt());

	// Setup debugging console
	Debug::setOutput(txtHistory);
	if(settings->value("Log_LogToFile", "false").toBool()) {
		QString date = QDate::currentDate().toString("yyyyMMdd");
		QString time = QTime::currentTime().toString("hhmmss");
		Debug::logToFile(Util::getLogsFilePath().append(QString("%1_%2.log").arg(date).arg(time)));
	}

	// Set up viewport list
	viewports = new QList<Viewport*>();

}

Interface::~Interface() {

	// Save settings
	settings->setValue("Connection_Port", cbPort->currentIndex());
	settings->setValue("Connection_Controller", cbController->currentIndex());
	settings->setValue("Connection_Tracker", cbTracker->currentIndex());
	settings->setValue("Connection_Navigation", cbNavigation->currentIndex());
	settings->setValue("Connection_EmulateHardware", checkEmulation->isChecked());
	settings->setValue("Connection_SafeMode", checkSafeMode->isChecked());
	settings->setValue("Window_X", this->geometry().x());
	settings->setValue("Window_Y", this->geometry().y());
	settings->setValue("Window_Width", this->geometry().width());
	settings->setValue("Window_Height", this->geometry().height());
	settings->setValue("Controller_TargetSpeed", sliderSpeed->value());

	// Free heap...
	if(viewports) delete viewports;
	if(settings) delete settings;
	if(core) delete core;
}



void Interface::connectDisconnect() {

	if(core == NULL) {
		// Init our core modules...
		core = new Core();
		actionInterfaceSettingsCore->setEnabled(true);
	}

	// Connect or disconnect...
	if (core->isConnected() == false) {

		Debug::print("--------------------------------------------");
		Debug::print("[Interface] connecting on %1%2... ", cbPort->currentText(), ( checkEmulation->isChecked() ? " (emulated)" : ""));

		// Init the core and kick it off... :)
		if(core->connect(cbController->currentText().replace(" Controller",""), cbTracker->currentText().replace(" Tracker",""), cbNavigation->currentText().replace(" Navigation",""), cbPort->currentText(), checkSafeMode->isChecked(), checkEmulation->isChecked())) {

			core->run();

			if (core->controller->name == "emss") {

				connect(joystickEmssController, SIGNAL(yokeChanged(double,double)), core->joystick, SLOT(setYoke(double,double)));
				tabEmssController->setEnabled(true);
			}

			// Speed control
			connect(sliderSpeed, SIGNAL(valueChanged(int)), core->controller, SLOT(setTargetSpeed(int)));
			core->controller->setTargetSpeed(sliderSpeed->value());

			// Gui stuff
			connect(core->tracker, SIGNAL(moved(long,long,double)), this, SLOT(focusOnPoint(long, long)));
			connect(core->controller, SIGNAL(signalSensorDataUpdated()), robotControlPanel, SLOT(updateSensorData()));
			connect(core->controller, SIGNAL(signalSensorDataUpdated()), robotSpeedGraph, SLOT(updateSensorData()));
			robotControlPanel->setCore(core);
			robotSpeedGraph->setCore(core);
			connect(btnAbort, SIGNAL(clicked()), core, SLOT(abort()));
			taskEditor->setTaskManager(core->taskManager);
			taskEditor->setEnabled(true);
			taskEditor->refreshList();
			connect(core->taskManager, SIGNAL(taskListChanged()),taskEditor, SLOT(refreshList()) );
			weightEditor->setNavigation(core->navigation);
			weightEditor->setEnabled(true);
			weightEditor->refreshList();
			if(core->navigation->name == "System of Weights") connect(core->navigation, SIGNAL(weightsChanged()),weightEditor, SLOT(refreshList()) );
			btnConnect->setText("Disconnect");
			btnAbort->setEnabled(true);
			btnRefreshSensors->setEnabled(true);
			menuView->setEnabled(true);
			if(viewports->count() == 0) menuAction(actionViewNewViewport);

			Debug::print("[Interface] connected");

		}

	} else {

		Debug::print("[Interface] disconnecting...");

		// Get battery info
		int battery = -1;
		if(core->coil != NULL) battery = (int)((double)core->coil->readSensor(COIL::SENSOR_BATTERY_CHARGE) / (double)core->coil->readSensor(COIL::SENSOR_BATTERY_CAPACITY) * 100);

		// Call for disconnect
		core->stop();
		core->disconnect();

		// Gui stuff
		robotControlPanel->setCore(NULL);
		robotSpeedGraph->setCore(NULL);
		//tabBlockDriveController->setEnabled(false);
		//tabFluidDriveController->setEnabled(false);
		tabEmssController->setEnabled(false);
		btnConnect->setText("Connect");
		btnAbort->setEnabled(false);
		btnRefreshSensors->setEnabled(false);
		menuView->setEnabled(false);
		taskEditor->setEnabled(false);
		weightEditor->setEnabled(false);

		Debug::print("[Interface] disconnected, battery is at %1%", battery);

	}
}



void Interface::menuAction(QAction *action) {
	if(action == actionInterfaceExit) {

		if(core) core->disconnect();
		qApp->closeAllWindows();

	} else if(action == actionViewNewViewport) {

		Viewport *viewport = new Viewport(core->intSetting("Map_Scale"), settings->value("Viewport_AutoUpdateInterval", 0).toInt(), settings->value("Viewport_RenderMode", 0) == "opengl");
		viewport->setAntiAlias(false);

		viewport->registerMap(core->gridMap);
		viewport->registerMap(core->physicalMap);
		viewport->registerMap(core->terrainMap);
		viewport->registerMap(core->heatMap);
		viewport->registerMap(core->fadingCollisionMap);
		viewport->registerMap(core->structureMap);
		viewport->registerMap(core->objectMap);

		viewport->addToolbarAction("Task","task");
		viewport->addToolbarAction("Navigate","navigationtask");
		viewport->addToolbarAction("Nav Point","navpoint");
		viewport->addToolbarAction("Collision","collision");
		viewport->addToolbarAction("Open Area","openarea");
		viewport->addToolbarAction("Map Info","environmentinfo");
		viewport->addToolbarAction("Find Robot","findrobot");
		viewport->addToolbarAction("Raise Terrain","raiseterrain");
		viewport->addToolbarAction("Lower Terrain","lowerterrain");
		viewport->addToolbarAction("Move Robot","moverobot");
		viewport->addToolbarAction("Rotate Robot","rotaterobot");

		viewports->append(viewport);

		connect(viewport, SIGNAL(toolbarAction(Viewport*, QString,long,long)), this, SLOT(viewportAction(Viewport*, QString,long,long)));
		connect(viewport, SIGNAL(closed(Viewport*)), this, SLOT(viewportClosed(Viewport*)));

		viewport->setGeometry(this->geometry().x()+this->width()+20,this->geometry().y(), (int)(this->width()*1.5),this->height());
		viewport->setWindowTitle("emss Viewport");
		viewport->show();
		viewport->focusOnPoint(core->tracker->getX(), core->tracker->getY());

	} else if(action == actionViewNewMapOverview) {

		MapOverview *mapOverview = new MapOverview(core->mapWidth, core->mapHeight, core->scale, settings->value("MAPOVERVIEW_AUTOUPDATE_INTERVAL", 0).toInt(), false);
		mapOverview->setAntiAlias(false);

		mapOverview->registerMap(core->gridMap, false);
		mapOverview->registerMap(core->physicalMap, true);
		mapOverview->registerMap(core->terrainMap, true);
		mapOverview->registerMap(core->heatMap, true);
		mapOverview->registerMap(core->fadingCollisionMap, true);
		mapOverview->registerMap(core->structureMap, true);
		mapOverview->registerMap(core->objectMap, true);

		//mapOverview->setGeometry(this->geometry().x()+this->width()+20,this->geometry().y(), (int)(this->width()*1.5),this->height());
		mapOverview->setWindowTitle("emss Map Overview");
		mapOverview->show();

	} else if(action == actionViewTaskEditor) {

		// Get a valid task list from navigator and send pointer to TaskEditor widget...
		TaskEditor *taskEditor = new TaskEditor(core->taskManager);
		taskEditor->show();
		connect(core->taskManager, SIGNAL(taskListChanged()),taskEditor, SLOT(refreshList()) );

	} else if(action == actionViewWeightEditor) {

		// Show editor based on the current Navigation module. The editor will worry if it is not the correct type...
		WeightEditor *weightEditor = new WeightEditor(core->navigation);
		connect(core->navigation, SIGNAL(weightsChanged()),weightEditor, SLOT(refreshList()) );
		weightEditor->show();

	} else if(action == actionViewCameraView) {

		// Show a new camera view
		CameraView *cameraView = new CameraView(core->stringSetting("Camera_CaptureToolPath"),
												core->stringSetting("Camera_CaptureToolCommand"),
												QString("%1%2").arg(Util::getResourcesFilePath()).arg(core->stringSetting("Camera_CaptureToolOutFile")),
												core->intSetting("Camera_AutoRefreshInterval"));
		cameraView->show();

	} else if(action == actionViewTextToSpeechView) {

		// Show a new text to speech view
		TextToSpeechView *speechView = new TextToSpeechView(core->stringSetting("TextToSpeech_SpeakToolPath"),
															core->stringSetting("TextToSpeech_SpeakToolCommand"));
		speechView->show();

	} else if(action == actionViewRemoteInterface) {

		// Show a new remote interface view
		RemoteInterfaceView *remoteInterfaceView = new RemoteInterfaceView(core->remoteInterface);
		remoteInterfaceView->show();

	} else if(action == actionViewHeapLogger) {

		// Show a new heap logger view
		HeapLoggerView *heapLoggerView = new HeapLoggerView();
		heapLoggerView->show();

	} else if (action == actionInterfaceReset){

		// Close all viewports and reset
		ignoreViewportClosedSignal = true;
		for(int i = 0; i < viewports->count(); i++) {
			viewports->at(i)->close();
		}
		viewports->clear();
		ignoreViewportClosedSignal = false;
		if(core) {
			// Disconnect, reset, connect
			if (core->isConnected()) connectDisconnect();
			core->reset();
			connectDisconnect();
		}

	} else if (action == actionInterfaceSettingsCore){

		SettingsEditor *settingsEditor = new SettingsEditor(core->settings);
		settingsEditor->show();

	} else if (action == actionInterfaceSettingsController){

		SettingsEditor *settingsEditor = new SettingsEditor(this->settings);
		settingsEditor->show();

	}
}



void Interface::viewportAction(Viewport *viewport, QString value, long x, long y) {

	// Convert to mm
	x = core->pixelsTomm(x);
	y = core->pixelsTomm(y);

	if(value == "navpoint") {

		core->navigation->addNavPoint(x, y);

	} else if(value == "task") {

		QString task = OptionsDialog::choose("Straight Path Move|Test Move|Wall Follower|Roaming Task|Discovery|Discovery2|Navigation|Spline Navigation|Fingerprint Navigation|Find Homebase|Dock and Charge", "Please select a task:", "New Task");
		if(task == "Straight Path Move") {
			core->addTask(new StraightPathMoveTask(core,x, y));
		} else if (task == "Test Move") {
			QString move = OptionsDialog::choose("Square|Rotate 90|Rotate 360|Triangle|Straight|Circle|Vector Circle|Accuracy Test", "Please select a specific move:", "Test Move");
			if(move != "") core->addTask(new TestMoveTask(core, move));
		} else if (task == "Discovery") {
			core->addTask(new DiscoveryTask(core, this->sliderSpeed->value()));
		} else if (task == "Discovery2") {
			core->addTask(new DiscoveryTask2(core));
	    } else if (task == "Navigation") {
			core->addTask(new NavigationTask(core, this->sliderSpeed->value()));
		} else if (task == "Spline Navigation") {
			core->addTask(new SplineNavigationTask(core, this->sliderSpeed->value()));
		} else if (task == "Wall Follower") {
			core->addTask(new WallFollowerTask(core, this->sliderSpeed->value()));
		} else if (task == "Roaming Task") {
			core->addTask(new RoamingTask(core, this->sliderSpeed->value()));
		} else if (task == "Fingerprint Navigation") {
			core->addTask(new FingerprintNavigationTask(core, 6000, 2000));
		}
		viewport->resetToolbar();

	} else if(value == "collision") {

		core->heatMap->registerHeat(ColorMap::CollisionAreaChannel, x, y);

	} else if(value == "openarea") {

		core->heatMap->registerHeat(ColorMap::OpenAreaChannel, x, y);

	} else if(value == "environmentinfo") {

		// Get physical map area type as string
		QString physicalMapAreaType = "unknown";
		if(core->physicalMap->getAreaType(x, y) == PhysicalMap::PhysicalMapAreaTypeWall) physicalMapAreaType = "wall";
		else if(core->physicalMap->getAreaType(x, y) == PhysicalMap::PhysicalMapAreaTypeOpen) physicalMapAreaType = "open";
		else if(core->physicalMap->getAreaType(x, y) == PhysicalMap::PhysicalMapAreaTypeStairs) physicalMapAreaType = "stairs";
		else if(core->physicalMap->getAreaType(x, y) == PhysicalMap::PhysicalMapTypeUnreachable) physicalMapAreaType = "unreachable";

		// Get GPS coordinates based on an offset added with the degrees based on x,y meters at that offset
		GPSPosition gps = Util::gpsPositionFromString(core->stringSetting("Map_GPSOffset"));
		double x_m = x / 1000.0; // meters
		double y_m = y / 1000.0; // meters
		gps.latitude += gps.latitudesPerMeters(x_m);
		gps.longitude += gps.longitudesPerMeters(y_m);

		// Dump map info's
		Debug::print("[Interface] Local position: (%1,%2) mm", (int)x, (int)y);
		Debug::print("[Interface] Global position: (%1,%2)", gps.latitude, gps.longitude);
		Debug::print("[Interface] Collision Area Heat: %1", core->heatMap->getChannelValue(HeatMap::CollisionAreaChannel, x, y));
		Debug::print("[Interface] Open Area Heat: %1", core->heatMap->getChannelValue(HeatMap::OpenAreaChannel, x, y));
		Debug::print("[Interface] Physical Map: %1", physicalMapAreaType);
		Debug::print("[Interface] Terrain Map: %1", core->terrainMap->getHeight(x,y));

	} else if(value == "findrobot") {

		viewport->focusOnPoint(core->tracker->getX(), core->tracker->getY());
		viewport->resetToolbar();

	} else if(value == "raiseterrain") {

		core->terrainMap->raise(x, y);

	} else if(value == "lowerterrain") {

		core->terrainMap->lower(x, y);

	} else if(value == "moverobot") {

		core->tracker->setTransformation(core->tracker->getTransformation().inv() * core->tracker->getTransformation() * Trafo2D::trans(x,y));
		core->robotMoved(core->tracker->getTranslation().x(),core->tracker->getTranslation().y(),core->tracker->getRotation());
		viewport->resetToolbar();

	} else if(value == "rotaterobot") {

		core->tracker->setTransformation(core->tracker->getTransformation() * Trafo2D::rot(Rad(30)));
		core->robotMoved(core->tracker->getTranslation().x(),core->tracker->getTranslation().y(),core->tracker->getRotation());
		viewport->resetToolbar();

	} else if(value == "navigationtask") {

		core->addTask(new NavigationTask(core, this->sliderSpeed->value()));
		viewport->resetToolbar();

	}

}


void Interface::refreshSensors() {


	txtSensorData->clear();
	int* data = core->coil->getAllSensors();
	if(data != NULL) {

		txtSensorData->append(QString("BUMPS_AND_WHEEL_DROPS:\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_BUMPS_AND_WHEEL_DROPS)));
		txtSensorData->append(QString("WALL:\t\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_WALL)));
		txtSensorData->append(QString("CLIFF_LEFT:\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_LEFT)));
		txtSensorData->append(QString("CLIFF_FRONT_LEFT:\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_FRONT_LEFT)));
		txtSensorData->append(QString("CLIFF_FRONT_RIGHT:\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_FRONT_RIGHT)));
		txtSensorData->append(QString("CLIFF_RIGHT:\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_RIGHT)));
		txtSensorData->append(QString("VIRTUAL_WALL:\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_VIRTUAL_WALL)));
		txtSensorData->append(QString("OVERCURRENT:\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_OVERCURRENT)));
		txtSensorData->append(QString("INFRARED:\t\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_INFRARED)));
		txtSensorData->append(QString("BUTTONS:\t\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_BUTTONS)));
		txtSensorData->append(QString("DISTANCE:\t\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_DISTANCE)));
		txtSensorData->append(QString("ANGLE:\t\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_ANGLE)));
		txtSensorData->append(QString("CHARGING_STATE:\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_CHARGING_STATE)));
		txtSensorData->append(QString("VOLTAGE:\t\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_VOLTAGE)));
		txtSensorData->append(QString("CURRENT:\t\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_CURRENT)));
		txtSensorData->append(QString("BATTERY_TEMP:\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_BATTERY_TEMP)));
		txtSensorData->append(QString("BATTERY_CHARGE:\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_BATTERY_CHARGE)));
		txtSensorData->append(QString("BATTERY_CAPACITY:\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_BATTERY_CAPACITY)));
		txtSensorData->append(QString("WALL_SIGNAL:\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_WALL_SIGNAL)));
		txtSensorData->append(QString("CLIFF_LEFT_SIGNAL:\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_LEFT_SIGNAL)));
		txtSensorData->append(QString("CLIFF_FRONT_LEFT_SIGNAL:\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_FRONT_LEFT_SIGNAL)));
		txtSensorData->append(QString("CLIFF_FRONT_RIGHT_SIGNAL:\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_FRONT_RIGHT_SIGNAL)));
		txtSensorData->append(QString("CLIFF_RIGHT_SIGNAL:\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_RIGHT_SIGNAL)));
		txtSensorData->append(QString("DIGITAL_INPUTS:\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_DIGITAL_INPUTS)));
		txtSensorData->append(QString("ANALOG_SIGNAL:\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_ANALOG_SIGNAL)));
		txtSensorData->append(QString("CHARGING_SOURCES_AVAILABLE:\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_CHARGING_SOURCES_AVAILABLE)));
		txtSensorData->append(QString("OI_MODE:\t\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_OI_MODE)));
		txtSensorData->append(QString("SONG_NUMBER:\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_SONG_NUMBER)));
		txtSensorData->append(QString("SONG_IS_PLAYING:\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_SONG_IS_PLAYING)));
		txtSensorData->append(QString("NUM_STREAM_PACKETS:\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_NUM_STREAM_PACKETS)));
		txtSensorData->append(QString("REQUESTED_VELOCITY:\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_REQUESTED_VELOCITY)));
		txtSensorData->append(QString("REQUESTED_RADIUS:\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_REQUESTED_RADIUS)));
		txtSensorData->append(QString("REQUESTED_RIGHT_VEL:\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_REQUESTED_RIGHT_VEL)));
		txtSensorData->append(QString("REQUESTED_LEFT_VEL:\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_REQUESTED_LEFT_VEL)));
		txtSensorData->append(QString("BATTERY_CHARGE:\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_BATTERY_CHARGE)));
		txtSensorData->append(QString("BATTERY_CAPACITY:\t\t%1").arg(core->coil->extractSensorFromData(data, COIL::SENSOR_BATTERY_CAPACITY)));

		free(data);

	}

}

void Interface::focusOnPoint(long x, long y){
	foreach(Viewport *viewport, *viewports){
		if(viewport) viewport->autoFocus(core->tracker->getX(), core->tracker->getY());
	}
}

void Interface::viewportClosed(Viewport *viewport) {
	if(!ignoreViewportClosedSignal) viewports->removeAll(viewport);
}

void Interface::closeEvent(QCloseEvent *event) {

	// Close all open viewports...
	ignoreViewportClosedSignal = true;
	for(int i = 0; i < viewports->count(); i++) {
		if(viewports->at(i)) viewports->at(i)->close();
	}
	viewports->clear();
	ignoreViewportClosedSignal = false;

	// Disconnect
	if (core && core->isConnected()) connectDisconnect();

	event->accept();
}

