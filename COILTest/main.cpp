/*
 *  main.cpp
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

#include <QtCore>
#include <QCoreApplication>

#include "../Core/Core.h"
#include "../Core/COIL/COIL.h"
#include "../Core/Library/Debug.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Debug::setOutput(NULL);
    Debug::print("Opening port...");

    COIL coil("COM4");
    coil.startOI ();
	coil.enterSafeMode ();

	Debug::print("Port openend.");

	//waits a sec
	//coil.waitTime(1000);

	//simple tests

	// LED test
	//for(int color = 0; color < 255; color++) {
	//	coil.setLEDState(QCOIL::LED_PLAY, color, 255);
	//	//coil.waitTime(100);
	//}

	// 100mm straight forward
	//&int result = coil.driveDistance(100,0,100,0);
	//QString s;
	//s.setNum(result);
	//Debug::print("Distance: " + s);

	// 1000mm straight forward
	//int result = coil.driveDistance(100,0,1000,0);
	//QString s;
	//s.setNum(result);
	//Debug::print("Distance: " + s);

	// 100mm straight forward
	//int result = coil.driveDistance(100,0,100,0);
	//QString s;
	//s.setNum(result);
	//Debug::print("Distance: " + s);

	// 10mm straight forward
	//int result = coil.driveDistance(100,0,10,0);
	//QString s;
	//s.setNum(result);
	//Debug::print("Distance: " + s);

	// turn 90 degrees
	//int result = coil.turn(100,1,90,0);
	//QString s;
	//s.setNum(result);
	//Debug::print("Angle turned: " + s);


	// 100mm forward and then 100mm backwards
	QString s1,s2;
	int result1 = coil.driveDistance(100,0,100,0);
	int result2 = coil.driveDistance(-100,0,100,0);
	Debug::print("Distance forward: " + s1.setNum(result1) + "\nDistance backward: " + s2.setNum(result2));

	// drive in a circle
	//int result = coil.turn(100,50,360,0);
	//QString s;
	//s.setNum(result);
	//Debug::print("Angle turned: " + s);

    // Drive in a square test
	/*
	int turns = 0;
	while (turns < 4)
	{
		coil.driveDistance (300, 0, 1000, 1);
		coil.turn (200, 1, 90, 1);
		turns++;
	}
	*/

	Debug::print("Stopping COIL...");

	coil.stopOI();

	Debug::print("stopped");


    return 1;
}
