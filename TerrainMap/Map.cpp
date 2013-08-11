/*
 *  Map.cpp
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

#include "Map.h"

#include <QQueue>
#include <QPoint>

#include <iostream>

Map::Map(QWidget *parent) :
	QWidget(parent) {
	this->setFixedSize(400, 400);

	mode = "Discovery";
	cutLevel = MAP_SIZE/2;

	//int heights[this->width() / GRID_SIZE][this->height() / GRID_SIZE];
	//int explore[this->width() / GRID_SIZE][this->height() / GRID_SIZE];

	for (int x = 0; x < this->width() / GRID_SIZE; x++) {
		for (int y = 0; y < this->height() / GRID_SIZE; y++) {
			heights[x][y] = 0;
			explore[x][y] = Unknown;
		}
	}
}

Map::~Map() {

}

void Map::paintEvent(QPaintEvent *event) {
	this->paint();
}

void Map::paint() {

	QPainter painter(this);
	QPen pen;
	QBrush brush;
	painter.setRenderHint(QPainter::Antialiasing, true);

	painter.fillRect(0, 0, this->width(), this->height(), Qt::black);

	QColor color;
	int gridLineWidth = 0;

	int maxLocalHeight = 0;
	for (int x = 0; x < this->width() / GRID_SIZE; x++) {
		for (int y = 0; y < this->height() / GRID_SIZE; y++) {
			if(heights[x][y] > maxLocalHeight) maxLocalHeight = heights[x][y];
		}
	}

	for (int x = 0; x < this->width() / GRID_SIZE; x++) {
		for (int y = 0; y < this->height() / GRID_SIZE; y++) {
			int colorUnknown = ((heights[x][y] / (double)maxLocalHeight))*255.0;
			switch (explore[x][y]) {
			case Unknown:
				color = QColor(colorUnknown, colorUnknown, colorUnknown, 255);
				break;
			case Discovered:
				color = QColor(0, 255, 0, 255);
				break;
			case Obstacle:
				color = QColor(255, 0, 0, 255);
				break;
			}
			if (heights[x][y] == cutLevel){
				color = QColor(0,0,255,255);
			}
			painter.setBrush(color);
			painter.setPen(Qt::NoPen);
			painter.drawRect(x * GRID_SIZE, MAP_SIZE - (y * GRID_SIZE),
					GRID_SIZE - gridLineWidth, -(GRID_SIZE - gridLineWidth));

		}
	}

	painter.setBrush(Qt::black);
	//painter.drawRect(peak.x * GRID_SIZE,MAP_SIZE - (peak.y * GRID_SIZE), GRID_SIZE - 1,-(GRID_SIZE - 1) );
	painter.drawEllipse(peak.x * GRID_SIZE,MAP_SIZE - (peak.y * GRID_SIZE), GRID_SIZE - gridLineWidth,-(GRID_SIZE - gridLineWidth) );

}

void Map::mousePressEvent(QMouseEvent *event) {
	int x = event->x() / GRID_SIZE;
	int y = (MAP_SIZE / GRID_SIZE) - (event->y() / GRID_SIZE) - 1;

	if (mode == "Discovery"){
		explore[x][y] = Discovered;
	}
	else if (mode == "Obstacle"){
		explore[x][y] = Obstacle;
	}

	calculateHeights(x, y);
	this->repaint();
}

void Map::calculateHeights(int x, int y) {
	for (int xx = 0; xx < this->width() / GRID_SIZE; xx++) {
		for (int yy = 0; yy < this->height() / GRID_SIZE; yy++) {
			heights[xx][yy] = MAX_HEIGHT;
		}
	}

	int h;
	peak = TerrainPointT<int>();

	QQueue<TerrainPointT<int> > queue;
	queue.enqueue(TerrainPointT<int> (x, y, 0));

	while (!queue.isEmpty()) {
		TerrainPointT<int> p = queue.dequeue();

		if (p.height + 5 > MAX_HEIGHT){
			h = MAX_HEIGHT;
		}
		else {
			h = p.height + 5;
		}


		for (int k = -1; k <= 1; k++) {
			for (int j = -1; j <= 1; j++) {
				if (p.x + k < (MAP_SIZE / GRID_SIZE) && p.x + k >= 0 && p.y + j < (MAP_SIZE / GRID_SIZE) && p.y + j >= 0
						&& (heights[p.x + k][p.y + j] > h || heights[p.x + k][p.y + j] == MAX_HEIGHT)) {
					switch (explore[p.x + k][p.y + j]) {
					case Obstacle:
						heights[p.x + k][p.y + j] = 0;
						queue.enqueue(TerrainPointT<int> (p.x+k, p.y+j, 0));
						break;
					case Discovered:
						heights[p.x + k][p.y + j] = 0;
						queue.enqueue(TerrainPointT<int> (p.x+k, p.y+j, 0));
						break;
					case Unknown:
						heights[p.x + k][p.y + j] = h;
						queue.enqueue(TerrainPointT<int> (p.x+k, p.y+j, h));
//						if(h > peak.height){
//							peak.x = p.x+k;
//							peak.y = p.y+j;
//							peak.height = h;
//						}
						break;
					}
				}
			}
		}
	}
	for (int x = 0; x < this->width() / GRID_SIZE; x++) {
			for (int y = 0; y < this->height() / GRID_SIZE; y++) {
				if (heights[x][y] > peak.height){
					peak.height = heights[x][y];
					peak.x = x;
					peak.y = y;
				}
			}
		}

}

void Map::setDiscovery(){
	this->mode = "Discovery";
}

void Map::setObstacle(){
	this->mode = "Obstacle";
}

void Map::cutLevelChanged(int level){
	this->cutLevel = level;
	this->repaint();
}

