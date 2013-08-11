/*
 *  SettingsEditor.h
 *
 *  Any emss application, including the Core itself, may have various settings.
 *  The Settings Editor class provides a simple text field where all
 *  settings are displayed and may be edited. The class accepts any standard
 *  QSettings data structure, making it plug-and-play with all Qt settings
 *  objects.
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

#ifndef SETTINGSEDITOR_H_
#define SETTINGSEDITOR_H_

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QSettings>
#include <QTabWidget>
#include <QHash>


class SettingsEditor : public QWidget {

	Q_OBJECT

private:
	QSettings *settings;
	QHash<QString,QTextEdit*> *tabs;

private:
	QTabWidget *tabPanel;
	QPushButton *btnSave;

public:
	SettingsEditor(QSettings *settings, QWidget *parent = 0);
	virtual ~SettingsEditor();

public slots:
	void loadSettings();
	void saveSettings();

};

#endif /* SETTINGSEDITOR_H_ */
