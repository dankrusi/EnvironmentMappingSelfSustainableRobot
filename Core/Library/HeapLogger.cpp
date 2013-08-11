/*
 * HeapLogger.cpp
 *
 *  Created on: Apr 6, 2009
 *      Author: dgrob
 */
#include "HeapLogger.h"

#include <QReadLocker>

#include "../CoreObject.h"
#include "../CoreThread.h"

#include "Debug.h"



QHash<QString, int> HeapLogger::logger;
QReadWriteLock HeapLogger::lock;

void HeapLogger::registerHeapMemory(CoreObject *coreObject){
#ifdef HEAPLOGGER_ENABLED
	registerHeapMemory(coreObject->name);
#endif
}

void HeapLogger::freeHeapMemory(CoreObject *coreObject){
#ifdef HEAPLOGGER_ENABLED
	freeHeapMemory(coreObject->name);
#endif
}

void HeapLogger::registerHeapMemory(CoreThread *coreThread){
#ifdef HEAPLOGGER_ENABLED
	registerHeapMemory(coreThread->name);
#endif
}

void HeapLogger::freeHeapMemory(CoreThread *coreThread){
#ifdef HEAPLOGGER_ENABLED
	freeHeapMemory(coreThread->name);
#endif
}

void HeapLogger::registerHeapMemory(QString name){
#ifdef HEAPLOGGER_ENABLED
	lock.lockForWrite();
	{
		int i = logger.take(name);
		i++;
		logger.insert(name, i);
	}
	lock.unlock();
#endif
}

void HeapLogger::freeHeapMemory(QString name){
#ifdef HEAPLOGGER_ENABLED
	lock.lockForWrite();
	{
		int i = logger.take(name);
		if (i == 0){
			lock.unlock();
			return;
		}
		else {
			i--;
			logger.insert(name, i);
		}
	}
	lock.unlock();
#endif
}

void HeapLogger::printMemoryinUse(){
#ifdef HEAPLOGGER_ENABLED
	lock.lockForRead();
	{
		 Debug::print("Memory still in use\n------------------");
		 QHash<QString, int>::const_iterator i = logger.begin();
		 while (i != logger.constEnd()) {
			 Debug::print("%1 : %2", i.key(), i.value());
			 i++;
		 }
	}
	lock.unlock();
#endif
}


QHash<QString, int> HeapLogger::getHeapSnapshot() {
#ifdef HEAPLOGGER_ENABLED
	QReadLocker locker(&lock);
#endif
	return logger;
}
