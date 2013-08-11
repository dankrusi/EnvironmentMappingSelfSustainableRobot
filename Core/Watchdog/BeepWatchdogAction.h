/*
 * BeepWatchdogAction.h
 *
 *  Created on: Mar 20, 2009
 *      Author: dankrusi
 */

#ifndef BEEPWATCHDOGACTION_H_
#define BEEPWATCHDOGACTION_H_

#include "WatchdogAction.h"

#include "../Library/Debug.h"
#include "../Library/Util.h"
#include "../Library/Audio.h"

class BeepWatchdogAction : public WatchdogAction
{
	Q_OBJECT

private:
	QAlsaSound *beep;

public:

	BeepWatchdogAction(Core *core) : WatchdogAction("Beep", core) {
		beep = new QAlsaSound(Util::getResourcesFilePath().append(core->stringSetting("Watchdog_BeepWatchdogAction_BeepFile")));
	}

	virtual ~BeepWatchdogAction() {
		delete beep;
	}

	virtual void process() {
		if(beep->isAvailable()) beep->play(false);
	}

	virtual QString description() {
		return QString("Name:\t%1").arg(name);
	}

};


#endif /* BEEPWATCHDOGACTION_H_ */
