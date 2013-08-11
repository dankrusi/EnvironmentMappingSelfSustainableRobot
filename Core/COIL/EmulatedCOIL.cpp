/*
 * EmulatedCOIL.cpp
 *
 *  Created on: 24.02.2009
 *      Author: dgrob
 */


#include "EmulatedCOIL.h"

#include "../Library/Math.h"
#include "../Tracker/Tracker.h"
#include "../Map/PhysicalMap.h"
#include "../Library/SleeperThread.h"
#include "../Object/MarkerObject.h"


EmulatedCOIL::EmulatedCOIL(Core *core) : COIL(core)  {
	this->name = QString("EmulatedCOIL");
	emulatedLwheel = 0;
	emulatedRwheel = 0;
	deltaChangedAngle = 0.0;
	deltaChangedDistance = 0.0;
	lastGetDistanceTimestamp = QTime();
	lastGetAngleTimestamp = QTime();
	mode = OI_MODE_OFF;
}

EmulatedCOIL::~EmulatedCOIL(){

}

int EmulatedCOIL::startOI ()
{
	startTime.restart();
	mode = OI_MODE_PASSIVE;
	return 0;
}

int EmulatedCOIL::setBaud (BaudRateType rate)
{
	return 0;
}

int EmulatedCOIL::enterPassiveMode()
{
	mode = OI_MODE_PASSIVE;
	return 0;
}

int EmulatedCOIL::enterSafeMode ()
{
	mode = OI_MODE_SAFE;
	return 0;
}

int EmulatedCOIL::enterFullMode ()
{
	mode = OI_MODE_FULL;
	return 0;
}

int EmulatedCOIL::runDemo (oi_demo demo)
{
	Debug::warning("[EmulatedCoil] runDemo not implemented!");
	return 0;
}

int EmulatedCOIL::runCoverDemo ()
{
	Debug::warning("[EmulatedCoil] runCoverDemo not implemented!");
	return 0;
}

int EmulatedCOIL::runCoverAndDockDemo ()
{
	Debug::warning("[EmulatedCoil] runCoverAndDockDemo not implemented!");
	return 0;
}

int EmulatedCOIL::runSpotDemo ()
{
	Debug::warning("[EmulatedCoil] runSpotDemo not implemented!");
	return 0;
}


int EmulatedCOIL::drive (short vel, short radius)
{
	if (vel > 500) vel = 500;
	if (vel < -500) vel = -500;

	if(radius < 0) radius = 0;

	int offset = core->doubleSetting("Robot_WheelOffset_mm") * 2;

	emulatedLwheel = vel;
	if (radius != 0){
		emulatedRwheel = ((emulatedLwheel * offset) / radius) + emulatedLwheel;
	}
	else {
		emulatedRwheel = vel;
	}

	return 0;
}

int EmulatedCOIL::directDrive (short Lwheel, short Rwheel)
{
	if (Lwheel > 500) Lwheel = 500;
	if (Rwheel > 500) Rwheel = 500;

	if (Lwheel < -500) Lwheel = -500;
	if (Rwheel < -500) Rwheel = -500;

	emulatedLwheel = Lwheel;
	emulatedRwheel = Rwheel;

	return 0;
}



int EmulatedCOIL::setLEDState (short lflags, byte pColor, byte pInten)
{
	return 0;
}

int EmulatedCOIL::setDigitalOuts (oi_output oflags)
{
	Debug::warning("[EmulatedCoil] setDigitalOuts not implemented!");
	return 0;
}

int EmulatedCOIL::setPWMLowSideDrivers (byte pwm0, byte pwm1, byte pwm2)
{
	Debug::warning("[EmulatedCoil] setPWMLowSideDrivers not implemented!");
	return 0;
}

int EmulatedCOIL::setLowSideDrivers (oi_output oflags)
{
	Debug::warning("[EmulatedCoil] setLowSideDrivers not implemented!");
	return 0;
}

int EmulatedCOIL::sendIRbyte (byte irbyte)
{
	Debug::warning("[EmulatedCoil] sendIRbyte not implemented!");
	return 0;
}

int EmulatedCOIL::writeSong (byte number, byte length, byte* song)
{
	Debug::warning("[EmulatedCoil] writeSong not implemented!");
	return 0;
}

int EmulatedCOIL::playSong (byte number)
{
	Debug::warning("[EmulatedCoil] playSong not implemented!");
	return 0;
}

int EmulatedCOIL::readRawSensor (oi_sensor packet, byte* buffer, int size)
{
	Debug::warning("[EmulatedCoil] readRawSensor not implemented!");
	return 0;
}

