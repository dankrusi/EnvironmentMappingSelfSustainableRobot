/*
 *  RobotSpeedGraph.h
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

#ifndef ROBOTSPEEDGRAPH_H_
#define ROBOTSPEEDGRAPH_H_

#include <QWidget>
#include <QPainter>
#include <QList>
#include <QTime>

#include "../Core.h"

class RobotSpeedGraph : public QWidget {

	Q_OBJECT

public:
	Core *core;
	QList<int> speedLeft;
	QList<int> speedRight;
	QTime interval;
	QTime lastDataAdded;
	int bufferSize;
	int updateInterval;
	int sumLeft;
	int countLeft;
	int sumRight;
	int countRight;

public:
	RobotSpeedGraph(Core *core, QWidget *parent = 0);
	virtual ~RobotSpeedGraph();

protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void resizeEvent(QResizeEvent *event);

public slots:
	void paint();
	void setCore(Core *core);
	void updateSensorData();
signals:


};


#endif /* ROBOTSPEEDGRAPH_H_ */
