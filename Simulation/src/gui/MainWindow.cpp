#include <QGridLayout>

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	// Gui setup
	ui.setupUi(this);
	this->setWindowTitle("emss Simulation");

	// Init simulation
	this->simulation = new Simulation();

	// Init viewport
	this->viewport = new Viewport(simulation);
	this->setCentralWidget(this->viewport);

	// Connections
	connect(viewport, SIGNAL(statusUpdate(QString)), this, SLOT(showStatusUpdate(QString)));
	connect(ui.menubar, SIGNAL(triggered(QAction*)), this, SLOT(menuAction(QAction*)));
}

MainWindow::~MainWindow()
{

}

void MainWindow::showStatusUpdate(QString status) {
	this->statusBar()->showMessage(status, 2000);
}

void MainWindow::menuAction(QAction *action) {
	if(action == ui.actionExit) {
		close();
	} else if(action == ui.actionStart) {
		simulation->start();
	} else if(action == ui.actionStop) {
		simulation->stop();
	} else if(action == ui.actionRestart) {
		simulation->restart();
	} else if(action == ui.actionNew_Viewport) {
		Viewport *newViewport = new Viewport(simulation);
		newViewport->setWindowTitle("emss Viewport");
		newViewport->show();
	}
}