int EmulatedCOIL::readSensor (oi_sensor packet)
{
	int result = 0;
	int maxMinutes = 1; // TODO: refactor to settings...
	int maxMilliseconds = maxMinutes * 60 * 1000;
	Trafo2D cliffPosition;

//	if(i%10 == 0){
//		//Debug::print("[EmulatedCoil]CliffPosition x: %1, y: %2", cliffPositionFrontLeft.trans().x()/core->scale, cliffPositionFrontLeft.trans().y()/core->scale);
//		//Debug::print("[EmulatedCoil]RobotPosition x: %1, y: %2", core->movementTracker->transformation.trans().x(), core->movementTracker->transformation.trans().y());
//
//	}
		switch (packet)
		{
			case SENSOR_BUMPS_AND_WHEEL_DROPS:
				result = getBumpsAndWheelDrops();
				break;
			case SENSOR_WALL:
				result = 0; // TODO: implement this
				break;
			case SENSOR_CLIFF_LEFT:
				cliffPosition = core->tracker->getTransformation() * Trafo2D::rot(Rad(core->intSetting("Robot_SideCliffSensorPositionAngle")));
				cliffPosition = cliffPosition * Trafo2D::trans(0,core->intSetting("Robot_BumperCollisionOffset_mm"));
				if (core->physicalMap->getAreaType(cliffPosition.trans().x(), cliffPosition.trans().y()) == PhysicalMap::PhysicalMapAreaTypeStairs){
					result = 1;
				}
				break;
			case SENSOR_CLIFF_FRONT_LEFT:
				cliffPosition = core->tracker->getTransformation() * Trafo2D::rot(Rad(core->intSetting("Robot_FrontCliffSensorPositionAngle")));
				cliffPosition = cliffPosition * Trafo2D::trans(0,core->intSetting("Robot_BumperCollisionOffset_mm"));
				if (core->physicalMap->getAreaType(cliffPosition.trans().x(), cliffPosition.trans().y()) == PhysicalMap::PhysicalMapAreaTypeStairs){
					result = 1;
				}
				break;
			case SENSOR_CLIFF_FRONT_RIGHT:
				cliffPosition = core->tracker->getTransformation() * Trafo2D::rot(Rad(-core->intSetting("Robot_FrontCliffSensorPositionAngle")));
				cliffPosition = cliffPosition * Trafo2D::trans(0,core->intSetting("Robot_BumperCollisionOffset_mm"));
				if (core->physicalMap->getAreaType(cliffPosition.trans().x(), cliffPosition.trans().y()) == PhysicalMap::PhysicalMapAreaTypeStairs){
					result = 1;
				}
				break;
			case SENSOR_CLIFF_RIGHT:
				cliffPosition = core->tracker->getTransformation() * Trafo2D::rot(Rad(-core->intSetting("Robot_SideCliffSensorPositionAngle")));
				cliffPosition = cliffPosition * Trafo2D::trans(0,core->intSetting("Robot_BumperCollisionOffset_mm"));
				if (core->physicalMap->getAreaType(cliffPosition.trans().x(), cliffPosition.trans().y()) == PhysicalMap::PhysicalMapAreaTypeStairs){
					result = 1;
				}
				break;
			case SENSOR_VIRTUAL_WALL:
				result = 0;
				break;
			case SENSOR_OVERCURRENT:
				result = 0;
				break;
			case SENSOR_INFRARED:
				result = 0;
				break;
			case SENSOR_BUTTONS:
				result = 0;
				break;
			case SENSOR_CHARGING_STATE:
				result = 0;
				break;
			case SENSOR_DIGITAL_INPUTS:
				result = 0;
				break;
			case SENSOR_CHARGING_SOURCES_AVAILABLE:
				result = 0;
				break;
			case SENSOR_OI_MODE:
				result =  mode;
				break;
			case SENSOR_SONG_NUMBER:
				result = 0;
				break;
			case SENSOR_SONG_IS_PLAYING:
				result = 0;
				break;
			case SENSOR_NUM_STREAM_PACKETS:
				result = 0;
				break;
			case SENSOR_BATTERY_TEMP:
				result = 0;
				break;
			case SENSOR_VOLTAGE:
				result = 0;
				break;
			case SENSOR_BATTERY_CHARGE:
				result = (int)((1.0 - (double)startTime.elapsed()/(double)maxMilliseconds) * 100);
				if(result < 0) result = 0;
				break;
			case SENSOR_BATTERY_CAPACITY:
				result = 100;
				break;
			case SENSOR_WALL_SIGNAL:
				result = 0;
				break;
			case SENSOR_CLIFF_LEFT_SIGNAL:
				//Not implemented
				result = 0;
				break;
			case SENSOR_CLIFF_FRONT_LEFT_SIGNAL:
				//Not implemented
				result = 0;
				break;
			case SENSOR_CLIFF_FRONT_RIGHT_SIGNAL:
				//Not implemented
				result = 0;
				break;
			case SENSOR_CLIFF_RIGHT_SIGNAL:
				//Not implemented
				result = 0;
				break;
			case SENSOR_ANALOG_SIGNAL:
				result = 0; // Not needed as getIRDistanceFromAnalogSignal just checks the map...
				break;
			case SENSOR_DISTANCE:
				result = (int) getDistance();
				break;
			case SENSOR_ANGLE:
				result = (int) getAngle();
				break;
			case SENSOR_CURRENT:
				result = 0;
				break;
			case SENSOR_REQUESTED_VELOCITY:
				result = getVelocity();
				break;
			case SENSOR_REQUESTED_RADIUS:
				result = getTurningRadius();
				break;
			case SENSOR_REQUESTED_RIGHT_VEL:
				result = emulatedRwheel;
				break;
			case SENSOR_REQUESTED_LEFT_VEL:
				result = emulatedLwheel;
				break;

			//any other input is invalid (including packet groups)
			default:
				return 0;
		}

		return result;
}

