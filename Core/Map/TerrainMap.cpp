/*
 *  TerrainMap.cpp
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

#include "TerrainMap.h"
#include "../Library/Debug.h"

TerrainMap::TerrainMap(Core *core, long width, long height) : ColorMap(core, width, height)
{
	this->name = "Terrain";
}

TerrainMap::~TerrainMap() {

}

// This alters the bits in the bitmap. It works on the pixel level instead of the mm level
// for efficiency and memory reasons. Thus it is private.
void TerrainMap::setBitmapHeight(long x, long y, long width, long height, double percent, QColor color) {
	lock.lockForWrite();
	{

		// Convert to pixel
//		x = core->mmToPixels(x);
//		y = core->mmToPixels(y);
//		width = core->mmToPixels(width);
//		height = core->mmToPixels(height);

		// Calculate color opacity
		double minimum = 0.01;
		double opacity = minimum + percent*(1.0-minimum);

		// Init painter and draw
		QPainter painter(bitmap);
		painter.setRenderHint(QPainter::Antialiasing, antiAlias);
		painter.setOpacity(opacity);
		painter.setCompositionMode(QPainter::CompositionMode_DestinationAtop); // Overwrite everything
		painter.fillRect(x, bitmap->height() - y, width, height, QBrush(color));
	}
	lock.unlock();
}

void TerrainMap::setHeight(long x, long y, long width, long height, double percent, QColor color) {
	// Convert to pixel
	x = core->mmToPixels(x);
	y = core->mmToPixels(y);
	width = core->mmToPixels(width);
	height = core->mmToPixels(height);

	// Call private method to actually set the bits
	setBitmapHeight(x, y, width, height, percent, color);
}

void TerrainMap::setHeight(long x, long y, double percent) {
	setBitmapHeight(core->mmToPixels(x), core->mmToPixels(y), 1, 1, percent, Qt::white);
}

void TerrainMap::raise(long x, long y) {
	raise(x, y, core->intSetting("Map_TerrainMap_RaiseLowerSize_mm"));
}

void TerrainMap::raise(long x, long y, long size) {

	// Convert to pixel
	x = core->mmToPixels(x);
	y = core->mmToPixels(y);
	size = core->mmToPixels(size);

	// Set the height on the given block
	for(long px = x - size/2; px < x + size/2; px++) {
		for(long py = y - size/2; py < y + size/2; py++) {
			double height = getHeight(core->pixelsTomm(px), core->pixelsTomm(py)); // Convert back when querying our underlying ColorMap...
			height += core->doubleSetting("Map_TerrainMap_RaiseLowerIncrement");
			if(height > 1.0) height = 1.0;
			setBitmapHeight(px, py, 1, 1, height, Qt::white);
		}
	}
}

void TerrainMap::lower(long x, long y) {
	lower(x, y, core->intSetting("Map_TerrainMap_RaiseLowerSize_mm"));
}

void TerrainMap::lower(long x, long y, long size) {
	// Convert to pixel
	x = core->mmToPixels(x);
	y = core->mmToPixels(y);
	size = core->mmToPixels(size);

	// Set the height on the given block
	for(long px = x - size/2; px < x + size/2; px++) {
			for(long py = y - size/2; py < y + size/2; py++) {
				double height = getHeight(core->pixelsTomm(px), core->pixelsTomm(py)); // Convert back when querying our underlying ColorMap...
				height -= core->doubleSetting("Map_TerrainMap_RaiseLowerIncrement");
				if(height < 0.0) height = 0.0;
				setBitmapHeight(px, py, 1, 1, height, Qt::white);
			}
	}
}

double TerrainMap::getHeight(long x, long y) {
	// BUG: This doesnt work yet!
	return getChannelValue(ColorMap::HeightChannel, x, y);
}


