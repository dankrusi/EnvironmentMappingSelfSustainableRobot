/*
 *  JoystickWeight.h
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

#ifndef JOYSTICKWEIGHT_H_
#define JOYSTICKWEIGHT_H_

#include "Weight.h"

#include "../../Controller/EmssController.h"

#include "../../Library/Joystick2D.h"

class JoystickWeight : public Weight {

	Q_OBJECT

public:
	JoystickWeight(Core *core) : Weight("Joystick", core) {};

	virtual ~JoystickWeight() {};

	virtual void process(Vector2D &v) {
		double yokeX = core->joystick->getYokeX();
		double yokeY = core->joystick->getYokeY();


		if (yokeY == 0 && yokeX == 0){
			return;
		}
		else if (yokeY <= 0.06 && yokeY >= -0.06) {
			v = Vector2D( -yokeX, yokeX );
		} else {
			v = Vector2D(yokeY - (yokeY * yokeX), yokeY + (yokeY * yokeX));
		}
	}
};


#endif /* JOYSTICKWEIGHT_H_ */
