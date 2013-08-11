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

#include <QLayout>
#include <iostream>
#include <QLabel>

#include "TerrainMap.h"

TerrainMap::TerrainMap(QWidget *parent) :
	QWidget(parent) {

	this->setWindowTitle("TerrainMap");
	this->setLayout(new QVBoxLayout());
	this->layout()->setContentsMargins(0, 0, 0, 0);

	terrain = new Map();
	this->layout()->addWidget(terrain);


	rbtnObstacle = new QRadioButton("mark as Obstacle");
	rbtnDiscoverd = new QRadioButton("mark as discovered");
	rbtnDiscoverd->setChecked(true);
	this->layout()->addWidget(rbtnObstacle);
	this->layout()->addWidget(rbtnDiscoverd);

	sliderCutLevel = new QSlider(Qt::Horizontal);
	sliderCutLevel->setMaximum(Map::MAX_HEIGHT);
	sliderCutLevel->setValue(50);
	this->layout()->addWidget(new QLabel("Cut Level"));
	this->layout()->addWidget(sliderCutLevel);

	connect(rbtnDiscoverd, SIGNAL(pressed()), terrain, SLOT(setDiscovery()) );
	connect(rbtnObstacle, SIGNAL(pressed()), terrain, SLOT(setObstacle()) );
	connect(sliderCutLevel,  SIGNAL(valueChanged(int)), terrain, SLOT(cutLevelChanged(int)));

}

TerrainMap::~TerrainMap() {

}


