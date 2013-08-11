/*
 *  TaskList.cpp
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

#include "TaskList.h"

TaskList::TaskList() : CoreObject("TaskList"){
	list = new QList<Task*>();
	currentTask = -1;
}

TaskList::~TaskList() {
	delete list;
}

void TaskList::add(Task *task) {
	_lock.lockForWrite();
	{
		list->append(task);
	}
	_lock.unlock();
}

int TaskList::count() {
	int retVal;
	_lock.lockForRead();
	{
		retVal = list->count();
	}
	_lock.unlock();
	return retVal;
}

Task* TaskList::getCurrentTask() {
	Task* retVal;
	_lock.lockForRead();
	{
		retVal = list->at(currentTask);
	}
	_lock.unlock();
	return retVal;
}

Task* TaskList::at(int index) {
	Task* retVal;
	_lock.lockForRead();
	{
		retVal = list->at(index);
	}
	_lock.unlock();
	return retVal;
}

Task* TaskList::getNextTask() {

	Task *task = NULL;

	_lock.lockForRead(); {

		// Make sure there is a next task..
		if(list->count() > 0 && currentTask+1 <= list->count()-1) {

			currentTask++;
			task = this->at(currentTask);
		}

	} _lock.unlock();

	return task;
}

QList<Task*> TaskList::getSpecificTasks(QString type) {
	QList<Task*> specificTasks;
	_lock.lockForRead(); {
		for(int i = 0; i < this->count(); i++) {
			if(this->at(i)->getName() == type) specificTasks.append(this->at(i));
		}
	} _lock.unlock();
	return specificTasks;

}

Task* TaskList::getNextSpecificTask(QString type) {
	_lock.lockForRead(); {
		Task *nextTask = this->getNextTask();
		while(nextTask != NULL) {
			if(nextTask->getName() == type) {
				_lock.unlock();
				return nextTask;
			}
			nextTask = this->getNextTask();
		}
	} _lock.unlock();
	return NULL;

}

