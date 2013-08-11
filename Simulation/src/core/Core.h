#ifndef CORE_H_
#define CORE_H_

#include <QPainter>

class Core {
	
public:
	enum AreaType { CollisionArea, OpenArea, UnknownArea };
	
public:
	static void inline resetPainter(QPainter &painter) {
		painter.setOpacity(1.0);
		painter.resetTransform();
		painter.resetMatrix();
		painter.setPen(Qt::black);
		painter.setBrush(Qt::white);
		painter.setFont(QFont());
	}
};

#endif /*CORE_H_*/
