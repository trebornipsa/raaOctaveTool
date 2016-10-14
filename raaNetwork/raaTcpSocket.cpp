#include "raaTcpMsg.h"
#include "raaNetwork.h"


#include "raaTcpSocket.moc"
#include "raaTcpSocket.h"
#include <iostream>


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
//			std::cout << "\tsent" << std::endl;
				raaTcpMsg *pTcpMsg = static_cast<raaTcpMsg*>(pEvent);

				int iSize = pTcpMsg->data().length();
				write((const char*)&iSize, sizeof(int));

//				std::cout << "Size -> " << iSize << std::endl;
 				write(pTcpMsg->data().data(), pTcpMsg->data().length());
/*
				if(waitForBytesWritten(-1))
					std::cout << "\tsent" << std::endl;
				else
					std::cout << "\tunsent" << std::endl;
*/
		}
	}
}
