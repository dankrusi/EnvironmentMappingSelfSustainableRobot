/*
 *  EmssController.h
 *
 *  This Controller is the most important Controller in our project. Its main
 *  function is to receive the differential steering commands and to pass along
 *  sensor data to other Core components. It pass the steering information of
 *  each wheel to COIL and waits for the next process execution. Currently,
 *  certain features from both the Block Drive Controller and Fluid Drive Controller
 *  are implemented within the Emss Controller, creating a set of Emss Controller
 *  modes such as Idle, Move, Turn, Joystick, WheelDrive, EmergencyStop. However,
 *  the only mode not used for diagnostic purposes (i.e. manually driving out of a
 *  corner) is the WheelDrive mode.
 *
 *  ===========================================================================
 *
 *  Copyright 2008-2009 Daniel Kruesi (Dan Krusi) and David Grob
 *
 *  This file is part of the emms framework.
 *
 *  The emms framework is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The emms framework  is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software. If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef EMSSCONTROLLER_H_
#define EMSSCONTROLLER_H_


#include "Controller.h"

class EmssController: public Controller {

	Q_OBJECT

public:
	double yokeX;
	double yokeY;
	short Lwheel;
	short Rwheel;
	double angleToTurn;			// Note: this is double for extra precision during emulation calculation...
	double angleTurned;			// Note: this is double for extra precision during emulation calculation...
	double distanceToMove;		// Note: this is double for extra precision during emulation calculation...
	double distanceMoved;		// Note: this is double for extra precision during emulation calculation...
	enum { Idle, WheelDrive, EmergencyStop } mode;

public:
	EmssController(Core *core, int speed, int interval);
	~EmssController();
	virtual void process();

private:
	virtual void _emergencyStop();

public slots:
	virtual void setWheelSpeed(int Lwheel, int Rwheel);

};

#endif /* EMSSCONTROLLER_H_ */
