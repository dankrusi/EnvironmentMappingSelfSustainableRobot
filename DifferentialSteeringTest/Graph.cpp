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
	this->setMinimumSize(400,400);
	scale = 1.0;
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

	// Draw interpolation
//	pen.setWidth(2);
//	pen.setColor(Qt::green);
//	painter.setBrush(QBrush(Qt::blue));
//	painter.setPen(pen);
//	foreach(Vector2D v, results){
//		painter.drawPoint(v.x()*scale,this->height() - v.y()*scale);
//	}

	int range = 360;
	//scale = scale/1000;

	for(int ti = -range; ti < range; ti++) {


		double p = range + ti;
		p = p / (double)(2*range);

		pen.setWidth(2);
		p = 1.0;
		pen.setColor(QColor(255,0,0,(int)(p*255)));
		painter.setBrush(QBrush(Qt::blue));
		painter.setPen(pen);

		double t = (double)ti;
		t = Rad(t);

		double x = Sin(t) * Cos(t);
		double y = t * Sin(t);

		painter.drawPoint(x*scale + this->width()/2,y*scale + this->height()/2);
	}

//	// Draw via Points
//	pen.setWidth(4);
//	pen.setColor(Qt::red);
//	painter.setBrush(QBrush(Qt::red));
//	painter.setPen(pen);
//	for(int i = 0; i < points.size(); i++){
//		int px = points[i].x();
//		int py = this->height() - points[i].y();
//		painter.drawPoint(px,py);
//		if(i < tangents.size()) painter.drawText(px+5, py+5, QString("dx/dy=%1").arg(tangents[i]));
//	}

//	int scale = 100;
//	pen.setWidth(1);
//	pen.setColor(Qt::red);
//	painter.setPen(pen);
//	for(int t = -314; t <= 314; t++) {
//		double tt = (double)t / 100;
//		painter.drawPoint(400+tt*scale,200+sinf(tt)*scale);
//	}
//
//	pen.setWidth(1);
//	pen.setColor(Qt::blue);
//	painter.setPen(pen);
//	for(int t = -314; t <= 314; t++) {
//		double tt = (double)t / 100;
//		painter.drawPoint(400+tt*scale,200+cosf(tt)*scale);
//	}
//
//	pen.setWidth(1);
//	pen.setColor(Qt::green);
//	painter.setPen(pen);
//	for(int t = -314; t <= 314; t++) {
//		double tt = (double)t / 100;
//		painter.drawPoint(400+cosf(tt)*scale,200+sinf(tt)*scale);
//	}
}

void Graph::setResults(std::vector<Vector2D> results) {

	this->results = results;
	this->repaint();
}

void Graph::setScale(double scale) {
	this->scale = scale;
}

void Graph::mousePressEvent(QMouseEvent *event)
{
	emit clicked((double)event->x(), (double)this->height()-event->y() );
}
