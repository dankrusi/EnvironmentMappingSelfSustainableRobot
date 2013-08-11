/*
 *  RobotControlPanel.cpp
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

#include "RobotControlPanel.h"

#include "../Library/Debug.h"

#include "../COIL/COIL.h"
#include "../Controller/Controller.h"

RobotControlPanel::RobotControlPanel(Core *core, QWidget *parent) : QWidget(parent) {
	this->core = core;
}

RobotControlPanel::~RobotControlPanel() {

}

void RobotControlPanel::paintEvent(QPaintEvent *event)
{
	this->paint();
}

void RobotControlPanel::paint() {

	// Set defaults and 'standby' values...
	double vL = 0;
	double vR = 0;
	double vMax = 500.0;
	double battery = 0.0;
	bool bumperLeft = false;
	bool bumperRight = false;
	bool cliffLeft = false;
	bool cliffFrontLeft = false;
	bool cliffRight = false;
	bool cliffFrontRight = false;
	bool wheelDropLeft = false;
	bool wheelDropRight = false;
	double ir = 0.0;
	double irMax = 0.0;
	double wall = 0.0;
	double wallMax = 0.0;
	bool drawDisabled = !this->isEnabled();

	if(core && core->controller && core->controller->sensorData) {
		// We have a valid controller and sensor data. Set all the values...
		vL = core->coil->extractSensorFromData(core->controller->sensorData, COIL::SENSOR_REQUESTED_LEFT_VEL);
		vR = core->coil->extractSensorFromData(core->controller->sensorData, COIL::SENSOR_REQUESTED_RIGHT_VEL);
		battery = (double)core->coil->extractSensorFromData(core->controller->sensorData, COIL::SENSOR_BATTERY_CHARGE) / (double)core->coil->extractSensorFromData(core->controller->sensorData, COIL::SENSOR_BATTERY_CAPACITY);
		ir = (double) core->coil->getIRSensorDistanceFromAnalogSignal(core->coil->extractSensorFromData(core->controller->sensorData, COIL::SENSOR_ANALOG_SIGNAL));
		irMax = core->doubleSetting("Robot_SharpIRSensorCutoffValue");
		wall = (double) core->coil->getWallSensorDistanceFromSignal(core->coil->extractSensorFromData(core->controller->sensorData, COIL::SENSOR_WALL_SIGNAL));
		wallMax = core->doubleSetting("Robot_WallSensorRange_mm");
		if(wall == INT_MAX) wall = wallMax;
		cliffFrontLeft = core->coil->extractSensorFromData(core->controller->sensorData, COIL::SENSOR_CLIFF_FRONT_LEFT);
		cliffFrontRight = core->coil->extractSensorFromData(core->controller->sensorData, COIL::SENSOR_CLIFF_FRONT_RIGHT);
		cliffLeft = core->coil->extractSensorFromData(core->controller->sensorData, COIL::SENSOR_CLIFF_LEFT);
		cliffRight = core->coil->extractSensorFromData(core->controller->sensorData, COIL::SENSOR_CLIFF_RIGHT);
		bumperLeft = (core->coil->extractSensorFromData(core->controller->sensorData, COIL::SENSOR_BUMPS_AND_WHEEL_DROPS) & COIL::BUMPWHEELDROP_BUMP_LEFT) == COIL::BUMPWHEELDROP_BUMP_LEFT;
		bumperRight = (core->coil->extractSensorFromData(core->controller->sensorData, COIL::SENSOR_BUMPS_AND_WHEEL_DROPS) & COIL::BUMPWHEELDROP_BUMP_RIGHT) == COIL::BUMPWHEELDROP_BUMP_RIGHT;
		wheelDropLeft = (core->coil->extractSensorFromData(core->controller->sensorData, COIL::SENSOR_BUMPS_AND_WHEEL_DROPS) & COIL::BUMPWHEELDROP_WHEELDROP_LEFT) == COIL::BUMPWHEELDROP_WHEELDROP_LEFT;
		wheelDropRight = (core->coil->extractSensorFromData(core->controller->sensorData, COIL::SENSOR_BUMPS_AND_WHEEL_DROPS) & COIL::BUMPWHEELDROP_WHEELDROP_RIGHT) == COIL::BUMPWHEELDROP_WHEELDROP_RIGHT;
	} else {
		// No valid controller or core, show disabled effect
		drawDisabled = true;
	}

	// Init painter and background
	QColor good = Qt::darkGray;
	QColor bad = Qt::red;
	QPainter painter(this);
	//painter.setRenderHint(QPainter::Antialiasing, true);
	painter.fillRect(0, 0, this->width(), this->height(), Qt::black);
	int padding = 10;
	int barWidth = (this->width()/2 - 4*padding) / 3; // Three bars, four paddings
	int barHeight = this->height() - 2*padding;

	// Bars
	for(int i = 0; i < 3; i++) {
		// Background
		int x = padding + padding*i + barWidth*i;
		QBrush brush = QBrush(QColor(30,30,30,255));
		painter.fillRect(x, padding, barWidth, barHeight, brush);
		double percent = 0.0;
		if(i == 0) {
			// Battery bar
			percent = battery;
			brush = QBrush(Qt::green);
			int h = (int)(barHeight*percent);
			int y = padding + barHeight - h;
			painter.fillRect(x, y, barWidth, h, brush);
		} else if(i == 1) {
			// vL Bar
			percent = vL/vMax;
			brush = QBrush(Qt::blue);
			int h = (int)(barHeight*percent/2);
			int y = padding + barHeight - h - barHeight/2;
			painter.fillRect(x, y, barWidth, h, brush);
		} else if(i == 2) {
			// vR Bar
			percent = vR/vMax;
			brush = QBrush(Qt::blue);
			int h = (int)(barHeight*percent/2);
			int y = padding + barHeight - h - barHeight/2;
			painter.fillRect(x, y, barWidth, h, brush);
		}
	}

	// Robot
	{
		// Init
		int size = 0;
		int sizeWidth = this->width()/2 - padding*2;
		int sizeHeight = (int)(this->height()/1.5 - padding*2);
		if(sizeWidth < sizeHeight) 	size = sizeWidth;
		else						size = sizeHeight;
		painter.translate(3.0/4.0*this->width(), 0.5*this->height()-size/7.0);
		QPen pen;

		// Draw bumpers
		painter.setPen(Qt::transparent);
		if(bumperLeft)	painter.setBrush(QBrush(bad));
		else			painter.setBrush(QBrush(good));
		painter.setClipRect(-size,-size,size,size*2);
		painter.drawEllipse(QRect(-size/2, (int)(-size/2 - 0.1*size), size, size));
		if(bumperRight)	painter.setBrush(QBrush(bad));
		else			painter.setBrush(QBrush(good));
		painter.setClipRect(0,-size,size,size*2);
		painter.drawEllipse(QRect(-size/2, (int)(-size/2 - 0.1*size), size, size));
		painter.setClipping(false);

		// Draw body
		painter.setBrush(QBrush(Qt::white));
		pen.setWidth(3);
		pen.setColor(Qt::darkGray);
		painter.setPen(pen);
		painter.drawEllipse(QRect(-size/2, -size/2, size, size));

		// Draw wheels
		pen.setColor(Qt::black);
		pen.setWidth(5);
		painter.setPen(pen);
		int wheelWidth =  (int)( (1.0/7.0) * (double)size );
		int wheelHeight = (int)( (1.0/4.0) * (double)size );
		if(wheelDropLeft)	painter.setBrush(QBrush(bad));
		else				painter.setBrush(QBrush(good));
		painter.drawRect(QRect(-size/2, -wheelHeight/2, wheelWidth, wheelHeight));
		if(wheelDropRight)	painter.setBrush(QBrush(bad));
		else				painter.setBrush(QBrush(good));
		painter.drawRect(QRect(size/2 - wheelWidth, -wheelHeight/2, wheelWidth, wheelHeight));

		// Wall sensor
		int cliffSize = size / 15;
		{
			painter.setPen(Qt::transparent);
			QPoint irOrigin((int)(+size/2-size/14.0-cliffSize/2),(int)(-size/2.5+cliffSize/2));
			int irSpread = size/10;
			int irWidth = irOrigin.x()+size/8;
			QPoint irPoints[3];
			irPoints[0] = irOrigin;
			irPoints[1] = QPoint(irWidth,irOrigin.y()-irSpread);
			irPoints[2] = QPoint(irWidth,irOrigin.y()+irSpread);
			painter.setBrush(QBrush(bad));
			painter.drawPolygon(irPoints,3); // background
			painter.setBrush(QBrush(good));
			painter.setClipRect(irOrigin.x(),-size*2,(int)((irWidth-irOrigin.x())*(wall/wallMax)),size*4);
			painter.drawPolygon(irPoints,3); // good area
			painter.setClipping(false);
		}

		// Draw cliffs
		painter.setPen(Qt::black);
		if(cliffLeft)		painter.setBrush(QBrush(bad));
		else				painter.setBrush(QBrush(good));
		painter.drawEllipse((int)(-size/2+size/14.0),(int)(-size/2.5),cliffSize,cliffSize);
		if(cliffFrontLeft)	painter.setBrush(QBrush(bad));
		else				painter.setBrush(QBrush(good));
		painter.drawEllipse((int)(-size/2+size/3.4),(int)(-size/1.72),cliffSize,cliffSize);
		if(cliffRight)		painter.setBrush(QBrush(bad));
		else				painter.setBrush(QBrush(good));
		painter.drawEllipse((int)(+size/2-size/14.0-cliffSize),(int)(-size/2.5),cliffSize,cliffSize);
		if(cliffFrontRight)	painter.setBrush(QBrush(bad));
		else				painter.setBrush(QBrush(good));
		painter.drawEllipse((int)(+size/2-size/3.4-cliffSize),(int)(-size/1.72),cliffSize,cliffSize);

		// Draw directon arrow
		painter.setPen(Qt::black);
		painter.setFont(QFont("Arial Black", 42, QFont::Bold));
		painter.drawText(-12, 6, QString(0x2191));

		// IR sensor
		{
			painter.setPen(Qt::transparent);
			QPoint irOrigin(-size/2,(int)(size/1.4));
			int irSize = 15;
			int irSpread = size/5;
			int irWidth = irOrigin.x()+size;
			QPoint irPoints[3];
			irPoints[0] = irOrigin;
			irPoints[1] = QPoint(irWidth,irOrigin.y()-irSpread);
			irPoints[2] = QPoint(irWidth,irOrigin.y()+irSpread);
			painter.setBrush(QBrush(bad));
			painter.drawPolygon(irPoints,3); // background
			painter.setBrush(QBrush(good));
			painter.setClipRect(irOrigin.x(),(int)-size,(int)(irWidth*(ir/irMax)*2),size*2);
			painter.drawPolygon(irPoints,3); // good area
			painter.setClipping(false);
			painter.setBrush(QBrush(good));
			painter.drawEllipse(irOrigin.x()-irSize/2,irOrigin.y()-irSize/2,irSize,irSize); // origin
		}
	}

	// Make disabled effect
	if(drawDisabled) {
		painter.resetTransform();
		painter.setOpacity(0.5);
		painter.fillRect(0, 0, this->width(), this->height(), Qt::white);
	}

}

void RobotControlPanel::setCore(Core *core) {
	this->core = core;
}

void RobotControlPanel::updateSensorData() {
	if(this->isVisible()) this->repaint();
}

void RobotControlPanel::mousePressEvent(QMouseEvent *event)
{

}

void RobotControlPanel::mouseReleaseEvent(QMouseEvent *event)
{

}

void RobotControlPanel::mouseMoveEvent(QMouseEvent *event) {

}

void RobotControlPanel::keyPressEvent(QKeyEvent *event)
{

}

void RobotControlPanel::resizeEvent(QResizeEvent *event) {

}
