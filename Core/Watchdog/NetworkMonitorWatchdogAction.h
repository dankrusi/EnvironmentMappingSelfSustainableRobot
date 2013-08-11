/*
 * NetworkMonitorWatchdogAction.h
 *
 *  Created on: Mar 20, 2009
 *      Author: dankrusi
 */

#ifndef NETWORKMONITORWATCHDOGACTION_H_
#define NETWORKMONITORWATCHDOGACTION_H_

#include "WatchdogAction.h"

#include "../Controller/Controller.h"

#include "../Library/Debug.h"

#include <QNetworkInterface>


class NetworkMonitorWatchdogAction : public WatchdogAction
{
	Q_OBJECT

private:
	bool reactedOnNoWANIP;

public:

	NetworkMonitorWatchdogAction(Core *core) : WatchdogAction("Beep", core) {
		reactedOnNoWANIP = false;
	}

	virtual ~NetworkMonitorWatchdogAction() {

	}

	virtual void process() {

		// Go through all the interfaces looking for a match with one of the
		// suffixes provided in the config file of the format
		// Watchdog_NetworkMonitorWatchdogAction_WANIPs=152.97.|152.96.
		// For multiple WAN matches use the | character.
		bool wanIPFound = false;
		QStringList wanIPs = core->stringSetting("Watchdog_NetworkMonitorWatchdogAction_WANIPs").split("|");
		foreach(QHostAddress adr, QNetworkInterface::allAddresses()) {
			foreach(QString wanIP, wanIPs) {
				if(adr.toString().startsWith(wanIP)) {
					wanIPFound = true;
					break;
				}
			}
		}

		// If no WAN IP found then tell core to abort
		if(!wanIPFound && !reactedOnNoWANIP) {
			Debug::warning("[NetworkMonitorWatchdogAction] no WAN IP was found");
			core->controller->emergencyStop();
			core->stop();
			reactedOnNoWANIP = true;
		} else if(wanIPFound) {
			reactedOnNoWANIP = false;
		}


	}

	virtual QString description() {
		return QString("Name:\t%1").arg(name);
	}

};

#endif /* NETWORKMONITORWATCHDOGACTION_H_ */
