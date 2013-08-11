/*
 *  TextToSpeechView.cpp
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

#include "TextToSpeechView.h"

#include "../Library/Util.h"

#include <QVBoxLayout>
#include <QProcess>

TextToSpeechView::TextToSpeechView(QString ttsPath, QString ttsCommand, QWidget *parent) : QWidget(parent) {
	// Init
	this->ttsPath = ttsPath;
	this->ttsCommand = ttsCommand;

	// Gui stuff...
	this->setMinimumSize(420, 320);
	this->setAttribute(Qt::WA_DeleteOnClose, true);
	this->setWindowTitle("emss Text to Speech View");
	this->setLayout(new QVBoxLayout());
	inputBox = new QTextEdit();
	Util::setTextEditAsLogger(inputBox);
	inputBox->setReadOnly(false);
	this->layout()->addWidget(inputBox);

	// Connections
	connect(inputBox,SIGNAL(textChanged()), this, SLOT(speakLatestInputLine()));

}

TextToSpeechView::~TextToSpeechView() {

}

void TextToSpeechView::speakLatestInputLine() {
	if(inputBox->toPlainText().endsWith("\n")) {
		QStringList lines = inputBox->toPlainText().split("\n");
		speakText(lines.at(lines.count()-2));
	}
}

void TextToSpeechView::speakText(QString text) {


	// Make sure no two GUI threads want to draw at the same time...
	_mutex.lock(); {

		// Create the image from gstreamer by creating a gst-launch process...
		text = text.replace(" ", "_");
		QString args = QString(ttsCommand).replace("SPEECH_TEXT", text);
		QProcess *tts = new QProcess(this);
		tts->start(ttsPath, args.split(' '));
		tts->waitForFinished();

		// Show error (if any)
		if(tts->exitCode() != 0) {
			Debug::error("[TextToSpeechView] Could not execute tts '%1' with arguments '%2'. Exit code was %3!", ttsPath, args, tts->exitCode());
		}

		// Cleanup
		if(tts) delete tts;

	} _mutex.unlock();
}



