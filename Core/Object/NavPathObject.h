/*
 *  NavPathObject.h
 *
 *  Draws a navigation path which the robot has driven :).
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

#ifndef NAVPATHOBJECT_H_
#define NAVPATHOBJECT_H_

#include "Object.h"

#include <QPainterPath>
#include <QColor>

class NavPathObject : public Object
{

	Q_OBJECT

private:
	QPainterPath path;
	QColor color;
	long pathCount;

public:
	NavPathObject(ObjectMap *map, QColor color = Qt::blue);
	virtual ~NavPathObject() {};
	virtual void paint(QPainter &painter, QRect view, int scale);

public slots:
	void addNavPoint(long x, long y);

};

#endif /* NAVPATHOBJECT_H_ */
