/*
 *  RemoteInterfaceServer.h
 *
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

#ifndef REMOTEINTERFACE_H_
#define REMOTEINTERFACE_H_

#include <QString>
#include <QMultiMap>
#include <QQueue>
#include <QThread>
#include <QList>

#include "../CoreObject.h"

#include "RemoteInterfaceMessage.h"

class RemoteInterfaceListener;
class RemoteInterfaceSender;

class RemoteInterface : public CoreObject
{
	Q_OBJECT

public:
	QString ip;
	quint16 port;

	QMultiMap<QString,quint16> *subscribers;
	QQueue<RemoteInterfaceMessage*> *sendQueue;
	QList<RemoteInterfaceMessage*> *messages;

	RemoteInterfaceListener *listener;
	RemoteInterfaceSender *sender;

public:
	RemoteInterface(Core *core, QString ip, quint16 port);
	virtual ~RemoteInterface();
	void stop();
	void start();
	void terminate();
	void start(QThread::Priority priority);
	void broadcastMessage(RemoteInterfaceMessage *message);
	bool sendMessage(QString ip, quint16 port, RemoteInterfaceMessage *message);
	void sendConfirm(QString ip, quint16 port, QString confirm);
	void sendError(QString ip, quint16 port, QString error);
	void subscribeToServer(QString ip, quint16 port);
	void unsubscribeFromServer(QString ip, quint16 port);
	void processMessage(RemoteInterfaceMessage *message);
	void addMessageToSent(RemoteInterfaceMessage *message);

signals:
	void messageReceived(RemoteInterfaceMessage *message);
	void messageSent(RemoteInterfaceMessage *message);
	void messageReceived();
	void hostUnsubscribed();
	void hostSubscribed();
};

#endif /* REMOTEINTERFACE_H_ */
