#include "GridMap.h"

GridMap::GridMap(int gridSize)
{
	this->gridSize = gridSize;
}

GridMap::~GridMap()
{
}

void GridMap::paint(QPainter &painter, QRectF view) 
{
	painter.setPen(Qt::white);
	painter.setOpacity(0.3);

	// Draw vertical lines
	for(int i = 0; i < view.width() / gridSize + 1; i++) {
		int x = (gridSize * i) - ((int)view.x() % gridSize);
		painter.drawLine(x, 0, x, (int)view.height());
	}
	
	// Draw horizontal lines
	for(int i = 0; i < view.height() / gridSize + 1; i++) {
		int y = (gridSize * i) - ((int)view.y() % gridSize);
		painter.drawLine(0, y, (int)view.width(), y);
	}
}
