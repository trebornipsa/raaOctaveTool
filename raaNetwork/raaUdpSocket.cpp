

#include "raaUdpMsg.h"
#include "raaNetwork.h"
#include "raaUdpSocket.moc"
#include "raaUdpSocket.h"


raaNet::raaUdpSocket::raaUdpSocket(QHostAddress &address, quint16 uiPort, QObject *pParent) : QUdpSocket(pParent)
{
	m_Address = address;
	m_uiPort = uiPort;
}

raaNet::raaUdpSocket::raaUdpSocket(QObject* pParent)
{
	m_uiPort = 0;
}

raaNet::raaUdpSocket::~raaUdpSocket()
{
}

void raaNet::raaUdpSocket::setConnection(const QHostAddress& address, quint16 uiPort)
{
	m_Address = address;
	m_uiPort = uiPort;
}

void raaNet::raaUdpSocket::customEvent(QEvent* pEvent)
{
	if (pEvent)
	{
		if (pEvent->type() == raaNetwork::udpWriteEvent())
		{
			raaUdpMsg *pUdpMsg = static_cast<raaUdpMsg*>(pEvent);
/*
			if (m_uiPort)
				writeDatagram(pUdpMsg->data().data(), m_Address, m_uiPort);
			else
				write(pUdpMsg->data().data(), pUdpMsg->data().length());
				*/
//			int iSize = pUdpMsg->data().length();
//			write((const char*)&iSize, sizeof(int));
//			writeDatagram((const char*)&iSize, sizeof(int), m_Address, m_uiPort);
			//				std::cout << "Size -> " << iSize << std::endl;
			//			write(pUdpMsg->data().data(), pUdpMsg->data().length());
			writeDatagram(pUdpMsg->data().data(), pUdpMsg->data().length(), m_Address, m_uiPort);

		}
	}
}
