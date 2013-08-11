/*
 * EmulatedCOIL.h
 *
 *  Created on: 24.02.2009
 *      Author: dgrob
 */

#ifndef EMULATEDCOIL_H_
#define EMULATEDCOIL_H_

#include "COIL.h"

#include <QTime>

class EmulatedCOIL : public COIL {

public:
	EmulatedCOIL(Core *core);
	~EmulatedCOIL();

private:
	int emulatedLwheel;
	int emulatedRwheel;
	double deltaChangedAngle; // This the amount of angle change as double, used internally to compensate for OIL's use of integers where rounding goes to zero if polled to often. So we store the value here until it reaches > 1.0 and then reset it :).
	double deltaChangedDistance; // This the amount of distance change as double, used internally to compensate for OIL's use of integers where rounding goes to zero if polled to often. So we store the value here until it reaches > 1.0 and then reset it :).
	QTime lastGetDistanceTimestamp;
	QTime lastGetAngleTimestamp;
	oi_mode mode;
	QTime startTime;

public:
    int startOI ();
    int setBaud (BaudRateType rate);
    int enterPassiveMode ();
    int enterSafeMode ();
    int enterFullMode ();
    int runDemo (oi_demo demo);
    int runCoverDemo ();
    int runCoverAndDockDemo ();
    int runSpotDemo ();
    int drive (short vel, short rad);
    int directDrive (short Lwheel, short Rwheel);
    int setLEDState (short lflags, byte pColor, byte pInten);
    int setDigitalOuts (oi_output oflags);
    int setPWMLowSideDrivers (byte pwm0, byte pwm1, byte pwm2);
    int setLowSideDrivers (oi_output oflags);
    int sendIRbyte (byte irbyte);
    int writeSong (byte number, byte length, byte* song);
    int playSong (byte number);
    int readRawSensor (oi_sensor packet, byte* buffer, int size);
    int readSensor (oi_sensor packet);
    int getDistance ();
    int getAngle ();
    int getVelocity ();
    int getTurningRadius ();
    int getBumpsAndWheelDrops ();
    int getCliffs ();
    int getIRSensorDistanceFromAnalogSignal (int analogSignal);
    int getWallSensorDistanceFromSignal (int signal);
    int readRawSensorList (oi_sensor* packet_list, byte num_packets, byte* buffer, int size);
    int writeScript (byte* script, byte size);
    int playScript ();
    byte* getScript ();
    int stopWait ();
    int stopOI ();
	int* getAllSensors(int* data);

private:
	bool checkBumpers(int angle);

};

#endif /* EMULATEDCOIL_H_ */
