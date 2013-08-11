#include "RobotObject.h"

void RobotObject::paint(QPainter &painter, QRectF view) {
	double ellipseWidth = 50;
	double ellipseHeight = 50;

	QTransform t;
	t.translate(x - view.x(), y - view.y());
	t.rotate(rotation);
	painter.setTransform(t);
	
	painter.setBrush(QBrush(Qt::blue));
	painter.drawRect(QRectF(-ellipseWidth/2, -ellipseHeight/2, ellipseWidth, ellipseHeight));
	
	painter.setBrush(QBrush(Qt::green));
	painter.drawRect(QRectF(-ellipseWidth/2, -10/2, 5, 10));
	painter.drawRect(QRectF(ellipseWidth/2 - 5, -10/2, 5, 10));
	
	painter.setPen(Qt::white);
	painter.setFont(QFont("Arial Black", 20, QFont::Bold));
	
	painter.drawText(-7, 7, QString(0x2191));;

	painter.resetTransform();
	painter.setFont(QFont("Times", 12, QFont::Normal));
}
