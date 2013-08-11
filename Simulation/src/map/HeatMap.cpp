#include <QPainter>

#include "HeatMap.h"

const double 			HeatMap::HEATSPOT_SIZE;
const double 			HeatMap::HEATSPOT_OPACITY;
const Qt::GlobalColor	HeatMap::COLLISIONAREA_COLOR;
const Qt::GlobalColor	HeatMap::OPENAREA_COLOR;

HeatMap::HeatMap(double width, double height)
{
	this->width = width;
	this->height = height;
	this->antiAlias = true;
	this->heat = new QImage((int)width, (int)height, QImage::Format_ARGB32);
}

HeatMap::~HeatMap()
{
	delete heat;
}

void HeatMap::paint(QPainter &painter, QRectF view) {
	// Register aliasing for next registionHeat()
	antiAlias = painter.renderHints() & QPainter::Antialiasing;
	
	// Draw heat map image
	double width = view.width();
	double height = view.height();
	double x = view.x();
	double y = view.y();
	if(x + width > this->heat->width()) width = this->heat->width() - x;
	if(y + height > this->heat->height()) height = this->heat->height() - y;
	QRectF target = QRectF(0, 0, width, height);
	QRectF source = QRectF(x, y, width, height);
	painter.drawImage(target, (*heat), source);
	
	// Paint all the objects...
	paintObjects(painter, view);
}

void HeatMap::registerHeat(double x, double y, Core::AreaType type, double opacity, double size) {
	QPainter painter(heat);
	painter.setRenderHint(QPainter::Antialiasing, antiAlias);
	painter.setPen(Qt::transparent);
	painter.setOpacity(opacity);
	if(type == Core::CollisionArea)		painter.setBrush(QBrush(COLLISIONAREA_COLOR));
	else if(type == Core::OpenArea)		painter.setBrush(QBrush(OPENAREA_COLOR));
	painter.drawEllipse(QRectF(x - (size/2), y - (size/2), size, size));
}

int HeatMap::getCollisionAreaHeat(double x, double y) {
	return qRed(heat->pixel((int)x, (int)y));
}


