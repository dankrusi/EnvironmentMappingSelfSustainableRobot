/*
 * RemoteInterfaceMessage.cpp
 *
 *  Created on: 17.02.2009
 *      Author: dkruesi
 */

#include "RemoteInterfaceMessage.h"

#include <QStringList>

RemoteInterfaceMessage::RemoteInterfaceMessage(RemoteInterfaceMessage &message) : CoreObject("RemoteInterfaceMessage") {
	// Copy all contents
	this->type = message.type;
	this->properties = new QMap<QString,QString>();
	this->sourceIp = message.sourceIp;
	this->sourcePort = message.sourcePort;
	this->destinationIp = message.destinationIp;
	this->destinationPort = message.destinationPort;
	this->timeStamp = message.timeStamp;
	QMap<QString,QString>::iterator k = message.properties->begin();
	while(k != message.properties->end()){
		this->properties->insert(k.key(),k.value());
		k++;
	}
}

RemoteInterfaceMessage::RemoteInterfaceMessage(QString type) : CoreObject("RemoteInterfaceMessage") {
	// Init
	this->type = type;
	this->properties = new QMap<QString,QString>();
	this->sourceIp = "";
	this->sourcePort = 0;
	this->destinationIp = "";
	this->destinationPort = 0;
	timeStamp = QDateTime::currentDateTime();
}

RemoteInterfaceMessage::RemoteInterfaceMessage(QByteArray *data) : CoreObject("RemoteInterfaceMessage"){
	// Init
	this->type = "unknown";
	this->properties = new QMap<QString,QString>();
	this->sourceIp = "";
	this->sourcePort = 0;
	this->destinationIp = "";
	this->destinationPort = 0;
	timeStamp = QDateTime::currentDateTime();

	// Unpack message
	QStringList keyvalues = QString(*data).split('\n');
	for(int i = 0; i < keyvalues.size(); i++) {
		if(keyvalues.value(i) != "") {
			QStringList keyvalue = keyvalues.value(i).split('=');
			if(keyvalue.count() > 0 && keyvalue.value(0) == "message") this->type = keyvalue.value(1);
			else if(keyvalue.count() == 2) properties->insert(keyvalue.value(0),keyvalue.value(1));
		}
	}
}

RemoteInterfaceMessage::~RemoteInterfaceMessage() {
	delete properties;
}

QString RemoteInterfaceMessage::toString() {
	QString data;
	data += "message=";
	data += this->type;
	QMap<QString,QString>::iterator k = this->properties->begin();
	while(k != this->properties->end()){
		data += "\n";
		data += k.key();
		data += "=";
		data += k.value();
		k++;
	}
	return data;
}

QString RemoteInterfaceMessage::property(QString key) {
	return properties->value(key);
}

QString RemoteInterfaceMessage::replyIp() {
	if(properties->value("reply-ip") != "") 	return properties->value("reply-ip");
	else										return sourceIp;
}

quint16 RemoteInterfaceMessage::replyPort() {
	if(properties->value("reply-port") != "") 	return properties->value("reply-port").toInt(NULL,10);
	else										return sourcePort;
}