int EmulatedCOIL::getDistance ()
{
	// Get the elapsed time since last call
	double elapsedTimeSinceGetDistance = (double)lastGetDistanceTimestamp.restart();

	// We need to return a value (mm) based on the wheel speed (mm/s) and elapsed time (ms)...
	double wheelSpeed = ((double)emulatedLwheel + (double)emulatedRwheel) / 2.0; // Average it out
	double distance = (wheelSpeed * (elapsedTimeSinceGetDistance / 1000.0));

	// Add the value to our delta value. This is used internally to
	// compensate for OIL's use of integers where rounding goes to
	// zero if polled to often. So we store the value here until
	// it reaches > 1.0 and then reset it :).
	deltaChangedDistance += distance;
	if(deltaChangedDistance >= 1.0 || deltaChangedDistance <= -1.0) {
		// Return as int cast and reset the delta value
		int ret = (int)deltaChangedDistance;
		deltaChangedDistance -= ret; // Include the rest here
		return ret;
	} else {
		// Signal as a zero change until deltaChangedDistance >= 1.0
		return 0;
	}
}

int EmulatedCOIL::getAngle ()
{

	// Get the elapsed time since last call
	double elapsedTimeSinceGetAngle = (double)lastGetAngleTimestamp.restart();
	double changeInAngle = 0.0;

	// Calculate change of angle using physics per [KrusiGrob08] pg 36...
	if (emulatedLwheel != emulatedRwheel) {
		double d = core->doubleSetting("Robot_WheelOffset_mm") * 2;
		double angleAlpha;
		if(emulatedLwheel != 0) {
			// Based the radius of the circular motion on the left wheel. If emulatedLwheel is zero,
			// then we cannot do this because it is not a circle and the change in angle is infinately large...
			double arclenL = emulatedLwheel * (elapsedTimeSinceGetAngle / 1000.0);
			double radiusL = (emulatedLwheel*d) / (emulatedRwheel-emulatedLwheel);
			angleAlpha = Deg(arclenL / radiusL);
		} else {
			// If we are here, then emulatedLwheel is zero so we simplify angleAlpgha accordingly
			double arclenR = emulatedRwheel * (elapsedTimeSinceGetAngle / 1000.0);
			angleAlpha = Deg(arclenR / d);
		}
		double angleBeta = 180.0 - 90.0 - angleAlpha;
		double angleGamma = 90.0 - angleBeta;
		changeInAngle = angleGamma;
	}


	// Add the value to our delta value. This is used internally to
	// compensate for OIL's use of integers where rounding goes to
	// zero if polled to often. So we store the value here until
	// it reaches > 1.0 and then reset it :).
	deltaChangedAngle += changeInAngle;
	if(deltaChangedAngle >= 1.0 || deltaChangedAngle <= -1.0) {
		// Return as int cast and reset the delta value
		int ret = (int)deltaChangedAngle;
		deltaChangedAngle -= ret; // Include the rest here
		return ret;
	} else {
		// Signal as a zero change until deltaChangedDistance >= 1.0
		return 0;
	}


}

