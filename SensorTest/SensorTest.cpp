/*
 *  SensorTest.cpp
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

#include <QLayout>

#include "SensorTest.h"

#include <iostream>

double avg = 0;
int avgi = 0;

SensorTest::SensorTest(QWidget *parent) : QWidget(parent)
{
	// Main window
	this->setLayout(new QVBoxLayout());
	this->setWindowTitle("emss Sensor Test");
	this->layout()->setMargin(5);

	// Connect box
	frameConnect = new QGroupBox("Conneciton");
	this->layout()->addWidget(frameConnect);
	frameConnect->setLayout(new QHBoxLayout());
	cbPort = new QComboBox();
	cbPort->addItem("COM1");
	cbPort->addItem("COM2");
	cbPort->addItem("COM3");
	cbPort->addItem("COM4");
	cbPort->addItem("COM5");
	cbPort->addItem("/dev/ttyUSB0");
	cbPort->addItem("/dev/ttyUSB1");
	cbPort->addItem("/dev/ttyUSB2");
	cbPort->addItem("/dev/ttyUSB3");
	cbPort->addItem("/dev/ttyUSB4");
	cbPort->addItem("/dev/ttyUSB5");
	frameConnect->layout()->addWidget(cbPort);
	btnConnect = new QPushButton("Connect");
	frameConnect->layout()->addWidget(btnConnect);

	// Data box
	frameData = new QGroupBox("Sensor Data");
	this->layout()->addWidget(frameData);
	frameData->setLayout(new QVBoxLayout());
	txtSensorData = new QTextEdit();
	frameData->layout()->addWidget(txtSensorData);

	// Other
	timer = new QTimer();

	// Connections
	connect(btnConnect, SIGNAL(clicked()), this, SLOT(connectDisconnect()));
	connect(timer, SIGNAL(timeout()), this, SLOT(refreshSensors()));

	// Init
	coil = NULL;
	frameData->setEnabled(false);
	cbPort->setCurrentIndex(3);
}

SensorTest::~SensorTest()
{
	if(coil) delete coil;
	if(timer) delete timer;
}

void SensorTest::connectDisconnect() {
	avg = 0;
	avgi = 0;
	if (!coil) {

		coil = new COIL(cbPort->currentText());
		coil->startOI();
		coil->enterSafeMode();

		timer->start(100);

		btnConnect->setText("Disconnect");
		frameData->setEnabled(true);

	} else {

		timer->stop();

		coil->stopOI();
		delete coil;
		coil = NULL;

		btnConnect->setText("Connect");
		frameData->setEnabled(false);

	}
}



void SensorTest::refreshSensors() {



	txtSensorData->clear();
	int* data = coil->getAllSensors();

	if(data != NULL) {

//		avg += coil->extractSensorFromData(data, COIL::SENSOR_WALL_SIGNAL);
//		avgi++;
//		std::cout << avg/(double)avgi << std::endl;

//		std::cout << coil->getIRSensorDistanceFromAnalogSignal(coil->extractSensorFromData(data, COIL::SENSOR_ANALOG_SIGNAL)) << std::endl;

		int x = coil->getIRSensorDistanceFromAnalogSignal(coil->extractSensorFromData(data, COIL::SENSOR_ANALOG_SIGNAL));
		if(x < 800) {
			std::cout << x << "     ";
		}
		//return;

		txtSensorData->append(QString("BUMPS_AND_WHEEL_DROPS:\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_BUMPS_AND_WHEEL_DROPS)));
		txtSensorData->append(QString("WALL:\t\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_WALL)));
		txtSensorData->append(QString("CLIFF_LEFT:\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_LEFT)));
		txtSensorData->append(QString("CLIFF_FRONT_LEFT:\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_FRONT_LEFT)));
		txtSensorData->append(QString("CLIFF_FRONT_RIGHT:\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_FRONT_RIGHT)));
		txtSensorData->append(QString("CLIFF_RIGHT:\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_RIGHT)));
		txtSensorData->append(QString("VIRTUAL_WALL:\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_VIRTUAL_WALL)));
		txtSensorData->append(QString("OVERCURRENT:\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_OVERCURRENT)));
		txtSensorData->append(QString("INFRARED:\t\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_INFRARED)));
		txtSensorData->append(QString("BUTTONS:\t\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_BUTTONS)));
		txtSensorData->append(QString("DISTANCE:\t\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_DISTANCE)));
		txtSensorData->append(QString("ANGLE:\t\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_ANGLE)));
		txtSensorData->append(QString("CHARGING_STATE:\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_CHARGING_STATE)));
		txtSensorData->append(QString("VOLTAGE:\t\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_VOLTAGE)));
		txtSensorData->append(QString("CURRENT:\t\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_CURRENT)));
		txtSensorData->append(QString("BATTERY_TEMP:\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_BATTERY_TEMP)));
		txtSensorData->append(QString("BATTERY_CHARGE:\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_BATTERY_CHARGE)));
		txtSensorData->append(QString("BATTERY_CAPACITY:\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_BATTERY_CAPACITY)));
		txtSensorData->append(QString("WALL_SIGNAL:\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_WALL_SIGNAL)));
		txtSensorData->append(QString("CLIFF_LEFT_SIGNAL:\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_LEFT_SIGNAL)));
		txtSensorData->append(QString("CLIFF_FRONT_LEFT_SIGNAL:\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_FRONT_LEFT_SIGNAL)));
		txtSensorData->append(QString("CLIFF_FRONT_RIGHT_SIGNAL:\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_FRONT_RIGHT_SIGNAL)));
		txtSensorData->append(QString("CLIFF_RIGHT_SIGNAL:\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_RIGHT_SIGNAL)));
		txtSensorData->append(QString("DIGITAL_INPUTS:\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_DIGITAL_INPUTS)));
		txtSensorData->append(QString("ANALOG_SIGNAL:\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_ANALOG_SIGNAL)));
		txtSensorData->append(QString("CHARGING_SOURCES_AVAILABLE:\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_CHARGING_SOURCES_AVAILABLE)));
		txtSensorData->append(QString("OI_MODE:\t\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_OI_MODE)));
		txtSensorData->append(QString("SONG_NUMBER:\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_SONG_NUMBER)));
		txtSensorData->append(QString("SONG_IS_PLAYING:\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_SONG_IS_PLAYING)));
		txtSensorData->append(QString("NUM_STREAM_PACKETS:\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_NUM_STREAM_PACKETS)));
		txtSensorData->append(QString("REQUESTED_VELOCITY:\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_REQUESTED_VELOCITY)));
		txtSensorData->append(QString("REQUESTED_RADIUS:\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_REQUESTED_RADIUS)));
		txtSensorData->append(QString("REQUESTED_RIGHT_VEL:\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_REQUESTED_RIGHT_VEL)));
		txtSensorData->append(QString("REQUESTED_LEFT_VEL:\t\t%1").arg(coil->extractSensorFromData(data, COIL::SENSOR_REQUESTED_LEFT_VEL)));
		txtSensorData->append(QString("SHARP_IR:\t\t%1").arg(coil->getIRSensorDistanceFromAnalogSignal(coil->extractSensorFromData(data, COIL::SENSOR_ANALOG_SIGNAL))));
		txtSensorData->append(QString("WALL_IR:\t\t%1").arg(coil->getWallSensorDistanceFromSignal(coil->extractSensorFromData(data, COIL::SENSOR_WALL_SIGNAL))));

		delete data;

	}

}
