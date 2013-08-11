/********************************************************************************
** Form generated from reading ui file 'MainWindow.ui'
**
** Created: Sun 28. Sep 15:04:45 2008
**      by: Qt User Interface Compiler version 4.4.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *actionExit;
    QAction *actionStart;
    QAction *actionStop;
    QAction *actionRestart;
    QAction *actionNew_Viewport;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menuSimulation;
    QMenu *menuViews;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindowClass)
    {
    if (MainWindowClass->objectName().isEmpty())
        MainWindowClass->setObjectName(QString::fromUtf8("MainWindowClass"));
    MainWindowClass->resize(519, 409);
    actionExit = new QAction(MainWindowClass);
    actionExit->setObjectName(QString::fromUtf8("actionExit"));
    actionStart = new QAction(MainWindowClass);
    actionStart->setObjectName(QString::fromUtf8("actionStart"));
    actionStop = new QAction(MainWindowClass);
    actionStop->setObjectName(QString::fromUtf8("actionStop"));
    actionRestart = new QAction(MainWindowClass);
    actionRestart->setObjectName(QString::fromUtf8("actionRestart"));
    actionNew_Viewport = new QAction(MainWindowClass);
    actionNew_Viewport->setObjectName(QString::fromUtf8("actionNew_Viewport"));
    centralwidget = new QWidget(MainWindowClass);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    MainWindowClass->setCentralWidget(centralwidget);
    menubar = new QMenuBar(MainWindowClass);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 519, 21));
    menuSimulation = new QMenu(menubar);
    menuSimulation->setObjectName(QString::fromUtf8("menuSimulation"));
    menuViews = new QMenu(menubar);
    menuViews->setObjectName(QString::fromUtf8("menuViews"));
    MainWindowClass->setMenuBar(menubar);
    statusbar = new QStatusBar(MainWindowClass);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    MainWindowClass->setStatusBar(statusbar);

    menubar->addAction(menuSimulation->menuAction());
    menubar->addAction(menuViews->menuAction());
    menuSimulation->addAction(actionStart);
    menuSimulation->addAction(actionStop);
    menuSimulation->addAction(actionRestart);
    menuSimulation->addSeparator();
    menuSimulation->addAction(actionExit);
    menuViews->addAction(actionNew_Viewport);

    retranslateUi(MainWindowClass);

    QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
    MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "MainWindow", 0, QApplication::UnicodeUTF8));
    actionExit->setText(QApplication::translate("MainWindowClass", "Exit", 0, QApplication::UnicodeUTF8));
    actionStart->setText(QApplication::translate("MainWindowClass", "Start", 0, QApplication::UnicodeUTF8));
    actionStop->setText(QApplication::translate("MainWindowClass", "Stop", 0, QApplication::UnicodeUTF8));
    actionRestart->setText(QApplication::translate("MainWindowClass", "Restart", 0, QApplication::UnicodeUTF8));
    actionNew_Viewport->setText(QApplication::translate("MainWindowClass", "New Viewport", 0, QApplication::UnicodeUTF8));
    menuSimulation->setTitle(QApplication::translate("MainWindowClass", "Simulation", 0, QApplication::UnicodeUTF8));
    menuViews->setTitle(QApplication::translate("MainWindowClass", "Views", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
