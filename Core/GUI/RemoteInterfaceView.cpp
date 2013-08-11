/*
 *  RemoteInterfaceView.cpp
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

#include "RemoteInterfaceView.h"

#include "../RemoteInterface/RemoteInterface.h"

#include "../Library/Util.h"

#include <QVBoxLayout>
#include <QModelIndexList>

RemoteInterfaceView::RemoteInterfaceView(RemoteInterface *remoteInterface, QWidget *parent) : QWidget(parent) {

	// Init
	this->setAttribute(Qt::WA_DeleteOnClose, true);
	selectedMessage = NULL;

	// Gui stuff...
	this->setWindowTitle("emss Remote Interface");
	this->setLayout(new QVBoxLayout());

	// Settings/info
	this->layout()->addWidget(new QLabel("Server Settings:"));
	lblSettings = new QLabel("");
	this->layout()->addWidget(lblSettings);

	// Messages
	this->layout()->addWidget(new QLabel("Messages:"));
	lstMessages = new QListWidget();
	this->layout()->addWidget(lstMessages);

	// Message info
	this->layout()->addWidget(new QLabel("Message Contents:"));
	txtMessageInfo = new QTextEdit();
	Util::setTextEditAsLogger(txtMessageInfo);
	this->layout()->addWidget(txtMessageInfo);

	// Subscribers
	this->layout()->addWidget(new QLabel("Subscribers:"));
	lstSubscribers = new QListWidget();
	this->layout()->addWidget(lstSubscribers);

	// Close button
	btnClose = new QPushButton("Close");
	this->layout()->addWidget(btnClose);

	// Set the interface (requires a initialzied gui)
	this->setRemoteInterface(remoteInterface);

	// Connections
	connect(btnClose, SIGNAL(clicked()), this, SLOT(close()));
	connect(lstMessages, SIGNAL(clicked(QModelIndex)), this, SLOT(showMessage(QModelIndex)));
}

RemoteInterfaceView::~RemoteInterfaceView() {
}


void RemoteInterfaceView::addMessageToList(RemoteInterfaceMessage *message) {
	if(message) {
		bool incomming = false;
		if(message->destinationIp == remoteInterface->ip && message->destinationPort == remoteInterface->port) incomming = true;
		QString direction = (incomming ? "From" : "To");
		QColor color = (incomming ? Qt::blue : Qt::red);
		QListWidgetItem *item = new QListWidgetItem(QString("%1 %2:%3 - %4").arg(direction).arg( (incomming ? message->sourceIp : message->destinationIp) ).arg( (incomming ? message->sourcePort : message->destinationPort) ).arg(message->type));
		item->setData(Qt::TextColorRole, color);
		item->setData(Qt::UserRole, (int)message); // This is bullshit, why can a listitem not be connected with a QObject? Fine! I will just cast the pointer to int (yes, a QVariant) and then backcast it later...
		lstMessages->addItem(item);
	}
}

void RemoteInterfaceView::messageSent(RemoteInterfaceMessage *message) {
	addMessageToList(message);
}

void RemoteInterfaceView::messageReceived(RemoteInterfaceMessage *message) {
	addMessageToList(message);
}

void RemoteInterfaceView::subscribersChanged() {
	// Refresh the entire list
	lstSubscribers->clear();
	if(remoteInterface) {
		for(int i = 0; i < remoteInterface->subscribers->count(); i++) {
			lstSubscribers->addItem(new QListWidgetItem(QString("%1:%2").arg(remoteInterface->subscribers->keys().at(i)).arg(remoteInterface->subscribers->values().at(i))));
		}
	}
}

void RemoteInterfaceView::showMessage(QModelIndex index) {

	// This is bullshit, why can a listitem not be connected with a QObject? Fine! I will just cast the pointer to int (yes, a QVariant) and then backcast it later...
	QListWidgetItem *item = lstMessages->item(index.row());
	RemoteInterfaceMessage *message = (RemoteInterfaceMessage*)(item->data(Qt::UserRole)).toInt(NULL);

	// Show message information
	txtMessageInfo->setText("");
	if(message) {
		txtMessageInfo->append(QString("Type:\t\t%1").arg(message->type));
		txtMessageInfo->append(QString("Source:\t\t%1:%2").arg(message->sourceIp).arg(message->sourcePort));
		txtMessageInfo->append(QString("Destination:\t%1:%2").arg(message->destinationIp).arg(message->destinationPort));
		txtMessageInfo->append(QString("Reply:\t\t%1:%2").arg(message->replyIp()).arg(message->replyPort()));
		txtMessageInfo->append(QString("Timestamp:\t\t%1").arg(message->timeStamp.toString("hh:mm:ss:zzz")));
		txtMessageInfo->append("");
		txtMessageInfo->append(message->toString());
	}
}


void RemoteInterfaceView::setRemoteInterface(RemoteInterface *remoteInterface) {

	this->remoteInterface = remoteInterface;

	lstMessages->clear();
	lstSubscribers->clear();

	if(remoteInterface) {
		// Show all subscribers
		subscribersChanged();

		// Load existing messages...
		for(int i = 0; i < remoteInterface->messages->count(); i++) {
			RemoteInterfaceMessage *message = remoteInterface->messages->at(i);
			addMessageToList(message);
		}

		// Make connections
		connect(remoteInterface, SIGNAL(messageSent(RemoteInterfaceMessage*)), this, SLOT(messageSent(RemoteInterfaceMessage*)));
		connect(remoteInterface, SIGNAL(messageReceived(RemoteInterfaceMessage*)), this, SLOT(messageReceived(RemoteInterfaceMessage*)));
		connect(remoteInterface, SIGNAL(hostUnsubscribed()), this, SLOT(subscribersChanged()));
		connect(remoteInterface, SIGNAL(hostSubscribed()), this, SLOT(subscribersChanged()));

		// Set server info
		lblSettings->setText(QString("IP:\t%1\nPort:\t%2").arg(remoteInterface->ip).arg(remoteInterface->port));

	} else {
		lblSettings->setText(QString("Server IP:\nServer Port:"));
	}
}
