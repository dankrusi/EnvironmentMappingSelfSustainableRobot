#ifndef VIEWPORT_H_
#define VIEWPORT_H_

#include <QWidget>
#include <QPainter>
#include <QList>
#include <QMap>
#include <QGridLayout>
#include <QScrollBar>
#include <QMenu>

#include "../sim/Simulation.h"

#include "../map/Map.h" 

class MapMeta {
	
public:
	bool show;
	
public:
	MapMeta(bool show) { this->show = show; };
	virtual ~MapMeta() {};
};

class Viewport : public QWidget
{
	Q_OBJECT
	
private:
	
public:
	QGridLayout			*gridLayout;
	QScrollBar			*scrollHorizontal;
	QScrollBar			*scrollVertical;
	
	QMenu *menuMain;
		QAction *actionAntiAlias;
		QAction *actionGrid;
		QMenu *menuPhysicalMap;
			QAction *actionShowPhysicalMap;
		QMenu *menuHeatMap;
			QAction *actionShowHeatMap;
	
	QRectF				view;
	QMap<Map*,MapMeta*>	*maps;
	Simulation			*simulation;
	bool				antiAlias;
	
public:
	Viewport(Simulation *simulation, QWidget *parent = 0);
	virtual ~Viewport();
	void moveMapUp();
	void moveMapDown();
	void moveMapLeft();
	void moveMapRight();
	
protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event); 
	void keyPressEvent(QKeyEvent *event);
	void resizeEvent(QResizeEvent *event);
	void contextMenuEvent(QContextMenuEvent *event);
     
public slots:
	void paint();
	void paintAndShow();
	void scroll(int value);
	void resizeScrollbars();
	void menuAction(QAction *action);
     
signals:
	void leftClick();
	void statusUpdate(QString status);
     
};

#endif /*VIEWPORT_H_*/
