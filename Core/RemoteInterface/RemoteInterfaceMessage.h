/*
 * RemoteInterfaceMessage.h
 *
 *  Created on: 17.02.2009
 *      Author: dkruesi
 */

#ifndef REMOTEINTERFACEMESSAGE_H_
#define REMOTEINTERFACEMESSAGE_H_


#include <QMap>
#include <QByteArray>
#include <QDateTime>

#include "../CoreObject.h"

class RemoteInterfaceMessage : public CoreObject {

	Q_OBJECT

public:
	QMap<QString,QString> *properties;
	QString type;
	QString sourceIp;
	quint16 sourcePort;
	QString destinationIp;
	quint16 destinationPort;
	QDateTime timeStamp;

public:
	RemoteInterfaceMessage(QString type);
	RemoteInterfaceMessage(QByteArray *data);
	RemoteInterfaceMessage(RemoteInterfaceMessage &message);
	virtual ~RemoteInterfaceMessage();
	QString toString();
	QString property(QString key);
	QString replyIp();
	quint16 replyPort();
};

#endif /* REMOTEINTERFACEMESSAGE_H_ */
