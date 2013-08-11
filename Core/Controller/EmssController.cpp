/*
 *  EmssController.cpp
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

#include "EmssController.h"

#include "../COIL/COIL.h"

#include "../Library/Debug.h"
#include "../Library/SleeperThread.h"

#include "../Watchdog/ThreadMonitorWatchdogAction.h"

#include "../Map/HeatMap.h"

#include "../Tracker/Tracker.h"


EmssController::EmssController(Core *core, int speed, int interval) :
	Controller("emss", core, interval) {

	targetSpeed = speed;
	yokeX = 0.0;
	yokeY = 0.0;
	angleToTurn = 0.0;
	angleTurned = 0.0;
	distanceToMove = 0.0;
	distanceMoved = 0.0;
	mode = Idle;

}

EmssController::~EmssController() {

}

void EmssController::process() {

	// Determine wheel speeds
	if (mode == EmssController::Idle || mode == EmssController::EmergencyStop) {

		// Idle mode!

		Lwheel = 0;
		Rwheel = 0;


	} else if (mode == EmssController::WheelDrive) {

		// No change, just drive at current wheel values...
	}

	// Get all the sensor data...
	if(sensorData) {
		core->coil->getAllSensors(sensorData);
		emit signalSensorDataUpdated();
	}

	// Get movement...
	double distanceDelta = core->coil->extractSensorFromData(sensorData,COIL::SENSOR_DISTANCE);
	double angleDelta = core->coil->extractSensorFromData(sensorData,COIL::SENSOR_ANGLE);

	// Emit signals for movement tracker
	emit signalMovedDistance(distanceDelta);
	emit signalChangedAngle(angleDelta);

	// Get other sensor data
	int sharpIRSensor = core->coil->getIRSensorDistanceFromAnalogSignal(core->coil->extractSensorFromData(sensorData,COIL::SENSOR_ANALOG_SIGNAL));
	int wallIRSensor = core->coil->getWallSensorDistanceFromSignal(core->coil->extractSensorFromData(sensorData,COIL::SENSOR_WALL_SIGNAL));
	int bumpsWheelDrop = core->coil->extractSensorFromData(sensorData,COIL::SENSOR_BUMPS_AND_WHEEL_DROPS);
	bool cliffLeft = core->coil->extractSensorFromData(sensorData,COIL::SENSOR_CLIFF_LEFT);
	bool cliffFrontLeft = core->coil->extractSensorFromData(sensorData,COIL::SENSOR_CLIFF_FRONT_LEFT);
	bool cliffFrontRight = core->coil->extractSensorFromData(sensorData,COIL::SENSOR_CLIFF_FRONT_RIGHT);
	bool cliffRight = core->coil->extractSensorFromData(sensorData,COIL::SENSOR_CLIFF_RIGHT);
	bool isForwardsDirection = (Lwheel > 0 && Rwheel > 0);

	// Log movement
	distanceMoved += distanceDelta;
	angleTurned += angleDelta;

	// Drop detected?
	if (cliffLeft || cliffFrontLeft || cliffFrontRight || cliffRight) {
		int angle;
		if (cliffLeft){
			angle = core->intSetting("Robot_SideCliffSensorPositionAngle");
			emit signalObjectDetected(core->intSetting("Robot_BumperCollisionOffset_mm"), angle,core->doubleSetting("Map_HeatMap_CliffCollisionOpacity"),core->intSetting("Map_HeatMap_CliffCollisionSize_mm") );
		} else if (cliffFrontLeft){
			angle = core->intSetting("Robot_FrontCliffSensorPositionAngle");
			emit signalObjectDetected(core->intSetting("Robot_BumperCollisionOffset_mm"), angle,core->doubleSetting("Map_HeatMap_CliffCollisionOpacity"),core->intSetting("Map_HeatMap_CliffCollisionSize_mm") );
		} else if (cliffFrontRight){
			angle = -core->intSetting("Robot_FrontCliffSensorPositionAngle");
			emit signalObjectDetected(core->intSetting("Robot_BumperCollisionOffset_mm"), angle,core->doubleSetting("Map_HeatMap_CliffCollisionOpacity"),core->intSetting("Map_HeatMap_CliffCollisionSize_mm") );
		} else if (cliffRight){
			angle = -core->intSetting("Robot_SideCliffSensorPositionAngle");
			emit signalObjectDetected(core->intSetting("Robot_BumperCollisionOffset_mm"), angle,core->doubleSetting("Map_HeatMap_CliffCollisionOpacity"),core->intSetting("Map_HeatMap_CliffCollisionSize_mm") );
		}
		if(isForwardsDirection && core->boolSetting("Controller_EmssController_EmergencyStopEnabled")) _emergencyStop();
	}

	// Bumper Collision?
	if (((COIL::BUMPWHEELDROP_BUMP_LEFT & bumpsWheelDrop) == COIL::BUMPWHEELDROP_BUMP_LEFT) || ((COIL::BUMPWHEELDROP_BUMP_RIGHT & bumpsWheelDrop) == COIL::BUMPWHEELDROP_BUMP_RIGHT)) {
		int angle = 0;
		// both bumper?
		if (((COIL::BUMPWHEELDROP_BUMP_LEFT & bumpsWheelDrop) == COIL::BUMPWHEELDROP_BUMP_LEFT) && ((COIL::BUMPWHEELDROP_BUMP_RIGHT & bumpsWheelDrop) == COIL::BUMPWHEELDROP_BUMP_RIGHT)){
			angle = 0;
		}
		// only left bumper?
		else if ((COIL::BUMPWHEELDROP_BUMP_LEFT & bumpsWheelDrop) == COIL::BUMPWHEELDROP_BUMP_LEFT){
			angle = 45;
		}
		// only right bumper?
		else if ((COIL::BUMPWHEELDROP_BUMP_LEFT & bumpsWheelDrop) == COIL::BUMPWHEELDROP_BUMP_LEFT){
			angle = -45;
		}
		emit signalObjectDetected(core->intSetting("Robot_BumperCollisionOffset_mm"), angle ,core->doubleSetting("Map_HeatMap_BumperCollisionOpacity"),core->intSetting("Map_HeatMap_BumperCollisionSize_mm") );
		if(isForwardsDirection && core->boolSetting("Controller_EmssController_EmergencyStopEnabled")) _emergencyStop();
	}

	// Wall detected?
	if(wallIRSensor < core->doubleSetting("Robot_WallSensorRange_mm")) {
		emit signalObjectDetected(core->intSetting("Robot_Diameter_mm")/2 + wallIRSensor, -90, core->doubleSetting("Map_HeatMap_WallCollisionOpacity"),core->intSetting("Map_HeatMap_WallCollisionSize_mm") ); // Angle is -90 because the sensor points straight out to the right...
	}

	// Object detected?
	if (sharpIRSensor < core->intSetting("Robot_SharpIRSensorCutoffValue")) {
		emit signalObjectDetected(sharpIRSensor, 0, core->doubleSetting("Map_HeatMap_IRCollisionOpacity"),core->intSetting("Map_HeatMap_IRCollisionSize_mm") ); // Angle is 0 because it is straight ahead always!
		//if(isForwardsDirection && core->boolSetting("Controller_EmssController_EmergencyStopEnabled") && sharpIRSensor < core->intSetting("Controller_EmssController_SharpIRSensorEmergencyStopBuffer_mm")) _emergencyStop();
	}

	// Send wheel speeds to COIL
	if(mode != EmssController::EmergencyStop) {
		// Set new wheel speed
		core->coil->directDrive(Lwheel, Rwheel);
		emit signalChangedWheelSpeed(Lwheel, Rwheel);

		// Register on heat map as safe area
		core->heatMap->registerHeat(ColorMap::OpenAreaChannel, core->tracker->getX(), core->tracker->getY(), core->doubleSetting("Map_HeatMap_OpenAreaOpacity"), core->intSetting("Map_HeatMap_OpenAreaSize_mm"));
	}

}

void EmssController::_emergencyStop() {

	core->coil->directDrive(0, 0);
	mode = EmssController::EmergencyStop;
	emit signalChangedWheelSpeed(0, 0);
	Debug::warning("[EmssController] emergency stop!");

}


void EmssController::setWheelSpeed(int Lwheel, int Rwheel) {

	lock.lockForWrite(); {

		if(mode == EmssController::EmergencyStop && (Lwheel > 0 || Rwheel > 0) ) {
			lock.unlock();
			return;
		}

		this->mode = EmssController::WheelDrive;
		this->Lwheel = (short)Lwheel;
		this->Rwheel = (short)Rwheel;

	} lock.unlock();

}
