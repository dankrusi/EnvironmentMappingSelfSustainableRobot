/********************************************************************************
** Form generated from reading ui file 'RemoteInterfaceTest.ui'
**
** Created: Tue Mar 31 13:17:56 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_REMOTEINTERFACETEST_H
#define UI_REMOTEINTERFACETEST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RemoteInterfaceTestClass
{
public:
    QGroupBox *groupListener;
    QListWidget *listenerMessages;
    QGroupBox *groupSender;
    QLabel *label_3;
    QPushButton *buttonSend;
    QPlainTextEdit *senderProperties;
    QLineEdit *senderType;
    QLabel *label_8;
    QLabel *label_11;
    QLineEdit *receiverIP;
    QLineEdit *receiverPort;
    QLabel *label_12;
    QCheckBox *chbBroadcastMessage;
    QGroupBox *groupServer;
    QLabel *label_6;
    QLabel *label_7;
    QLineEdit *serverPort;
    QPushButton *buttonStart;
    QLineEdit *serverIP;
    QGroupBox *groupBox;
    QTextEdit *console;
    QGroupBox *groupConnect;
    QLabel *label_9;
    QLabel *label_10;
    QLineEdit *connectPort;
    QPushButton *buttonConnect;
    QLineEdit *connectIP;
    QPushButton *buttonDisconnect;
    QGroupBox *groupSubscribers;
    QListWidget *subscribers;

    void setupUi(QWidget *RemoteInterfaceTestClass)
    {
        if (RemoteInterfaceTestClass->objectName().isEmpty())
            RemoteInterfaceTestClass->setObjectName(QString::fromUtf8("RemoteInterfaceTestClass"));
        RemoteInterfaceTestClass->resize(401, 585);
        groupListener = new QGroupBox(RemoteInterfaceTestClass);
        groupListener->setObjectName(QString::fromUtf8("groupListener"));
        groupListener->setEnabled(false);
        groupListener->setGeometry(QRect(10, 130, 231, 141));
        listenerMessages = new QListWidget(groupListener);
        listenerMessages->setObjectName(QString::fromUtf8("listenerMessages"));
        listenerMessages->setGeometry(QRect(10, 20, 211, 111));
        groupSender = new QGroupBox(RemoteInterfaceTestClass);
        groupSender->setObjectName(QString::fromUtf8("groupSender"));
        groupSender->setEnabled(false);
        groupSender->setGeometry(QRect(10, 280, 381, 171));
        label_3 = new QLabel(groupSender);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 80, 46, 14));
        buttonSend = new QPushButton(groupSender);
        buttonSend->setObjectName(QString::fromUtf8("buttonSend"));
        buttonSend->setGeometry(QRect(60, 140, 101, 23));
        senderProperties = new QPlainTextEdit(groupSender);
        senderProperties->setObjectName(QString::fromUtf8("senderProperties"));
        senderProperties->setGeometry(QRect(60, 80, 311, 51));
        senderType = new QLineEdit(groupSender);
        senderType->setObjectName(QString::fromUtf8("senderType"));
        senderType->setGeometry(QRect(60, 20, 101, 20));
        label_8 = new QLabel(groupSender);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 20, 46, 14));
        label_11 = new QLabel(groupSender);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(10, 52, 51, 16));
        receiverIP = new QLineEdit(groupSender);
        receiverIP->setObjectName(QString::fromUtf8("receiverIP"));
        receiverIP->setGeometry(QRect(60, 50, 101, 20));
        receiverPort = new QLineEdit(groupSender);
        receiverPort->setObjectName(QString::fromUtf8("receiverPort"));
        receiverPort->setGeometry(QRect(270, 48, 101, 20));
        label_12 = new QLabel(groupSender);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(240, 50, 51, 16));
        chbBroadcastMessage = new QCheckBox(groupSender);
        chbBroadcastMessage->setObjectName(QString::fromUtf8("chbBroadcastMessage"));
        chbBroadcastMessage->setGeometry(QRect(240, 20, 91, 19));
        groupServer = new QGroupBox(RemoteInterfaceTestClass);
        groupServer->setObjectName(QString::fromUtf8("groupServer"));
        groupServer->setGeometry(QRect(10, 10, 131, 111));
        label_6 = new QLabel(groupServer);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 20, 46, 14));
        label_7 = new QLabel(groupServer);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 50, 46, 14));
        serverPort = new QLineEdit(groupServer);
        serverPort->setObjectName(QString::fromUtf8("serverPort"));
        serverPort->setGeometry(QRect(40, 50, 81, 20));
        buttonStart = new QPushButton(groupServer);
        buttonStart->setObjectName(QString::fromUtf8("buttonStart"));
        buttonStart->setGeometry(QRect(40, 80, 81, 23));
        serverIP = new QLineEdit(groupServer);
        serverIP->setObjectName(QString::fromUtf8("serverIP"));
        serverIP->setGeometry(QRect(40, 20, 81, 20));
        groupBox = new QGroupBox(RemoteInterfaceTestClass);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 460, 381, 111));
        console = new QTextEdit(groupBox);
        console->setObjectName(QString::fromUtf8("console"));
        console->setGeometry(QRect(10, 20, 361, 81));
        groupConnect = new QGroupBox(RemoteInterfaceTestClass);
        groupConnect->setObjectName(QString::fromUtf8("groupConnect"));
        groupConnect->setEnabled(false);
        groupConnect->setGeometry(QRect(150, 10, 241, 111));
        label_9 = new QLabel(groupConnect);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(10, 20, 46, 14));
        label_10 = new QLabel(groupConnect);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(10, 50, 46, 14));
        connectPort = new QLineEdit(groupConnect);
        connectPort->setObjectName(QString::fromUtf8("connectPort"));
        connectPort->setGeometry(QRect(40, 50, 191, 20));
        buttonConnect = new QPushButton(groupConnect);
        buttonConnect->setObjectName(QString::fromUtf8("buttonConnect"));
        buttonConnect->setGeometry(QRect(40, 80, 91, 23));
        connectIP = new QLineEdit(groupConnect);
        connectIP->setObjectName(QString::fromUtf8("connectIP"));
        connectIP->setGeometry(QRect(40, 20, 191, 20));
        buttonDisconnect = new QPushButton(groupConnect);
        buttonDisconnect->setObjectName(QString::fromUtf8("buttonDisconnect"));
        buttonDisconnect->setGeometry(QRect(140, 80, 91, 23));
        groupSubscribers = new QGroupBox(RemoteInterfaceTestClass);
        groupSubscribers->setObjectName(QString::fromUtf8("groupSubscribers"));
        groupSubscribers->setEnabled(false);
        groupSubscribers->setGeometry(QRect(250, 130, 141, 141));
        subscribers = new QListWidget(groupSubscribers);
        subscribers->setObjectName(QString::fromUtf8("subscribers"));
        subscribers->setGeometry(QRect(10, 20, 121, 111));

        retranslateUi(RemoteInterfaceTestClass);

        QMetaObject::connectSlotsByName(RemoteInterfaceTestClass);
    } // setupUi

    void retranslateUi(QWidget *RemoteInterfaceTestClass)
    {
        RemoteInterfaceTestClass->setWindowTitle(QApplication::translate("RemoteInterfaceTestClass", "RemoteInterfaceTest", 0, QApplication::UnicodeUTF8));
        groupListener->setTitle(QApplication::translate("RemoteInterfaceTestClass", "Messages", 0, QApplication::UnicodeUTF8));
        groupSender->setTitle(QApplication::translate("RemoteInterfaceTestClass", "Send Message", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("RemoteInterfaceTestClass", "Props.:", 0, QApplication::UnicodeUTF8));
        buttonSend->setText(QApplication::translate("RemoteInterfaceTestClass", "Send", 0, QApplication::UnicodeUTF8));
        senderProperties->setPlainText(QApplication::translate("RemoteInterfaceTestClass", "task=fingerprintnavigation\n"
"interval=5000\n"
"wait-time=1000", 0, QApplication::UnicodeUTF8));
        senderType->setText(QApplication::translate("RemoteInterfaceTestClass", "start-task", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("RemoteInterfaceTestClass", "Type:", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("RemoteInterfaceTestClass", "IP:", 0, QApplication::UnicodeUTF8));
        receiverIP->setText(QApplication::translate("RemoteInterfaceTestClass", "127.0.0.1", 0, QApplication::UnicodeUTF8));
        receiverPort->setText(QApplication::translate("RemoteInterfaceTestClass", "9000", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("RemoteInterfaceTestClass", "Port:", 0, QApplication::UnicodeUTF8));
        chbBroadcastMessage->setText(QApplication::translate("RemoteInterfaceTestClass", "Broadcast", 0, QApplication::UnicodeUTF8));
        groupServer->setTitle(QApplication::translate("RemoteInterfaceTestClass", "Server", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("RemoteInterfaceTestClass", "IP:", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("RemoteInterfaceTestClass", "Port:", 0, QApplication::UnicodeUTF8));
        serverPort->setText(QApplication::translate("RemoteInterfaceTestClass", "9000", 0, QApplication::UnicodeUTF8));
        buttonStart->setText(QApplication::translate("RemoteInterfaceTestClass", "Start", 0, QApplication::UnicodeUTF8));
        serverIP->setText(QApplication::translate("RemoteInterfaceTestClass", "127.0.0.1", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("RemoteInterfaceTestClass", "Console", 0, QApplication::UnicodeUTF8));
        groupConnect->setTitle(QApplication::translate("RemoteInterfaceTestClass", "Remote Subscriptions", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("RemoteInterfaceTestClass", "IP:", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("RemoteInterfaceTestClass", "Port:", 0, QApplication::UnicodeUTF8));
        connectPort->setText(QApplication::translate("RemoteInterfaceTestClass", "9000", 0, QApplication::UnicodeUTF8));
        buttonConnect->setText(QApplication::translate("RemoteInterfaceTestClass", "Subscribe", 0, QApplication::UnicodeUTF8));
        connectIP->setText(QApplication::translate("RemoteInterfaceTestClass", "127.0.0.1", 0, QApplication::UnicodeUTF8));
        buttonDisconnect->setText(QApplication::translate("RemoteInterfaceTestClass", "Unsubscribe", 0, QApplication::UnicodeUTF8));
        groupSubscribers->setTitle(QApplication::translate("RemoteInterfaceTestClass", "Subscribers", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(RemoteInterfaceTestClass);
    } // retranslateUi

};

namespace Ui {
    class RemoteInterfaceTestClass: public Ui_RemoteInterfaceTestClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REMOTEINTERFACETEST_H
