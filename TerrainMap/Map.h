/*
 *  Map.h
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

#ifndef MAP_H_
#define MAP_H_

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QVarLengthArray>

#include "TerrainPoint.h"

class Map : public QWidget {

	Q_OBJECT

public:
	static const long GRID_SIZE = 10;
	static const long MAP_SIZE = 400;
	static const long MAX_HEIGHT = 191;

private:
	int heights[MAP_SIZE/GRID_SIZE][MAP_SIZE/GRID_SIZE];
	int explore[MAP_SIZE/GRID_SIZE][MAP_SIZE/GRID_SIZE];

	TerrainPointT<int> peak;
	QString mode;
	int cutLevel;

public:
	enum { Discovered, Obstacle, Unknown} status;

private:
	void paint();
	void calculateHeights(int x, int y);

public:
	Map(QWidget *parent = 0);
	virtual ~Map();

protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);

public slots:
	void setObstacle();
	void setDiscovery();
	void cutLevelChanged(int level);

};

#endif /* MAP_H_ */
