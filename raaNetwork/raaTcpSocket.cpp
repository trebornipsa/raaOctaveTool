#include "raaTcpMsg.h"
#include "raaNetwork.h"

#include "raaTcpSocket.moc"
#include "raaTcpSocket.h"
//#include <iostream>

raaNet::raaTcpSocket::raaTcpSocket(QObject *pParent) : QTcpSocket(pParent)
{
}

raaNet::raaTcpSocket::~raaTcpSocket()
{
}

void raaNet::raaTcpSocket::customEvent(QEvent* pEvent)
{
	if (pEvent)
	{
		if (pEvent->type() == raaNetwork::tcpWriteEvent())
		{
				m_Mutex.lock();
				raaTcpMsg *pTcpMsg = static_cast<raaTcpMsg*>(pEvent);
				QByteArray &b = pTcpMsg->data();
				int iSize = b.length();
				write((const char*)&iSize, sizeof(int));
				write(b.data(), b.length());
				waitForBytesWritten();
				m_Mutex.unlock();
		}
	}
}
