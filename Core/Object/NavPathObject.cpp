/*
 *  NavPathObject.cpp
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

#include "NavPathObject.h"

#include "../Map/Map.h"

NavPathObject::NavPathObject(ObjectMap *map, QColor color) : Object(map, "Nav Path", 0, 0, 0, 0)
{
	this->color = color;
	pathCount = 0;
}

void NavPathObject::paint(QPainter &painter, QRect view, int scale)
{
	// Translate, scale, and rotate...
	QTransform t;
	t.scale(1.0/scale, 1.0/scale);
	t.translate(x - view.x()*scale, view.height()*scale - y + view.y()*scale);
	painter.setTransform(t);

	painter.setBrush(Qt::transparent);
	QPen pen(this->color);
	pen.setWidth(2*scale);
	painter.setPen(pen);
	painter.drawPath(path);
}

void NavPathObject::addNavPoint(long x, long y) {

	if(x > width) width = x;
	if(y > height) height = y;

	y = -y; // Invert coordinates for our special y-axis...
	if(pathCount == 0) path.moveTo(x, y);
	path.lineTo(x, y);
	pathCount++;

}
