/*
 *  TaskEditor.cpp
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

#include "TaskEditor.h"

#include "../Library/Util.h"

#include <QVBoxLayout>
#include <QModelIndexList>

TaskEditor::TaskEditor(TaskManager *taskManager, QWidget *parent) : QWidget(parent) {

	// Init
	this->taskManager = taskManager;
	this->setAttribute(Qt::WA_DeleteOnClose, true);
	idleTaskInList = false;
	selectedTask = NULL;

	// Gui stuff...
	this->setWindowTitle("emss Task Editor");
	this->setLayout(new QVBoxLayout());

	lstTasks = new QListWidget();
	this->layout()->addWidget(lstTasks);

	txtTaskInfo = new QTextEdit();
	Util::setTextEditAsLogger(txtTaskInfo);
	this->layout()->addWidget(txtTaskInfo);

	btnStopTask = new QPushButton("Stop Task");
	btnStopTask->setEnabled(false);
	this->layout()->addWidget(btnStopTask);

	btnClose = new QPushButton("Close");
	this->layout()->addWidget(btnClose);

	// Load tasks
	refreshList();

	// Connections
	connect(btnClose, SIGNAL(clicked()), this, SLOT(close()));
	connect(btnStopTask, SIGNAL(clicked()), this, SLOT(stopTask()));
	connect(lstTasks, SIGNAL(clicked(QModelIndex)), this, SLOT(showTask(QModelIndex)));
}

TaskEditor::~TaskEditor() {
}

void TaskEditor::refreshList() {

	// Load tasks
	lstTasks->clear();
	if(!taskManager) return;
	if(taskManager->getIdleTask()) {
		addTaskToList(taskManager->getIdleTask(), "Idle Task");
		idleTaskInList = true;
	}
	for(int i = 0; i < taskManager->tasks->count(); i++) {
		addTaskToList(taskManager->tasks->at(i), QString("Task %1").arg(i));
	}
}


void TaskEditor::addTaskToList(Task *task, QString title) {
	if(!task) return;
	QString status;
	if     (task->status == Task::Waiting) status = "Waiting";
	else if(task->status == Task::Running) status = "Running";
	else if(task->status == Task::Finished) status = "Finished";
	else if(task->status == Task::Interrupted) status = "Interrupted";
	QListWidgetItem *item = new QListWidgetItem(QString("%1: %2 - %3").arg(title).arg(task->getName()).arg(status));
	//item->setData(Qt::UserRole, index);
	if(task->status == Task::Running) item->setData(Qt::TextColorRole, Qt::blue);
	else if(task->status == Task::Finished) item->setData(Qt::TextColorRole, Qt::gray);
	else if(task->status == Task::Interrupted) item->setData(Qt::TextColorRole, Qt::red);
	lstTasks->addItem(item);
}

void TaskEditor::showTask(QModelIndex index) {

	// disable stop task button
	btnStopTask->setEnabled(false);

	// view task information
	QString id;
	if(idleTaskInList && index.row() != 0) {
		selectedTask = taskManager->tasks->at(index.row()-1);
		id = QString("%1").arg(index.row()-1);
	} else if(idleTaskInList && index.row() == 0) {
		selectedTask = taskManager->getIdleTask();
		id = "Idle Task";
	} else {
		selectedTask = taskManager->tasks->at(index.row());
		id = QString("%1").arg(index.row());
	}

	QString status;
	if     (selectedTask->status == Task::Waiting) status = "Waiting";
	else if(selectedTask->status == Task::Running) {
		status = "Running";
		btnStopTask->setEnabled(true);
	}
	else if(selectedTask->status == Task::Finished) status = "Finished";
	else if(selectedTask->status == Task::Interrupted) status = "Interrupted";
	txtTaskInfo->setText(QString("ID:\t%1\nTask:\t%2\nStatus:\t%3\nInterval:\t%4\n%5").arg(id).arg(selectedTask->getName()).arg(status).arg(selectedTask->getInterval()).arg(selectedTask->description()));
}

void TaskEditor::stopTask(){
	if(selectedTask) selectedTask->interrupt();
}


void TaskEditor::setTaskManager(TaskManager *taskManager) {
	this->taskManager = taskManager;
}
