
#include <iostream>

#include <QtCore/QEvent>
#include <QtCore/QCoreApplication>

#include "raaTcpServer.h"
#include "raaTcpThread.h"
#include "raaTcpMsg.h"

#include "raaNetwork.h"
#include "raaNetworkServer.h"
#ifndef QTCREATOR
    #include "raaNetworkServer.moc"
#endif


raaNetworkServer::raaNetworkServer(unsigned int uiTcpPort, unsigned int uiUDPPort, QObject *pParent) : QObject(pParent)
{
	raaNetwork::start();
	m_uiTcpPort = 0;
	m_uiUdpPort = 0;
	m_pTcpServer = 0;

	if(uiTcpPort)
	{
		m_uiTcpPort = uiTcpPort;
		m_pTcpServer = new raaTcpServer(this);
		m_pTcpServer->startServer(m_uiTcpPort);
	}
}

raaNetworkServer::~raaNetworkServer()
{
	if (m_pTcpServer) m_pTcpServer->close();
}

void raaNetworkServer::write(QString sSocket, QByteArray data)
{
	if(m_pTcpServer)
	{
		raaTcpThread *pSocket = m_pTcpServer->socketByName(sSocket);
		if(pSocket)
		{
			QCoreApplication::postEvent(pSocket, new raaTcpMsg(data, raaNetwork::tcpWrite()));
		}
	}
}

void raaNetworkServer::stream(QByteArray& data)
{
}

const raaTcpServer* raaNetworkServer::tcpServer()
{
	return m_pTcpServer;
}

void raaNetworkServer::customEvent(QEvent* pEvent)
{
	if (pEvent->type() == raaNetwork::tcpRead())
	{
		raaTcpMsg *pMsg = static_cast<raaTcpMsg*>(pEvent);
		std::cout << "NetworkServer Event -> " << QString(pMsg->data()).toStdString() << std::endl;
		emit read(pMsg);

		pEvent->accept();

//		delete pEvent;
	}
}

