#ifndef MAP_H_
#define MAP_H_

#include <QObject>
#include <QPainter>
#include <QString>
#include <QList>

#include "../core/Core.h"

#include "../object/Object.h"
//class Object;

class Map : public QObject
{
	Q_OBJECT

public:
	double width;
	double height;

public:
	QList<Object*> *objects;

private:


public:
	Map();
	virtual ~Map();
	virtual void paint(QPainter &painter, QRectF view) = 0;
	virtual void paintObjects(QPainter &painter, QRectF view);
	virtual Core::AreaType checkForCollision(double x, double y, Object *exclude = NULL);

public slots:

signals:
	void sizeChanged();

};

#endif /*MAP_H_*/
