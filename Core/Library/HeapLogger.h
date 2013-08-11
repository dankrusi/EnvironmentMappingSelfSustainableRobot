/*
 * HeapLogger.h
 *
 *  Created on: Apr 6, 2009
 *      Author: dgrob
 */

#ifndef HEAPLOGGER_H_
#define HEAPLOGGER_H_

class CoreObject;
class CoreThread;

#include <QHash>
#include <QReadWriteLock>

// Remove the HEAPLOGGER_ENABLED definition to disable all heap logging calls at compile
// time. GNU G++ should optimize away the empty functions even without the -O flag.
#define HEAPLOGGER_ENABLED

class HeapLogger{

private:
	static QReadWriteLock lock;
	static QHash<QString, int> logger;
public:
	static void registerHeapMemory(CoreObject *coreObject);
	static void freeHeapMemory(CoreObject *coreObject);
	static void registerHeapMemory(CoreThread *coreThread);
	static void freeHeapMemory(CoreThread *coreThread);
	static void registerHeapMemory(QString name);
	static void freeHeapMemory(QString name);
	static void printMemoryinUse();
	static QHash<QString, int> getHeapSnapshot();

};

#endif /* HEAPLOGGER_H_ */
