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
				raaTcpMsg *pTcpMsg = static_cast<raaTcpMsg*>(pEvent);
//				std::cout << "write -> " << pTcpMsg->asUInt(2) << std::endl;

				int iSize = pTcpMsg->data().length();
				write((const char*)&iSize, sizeof(int));

 				write(pTcpMsg->data().data(), pTcpMsg->data().length());
		}
	}
}
