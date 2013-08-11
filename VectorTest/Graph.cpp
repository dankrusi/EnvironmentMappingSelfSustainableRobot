/*
 *  Graph.cpp
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

#include "Graph.h"

Graph::Graph(QWidget *parent) : QWidget(parent) {
	px = 0;
	py = 0;
	ux = 0;
	uy = 0;
	vx = 0;
	vy = 0;
}

Graph::~Graph() {

}

void Graph::paintEvent(QPaintEvent *event)
{
	this->paint();
}

void Graph::paint() {

	// Params
	int POINT_SIZE = 6;
	int OFFSET_X = this->width() / 2;
	int OFFSET_Y = this->height() / 2;

	// Init painter and background
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.fillRect(0, 0, this->width(), this->height(), Qt::black);
	QPen pen;



	// Draw u
	pen.setWidth(3);
	pen.setColor(Qt::red);
	painter.setBrush(QBrush(Qt::red));
	painter.setPen(pen);
	painter.drawLine(OFFSET_X + px, this->height()-py-OFFSET_Y, OFFSET_X + ux, this->height()-uy-OFFSET_Y);
	//painter.drawEllipse(OFFSET_X + ux-(POINT_SIZE/2),this->height()-uy-(POINT_SIZE/2)-OFFSET_Y, POINT_SIZE, POINT_SIZE);

	// Draw v
	pen.setWidth(3);
	pen.setColor(Qt::blue);
	painter.setBrush(QBrush(Qt::blue));
	painter.setPen(pen);
	painter.drawLine(OFFSET_X + px, this->height()-py-OFFSET_Y, OFFSET_X + vx, this->height()-vy-OFFSET_Y);
	//painter.drawEllipse(OFFSET_X + vx-(POINT_SIZE/2),this->height()-vy-(POINT_SIZE/2), POINT_SIZE, POINT_SIZE);

	// Draw p
	//pen.setWidth(3);
	//pen.setColor(Qt::green);
	//painter.setBrush(QBrush(Qt::green));
	//painter.setPen(pen);
	//painter.drawLine(0, this->height(), px, this->height()-py);
	//painter.drawEllipse(px-(POINT_SIZE/2),this->height()-py-(POINT_SIZE/2), POINT_SIZE, POINT_SIZE);
}

void Graph::draw(double px, double py, double ux, double uy, double vx, double vy, double scale) {
	this->px = (int)(scale*px);
	this->py = (int)(scale*py);
	this->ux = (int)(scale*ux);
	this->uy = (int)(scale*uy);
	this->vx = (int)(scale*vx);
	this->vy = (int)(scale*vy);

	paint();
	this->repaint();
}
