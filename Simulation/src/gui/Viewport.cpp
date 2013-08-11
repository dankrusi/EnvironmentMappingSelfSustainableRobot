#include "Viewport.h"

#include <QPainter>
#include <QRect>
#include <QPen>
#include <QMouseEvent>
#include <QKeyEvent>


Viewport::Viewport(Simulation *simulation, QWidget *parent) : QWidget(parent)
{
	// Init
	this->simulation = simulation;
	this->maps = new QMap<Map*,MapMeta*>();
	this->antiAlias = true;
	
	// Add defualt maps from simulation
	this->maps->insert(simulation->physicalMap, new MapMeta(true));
	this->maps->insert(simulation->heatMap, new MapMeta(true));
	this->maps->insert(simulation->gridMap, new MapMeta(true));
	
	// Add scrollbars...
	this->gridLayout = new QGridLayout();
	this->gridLayout->setContentsMargins(0,0,0,0);
	this->setLayout(gridLayout);
	this->scrollHorizontal = new QScrollBar(Qt::Horizontal);
	this->scrollVertical = new QScrollBar(Qt::Vertical);
	this->gridLayout->addWidget(scrollHorizontal,2,1);
	this->gridLayout->addWidget(scrollVertical,1,2);
	this->resizeScrollbars();
	
	// Build menu
	menuMain = new QMenu(this);
	actionAntiAlias = new QAction("Anti Alias", menuMain);
	actionAntiAlias->setCheckable(true);
	actionAntiAlias->setChecked(antiAlias);
	actionGrid = new QAction("Grid", menuMain);
	actionGrid->setCheckable(true);
	actionGrid->setChecked(true);
	menuPhysicalMap = new QMenu("Physical Map");
	actionShowPhysicalMap = new QAction("Show", menuPhysicalMap);
	actionShowPhysicalMap->setCheckable(true);
	actionShowPhysicalMap->setChecked(antiAlias);
	menuPhysicalMap->addAction(actionShowPhysicalMap);
	menuHeatMap = new QMenu("Heat Map");
	actionShowHeatMap = new QAction("Show", menuHeatMap);
	actionShowHeatMap->setCheckable(true);
	actionShowHeatMap->setChecked(antiAlias);
	menuHeatMap->addAction(actionShowHeatMap);
	menuMain->addAction(actionAntiAlias);
	menuMain->addAction(actionGrid);
	menuMain->addMenu(menuPhysicalMap);
	menuMain->addMenu(menuHeatMap);
	
	// Register events
	this->setFocusPolicy(Qt::StrongFocus);
	this->setMouseTracking(true);
	
	// Connections
	connect(scrollHorizontal, SIGNAL(valueChanged(int)), this, SLOT(scroll(int)));
	connect(scrollVertical, SIGNAL(valueChanged(int)), this, SLOT(scroll(int)));
	connect(simulation, SIGNAL(repaint()), this, SLOT(paintAndShow()));
	connect(menuMain, SIGNAL(triggered(QAction*)), this, SLOT(menuAction(QAction*)));
}

Viewport::~Viewport()
{
}

void Viewport::resizeScrollbars() 
{
	double maxWidth = 0.0;
	double maxHeight = 0.0;
	for(int i = 0; i < this->maps->keys().count(); i++) {
		if(this->maps->keys().at(i)->width > maxWidth) 		maxWidth = this->maps->keys().at(i)->width;
		if(this->maps->keys().at(i)->height > maxHeight) 	maxHeight = this->maps->keys().at(i)->height;
	}
	if((int)maxWidth - this->width() > 0) 		scrollHorizontal->setMaximum((int)maxWidth - this->width());
	else										scrollHorizontal->setMaximum(0);
	if((int)maxHeight - this->height() > 0) 	scrollVertical->setMaximum((int)maxHeight - this->height());
	else										scrollVertical->setMaximum(0);
}

void Viewport::paintEvent(QPaintEvent *event)
{
	paint();
}

void Viewport::scroll(int value)
{
	paintAndShow();
}

void Viewport::paintAndShow() {
	paint();
	this->repaint();
}

void Viewport::paint() {

	// Init painting region
	this->view.setX((double)this->scrollHorizontal->value());
	this->view.setY((double)this->scrollVertical->value());
	this->view.setWidth(this->width());
	this->view.setHeight(this->height());
	
	// Init painter and background
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, antiAlias);
	painter.fillRect(0, 0, this->width(), this->height(), Qt::black);
	
	// Draw all maps :)
	for(int i = 0; i < this->maps->keys().count(); i++) {
		// Paint the map. Note: Map is responsible for painting it's objects
		if(maps->values().at(i)->show == true) {
			Core::resetPainter(painter);
			maps->keys().at(i)->paint(painter, this->view);
		}
	}
}

void Viewport::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		PersonObject *newPerson = new PersonObject(simulation->physicalMap, (double)event->pos().x() + view.x(), (double)event->pos().y() + view.y());
		newPerson->rotation = qrand() % 360;
		simulation->physicalMap->objects->append(newPerson);
		simulation->controllers->append(new PersonController(simulation, newPerson, 5.0, 10.0));
		this->repaint();
	} else {
		QWidget::mousePressEvent(event);
	}
}

void Viewport::mouseMoveEvent(QMouseEvent *event) {
	QString label;
	switch(simulation->physicalMap->checkForCollision((double)event->pos().x() + view.x(), (double)event->pos().y() + view.y())) {
		case Core::CollisionArea: 	label = "Collision"; break;
		case Core::OpenArea: 		label = "Open"; break;
		case Core::UnknownArea: 	label = "Unknown"; break;
	}
	emit statusUpdate(QString("%1 Area: %2,%3").arg(label).arg(event->pos().x() + view.x()).arg(event->pos().y() + view.y()));
}

void Viewport::keyPressEvent(QKeyEvent *event)
{
	switch(event->key()) {
		case Qt::Key_Up: 		moveMapUp(); break;
		case Qt::Key_Down: 		moveMapDown(); break;
		case Qt::Key_Left: 		moveMapLeft(); break;
		case Qt::Key_Right: 	moveMapRight(); break;
		default:				QWidget::keyPressEvent(event);
	}
}

void Viewport::resizeEvent(QResizeEvent *event) {
	this->resizeScrollbars();
}

void Viewport::moveMapUp() {
	scrollVertical->setValue(scrollVertical->value() - 20);
}

void Viewport::moveMapDown() {
	scrollVertical->setValue(scrollVertical->value() + 20);
}

void Viewport::moveMapLeft() {
	scrollHorizontal->setValue(scrollHorizontal->value() - 20);
}

void Viewport::moveMapRight() {
	scrollHorizontal->setValue(scrollHorizontal->value() + 20);
}

void Viewport::contextMenuEvent(QContextMenuEvent *event)
{
	menuMain->exec(event->globalPos());
}

void Viewport::menuAction(QAction *action) {
	if(action == actionAntiAlias) {
		antiAlias = actionAntiAlias->isChecked();
	} else if(action == actionGrid) {
		maps->value(simulation->gridMap)->show = actionGrid->isChecked();
	} else if(action == actionShowHeatMap) {
		maps->value(simulation->heatMap)->show = actionShowHeatMap->isChecked();
	} else if(action == actionShowPhysicalMap) {
		maps->value(simulation->physicalMap)->show = actionShowPhysicalMap->isChecked();
	}
}