int EmulatedCOIL::getVelocity ()
{
	// According to manual...
	return (emulatedLwheel+emulatedRwheel)/2;
}


int EmulatedCOIL::getTurningRadius ()
{
	if (emulatedLwheel == emulatedRwheel) return 0;
	int offset = core->doubleSetting("Robot_WheelOffset_mm") * 2;
	return abs((emulatedLwheel * offset) / ( emulatedRwheel - emulatedLwheel));
}


int EmulatedCOIL::getBumpsAndWheelDrops ()
{
	int result = 0;
	int leftSideCliffAngle = core->intSetting("Robot_SideCliffSensorPositionAngle");
	int rightSideCliffAngle = - leftSideCliffAngle;
	int leftFrontCliffAngle = core->intSetting("Robot_FrontCliffSensorPositionAngle");
	int rightFrontCliffAngle = - leftFrontCliffAngle;

	// Check left bumper
	for(int i = leftSideCliffAngle; i > leftFrontCliffAngle; i--){
		if( checkBumpers(i) ){
				result |= BUMPWHEELDROP_BUMP_LEFT;
				break;
		}
	}

	// Check right bumper
	for(int i = rightSideCliffAngle; i < rightFrontCliffAngle; i++){
		if( checkBumpers(i) ){
				result |= BUMPWHEELDROP_BUMP_RIGHT;
				break;
		}
	}


	// Check both bumper
	if( result != BUMPWHEELDROP_BUMP_LEFT_AND_RIGHT ){
		for(int i = rightFrontCliffAngle; i <= rightSideCliffAngle; i++){
			if( checkBumpers(i) ){
				result |= BUMPWHEELDROP_BUMP_LEFT_AND_RIGHT;
				break;
			}
		}
	}

	// Check left wheel drop
	Trafo2D leftWheelPosition = core->tracker->getTransformation()  * Trafo2D::trans(-core->doubleSetting("Robot_WheelOffset_mm"),0);
	if (core->physicalMap->getAreaType(leftWheelPosition.trans().x(), leftWheelPosition.trans().y()) == PhysicalMap::PhysicalMapAreaTypeStairs){
		result |= BUMPWHEELDROP_WHEELDROP_LEFT;
	}

	// Check right wheel drop
	Trafo2D rightWheelPosition = core->tracker->getTransformation()  * Trafo2D::trans(core->doubleSetting("Robot_WheelOffset_mm"),0);
	if (core->physicalMap->getAreaType((long)rightWheelPosition.trans().x(), (long)rightWheelPosition.trans().y()) == PhysicalMap::PhysicalMapAreaTypeStairs){
		result |= BUMPWHEELDROP_WHEELDROP_RIGHT;
	}

	return result;
}


int EmulatedCOIL::getCliffs ()
{
	//TODO: Implement this using a different color on the physical map :)
	return 0;
}

int EmulatedCOIL::getIRSensorDistanceFromAnalogSignal (int analogSignal)
{
	// We start at 0 and scan to the set cuttoff value for a object collision on the physical map
	for(int distanceFromRobot = 0; distanceFromRobot < core->intSetting("Robot_SharpIRSensorCutoffValue"); distanceFromRobot++) {
		for (int angle = -3; angle <=3; angle++){
			Trafo2D collisionPoint = core->tracker->getTransformation() * Trafo2D::rot(Rad(angle)) * Trafo2D::trans(0, distanceFromRobot);
			if(core->physicalMap->getAreaType((long)collisionPoint.trans().x(), (long)collisionPoint.trans().y()) == PhysicalMap::PhysicalMapAreaTypeWall) return distanceFromRobot;
		}
	}
	return core->intSetting("Robot_SharpIRSensorCutoffValue");
}

int EmulatedCOIL::getWallSensorDistanceFromSignal (int signal){
	//TODO: cleanup and incorporate in settings...
	for (int i = 0; i <= core->doubleSetting("Robot_WallSensorRange_mm"); i+= 10){
		Trafo2D wallPosition = core->tracker->getTransformation()  * Trafo2D::trans(core->doubleSetting("Robot_WheelOffset_mm")+i ,80);
		if (core->physicalMap->getAreaType((long) wallPosition.trans().x(),(long) wallPosition.trans().y()) == PhysicalMap::PhysicalMapAreaTypeWall){
			return i;
		}
	}
	return INT_MAX;
}


int EmulatedCOIL::readRawSensorList (oi_sensor* packet_list, byte num_packets, byte* buffer, int size)
{
	Debug::warning("[EmulatedCoil] readRawSensorList not implemented!");
	return 0;
}


