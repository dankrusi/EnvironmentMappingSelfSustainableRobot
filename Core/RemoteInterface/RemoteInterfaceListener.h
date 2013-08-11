/*
 * RemoteInterfaceListener.h
 *
 *  Created on: 17.02.2009
 *      Author: dkruesi
 */

#ifndef REMOTEINTERFACELISTENER_H_
#define REMOTEINTERFACELISTENER_H_

#include "../CoreThread.h"
#include "RemoteInterface.h"


class RemoteInterfaceListener : public CoreThread {

	Q_OBJECT

private:
	bool stopRequested;

public:
	RemoteInterface *server;

public:
	RemoteInterfaceListener(Core *core, RemoteInterface *server);
	virtual ~RemoteInterfaceListener();
	virtual void run();
	void stop();
	void start(QThread::Priority priority);

signals:
	void messageRecieved(RemoteInterfaceMessage message);

};

#endif /* REMOTEINTERFACELISTENER_H_ */
