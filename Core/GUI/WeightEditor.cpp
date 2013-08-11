/*
 *  WeightEditor.cpp
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

#include "WeightEditor.h"

#include "../Library/Util.h"
#include "../GUI/OptionsDialog.h"
#include "../Navigation/Weight/Weight.h"
#include "../Navigation/Weight/AccelerationFilterWeight.h"
#include "../Navigation/Weight/CollisionAvoidanceWeight.h"
#include "../Navigation/Weight/ControllerSpeedWeight.h"
#include "../Navigation/Weight/FullSpeedWeight.h"
#include "../Navigation/Weight/OrientationWeight.h"
#include "../Navigation/Weight/WallFollowerWeight.h"

#include <QVBoxLayout>
#include <QModelIndexList>

WeightEditor::WeightEditor(Navigation *navigation, QWidget *parent) : QWidget(parent) {

	// Init
	if(navigation != NULL && navigation->name == "System Of Weights") 	this->navigation = (SystemOfWeightsNavigation*)navigation;
	else																this->navigation = NULL;
	this->setAttribute(Qt::WA_DeleteOnClose, true);

	// Gui stuff...
	this->setWindowTitle("emss Weight Editor");
	this->setLayout(new QVBoxLayout());

	lstWeights = new QListWidget();
	this->layout()->addWidget(lstWeights);

	txtWeightInfo = new QTextEdit();
	Util::setTextEditAsLogger(txtWeightInfo);
	this->layout()->addWidget(txtWeightInfo);

	QWidget *frameMoveUpDown = new QWidget();
	frameMoveUpDown->setLayout(new QHBoxLayout());
	btnMoveDown = new QPushButton("Move Down");
	frameMoveUpDown->layout()->addWidget(btnMoveDown);
	btnMoveUp = new QPushButton("Move Up");
	frameMoveUpDown->layout()->addWidget(btnMoveUp);
	this->layout()->addWidget(frameMoveUpDown);

	btnDelete = new QPushButton("Remove Weight");
	this->layout()->addWidget(btnDelete);

	btnAdd = new QPushButton("Add New Weight");
	this->layout()->addWidget(btnAdd);

	btnClose = new QPushButton("Close");
	this->layout()->addWidget(btnClose);

	// Load weights
	refreshList();

	// Connections
	connect(btnMoveDown, SIGNAL(clicked()), this, SLOT(moveWeightDown()));
	connect(btnMoveUp, SIGNAL(clicked()), this, SLOT(moveWeightUp()));
	connect(btnAdd, SIGNAL(clicked()), this, SLOT(addWeight()));
	connect(btnDelete, SIGNAL(clicked()), this, SLOT(deleteWeight()));
	connect(btnClose, SIGNAL(clicked()), this, SLOT(close()));
	connect(lstWeights, SIGNAL(clicked(QModelIndex)), this, SLOT(showWeight(QModelIndex)));

}

WeightEditor::~WeightEditor() {

}

void WeightEditor::refreshList() {

	btnDelete->setEnabled(false);
	btnMoveUp->setEnabled(false);
	btnMoveDown->setEnabled(false);
	btnAdd->setEnabled(false);

	if(!navigation) return;

	// Load tasks
	lstWeights->clear();
	for(int i = 0; i < navigation->getWeights()->count(); i++) {
		QListWidgetItem *item = new QListWidgetItem(QString("%1: %2").arg(i).arg(navigation->getWeights()->at(i)->name));
		item->setData(Qt::UserRole, i);
		lstWeights->addItem(item);
	}

	btnAdd->setEnabled(true);
}

void WeightEditor::showWeight(QModelIndex index) {

	selectedIndex = index.row();
	btnDelete->setEnabled(true);
	btnMoveUp->setEnabled(index.row()!=0);
	btnMoveDown->setEnabled(index.row()!=navigation->getWeights()->count()-1);

	// Show general weight information
	Weight *weight = navigation->getWeights()->at(index.row());
	txtWeightInfo->setText(QString("ID:\t%1\nWeight:\t%2").arg(index.row()).arg(weight->name));
}


void WeightEditor::addWeight() {

	QString weight = OptionsDialog::choose("Acceleration Filter|Collision Avoidance|Controller Speed|Full Speed|Orientation|Wall Follower", "Please select a weight:", "New Weight");
	if(weight == "Acceleration Filter")		navigation->addWeight(new AccelerationFilterWeight(navigation->core));
	if(weight == "Collision Avoidance")		navigation->addWeight(new CollisionAvoidanceWeight(navigation->core));
	if(weight == "Controller Speed")		navigation->addWeight(new ControllerSpeedWeight(navigation->core));
	if(weight == "Full Speed")				navigation->addWeight(new FullSpeedWeight(navigation->core));
	if(weight == "Orientation")				navigation->addWeight(new OrientationWeight(navigation->core));
	if(weight == "Wall Follower")			navigation->addWeight(new WallFollowerWeight(navigation->core));

	refreshList();
}

void WeightEditor::deleteWeight() {

	Weight *weight = navigation->getWeights()->at(lstWeights->currentIndex().row());
	navigation->removeWeight(weight);
	delete weight;

	refreshList();
}

void WeightEditor::moveWeightUp() {
	Weight *weight = navigation->getWeights()->at(lstWeights->currentIndex().row());
	navigation->moveWeightUp(weight);

	int index = lstWeights->currentIndex().row();
	refreshList();
	lstWeights->setCurrentRow(index-1);
	showWeight(lstWeights->currentIndex());
}

void WeightEditor::moveWeightDown() {
	Weight *weight = navigation->getWeights()->at(lstWeights->currentIndex().row());
	navigation->moveWeightDown(weight);

	int index = lstWeights->currentIndex().row();
	refreshList();
	lstWeights->setCurrentRow(index+1);
	showWeight(lstWeights->currentIndex());
}

void WeightEditor::setNavigation(Navigation *navigation) {
	if(navigation != NULL && navigation->name == "System Of Weights") 	this->navigation = (SystemOfWeightsNavigation*)navigation;
	else																this->navigation = NULL;
}

