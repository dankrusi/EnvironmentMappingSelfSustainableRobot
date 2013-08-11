/*
 * RemoteInterfaceServer.cpp
 *
 *  Created on: 17.02.2009
 *      Author: dkruesi
 */

#include "RemoteInterface.h"

#include "../Library/Debug.h"
#include "RemoteInterfaceListener.h"
#include "RemoteInterfaceSender.h"
#include "RemoteInterfaceMessage.h"

#include "../Task/TaskManager.h"
#include "../Task/FingerprintNavigationTask.h"

RemoteInterface::RemoteInterface(Core *core, QString ip, quint16 port) : CoreObject("RemoteInterface", core){
	this->ip = ip;
	this->port = port;

	subscribers = new QMultiMap<QString,quint16>();
	sendQueue = new QQueue<RemoteInterfaceMessage*>();
	messages = new QList<RemoteInterfaceMessage*>();

	listener = new RemoteInterfaceListener(core, this);
	sender = new RemoteInterfaceSender(core, this);
}

RemoteInterface::~RemoteInterface() {
	delete subscribers;
	delete sendQueue;
	delete messages;
	delete listener;
	delete sender;
}

void RemoteInterface::stop() {

	listener->stop();
	sender->stop();

	Debug::print("[RemoteInterface] stopped");

}

void RemoteInterface::terminate() {
	listener->terminate();
	sender->terminate();
}

void RemoteInterface::start() {
	this->start(QThread::NormalPriority);
}

void RemoteInterface::start(QThread::Priority priority) {

	listener->start(priority);
	sender->start(priority);

	Debug::print("[RemoteInterface] started on %1:%2", ip, port);

}

void RemoteInterface::broadcastMessage(RemoteInterfaceMessage *message) {
	// Enqueue a  message to all our subscribers
	QMap<QString,quint16>::iterator i = subscribers->begin();
	while(i != subscribers->end()){
		// Create a copy of this message based on the given message
		// and change the destination
		RemoteInterfaceMessage *msg = new RemoteInterfaceMessage(*message);
		msg->destinationIp = i.key();
		msg->destinationPort = i.value();
		sendQueue->enqueue(msg);
		i++;
	}

	// Delete the original message
	delete message;
}

bool RemoteInterface::sendMessage(QString ip, quint16 port, RemoteInterfaceMessage *message) {
	// Update destination and enqueue
	message->destinationIp = ip;
	message->destinationPort = port;
	message->sourceIp = this->ip;
	message->sourcePort = this->port;
	sendQueue->enqueue(message);
	return true; // TODO: because of the send queue, this information here is inaccurate...
}

void RemoteInterface::subscribeToServer(QString ip, quint16 port) {
	RemoteInterfaceMessage *message = new RemoteInterfaceMessage("subscribe");
	sendMessage(ip, port, message);
}

void RemoteInterface::unsubscribeFromServer(QString ip, quint16 port) {
	RemoteInterfaceMessage *message = new RemoteInterfaceMessage("unsubscribe");
	sendMessage(ip, port, message);
}

void RemoteInterface::processMessage(RemoteInterfaceMessage *message) {

	// Emit that we received a message
	emit messageReceived(message);

	// Is it a connection message? If so do the necessary bookkeeping...
	if(message->type == "unknown") {
		// This message is not in the correct format, it has no message property
		Debug::warning("[RemoteInterface] received invalid message from %1:%2", ip, message->property("port"));
	} else if(message->type == "subscribe") {
		// Add this to the clients list
		QString ip = message->replyIp();
		quint16 port = message->replyPort();
		if(!subscribers->contains(ip,port)) {
			subscribers->insert(ip,port);
			sendConfirm(ip, port, "subscribe");
			emit hostSubscribed();
			Debug::print("[RemoteInterface] %1:%2 subscribed", ip, message->property("port"));
		} else {
			sendError(ip, port, "already-subscribed");
			Debug::warning("[RemoteInterface] %1:%2 is already subscribed", ip, message->property("port"));
		}
	} else if(message->type == "unsubscribe") {
		// Add this to the clients list
		QString ip = message->replyIp();
		quint16 port = message->replyPort();
		if(subscribers->contains(ip,port)) {
			subscribers->remove(ip,port);
			sendConfirm(ip, port, "unsubscribe");
			emit hostUnsubscribed();
			Debug::print("[RemoteInterface] %1:%2 unsubscribed", ip, message->property("port"));
		} else {
			sendError(ip, port, "not-subscribed");
			Debug::warning("[RemoteInterface] %1:%2 is not subscribed", ip, message->property("port"));
		}
	}
	else if(message->type == "start-task") {
		QString error = "";
		QString task = message->property("task");

		if (task == "fingerprintnavigation"){
			if(message->property("interval") != "" && message->property("wait-time") != "" ) {
				int interval = message->property("interval").toInt();
				int waittime = message->property("wait-time").toInt();
				core->taskManager->addTask(new FingerprintNavigationTask(core, interval, waittime));
			} else {
				error = "missing-keys";
			}
		} else {
			error = "task-undefined";
		}

		if(error == ""){
			sendConfirm(message->replyIp(), message->replyPort(), "start-task");
			Debug::print("[RemoteInterface] started task %1", task);
		} else {
			sendError(message->replyIp(), message->replyPort(), error);
			Debug::warning("[RemoteInterface] error starting %1 task: %2", task, error);
		}

	} else {
		sendError(message->replyIp(), message->replyPort(), "invalid-message-type");
		Debug::warning("[RemoteInterface] invalid message type: %1", message->type);
	}


}

void RemoteInterface::sendConfirm(QString ip, quint16 port, QString confirm){
	RemoteInterfaceMessage *message = new RemoteInterfaceMessage("confirm");
	message->properties->insert("confirm", confirm);
	sendMessage(ip, port, message);
}

void RemoteInterface::sendError(QString ip, quint16 port, QString error){
	RemoteInterfaceMessage *message = new RemoteInterfaceMessage("error");
	message->properties->insert("error", error);
	sendMessage(ip, port, message);
}

void RemoteInterface::addMessageToSent(RemoteInterfaceMessage *message) {
	messages->append(message); // Add to sent list
	emit messageSent(message); // Let others know that a new message was sent
}
