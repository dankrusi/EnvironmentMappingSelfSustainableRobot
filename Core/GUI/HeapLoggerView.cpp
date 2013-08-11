/*
 *  HeapLoggerView.h
 *
 *
 *  ===========================================================================
 *
 *  Copyright 2008-2009 Daniel Kruesi (Dan Krusi) and David Grob
 *
 *  This file is part of the emms framework.
 *
 *  The emms framework is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The emms framework  is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "HeapLoggerView.h"

#include "../Library/Util.h"
#include "../Library/HeapLogger.h"

#include <QVBoxLayout>

HeapLoggerView::HeapLoggerView(QWidget *parent) : QWidget(parent) {

	// Init
	this->setAttribute(Qt::WA_DeleteOnClose, true);

	// Gui stuff...
	this->setWindowTitle("emss Heap Logger");
	this->setLayout(new QVBoxLayout());

	// Heap Items
	this->layout()->addWidget(new QLabel("Core Objects:"));
	lstHeapItems = new QListWidget();
	this->layout()->addWidget(lstHeapItems);


	// Buttons
	btnRefresh = new QPushButton("Refresh");
	this->layout()->addWidget(btnRefresh);
	btnClose = new QPushButton("Close");
	this->layout()->addWidget(btnClose);


	refreshHeapItems();

	// Connections
	connect(btnRefresh, SIGNAL(clicked()), this, SLOT(refreshHeapItems()));
	connect(btnClose, SIGNAL(clicked()), this, SLOT(close()));
}

HeapLoggerView::~HeapLoggerView() {
}


void HeapLoggerView::refreshHeapItems() {
	lstHeapItems->clear();
	QHash<QString, int> logger =  HeapLogger::getHeapSnapshot();
	QHash<QString, int>::const_iterator i =logger.begin();
	while (i != logger.constEnd()) {
		QListWidgetItem *item = new QListWidgetItem(QString("%1: %2").arg(i.key()).arg(i.value()));
		QColor color = (i.value()==0 ? Qt::blue : Qt::red);
		item->setData(Qt::TextColorRole, color);
		lstHeapItems->addItem(item);
		i++;
	}
}

