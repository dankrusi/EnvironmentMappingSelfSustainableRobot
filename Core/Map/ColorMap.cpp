/*
 *  ColorMap.cpp
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

#include <QPainter>

#include "ColorMap.h"

ColorMap::ColorMap(Core *core, long width, long height) : Map("Color", core)
{
	this->antiAlias = true;
	this->bitmap = new QImage((int)core->mmToPixels(width), (int)core->mmToPixels(height), QImage::Format_ARGB32);
}

ColorMap::~ColorMap()
{
	delete bitmap;
}

void ColorMap::paint(QPainter &painter, QRect view) {
	lock.lockForRead();
	{
		// Extract alaising from render hints so that subclasses will know whether to alias...
		antiAlias = painter.renderHints() & QPainter::Antialiasing;

		// Draw heat map image
		QRectF target = QRect(0, 0, view.width(), view.height());
		QRectF source = QRect(view.x(), bitmap->height() - view.y() - view.height(), view.width(), view.height());
		painter.drawImage(target, (*bitmap), source);

	}
	lock.unlock();
}

void ColorMap::resizeIfNeeded(long x, long y) {
	// TODO: Dynamic resizing of heat map
}

// Return heat in percent
double ColorMap::getChannelValue(ColorMap::MapChannel channel, long x, long y) {

	// Convert to pixel scale
	double retVal;
	x = core->mmToPixels(x);
	y = core->mmToPixels(y);

	lock.lockForRead();
	{
		// Check bounds
		if(x > bitmap->width() || bitmap->height() - y > bitmap->height()) return ColorMap::OpenAreaChannel;

		// See what the physical map states this area is...
		QRgb rgb = bitmap->pixel((int)x, bitmap->height() - (int)y);
		int heat = 0;
		if		(channel == ColorMap::CollisionAreaChannel) 	heat = qRed(rgb);
		else if	(channel == ColorMap::OpenAreaChannel) 			heat = qGreen(rgb);
		else if	(channel == ColorMap::HeightChannel) 			heat = qRed(rgb); // Any color channel will do :)
		retVal = (double)(qAlpha(rgb)*heat) / (255.0*255.0);
	}
	lock.unlock();
	return retVal;
}

long ColorMap::width() {
	return core->pixelsTomm(this->bitmap->width());
}

long ColorMap::height() {
	return core->pixelsTomm(this->bitmap->height());
}

