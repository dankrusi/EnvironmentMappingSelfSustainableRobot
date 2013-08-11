/*
 *  Interface.h
 *
 *  The Interface application emssInterface is the predominant graphical user
 *  interface to control the emss robot and other Core components. The application
 *  is divided into three groups: Connection, Controller, and Data. The Connection
 *  group allows one to connect and disconnect with the emss hardware. It supports
 *  the selection of all the implemented Core Controllers and Movement Trackers.
 *  The Controller panel features various tabs each designed specifically for the
 *  individual Controllers. The important diagnostic panel History is located inside
 *  the Data group box. The history widget provides all necessary diagnostic and
 *  debugging information which streams from the various Core components. Along
 *  with the History panel, other data sources are provided such as Serial Port
 *  and Sensors. The emss Interface allows multiple instances of Core Viewports,
 *  as well as modal windows such as the Task Editor and Settings Editor of both
 *  the Core configuration settings and the Interface settings.
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

#ifndef Interface_H
#define Interface_H

#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include <QSlider>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QTextEdit>
#include <QLCDNumber>
#include <QMenuBar>
#include <QGroupBox>
#include <QTabWidget>
#include <QSettings>
#include <QApplication>
#include <QList>
#include <QCloseEvent>


#include "../Core/Core.h"

#include "../Core/GUI/Viewport.h"
#include "../Core/GUI/Joystick.h"
#include "../Core/GUI/RobotControlPanel.h"
#include "../Core/GUI/RobotSpeedGraph.h"
#include "../Core/GUI/TaskEditor.h"
#include "../Core/GUI/WeightEditor.h"
#include "../Core/GUI/MapOverview.h"

#include "../Core/Library/Debug.h"


class Interface : public QWidget
{
    Q_OBJECT

private:

	QMenuBar 	*menu;
	QMenu 		*menuInterface;
	QAction	 	*actionInterfaceExit;
	QAction	 	*actionInterfaceReset;
	QAction	 	*actionInterfaceSettingsCore;
	QAction	 	*actionInterfaceSettingsController;
	QMenu 		*menuView;
	QAction	 	*actionViewNewViewport;
	QAction	 	*actionViewNewMapOverview;
	QAction	 	*actionViewTaskEditor;
	QAction	 	*actionViewWeightEditor;
	QAction	 	*actionViewCameraView;
	QAction	 	*actionViewTextToSpeechView;
	QAction	 	*actionViewRemoteInterface;
	QAction	 	*actionViewHeapLogger;

	QFrame *frameTop;
	QFrame *frameTopLeft;
	QGroupBox *frameTopLeftBottom;
	QGroupBox *frameBottom;
	QFrame *frameSensorData;

	QWidget *tabEmssController;
	QWidget *tabConnection;

	QPushButton *btnConnect;
	QPushButton *btnAbort;
	QPushButton *btnRefreshSensors;

	QSlider *sliderSpeed;

	QComboBox *cbPort;
	QComboBox *cbTracker;
	QComboBox *cbController;
	QComboBox *cbNavigation;

	QCheckBox *checkEmulation;
	QCheckBox *checkSafeMode;

	QLabel *lblSpeed;

	QTextEdit *txtHistory;
	QTextEdit *txtSerialPort;
	QTextEdit *txtSensorData;

	QTabWidget *tabLoggers;
	QTabWidget *tabNavigation;

	QLCDNumber *lcdSpeed;

	Joystick *joystickEmssController;

	RobotControlPanel *robotControlPanel;
	RobotSpeedGraph *robotSpeedGraph;

	QList<Viewport*> *viewports;

	TaskEditor *taskEditor;
	WeightEditor *weightEditor;

	bool ignoreViewportClosedSignal;

public:
	Core *core;
	QSettings *settings;

public:
    Interface(QWidget *parent = 0);
    ~Interface();
    double calculateAccuracy(int expected, int result);

protected:
	void closeEvent(QCloseEvent *event);

public slots:
	void connectDisconnect();
	void refreshSensors();
	void menuAction(QAction *action);
	void viewportAction(Viewport *viewport, QString value, long x, long y);
	void viewportClosed(Viewport *viewport);
	void focusOnPoint(long x, long y);
};

#endif // Interface_H
