/*
 * CoreObject.h
 *
 *  Created on: Apr 6, 2009
 *      Author: dgrob
 */

#ifndef COREOBJECT_H_
#define COREOBJECT_H_

#include <QObject>

#include "Core.h"

#include "Library/HeapLogger.h"

class CoreObject : public QObject
{
	Q_OBJECT


public:
	QString name;
	Core *core;

public:
	CoreObject(QString name, Core *core = NULL) {
		this->name = name;
		this->core = core;
		HeapLogger::registerHeapMemory(this);
	}


	virtual ~CoreObject() {
		HeapLogger::freeHeapMemory(this);
	}


};

#endif /* COREOBJECT_H_ */
