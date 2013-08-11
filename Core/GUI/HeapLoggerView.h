/*
 *  HeapLoggerView.h
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

#ifndef HEAPLOGGERVIEW_H_
#define HEAPLOGGERVIEW_H_

#include <QWidget>
#include <QListWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QMultiMap>

class HeapLoggerView : public QWidget {

	Q_OBJECT

	private:

		QListWidget *lstHeapItems;
		QPushButton *btnRefresh;
		QPushButton *btnClose;

	public:
		HeapLoggerView(QWidget *parent = 0);
		virtual ~HeapLoggerView();

	private slots:
		void refreshHeapItems();

};

#endif /* HEAPLOGGERVIEW_H_ */
