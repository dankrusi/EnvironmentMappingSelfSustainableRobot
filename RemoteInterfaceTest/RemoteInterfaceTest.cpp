#include "RemoteInterfaceTest.h"



RemoteInterfaceTest::RemoteInterfaceTest(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	Util::setTextEditAsLogger(ui.console);
	Debug::setOutput(ui.console);
	core = NULL;
	connect(ui.buttonStart, SIGNAL(clicked()), this, SLOT(start()));
	connect(ui.buttonSend, SIGNAL(clicked()), this, SLOT(send()));
	connect(ui.buttonConnect, SIGNAL(clicked()), this, SLOT(subscribeToServer()));
	connect(ui.buttonDisconnect, SIGNAL(clicked()), this, SLOT(unsubscribeFromServer()));
}

RemoteInterfaceTest::~RemoteInterfaceTest()
{
	if(core) {
		start();
		delete core;
	}
}

void RemoteInterfaceTest::start() {
	if(!core) {

		// Create core
		core = new Core();

		// Init task manager
		core->taskManager = new TaskManager(core);
		core->taskManager->start(QThread::NormalPriority);

		// Init remote interface
		core->remoteInterface = new RemoteInterface(core, ui.serverIP->text(), ui.serverPort->text().toInt(NULL,10));
		core->remoteInterface->start(QThread::NormalPriority);
		connect(core->remoteInterface, SIGNAL(hostSubscribed()), this, SLOT(refreshSubscribers()));
		connect(core->remoteInterface, SIGNAL(hostUnsubscribed()), this, SLOT(refreshSubscribers()));
		connect(core->remoteInterface, SIGNAL(messageReceived(RemoteInterfaceMessage*)), this, SLOT(receiveMessage(RemoteInterfaceMessage*)));

		ui.groupSubscribers->setEnabled(true);
		ui.groupListener->setEnabled(true);
		ui.groupSender->setEnabled(true);
		ui.groupConnect->setEnabled(true);
		ui.buttonStart->setText("Stop");

	} else {

		// Shutdown
		core->taskManager->stop();
		core->remoteInterface->stop();

		// Cleanup
		delete core;
		core = NULL;

		ui.groupSubscribers->setEnabled(false);
		ui.groupListener->setEnabled(false);
		ui.groupSender->setEnabled(false);
		ui.groupConnect->setEnabled(false);
		ui.buttonStart->setText("Start");

	}
}

void RemoteInterfaceTest::send() {
	QByteArray data(ui.senderProperties->toPlainText().prepend(QString("message=%1\n").arg(ui.senderType->text())).toAscii());
	RemoteInterfaceMessage *message = new RemoteInterfaceMessage(&data);
	if(ui.chbBroadcastMessage->isChecked()){
		core->remoteInterface->broadcastMessage(message);
	}
	else {
		core->remoteInterface->sendMessage(ui.receiverIP->text(),ui.receiverPort->text().toInt(),message);
	}
}

void RemoteInterfaceTest::subscribeToServer() {
	core->remoteInterface->subscribeToServer(ui.connectIP->text(),ui.connectPort->text().toInt());
}

void RemoteInterfaceTest::unsubscribeFromServer() {
	core->remoteInterface->unsubscribeFromServer(ui.connectIP->text(),ui.connectPort->text().toInt());
}

void RemoteInterfaceTest::refreshSubscribers() {
	ui.subscribers->clear();
	QMap<QString,quint16>::iterator i = core->remoteInterface->subscribers->begin();
	while(i != core->remoteInterface->subscribers->end()){
		ui.subscribers->addItem(QString("%1:%2").arg(i.key()).arg(i.value()));
		i++;
	}
}

void RemoteInterfaceTest::receiveMessage(RemoteInterfaceMessage *message) {
	ui.listenerMessages->addItem(message->toString().replace("\n"," | "));
}


