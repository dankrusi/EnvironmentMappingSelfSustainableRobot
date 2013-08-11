/*
 *  TextToSpeechView.h
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

#ifndef TEXTTOSPEECHVIEW_H_
#define TEXTTOSPEECHVIEW_H_

#include <QWidget>
#include <QTextEdit>
#include <QMutex>

class TextToSpeechView : public QWidget {

	Q_OBJECT

private:
	QMutex _mutex;
	QString ttsPath;
	QString ttsCommand;

public:
	QTextEdit *inputBox;

public:
	TextToSpeechView(QString ttsPath, QString ttsCommand, QWidget *parent = 0);
	virtual ~TextToSpeechView();

public slots:
	void speakLatestInputLine();
	void speakText(QString text);


};

#endif /* TEXTTOSPEECHVIEW_H_ */
