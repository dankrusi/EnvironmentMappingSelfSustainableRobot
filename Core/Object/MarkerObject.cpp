/*
 * MarkerObject.cpp
 *
 *  Created on: 24.02.2009
 *      Author: dkruesi
 */

#include "MarkerObject.h"

#include "../Library/Debug.h"

void MarkerObject::paint(QPainter &painter, QRect view, int scale) {

	// Translate, scale, and rotate...
	QTransform t;
	t.scale(1.0/scale, 1.0/scale);
	t.translate(x - view.x()*scale, view.height()*scale - y + view.y()*scale);
	painter.setTransform(t);

	for(int index = 0; index < transformationCount; index++) {
		long arrowLength = 1000*scale;
		long ballSize = 20*scale;
		Vector2D a = transformation[index].trans();
		Vector2D b = (transformation[index] * Trafo2D::trans(0, arrowLength)).trans();
		painter.setPen(transformationColors[index]);
		QPen pen = painter.pen();
		pen.setWidth(2*scale);
		painter.setPen(pen);
		painter.drawLine(a.x(), -a.y(), b.x(), -b.y());
		painter.setBrush(QBrush(transformationColors[index]));
		painter.drawEllipse(a.x()-ballSize/2, -(a.y()+ballSize/2), ballSize, ballSize);
	}
}

void MarkerObject::setAsVector(int index, Vector2D vector) {

}

void MarkerObject::setAsPoint(int index, long x, long y) {

}

void MarkerObject::setAsTransformation(int index, Trafo2D transformation, QColor color) {
	this->transformation[index] = transformation;
	this->transformationColors[index] = color;
	if(index+1 > transformationCount) transformationCount = index+1;
}


