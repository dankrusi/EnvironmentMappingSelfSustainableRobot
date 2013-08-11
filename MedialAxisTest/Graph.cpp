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

#import "Voronoi.h"

Graph::Graph(QWidget *parent) : QWidget(parent) {
	this->setMinimumSize(400,400);
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
	pen.setWidth(2);
	pen.setColor(Qt::green);
	painter.setBrush(QBrush(Qt::blue));
	painter.setPen(pen);
	for(int i = 0; i < points.size(); i++){
		painter.drawPoint(points.at(i).x(),this->height() - points.at(i).y());
	}

	VoronoiGraph *graph = Fortune::ComputeVoronoiGraph(&points);

	//delete graph;

//	Bitmap bmp = new Bitmap(weight, height);
//	VoronoiGraph graph = Fortune.ComputeVoronoiGraph(Datapoints);
//	Graphics g = Graphics.FromImage(bmp);
//	foreach (object o in graph.Vertizes)
//	{
//		Vector v = (Vector)o;
//		g.DrawEllipse(Pens.Black, (int)v[0]-2, (int)v[1]-2, 4, 4);
//	}
//	foreach (object o in Datapoints)
//	{
//		Vector v = (Vector)o;
//		g.DrawEllipse(Pens.Red, (int)v[0]-1, (int)v[1]-1, 2, 2);
//	}
//	foreach (object o in graph.Edges)
//	{
//		VoronoiEdge edge = (VoronoiEdge)o;
//		try
//		{
//			g.DrawLine(Pens.Brown, (int)edge.VVertexA[0], (int)edge.VVertexA[1], (int)edge.VVertexB[0], (int)edge.VVertexB[1]);
//		}
//		catch { }
//	}
//	return bmp;
}

void Graph::setPoints(QList<Vector2D> points) {

	this->points = points;

	//paint();
	this->repaint();
}

void Graph::mousePressEvent(QMouseEvent *event)
{
	emit clicked((double)event->x(), (double)this->height()-event->y() );
}
