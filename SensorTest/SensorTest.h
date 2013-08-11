/*
 *  SensorTest.h
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

#ifndef SENSORTEST_H
#define SENSORTEST_H

#include <QMainWindow>
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
#include <QTimer>

#include "../Core/Core.h"
#include "../Core/COIL/COIL.h"

class SensorTest : public QWidget
{
    Q_OBJECT

private:

	QGroupBox *frameConnect;
	QComboBox *cbPort;
	QPushButton *btnConnect;

	QGroupBox *frameData;
	QTextEdit *txtSensorData;

	QTimer *timer;
	COIL *coil;

public:
    SensorTest(QWidget *parent = 0);
    ~SensorTest();

public slots:
	void connectDisconnect();
	void refreshSensors();

};

#endif // SENSORTEST_H
