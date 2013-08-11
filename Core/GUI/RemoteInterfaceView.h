/*
 *  RemoteInterfaceView.h
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

#ifndef REMOTEINTERFACEVIEW_H_
#define REMOTEINTERFACEVIEW_H_

#include <QWidget>
#include <QListWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QMultiMap>

class RemoteInterface;
class RemoteInterfaceMessage;

class RemoteInterfaceView : public QWidget {

	Q_OBJECT

	private:
		RemoteInterface *remoteInterface;
		RemoteInterfaceMessage *selectedMessage;

		QListWidget *lstMessages;
		QListWidget *lstSubscribers;
		QTextEdit *txtMessageInfo;
		QPushButton *btnClose;
		QLabel *lblSettings;

	public:
		RemoteInterfaceView(RemoteInterface *remoteInterface, QWidget *parent = 0);
		virtual ~RemoteInterfaceView();

	private slots:
		void showMessage(QModelIndex index);
		void addMessageToList(RemoteInterfaceMessage *message);
		void messageSent(RemoteInterfaceMessage *message);
		void messageReceived(RemoteInterfaceMessage *message);
		void subscribersChanged();

	public slots:
		void setRemoteInterface(RemoteInterface *remoteInterface);
};

#endif /* REMOTEINTERFACEVIEW_H_ */
