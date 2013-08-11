/*
 *  Viewport.cpp
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

#include "Viewport.h"

#include <QPainter>
#include <QRect>
#include <QPen>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPushButton>
#include <QToolBar>
#include <QList>

#import "../Library/Debug.h"
#import "../Library/Util.h"


Viewport::Viewport(int scale, int autoUpdateInterval, bool openGL, QWidget *parent) : QWidget(parent)
{
	// Init
	this->scale = scale;
	maps = new QList<Map*>();
	mapMetas = new QList<MapMeta*>();
	antiAlias = true;
	selectedAction = NULL;
	timerAutoUpdate = new QTimer();
	timerAutoUpdate->setInterval(autoUpdateInterval);
	this->setAttribute(Qt::WA_DeleteOnClose, true);

	// Toolbar
	this->setLayout(new QVBoxLayout());
	this->layout()->setContentsMargins(0,0,0,0);
	toolbar = new QToolBar();
	toolbar->setAutoFillBackground(true);
	this->layout()->addWidget(toolbar);

	// Add scrollbars...
	gridLayout = new QGridLayout();
	gridLayout->setContentsMargins(0,0,0,0);
	QWidget *gridHolder = new QWidget();
	gridHolder->setLayout(gridLayout);
	this->layout()->addWidget(gridHolder);
	scrollHorizontal = new QScrollBar(Qt::Horizontal);
	scrollHorizontal->setMinimum(0);
	scrollVertical = new QScrollBar(Qt::Vertical);
	scrollVertical->setMinimum(0);
	gridLayout->addWidget(scrollHorizontal,2,1);
	gridLayout->addWidget(scrollVertical,0,2);

	// Canvas
	if(openGL)	canvas = new DrawCanvasGL();
	else		canvas = new DrawCanvas();
	gridLayout->addWidget(canvas, 0,1);

	// Build menu
	menuMain = new QMenu(this);
	QAction *actionLabelNativeOrGL = new QAction( (openGL ? "openGL Render" : "Native Render"), menuMain);
	actionLabelNativeOrGL->setEnabled(false);
	menuMain->addAction(actionLabelNativeOrGL);
	actionAntiAlias = new QAction("Anti Alias", menuMain);
	actionAntiAlias->setCheckable(true);
	actionAntiAlias->setChecked(antiAlias);
	menuMain->addAction(actionAntiAlias);
	actionAutoUpdate = new QAction("Auto Update", menuMain);
	actionAutoUpdate->setCheckable(true);
	actionAutoUpdate->setChecked(true);
	menuMain->addAction(actionAutoUpdate);
	actionAutoFocus = new QAction("Auto Focus", menuMain);
	actionAutoFocus->setCheckable(true);
	actionAutoFocus->setChecked(true);
	menuMain->addAction(actionAutoFocus);
	menuMaps = new QMenu("Maps");
	menuMain->addMenu(menuMaps);

	// Register events
	this->setFocusPolicy(Qt::StrongFocus);
	this->setMouseTracking(true);

	// Connections
	connect(menuMain, SIGNAL(triggered(QAction*)), this, SLOT(menuAction(QAction*)));
	connect(menuMaps, SIGNAL(triggered(QAction*)), this, SLOT(menuAction(QAction*)));
	connect(toolbar, SIGNAL(actionTriggered(QAction*)), this, SLOT(toolbarAction(QAction*)));
	connect(timerAutoUpdate, SIGNAL(timeout()), this, SLOT(paintAndShow()));
	connect(canvas,SIGNAL(draw(QPainter*)), this, SLOT(drawMaps(QPainter*)));
	connect(canvas,SIGNAL(mouseClick(QMouseEvent*)), this, SLOT(canvasClicked(QMouseEvent*)));

	// Start timers...
	resizeScrollbars();
	timerAutoUpdate->start();
}

Viewport::~Viewport()
{
	timerAutoUpdate->stop();
	delete timerAutoUpdate;
	delete maps;
	delete mapMetas;
}

void Viewport::registerMap(Map* map) {

	maps->append(map);
	mapMetas->append(new MapMeta(true));

	QAction *actionShowHideMap = new QAction(map->name, menuMaps);
	actionShowHideMap->setData("map");
	actionShowHideMap->setCheckable(true);
	actionShowHideMap->setChecked(true);
	menuMaps->addAction(actionShowHideMap);
}

void Viewport::resizeScrollbars()
{
	//BUG: the scroll bars are too big!!!
	double scale = 1;
	double maxWidth = 0.0;
	double maxHeight = 0.0;
	for(int i = 0; i < this->maps->count(); i++) {
		if(maps->at(i)->width() > maxWidth) 		maxWidth = maps->at(i)->width();
		if(maps->at(i)->height() > maxHeight) 		maxHeight = maps->at(i)->height();
	}
	if((int)maxWidth - canvas->width()*(1.0/scale) > 0) 	scrollHorizontal->setMaximum((int)(maxWidth - canvas->width()*(1.0/scale)));
	else													scrollHorizontal->setMaximum(0);
	if((int)maxHeight - canvas->height()*(1.0/scale) > 0) 	scrollVertical->setMaximum((int)(maxHeight - canvas->height()*(1.0/scale)));
	else													scrollVertical->setMaximum(0);

	scrollHorizontal->setMaximum((int)maxWidth);
	scrollVertical->setMaximum((int)maxHeight);
}



void Viewport::paintAndShow() {
	canvas->repaint();
}

void Viewport::drawMaps(QPainter *painter) {
	if(!this->isVisible()) return;

	// Init painting region
	this->view.setX((int)(scrollHorizontal->value()));
	this->view.setY((int)(scrollVertical->maximum() - scrollVertical->value()));
	this->view.setWidth((int)(canvas->width()));
	this->view.setHeight((int)(canvas->height()));

	// Init painter and background
	//QPainter painter(this);
	painter->setRenderHint(QPainter::Antialiasing, antiAlias);
	painter->fillRect(0, 0, canvas->width(), canvas->height(), Qt::black);

	// Set the clipping region
	painter->setClipRect(0,0,canvas->width(),canvas->height());

	// Draw all maps :)
	for(int i = 0; i < this->maps->count(); i++) {
		// Paint the map. Note: Map is responsible for painting it's objects
		if(mapMetas->at(i)->show == true) {
			Util::resetPainter(*painter);
			maps->at(i)->paint(*painter, this->view);
		}
	}

	// Make disabled effect
	if(this->isEnabled() == false) {
		painter->setOpacity(0.5);
		painter->fillRect(0, 0, canvas->width(), canvas->height(), Qt::white);
	}
}

void Viewport::canvasClicked(QMouseEvent *event)
{
	QList<QAction*> actions = toolbar->actions();
	for(int i = 0; i < actions.count(); i++) {
		if(actions.at(i)->isChecked() == true) {
			long x = (long)((double)(event->x())) + view.x();
			long y = view.height() - (long)((double)(event->y())) + view.y();
			emit toolbarAction(this, actions.at(i)->data().toString(), x, y);
		}
	}
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

	} else if(action == actionAutoUpdate) {

		if(actionAutoUpdate->isChecked() == true) 	timerAutoUpdate->start();
		else										timerAutoUpdate->stop();

	} else if(action->data() == "map") {

		// This is a map show/hide action, find the correct one and show/hide it...
		for(int i = 0; i < this->maps->count(); i++) {
			if(maps->at(i)->name == action->text()) {
				mapMetas->at(i)->show = action->isChecked();
			}
		}

	}

}

void Viewport::setAntiAlias(bool value) {
	antiAlias = value;
	actionAntiAlias->setChecked(antiAlias);
}

void Viewport::addToolbarAction(QString title, QString value) {
	QAction *action = toolbar->addAction(title);
	action->setCheckable(true);
	action->setData(value);
}

void Viewport::toolbarAction(QAction *action) {
	bool somethingChecked = false;
	QList<QAction*> actions = toolbar->actions();
	for(int i = 0; i < actions.count(); i++) {
		if(actions.at(i)->isChecked() == true) {
			somethingChecked = true;
			break;
		}
	}
	if(somethingChecked == true) 	this->setCursor(Qt::CrossCursor);
	else 							this->setCursor(Qt::ArrowCursor);
}

void Viewport::focusOnPoint(int x, int y) {
	double maxHeight = 0.0;
	for(int i = 0; i < this->maps->count(); i++) {
		if(maps->at(i)->height() > maxHeight) 	maxHeight = maps->at(i)->height();
	}
	scrollHorizontal->setValue(x/scale - this->width()/2);
	scrollVertical->setValue((int)(maxHeight - y/scale + this->height()/2));
}

void Viewport::resetToolbar() {
	QList<QAction*> actions = toolbar->actions();
	for(int i = 0; i < actions.count(); i++) {
		if(actions.at(i)->isChecked() == true) {
			actions.at(i)->setChecked(false);
		}
	}
	this->setCursor(Qt::ArrowCursor);
}

void Viewport::autoFocus(int x, int y) {
	if(actionAutoFocus->isChecked() == true && (previousAutoFocusX != x || previousAutoFocusY != y)) {
		previousAutoFocusX = x;
		previousAutoFocusY = y;
		focusOnPoint(x, y);
	}
}

void Viewport::closeEvent(QCloseEvent *event) {
	// Signal the close and accept the event
	emit closed(this);
	event->accept();
}

