#include <QtCore>
#include <QCoreApplication>
#include <QUdpSocket>

#include <iostream>

#define IP_CLIENT "127.0.0.1"
#define IP_SERVER "127.0.0.1"
#define PORT 31415
#define MESSAGE "Is anyone there? Hello?"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString role = argv[1];

	if(role == "server") {
		std::cout << "role: server\n";

		// Send a single UDP packet...
		QString data = MESSAGE;
		QUdpSocket socket(NULL);
		socket.writeDatagram(data.toAscii(), QHostAddress(IP_SERVER), PORT);
		std::cout << "sent message: ";
		std::cout << data.toAscii().data();
		std::cout << "\n";

	} else if (role == "client") {
		std::cout << "role: client\n";

		QUdpSocket udpSocket;
		if(udpSocket.bind(QHostAddress(IP_CLIENT), PORT, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
			// Infinately listen for UDP packets...
			while(true){
				QByteArray data;
				data.resize(256);
				int len = udpSocket.readDatagram(data.data(), data.size(), NULL, NULL);
				if (len > -1 ){
					std::cout << "received message: ";
					std::cout << data.data();
					std::cout << "\n";
				}
			}
		} else {
			std::cout << "bind failed!\n";
		}

	} else {
		std::cout << "Usage: UDPServerTest {server|role}\n";
	}

	std::cout << "done\n";
    return 1;
}
