/*
 *  RobotObject.cpp
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

#include "RobotObject.h"

RobotObject::RobotObject(ObjectMap *map, long x, long y, long width, long height, QColor color, double opacity) : Object(map, "Robot", x, y, width, height) {
	this->color = color;
	this->opacity = opacity;
}

void RobotObject::paint(QPainter &painter, QRect view, int scale) {

	// Translate, scale, and rotate...
	QTransform t;
	t.scale(1.0/scale, 1.0/scale);
	t.translate(x - view.x()*scale, view.height()*scale - y + view.y()*scale);
	t.rotate(-rotation);
	painter.setTransform(t);


	painter.setOpacity(opacity);

	// Draw body
	painter.setBrush(QBrush(color));
	painter.drawEllipse(QRect(-width/2, -height/2, width, height));

	// Draw wheels
	int wheelWidth =  (int)( (1.0/5.0) * (double)width  );
	int wheelHeight = (int)( (1.0/4.0) * (double)height );
	painter.setBrush(QBrush(Qt::darkGray));
	painter.drawRect(QRect(-width/2, -wheelHeight/2, wheelWidth, wheelHeight));
	painter.drawRect(QRect(width/2 - wheelWidth, -wheelHeight/2, wheelWidth, wheelHeight));

	// Draw directon arrow
	painter.setPen(Qt::black);
	painter.setFont(QFont("Arial Black", 12*scale, QFont::Bold));
	painter.drawText(-4*scale, 6*scale, QString(0x2191));

//	// Draw coordinates
//	painter.setPen(Qt::white);
//	painter.setFont(QFont("Arial Black", 6, QFont::Bold));
//	painter.drawText(width/2, height/2, QString("(%1,%2) @ %3").arg(x).arg(y).arg(rotation));

	// Cleanup...
	painter.resetTransform();
	painter.setFont(QFont("Times", 12, QFont::Normal));
}
