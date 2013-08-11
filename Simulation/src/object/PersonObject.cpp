#include "PersonObject.h"

const double PersonObject::BODY_WIDTH;
const double PersonObject::BODY_HEIGHT;
const double PersonObject::HEAD_WIDTH;
const double PersonObject::HEAD_HEIGHT;
const double PersonObject::FOOT_WIDTH;
const double PersonObject::FOOT_HEIGHT;
	
void PersonObject::paint(QPainter &painter, QRectF view) 
{
	// Init painter
	QTransform t;
	t.translate(x - view.x(), y - view.y());
	t.rotate(rotation);
	painter.setTransform(t);
	painter.setBrush(QBrush(Qt::white));
	
	// Draw body parts
	painter.drawRoundRect(QRectF(
			-BODY_WIDTH/3, 
			-FOOT_HEIGHT, 
			FOOT_WIDTH, 
			FOOT_HEIGHT));
	painter.drawRoundRect(QRectF(
			BODY_WIDTH/3 - FOOT_WIDTH, 
			-FOOT_HEIGHT, 
			FOOT_WIDTH, 
			FOOT_HEIGHT));
	painter.drawRoundRect(QRectF(
			-BODY_WIDTH/2, 
			-BODY_HEIGHT/2, 
			BODY_WIDTH, 
			BODY_HEIGHT));
	painter.drawEllipse(QRectF(
			-HEAD_WIDTH/2, 
			-HEAD_HEIGHT/2, 
			HEAD_WIDTH, 
			HEAD_HEIGHT));
}
