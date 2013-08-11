/*
 *  CameraView.cpp
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

#include "CameraView.h"

#include "../Library/Util.h"
#include "../GUI/DrawCanvas.h"

#include <QVBoxLayout>
#include <QProcess>

CameraView::CameraView(QString gstreamerPath, QString gstreamerCommand, QString filePath, int autoRefreshInterval, QWidget *parent) : QWidget(parent) {
	// Init
	this->gstreamerPath = gstreamerPath;
	this->gstreamerCommand = gstreamerCommand;
	this->filePath = filePath;
	this->setMinimumSize(420, 320);
	this->setAttribute(Qt::WA_DeleteOnClose, true);
	timerAutoRefresh = new QTimer();
	timerAutoRefresh->setInterval(autoRefreshInterval);

	// Gui stuff...
	this->setWindowTitle("emss Camera View");
	this->setLayout(new QVBoxLayout());
	canvas = new DrawCanvas();
	this->layout()->addWidget(canvas);
	chkAutoRefresh = new QCheckBox("Auto Refresh");
	chkAutoRefresh->setCheckState(Qt::Unchecked);
	this->layout()->addWidget(chkAutoRefresh);

	// Connections
	connect(timerAutoRefresh, SIGNAL(timeout()), canvas, SLOT(repaint()));
	connect(chkAutoRefresh, SIGNAL(stateChanged(int)), this, SLOT(changeAutoRefresh()));
	connect(canvas,SIGNAL(draw(QPainter*)), this, SLOT(refreshCamera(QPainter*)));

	// Start timers...
	if(chkAutoRefresh->checkState() == Qt::Checked) timerAutoRefresh->start();
}

CameraView::~CameraView() {

}

void CameraView::refreshCamera(QPainter *painter) {

	if(!this->isVisible()) return;

	// Make sure no two GUI threads want to draw at the same time...
	_mutex.lock(); {

		// Create the image from gstreamer by creating a gst-launch process...
		QString args = gstreamerCommand.replace("Camera_CaptureToolOutFile", filePath);
		QProcess *gstreamer = new QProcess(this);
		gstreamer->start(gstreamerPath, args.split(' '));
		gstreamer->waitForFinished();

		// Load the image and paint it
		QImage *image = NULL;
		if(gstreamer->exitCode() == 0) {
			image = new QImage(filePath);
			painter->drawImage(canvas->rect(), *image, image->rect());
		} else {
			Debug::error("[CameraView] Could not execute gstreamer '%1' with arguments '%2'. Exit code was %3!", gstreamerPath, args, gstreamer->exitCode());
		}

		// Cleanup
		if(gstreamer) delete gstreamer;
		if(image) delete image;

	} _mutex.unlock();
}

void CameraView::changeAutoRefresh() {
	if(chkAutoRefresh->checkState() == Qt::Checked) 	timerAutoRefresh->start();
	else												timerAutoRefresh->stop();
}



