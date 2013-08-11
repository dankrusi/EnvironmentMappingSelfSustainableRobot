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
	this->setMinimumSize(200,100);
}

Graph::~Graph() {

}

void Graph::paintEvent(QPaintEvent *event)
{
	this->paint();
}

void Graph::paint() {


	// Init painter and background
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.fillRect(0, 0, this->width(), this->height(), Qt::black);
	QPen pen;


	// Draw vector source
	pen.setWidth(3);
	pen.setColor(Qt::red);
	painter.setBrush(QBrush(Qt::red));
	painter.setPen(pen);
	painter.drawLine(0, this->height(), v1.x() , this->height()- v1.y());
	//painter.drawEllipse(OFFSET_X + ux-(POINT_SIZE/2),this->height()-uy-(POINT_SIZE/2)-OFFSET_Y, POINT_SIZE, POINT_SIZE);

	// Draw vector target
	pen.setWidth(3);
	pen.setColor(Qt::blue);
	painter.setBrush(QBrush(Qt::blue));
	painter.setPen(pen);
	painter.drawLine(0, this->height(), v2.x(), this->height()-v2.y());
	//painter.drawEllipse(OFFSET_X + vx-(POINT_SIZE/2),this->height()-vy-(POINT_SIZE/2), POINT_SIZE, POINT_SIZE);


	pen.setWidth(2);
	pen.setColor(Qt::white);
	painter.setBrush(QBrush(Qt::white));
	painter.setPen(pen);
	painter.drawLine(v1.x(),this->height()-v1.y(),v2.x(),this->height()-v2.y() );


	// Draw source rotation vector
	pen.setWidth(3);
	pen.setColor(Qt::green);
	painter.setBrush(QBrush(Qt::green));
	painter.setPen(pen);
	painter.drawLine(v1.x(), this->height()-v1.y(), e1.x(), this->height()-e1.y());
	//painter.drawEllipse(px-(POINT_SIZE/2),this->height()-py-(POINT_SIZE/2), POINT_SIZE, POINT_SIZE);

	// Draw source rotation vector


	pen.setWidth(3);
	pen.setColor(Qt::green);
	painter.setBrush(QBrush(Qt::green));
	painter.setPen(pen);
	painter.drawLine(v2.x(), this->height()-v2.y(), v2.x() - e2.x(), this->height()-e2.y() - v2.y());
	//painter.drawEllipse(px-(POINT_SIZE/2),this->height()-py-(POINT_SIZE/2), POINT_SIZE, POINT_SIZE);


}

void Graph::draw( Vector2T<long> v1, Vector2T<long> v2, Vector2T<long> e1, Vector2T<long> e2) {

	this->v1 = v1;
	this->v2 = v2;
	this->e1 = e1;
	this->e2 = e2;

	paint();
	this->repaint();
}

void Graph::mousePressEvent(QMouseEvent *event)
{

	emit clicked((long)event->x(), (long)(this->height() - event->y()));

}

void Graph::mouseReleaseEvent (QMouseEvent *event)
{
	emit released((long)event->x(), (long)(this->height() - event->y()));

}

