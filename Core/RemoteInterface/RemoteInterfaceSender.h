/*
 * RemoteInterfaceSender.h
 *
 *  Created on: 17.02.2009
 *      Author: dkruesi
 */

#ifndef REMOTEINTERFACESENDER_H_
#define REMOTEINTERFACESENDER_H_

#include <QUdpSocket>

#include "../CoreThread.h"
#include "RemoteInterface.h"


class RemoteInterfaceSender : public CoreThread {

	Q_OBJECT

private:
	bool stopRequested;

public:
	RemoteInterface *server;

public:
	RemoteInterfaceSender(Core *core, RemoteInterface *server);
	virtual ~RemoteInterfaceSender();
	virtual void run();
	void stop();
	void start(QThread::Priority priority);
	bool sendMessage(RemoteInterfaceMessage* message);
};

#endif /* REMOTEINTERFACESENDER_H_ */
