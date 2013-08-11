/*
 *  PhysicalMap.cpp
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

#include "PhysicalMap.h"

PhysicalMap::PhysicalMap(Core *core, QString filePath, long scale) : Map("Physical", core)
{
	// Init
	this->filePath = filePath;
	this->scale = scale;

	// Load image
	if(filePath != "") image = QImage(filePath);
}

PhysicalMap::~PhysicalMap()
{

}

void PhysicalMap::paint(QPainter &painter, QRect view) {

	// Draw map image
	QRectF target = QRect(0, 0, view.width(), view.height());
	int imageX = view.x() / this->scale;
	int imageY = image.height() - view.y()/this->scale - view.height()/this->scale;
	int imageWidth = view.width() / this->scale;
	int imageHeight = view.height() / this->scale;
	QRectF source = QRect(imageX, imageY, imageWidth, imageHeight);
	painter.drawImage(target, image, source);

}

PhysicalMap::PhysicalMapAreaType PhysicalMap::getAreaType(long x, long y) {

	// Convert to pixel measurement
	x = core->mmToPixels(x);
	y = core->mmToPixels(y);

	// Check bounds
	if(x/this->scale > image.width() || image.height() - y/this->scale > image.height()) return PhysicalMapAreaTypeOpen;

	// Get the area type based on alpha channel...
	QRgb pixel = image.pixel((int)(x/this->scale), (int)(image.height() - y/this->scale));
	int minColorWall = 240;
	int minColorStairs = 100;
	if(qAlpha(pixel) != 255) return PhysicalMapAreaTypeOpen;
	else if(qRed(pixel) > minColorWall && qGreen(pixel) > minColorWall && qBlue(pixel) > minColorWall) return PhysicalMapAreaTypeWall;
	else if(qRed(pixel) > minColorStairs && qGreen(pixel) > minColorStairs && qBlue(pixel) > minColorStairs) return PhysicalMapAreaTypeStairs;
	else if(qRed(pixel) == 0 && qBlue(pixel) == 0 && qGreen(pixel) == 0 )return PhysicalMapTypeUnreachable;
	else return PhysicalMapAreaTypeOpen;
}


long PhysicalMap::width() {
	return core->pixelsTomm(this->image.width());
}

long PhysicalMap::height() {
	return core->pixelsTomm(this->image.height());
}
