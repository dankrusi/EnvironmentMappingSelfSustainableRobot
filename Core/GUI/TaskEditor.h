/*
 *  TaskEditor.h
 *
 *  The Task Manager holds a list with all created Tasks. The Task Editor
 *  class allows an emss application to easily manage the Cores current
 *  Tasks. In addition to displaying Task Information, Running Tasks
 *  may be stopped.
 *
 *  ===========================================================================
 *
 *  Copyright 2008-2009 Daniel Kruesi (Dan Krusi) and David Grob
 *
 *  This file is part of the emms framework.
 *
 *  The emms framework is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The emms framework  is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef TASKEDITOR_H_
#define TASKEDITOR_H_

#include <QWidget>
#include <QListWidget>
#include <QTextEdit>
#include <QPushButton>

#include "../Task/TaskManager.h"

class TaskEditor : public QWidget {

	Q_OBJECT

private:
	TaskManager *taskManager;
	bool idleTaskInList;
	Task *selectedTask;

	QListWidget *lstTasks;
	QTextEdit *txtTaskInfo;
	QPushButton *btnClose;
	QPushButton *btnStopTask;

public:
	TaskEditor(TaskManager *taskManager, QWidget *parent = 0);
	virtual ~TaskEditor();

private slots:
	void showTask(QModelIndex index);
	void stopTask();
	void addTaskToList(Task *task, QString title);

public slots:
	void refreshList();
	void setTaskManager(TaskManager *taskManager);

};

#endif /* TASKEDITOR_H_ */
