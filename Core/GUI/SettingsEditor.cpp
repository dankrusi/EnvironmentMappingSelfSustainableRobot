/*
 *  SettingsEditor.cpp
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

#include "SettingsEditor.h"

#include "../Library/Util.h"

#include <QVBoxLayout>
#include <QModelIndexList>

SettingsEditor::SettingsEditor(QSettings *settings, QWidget *parent) : QWidget(parent) {
	// Init
	this->settings = settings;
	tabs = new QHash<QString,QTextEdit*>();

	// Gui stuff...
	this->setWindowTitle("emss Settings Editor");
	this->setLayout(new QVBoxLayout());
	this->setMinimumSize(500, 600);
	this->setAttribute(Qt::WA_DeleteOnClose, true);

	// Tab panel and save button
	tabPanel = new QTabWidget();
	this->layout()->addWidget(tabPanel);
	btnSave = new QPushButton("Save");
	this->layout()->addWidget(btnSave);

	// Load the settings into the text widget...
	loadSettings();

	// Connections
	connect(btnSave, SIGNAL(clicked()), this, SLOT(saveSettings()));
}

SettingsEditor::~SettingsEditor() {
	delete tabs;
}

void SettingsEditor::loadSettings() {
	// Load settings...
	for(int i = 0; i < settings->allKeys().count(); i++) {

		// Grab a tab key based on the setting name
		QString searchChar = "_";
		if(settings->allKeys().at(i).indexOf(searchChar) == -1) searchChar = "=";
		QString key = settings->allKeys().at(i).left(settings->allKeys().at(i).indexOf(searchChar));

		// If tab is not already existent, create one. Otherwise just
		// add to that tab...
		if(!tabs->contains(key)) {
			QTextEdit *txt = new QTextEdit();
			Util::setTextEditAsLogger(txt);
			txt->setReadOnly(false);
			tabs->insert(key, txt);
			tabPanel->addTab(txt, key);
		}

		tabs->value(key)->insertPlainText(QString("%1=%2\n").arg(settings->allKeys().at(i)).arg(settings->value(settings->allKeys().at(i)).toString()));
		btnSave->setEnabled(true);
	}
}

void SettingsEditor::saveSettings() {

	// Compile back from the tabs a single data pool and save to settings...
	QString textData;
	for(int i = 0; i < tabs->keys().count(); i++) {
		if(!textData.endsWith("\n") && i != 0) textData.append("\n");
		textData.append(tabs->value(tabs->keys().at(i))->toPlainText());
	}
	Util::setSettingsFromTextData(settings, textData);
	this->close();
}
