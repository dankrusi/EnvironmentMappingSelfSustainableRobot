/*
 *  TerrainMap.h
 *
 *  This application was built for testing the height map algorithm. It
 *  visualizes the exploration map together with a heat map. The red
 *  space is unexplored, the brighter the square the higher is the height
 *  map value. The two known obstacle and free space can be painted with
 *  the mouse. The yellow point represents the highest point in the map.
 *  It is also possible to show the cut points on different levels adjusted
 *  with a slider.
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

#ifndef TERRAINMAP_H_
#define TERRAINMAP_H_

#include <QWidget>
#include <QRadioButton>
#include <QVarLengthArray>
#include <QSlider>

#include "Map.h"

class TerrainMap : public QWidget
{

	Q_OBJECT

private:
	Map *terrain;
	QRadioButton *rbtnObstacle;
	QRadioButton *rbtnDiscoverd;
	QSlider *sliderCutLevel;

public:
	TerrainMap(QWidget *parent = 0);
	~TerrainMap();


};


#endif /* TERRAINMAP_H_ */
