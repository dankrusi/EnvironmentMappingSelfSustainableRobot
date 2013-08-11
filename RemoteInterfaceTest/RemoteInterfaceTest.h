#ifndef REMOTEINTERFACETEST_H
#define REMOTEINTERFACETEST_H

#include <QtGui/QWidget>
#include "ui_RemoteInterfaceTest.h"

#include "../Core/Core.h"
#include "../Core/Library/Debug.h"
#include "../Core/Library/Util.h"
#include "../Core/Task/TaskManager.h"
#include "../Core/RemoteInterface/RemoteInterface.h"
#include "../Core/RemoteInterface/RemoteInterfaceMessage.h"

class RemoteInterfaceTest : public QWidget
{
    Q_OBJECT

private:
	Core *core;

public:
    RemoteInterfaceTest(QWidget *parent = 0);
    ~RemoteInterfaceTest();

public slots:
	void start();
	void send();
	void subscribeToServer();
	void unsubscribeFromServer();
	void refreshSubscribers();
	void receiveMessage(RemoteInterfaceMessage *message);

private:
    Ui::RemoteInterfaceTestClass ui;
};

#endif // REMOTEINTERFACETEST_H