int EmulatedCOIL::writeScript (byte* script, byte size)
{
	Debug::warning("[EmulatedCoil] writeScript not implemented!");
	return 0;
}


int EmulatedCOIL::playScript ()
{
	Debug::warning("[EmulatedCoil] playScript not implemented!");
	return 0;
}


byte* EmulatedCOIL::getScript ()
{
	Debug::warning("[EmulatedCoil] getScript not implemented!");
	return NULL;
}


int EmulatedCOIL::stopWait()
{
  int shouldStop = 0;
  shouldStop = readSensor(SENSOR_BUMPS_AND_WHEEL_DROPS) + readSensor(SENSOR_OVERCURRENT) + getCliffs();
  return shouldStop;
}


int EmulatedCOIL::stopOI ()
{
	mode = OI_MODE_OFF;
	return 0;
}


int* EmulatedCOIL::getAllSensors(int* result)
{
	if (NULL == result)
	{
		Debug::error ("[COIL] Could not get all sensors:  Memory allocation failed");
		return NULL;
	}

	// Lock this to make sure that extractSensorData() doesn't collide from
	// another thread. Not entirely clean as the data source (*result) could
	// vary from different sources, but typically will never be the case.
	_mutex.lock(); {

		memset (result, 0, 36*sizeof(int)); // zero-out

		result[0] = readSensor(SENSOR_BUMPS_AND_WHEEL_DROPS);
		result[1] = readSensor(SENSOR_WALL);
		result[2] = readSensor(SENSOR_CLIFF_LEFT);
		result[3] = readSensor(SENSOR_CLIFF_FRONT_LEFT);
		result[4] = readSensor(SENSOR_CLIFF_FRONT_RIGHT);
		result[5] = readSensor(SENSOR_CLIFF_RIGHT);
		result[6] = readSensor(SENSOR_VIRTUAL_WALL);
		result[7] = readSensor(SENSOR_OVERCURRENT);
		result[8] = 0; //Sensor unused
		result[9] = 0; //Sensor unused
		result[10] = readSensor(SENSOR_INFRARED);
		result[11] = readSensor(SENSOR_BUTTONS);
		result[12] = readSensor(SENSOR_DISTANCE);
		result[13] = readSensor(SENSOR_ANGLE);
		result[14] = readSensor(SENSOR_CHARGING_STATE);
		result[15] = readSensor(SENSOR_VOLTAGE);
		result[16] = readSensor(SENSOR_CURRENT);
		result[17] = readSensor(SENSOR_BATTERY_TEMP);
		result[18] = readSensor(SENSOR_BATTERY_CHARGE);
		result[19] = readSensor(SENSOR_BATTERY_CAPACITY);
		result[20] = readSensor(SENSOR_WALL_SIGNAL);
		result[21] = readSensor(SENSOR_CLIFF_LEFT_SIGNAL);
		result[22] = readSensor(SENSOR_CLIFF_FRONT_LEFT_SIGNAL);
		result[23] = readSensor(SENSOR_CLIFF_FRONT_RIGHT_SIGNAL);
		result[24] = readSensor(SENSOR_CLIFF_RIGHT_SIGNAL);
		result[25] = readSensor(SENSOR_DIGITAL_INPUTS);
		result[26] = readSensor(SENSOR_ANALOG_SIGNAL);
		result[27] = readSensor(SENSOR_CHARGING_SOURCES_AVAILABLE);
		result[28] = readSensor(SENSOR_OI_MODE);
		result[29] = readSensor(SENSOR_SONG_NUMBER);
		result[30] = readSensor(SENSOR_SONG_IS_PLAYING);
		result[31] = readSensor(SENSOR_NUM_STREAM_PACKETS);
		result[32] = readSensor(SENSOR_REQUESTED_VELOCITY);
		result[33] = readSensor(SENSOR_REQUESTED_RADIUS);
		result[34] = readSensor(SENSOR_REQUESTED_RIGHT_VEL);
		result[35] = readSensor(SENSOR_REQUESTED_LEFT_VEL);

	} _mutex.unlock();

	return result;
}


bool EmulatedCOIL::checkBumpers(int angle){
	Trafo2D bumperPosition = core->tracker->getTransformation() * Trafo2D::rot(Rad(angle)) * Trafo2D::trans(0,core->intSetting("Robot_BumperCollisionOffset_mm"));
	if (core->physicalMap->getAreaType(bumperPosition.trans().x(), bumperPosition.trans().y()) == PhysicalMap::PhysicalMapAreaTypeWall){
		return true;
	}
	return false;
}



