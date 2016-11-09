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
				unsigned int iSize = pTcpMsg->data().length();
				write((const char*)&iSize, sizeof(unsigned int));
				write(pTcpMsg->data().data(), iSize);
//				waitForBytesWritten();
				m_Mutex.unlock();
		}
	}
}
