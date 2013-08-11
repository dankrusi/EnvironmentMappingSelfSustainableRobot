#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

#include "ui_MainWindow.h"

#include "../gui/Viewport.h"

#include "../sim/Simulation.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
private:
	Ui::MainWindowClass ui;
	
public:
	Viewport 	*viewport;
	Simulation	*simulation;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
    void showStatusUpdate(QString status);
    void menuAction(QAction *action);

};

#endif // MAINWINDOW_H
