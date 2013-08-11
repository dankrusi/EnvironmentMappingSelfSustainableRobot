/*
 *  Util.h
 *
 *  Collection of various practical utility methods.
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

#ifndef UTIL_H_
#define UTIL_H_

#include <QPainter>
#include <QTextEdit>
#include <QSettings>
#include <QFileInfo>
#include <QInputDialog>

#include "../Library/Math.h"
#include "../Library/Debug.h"

namespace Util {


static void inline resetPainter(QPainter &painter, double scale = 1.0) {
	painter.setOpacity(1.0);
	painter.resetTransform();
	painter.resetMatrix();
	painter.setPen(Qt::black);
	painter.setBrush(Qt::white);
	painter.setFont(QFont());
	painter.scale(scale, scale);
}

static void setTextEditAsLogger(QTextEdit *textEdit) {
	textEdit->setReadOnly(true);
	textEdit->setFontFamily("Lucida Console");
	textEdit->setWordWrapMode(QTextOption::NoWrap);
	QFont fntConsole = textEdit->font();
	//fntConsole.setPointSize(fntConsole.pointSize()+2);
	textEdit->setFont(fntConsole);
	QPalette palette = textEdit->palette();
	palette.setColor(QPalette::Base, Qt::darkBlue);
	palette.setColor(QPalette::Text, Qt::white);
	textEdit->setPalette(palette);
}

static void drawSpline(QPainter &painter, Spline1D *splineX, Spline1D *splineY, long movementScale, QColor color, int resolution) {
	// Draw nav spline
	QPen pen = painter.pen();
	pen.setWidth(1);
	pen.setColor(color);
	painter.setPen(pen);
	for (int i = 0; i < splineX->getNodeCount() - 1; i++) {

		for (int t = 1; t < resolution+1; t++) {
			double tt = ((double)t) / (double)resolution;
			double x = splineX->getValue(i, tt) / movementScale;
			double y = splineY->getValue(i, tt) / movementScale;
			painter.drawPoint((int)x, (int)-y);
		}
	}
}

static void setSettingsFromTextData(QSettings *settings, QString textData) {
	settings->clear();
	QStringList data = textData.split('\n');
	for(int i = 0; i < data.count(); i++) {
		QString line = data.at(i);
		line = line.replace("\\=", "{EQUAL_SIGN}");
		QStringList keyValue = line.split('=');
		if(keyValue[0] != "") {
			settings->setValue(keyValue[0], keyValue[1].replace("{EQUAL_SIGN}","\\="));
		}
	}
}

static QString getSettingsFilePath(QString name) {

	Debug::warning("[Util] getSettingsFilePath() is deprecated!");

	QSettings settings("emss", "Core");
	QString filePath = settings.value(QString("SETTINGS_%1").arg(name)).toString();
	QFileInfo fileInfo(filePath);
	if(fileInfo.exists() == false) {
		filePath = QInputDialog::getText(NULL, QString("%1 Settings").arg(name), "Please enter the full settings file path:");
		settings.setValue(QString("SETTINGS_%1").arg(name), filePath);
	}
	return filePath;
}

static QString getResourcesFilePath() {
	QSettings settings("emss", "Resources");
	QString filePath = settings.value(QString("RESOURCE_PATH")).toString();
	QFileInfo fileInfo(filePath);
	if(!(filePath.endsWith("\\") || filePath.endsWith("/")) || fileInfo.isDir() == false) {
		filePath = QInputDialog::getText(NULL, QString("emss Resource Path"), "Please enter the full emss Resources path including the trailing slash.\nOn Windows, such a path might look like 'C:\\emss\\Resources\\',\nwhile on Unix such a path will look like '/opt/emss/Resources/'.");
		settings.setValue(QString("RESOURCE_PATH"), filePath);
	}
	return filePath;
}

static QString getLogsFilePath() {
	QSettings settings("emss", "Resources");
	QString filePath = settings.value(QString("LOGS_PATH")).toString();
	QFileInfo fileInfo(filePath);
	if(fileInfo.isDir() == false) {
		filePath = QInputDialog::getText(NULL, QString("emss Log Path"), "Please enter the full emss Log-Files path including the trailing slash.\nOn Windows, such a path might look like 'C:\\emss\\Logs\\',\nwhile on Unix such a path will look like '/opt/emss/Logs/'.");
		settings.setValue(QString("LOGS_PATH"), filePath);
	}
	return filePath;
}

// Returns a GPSPosition structure based on a space seperated GPS Latitude-Longitude-Height coords...
// Note: The height is optional.
// Note: this function is not included in gps.h in order to decrease gps.h's dependency on Qt (QString)
// Examples: "47.227736 8.816255"-->returns GPS 47.227736-8.816255-0, "47.227736 8.816255 33.00", ""-->returns GPS 0-0-0
static GPSPosition gpsPositionFromString(QString latitudeLongitudeHeight) {
	QStringList llh = latitudeLongitudeHeight.split(" ");
	if(llh.count() == 2) {
		return GPSPosition(llh[0].toDouble(),llh[1].toDouble(),0);
	} else if(llh.count() == 3) {
		return GPSPosition(llh[0].toDouble(),llh[1].toDouble(),llh[2].toDouble());
	} else {
		return GPSPosition(0,0,0);
	}
}

};

#endif /* UTIL_H_ */
