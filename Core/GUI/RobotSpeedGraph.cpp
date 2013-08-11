/*
 *  RobotSpeedGraph.cpp
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

#include "RobotSpeedGraph.h"

#include "../Library/Debug.h"

#include "../COIL/COIL.h"
#include "../Controller/Controller.h"


RobotSpeedGraph::RobotSpeedGraph(Core *core, QWidget *parent) :
	QWidget(parent) {
	this->core = core;
	lastDataAdded = QTime::currentTime();
	bufferSize = 50;
	updateInterval = 100;
	sumLeft = 0;
	countLeft = 0;
	sumRight = 0;
	countRight = 0;

	//init buffer
	for(int i = 0; i < bufferSize; i++){
		speedLeft.push_back(0);
		speedRight.push_back(0);
	}
}

RobotSpeedGraph::~RobotSpeedGraph() {

}

void RobotSpeedGraph::paintEvent(QPaintEvent *event) {
	this->paint();
}

void RobotSpeedGraph::paint() {

	// Init
	int border = 5;
	int height = this->height() - border*2;
	int width = this->width();
	bool drawDisabled = !this->isEnabled() || !core || !core->coil;
	double maxSpeed = 500.0;
	double deltax = width / bufferSize;
	double deltay = height / maxSpeed / 2;
	int pointSize = 2;
	QPoint lastPoint;

	// Background
	QPainter painter(this);
	QPen pen;
	//painter.setRenderHint(QPainter::Antialiasing, true);
	painter.fillRect(0, 0, this->width(), this->height(), Qt::black);
	pen.setColor(Qt::gray);
	pen.setWidth(1);
	painter.setPen(pen);

	// Axis's
	painter.drawLine(0, this->height()/2, this->width(), this->height()/2);

	// Time labels
	painter.drawText((bufferSize * deltax) , this->height()/2-border, "0 s");
	painter.drawText(0, this->height()/2-border, QString::number(-bufferSize) + " s");

	// Velocity labels
	painter.drawText(2, height+border, "0 mm/s");
	painter.drawText(2, border*2, QString::number(maxSpeed) + " mm/s");

	// Left velocity
	painter.setBrush(QBrush(Qt::red));
	pen.setColor(Qt::green);
	painter.setPen(pen);
	for(int i = 0; i < speedLeft.count(); i++) {
		int x = (int)((i+1) * deltax);
		int y = border + (int)(height - (speedLeft.at(i) * deltay)) - height/2;
		painter.drawEllipse(x-pointSize/2, y-pointSize/2, pointSize, pointSize);
		if(i > 0) painter.drawLine(lastPoint.x(), lastPoint.y(), x, y);
		lastPoint = QPoint(x,y);
	}

	// Right velocity
	painter.setBrush(QBrush(Qt::red));
	pen.setColor(Qt::red);
	painter.setPen(pen);
	for(int i = 0; i < speedRight.count(); i++) {
		int x = (int)((i+1) * deltax);
		int y = border + (int)(height - (speedRight.at(i) * deltay)) - height/2;
		painter.drawEllipse(x-pointSize/2, y-pointSize/2, pointSize, pointSize);
		if(i > 0) painter.drawLine(lastPoint.x(), lastPoint.y(), x, y);
		lastPoint = QPoint(x,y);
	}

	// Make disabled effect
	if(drawDisabled) {
		painter.resetTransform();
		painter.setOpacity(0.5);
		painter.fillRect(0, 0, this->width(), this->height(), Qt::white);
	}


}

void RobotSpeedGraph::updateSensorData() {
	if(core && core->coil){

		// Keep track of the value, but only plot at every interval,
		// thus we take the average when we plot...
		sumLeft += core->coil->extractSensorFromData(core->controller->sensorData, COIL::SENSOR_REQUESTED_LEFT_VEL);
		countLeft++;
		sumRight += core->coil->extractSensorFromData(core->controller->sensorData, COIL::SENSOR_REQUESTED_RIGHT_VEL);
		countRight++;

		// Time to add to plot?
		if(this->lastDataAdded.msecsTo(QTime::currentTime()) > updateInterval) {
			this->lastDataAdded = QTime::currentTime();
			this->speedLeft.pop_front();
			this->speedLeft.push_back(sumLeft/countLeft);
			this->speedRight.pop_front();
			this->speedRight.push_back(sumRight/countRight);
			sumLeft = 0;
			countLeft = 0;
			sumRight = 0;
			countRight = 0;
			if(this->isVisible()) this->repaint();
		}
	}
}

void RobotSpeedGraph::setCore(Core *core) {
	this->core = core;
}

void RobotSpeedGraph::mousePressEvent(QMouseEvent *event) {

}

void RobotSpeedGraph::mouseReleaseEvent(QMouseEvent *event) {

}

void RobotSpeedGraph::mouseMoveEvent(QMouseEvent *event) {

}

void RobotSpeedGraph::keyPressEvent(QKeyEvent *event) {

}

void RobotSpeedGraph::resizeEvent(QResizeEvent *event) {

}

