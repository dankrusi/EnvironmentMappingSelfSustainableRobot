/*
 * CoreThread.h
 *
 *  Created on: Apr 6, 2009
 *      Author: dgrob
 */

#ifndef CORETHREAD_H_
#define CORETHREAD_H_

#include <QThread>

#include "Core.h"

#include "Library/HeapLogger.h"

class CoreThread : public QThread
{
	Q_OBJECT


public:
	QString name;
	Core *core;

public:
	CoreThread(QString name, Core *core) {
		this->name = name;
		this->core = core;
		HeapLogger::registerHeapMemory(this);
	}

	virtual ~CoreThread() {
		HeapLogger::freeHeapMemory(this);
	}


};

#endif /* CORETHREAD_H_ */
