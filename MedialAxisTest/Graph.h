/*
 *  Graph.h
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

#ifndef GRAPH_H_
#define GRAPH_H_

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

#include "vector2.h"

class Graph : public QWidget {

	Q_OBJECT

private:
	QList<Vector2D> points;


public:
	Graph(QWidget *parent = 0);
	virtual ~Graph();
	void setPoints(QList<Vector2D> points);

protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);

public slots:
	void paint();

signals:
	void clicked(double x, double y);

};

#endif /* GRAPH_H_ */
